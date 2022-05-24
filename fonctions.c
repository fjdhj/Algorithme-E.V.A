#include <header.h>

int getEndianness(){ 
	int n=1;
	if(*(char *)&n == 1){                           
		return 0;
	}
	return 1;
}

unsigned int reverseint(unsigned int a){ 
	unsigned int reversed;                         
	uint8_t *n1,*n2;				
	n1=(uint8_t *) &a;
	n2=(uint8_t *) &reversed;
	n2[0]=n1[3];
	n2[1]=n1[2];
	n2[2]=n1[1];
	n2[3]=n1[0];
	return reversed;
}

unsigned short reverseshort(unsigned short a){            
	unsigned short reversed;
	uint8_t *n1,*n2;				
	n1=(uint8_t *) &a;				
	n2=(uint8_t *) &reversed;
	n2[0]=n1[1];
	n2[1]=n1[0];
	return reversed;
}


void write4blocks(FILE* file,unsigned int val,int endianness){  
    //We want to write 4 bytes into FILE* file. This function assumes sizeof(int)==4.
    //We take into account endianness which may reverse byte writing order. If that's the case, we manually reverse the order
    //with reverseint() so the changes cancel out.
    //We use fwrite() to write bytes inside our file.
    if(endianness==0){ 
        unsigned int reversed=reverseint(val);
        if(fwrite(&reversed,4,1,file)!=1){
           printf("\nThere was a problem during compression.");
           exit(-1);
        }							
    
    }
    else{ 
	if(fwrite(&val,4,1,file)!=1){
           printf("\nThere was a problem during compression.");
           exit(-1);
	}
    }
}

void write2blocks(FILE* file,unsigned short val,int endianness){  
    //We want to write 2 bytes into FILE* file.This function assumes sizeof(short)==2.
    //We take into account endianness which may reverse byte writing order. If that's the case, we manually reverse the order
    //with reverseshort() so the changes cancel out.
    //We use fwrite() to write bytes inside our file.
    if(endianness==0){ 
        unsigned short reversed=reverseshort(val);
        if(fwrite(&reversed,2,1,file)!=1){
           printf("\nThere was a problem during compression.");
           exit(-1);
        }	
    }
    else{ 
        if(fwrite(&val,2,1,file)!=1){
           printf("\nThere was a problem during compression.");
           exit(-1);
        }
    }							
}

void write1blocks(FILE* file,char val){ 
    //We want to write a byte into FILE* file.This function assumes sizeof(char)==1.
    //We use fwrite() to write the byte inside our file.
    if(fwrite(&val,1,1,file)!=1){
        printf("\nThere was a problem during compression.");
        exit(-1);
    }
}

unsigned long timer(){
    //This function uses the getttimeofday() function, included in <sys/time.h>, which returns a timeval structure containing
    //seconds and microseconds elapsed since a fixed date.
    //To return a global value in microseconds, we add the microseconds attribute and the seconds attribute multiplied by 10**6.
    struct timeval tv;
    if(gettimeofday(&tv,NULL)==-1){
        printf("Error occured while accessing time.");
        exit(-1);
    }
    return (1000000*tv.tv_sec)+tv.tv_usec;
}

long int size(FILE* f){
    //This functions returns the size of FILE* file in bytes.
    //To get the size, we go to the end of the file with fseek() and use ftell() to know what number of bytes is before the end.
    long int filesize;
    if(f==NULL){
        printf("There was a problem accessing a file.");
        exit(-1);
    }
    fseek(f,0,2);
    filesize=ftell(f);
    return filesize;
}

void compression(char* filename,int endianness){
    /*
    Overview of our variables :
    name is a string which will cointain name of the compressed image ; cache is a int array which will contain the values of the 
    64 lastly seen pixels ; cacheindex is used to uptdate cache during compression ; previouspixel and currentpixel are the values of 
    said pixels ; output is an int used during our main loop to determine if next steps are to be followed or ignored ; i and j are 
    iterators ; width,height,range,colors are those of the ppm image ; samepixels is an int used to determine what number of consecutive
    same value pixels was seen up to now ; index,diffr,diffb,diffg,diffrg,diffbg are used as the EVA defines them (more detail under
    Steps 3 and 4) ; start and end are used to determine compression execution time ; compressedsize and uncompressedsize are size in
    bytes of the ppm image and compressed ppm image.
    */
    char* name;
    int cache[64],cacheindex=0;
    int previouspixel,currentpixel,output;
    int i,j,width,height,range,colors;
    int samepixels,index,diffr,diffb,diffg,diffrg,diffbg;
    unsigned long start,end;
    long int compressedsize,uncompressedsize;
	
    //Initialising cache.
    for(i=0;i<64;i++){
          cache[i]=0;
    }
/*
    printf("\nFILE %s : Give a name to the compressed file.",filename);
    while(scanf("%s",name)!=1)){                                  Partie susceptible de générer des erreurs
	printf("\nInvalid name.");
    } 
*/
	
/*                                                               Partie problématique 
    if(sprintf(path,"ppm/%s",filename)<=0){ 
	printf("\nThere was a problem while accessing the file.");
	exit(-1);
    }
*/
    //Getting the size of the ppm image.
    FILE* temporary=fopen("daftpunk.ppm","rb");
    if(!temporary){
        printf("\n There was a problem while opening the file.");
        exit(-1);
    }
    uncompressedsize=size(temporary);
    fclose(temporary);
	
    //Getting our first time value.
    start=timer();

    //Opening ppm image with a ppm stream (part of ppm_lib.c)
    PPM_IMG* old=ppmOpen("daftpunk.ppm");             
    if(!old){
        printf("\n There was a problem while opening the file.");
        exit(-1);
    }

    //Getting values of width,height,range,colors of our ppm image.
    width=ppmGetWidth(old);
    height=ppmGetHeight(old);
    range=ppmGetRange(old);
    colors=ppmGetColors(old);

    //Creating and opening a binary file in writing mode. It will become the compressed ppm image.
    FILE* new=fopen("Compressed","wb+");
    if(!new){
        printf("\n There was a problem.");
        exit(-1);
    }

    //Initialising pixel variables.
    previouspixel=-1;
    samepixels=0;

    //The first 16 bytes (4 ints) of the compressed image are its previous characteristics (width,height,range and colors in ppm format)
    write4blocks(new,width,endianness);
    write4blocks(new,height,endianness);
    write4blocks(new,range,endianness);
    write4blocks(new,colors,endianness);

    //Main loop
    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            output=0;
	
	    //Image is being read top to bottom, left to right.
            currentpixel=ppmRead(old,j,i);
		
            //Step 1
	    //Case in which sequences of same pixels values are or were read.
            if(currentpixel==previouspixel){
                if(samepixels==63){
		    //If maximum value for an EVA_BLK_SAME block is reached, we write that block and samepixels starts over.
                    write1blocks(new,EVA_BLK_SAME+samepixels-1);
                    samepixels=0;
                    output=1;
                }
                else{
		    //Else it is just incremented and current pixel was taken care of, hence output being 1.
                    samepixels++;
		    output=1;
                }
            }
            if(previouspixel!=currentpixel && samepixels>0){
		//If a sequence of same pixels was just interrupted, we need to write a SAME block but still manage current pixel.
                write1blocks(new,EVA_BLK_SAME+samepixels-1);
                samepixels=0;
                output=0;
            }
		
            //Step 2
	    //Case in which current pixel was seen recently.
            if(output==0){
		 //Use of EVA's formula to compute index of cache needed to be checked.
                 index=(3*red(currentpixel)+5*green(currentpixel)+7*blue(currentpixel))%64;
                 if(currentpixel==cache[index]){
                    write1blocks(new,EVA_BLK_INDEX+index);
                    output=1;
                }
            }
		
            //Step 3
	    //Case in which current pixel is very similar to the previous one.
            if(output==0){
		//We compute differrences between each individual rgb value and verify if they are all between -1 and 2.
                diffr=red(currentpixel)-red(previouspixel);
                diffg=green(currentpixel)-green(previouspixel);
                diffb=blue(currentpixel)-blue(previouspixel);
                if(((diffr>=-2 && diffr<=1) && diffg>=-2) && ((diffg<=1 && diffb>=-2) && diffb<=1)){
		    //In that case, a DIFF block is written.
                    write1blocks(new,EVA_BLK_DIFF+diffb+2+4*(diffg+2)+16*(diffr+2));
                    output=1;
                }
            }
		
            //Step 4
	    //Case in which green values are quite similar.
            if(output==0){
                if(diffg>=-32 && diffg<=31){
		    //We compute new differences between red and green values, blue and green values.
                    diffrg=diffr-diffg;
                    diffbg=diffb-diffg;
                    if((diffrg>=-8 && diffrg<=7) && (diffbg>=-8 && diffbg<=7)){
			//If they both are between -8 and 7, the 2 bytes of a LUMA block are written.
                        write2blocks(new,EVA_BLK_LUMA+diffbg+8+16*(diffrg+8)+256*(diffg+32),endianness);
                        output=1;
                    }
                }
            }
		
            //Step 5
	    //Case in which none of the previous steps were able to be followed.
            if(output==0){
		//Writing the 4 bytes of a RGB block.
                write4blocks(new,EVA_BLK_RGB+blue(currentpixel)+256*green(currentpixel)+65536*red(currentpixel),endianness);
                output=1;
            }
		
            //Step 6
	    //Used to debug function.
		
            //Step 7
            //Uptdating cache and value of previouspixel.
            cacheindex++;
            cache[cacheindex%64]=currentpixel;
            previouspixel=currentpixel;
        }
    }
	
    //Getting new file's size and closing streams.
    compressedsize=size(new);
    ppmClose(old);
    fclose(new);
	 
    //Getting the second time value, since compression just ended and files were closed.
    end=timer();
	
    //Output of time taken in seconds and compression ratio in percent.
    printf("\nCompressing file took %f seconds",(end-start)/1000000.0);
    printf("\nCompression ratio : compressed is %f%% the size of the original image.",(compressedsize/(uncompressedsize/1.0))*100);
}
