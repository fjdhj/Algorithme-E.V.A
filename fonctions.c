#include "header.h"
#include "ppm_lib.c"

int getEndianness(){
    //This function returns endianness of computer (0 for little endian, 1 for big endian)
    //We take a char pointer which points to the first byte of an int b of value 1, byte whose address is (char*)&b.
    //Adding (char*) before &b when assigning value is necessary because of the fact that b is an int, which causes type conflicts with char pointer p.
    //If first byte *p is 1, then the int was stored by a system using little endian. Else, *p is 0.
    char a;
    int b=1;
    char* p=NULL;
    p=(char*)&b;
    if(!p){
        printf("\nMemory error occured.");
        exit(-1);
    }
    a=*p;
    if(a==1){
        return 0;
    }
    return 1;

}


int power(int value, int p){
    //This function returns value to the power of p. Follows p^0 = 1.
    int result=1;
    if(p==0){
        return 1;
    }
    for(int i=1;i<=p;i++){
        result*=value;
    }
    return result;
}

unsigned int reverseint(unsigned int a){
    //This function returns the int a with reversed byte order. 0x01234567 becomes 0x67452301.
    //We take the last byte of a and add it to the first non-already seen byte of reversed. a is then shifted right the size of a byte. 
    //We repeat 4 times to complete reversed.
    /*
    Example : input 0xAAAABBBB
    1: a=0xAAAABB\BB ; reversed=0xBB000000
    2: a=0xAAAA\BB ; reversed=0xBBBB0000
    3: a=0xAA\AA ; reversed=0xBBBBAA00
    4: a=0x\AA ; reversed=0xBBBBAAAA
    */
    int temp;
    unsigned int reversed=0;
    for(int i=0;i<=3;i++){
        temp=a%256;
        a=a>>8;
        reversed+=power(256,3-i)*temp;
    }
    return reversed;
}

unsigned short reverseshort(unsigned short a){
    //This function returns the short a with reversed byte order. 0x0123 becomes 0x2301
    //We take the last byte of a and add it to the first non-already seen byte of reversed. a is then shifted right the size of a byte.
    //We repeat 2 times to complete reversed.
    int temp;
    unsigned short reversed=0;
    for(int i=0;i<=1;i++){
        temp=a%256;
        a=a>>8;
        reversed+=power(256,1-i)*temp;
    }
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

void read4blocks(FILE* file, unsigned int* val, int endianness){                
	if(fread(val, 4, 1, file) != 1){
           printf("\n[READ4BLOC] There was a problem during decompression.\n");
           exit(-1);
	}
	if(endianness==0){ 
		*val=reverseint(*val);
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

void read2blocks(FILE* file, unsigned short* val, int endianness){            
	if(fread(val, 2, 1, file) != 1){
			printf("\n[READ2BLOC] There was a problem during compression.\n");
			exit(-1);
	}
		
	if(endianness==0){ 
		*val=reverseshort(*val);
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

void read1blocks(FILE* file, unsigned char*	 val){
	if(fread(val, 1, 1, file) != 1){
		printf("\n[READ1BLOC] There was a problem during decompression.\n");
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

void compression(char* filename,int endianness, char* outputfile){
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
	
    //Getting the size of the ppm image.
    FILE* temporary=fopen(filename,"rb");
    if(!temporary){
        printf("\n There was a problem while opening the file.");
        exit(-1);
    }
    uncompressedsize=size(temporary);
    fclose(temporary);
	
    //Getting our first time value.
    start=timer();

    //Opening ppm image with a ppm stream (part of ppm_lib.c)
    PPM_IMG* old=ppmOpen(filename);             
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
    FILE* new=fopen(outputfile,"wb+");
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
            	samepixels++;
                if(samepixels==62){
		    //If maximum value for an EVA_BLK_SAME block is reached, we write that block and samepixels starts over.
                    write1blocks(new,EVA_BLK_SAME+samepixels-1);
                    samepixels=0;
                    output=1;
                }
                else{
		    //Else it is just incremented and current pixel was taken care of, hence output being 1.
                    
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
	
    if(samepixels != 0){
    	write1blocks(new,EVA_BLK_SAME+samepixels-1);
    }
	
    //Getting new file's size and closing streams.
    compressedsize=size(new);
    ppmClose(old);
    fclose(new);
	 
    //Getting the second time value, since compression just ended and files were closed.
    end=timer();
	
    //Output of time taken in seconds and compression ratio in percent.
    printf("\nCompressing file took %f seconds",(end-start)/1000000.0);
    printf("\nCompression ratio : compressed is %f%% the size of the original image.\n",(compressedsize/(uncompressedsize/1.0))*100);
}

void nextPixel(long* x, long* y, unsigned int width){
	(*x)++;
	(*y)+=(*x)/width;
	(*x)=(*x)%width;
}

void decompression(char* filename, int endianness, char* outputfile){
	unsigned int cache[64] = {0};
	unsigned int previouspixel = 0, currentpixel = 0;
	unsigned long start = 0, end = 0;
	int cacheIndex = 0, nbPixelSuite = 0;
	
	int width = 0, height = 0, range = 0, color = 0;
	
	unsigned char bloc = 0, redP = 0, greenP = 0, blueP = 0;
	
	unsigned char diffG = 0, diffR = 0, diffB = 0;
	
	long x = 0, y = 0;
	
	FILE* compress = fopen(filename, "rb");
	if(compress == NULL){
		printf("\n There was a problem while opening the file.\n");
		exit(-1);
	}
	
	start = timer();
	
	//The first 16 byte are for the width, height, range and color, we need to get them
	read4blocks(compress, &width, endianness);
	read4blocks(compress, &height, endianness);
	read4blocks(compress, &range, endianness);
	read4blocks(compress, &color, endianness);        
	printf("Width : %d, Height : %d, Range : %d, Color : %d\n", width, height, range, color);
	
	PPM_IMG* uncompressed = ppmNew(width, height, range, color);
	
	while(y != height){
		read1blocks(compress, &bloc);
		
		//Chec if we have a EVA_BLK_RGB_READER bloc
		if(bloc == EVA_BLK_RGB_READER){
		
			read1blocks(compress, &redP);
			read1blocks(compress, &greenP);
			read1blocks(compress, &blueP);
			
			currentpixel = pixel(redP, greenP, blueP);
			ppmWrite(uncompressed, x, y, currentpixel);
			nextPixel(&x, &y, width);
			
		//This condition means if the 2 strong bite are on 1, we have a EVA_BLK_SAME bloc
		}else if((bloc >> 6) == 3){
		
			nbPixelSuite = (bloc-EVA_BLK_SAME)+1;
			
			//Add the number of pixel give by the bloc
			for(int i = 0; i < nbPixelSuite; i++){
				ppmWrite(uncompressed, x, y, currentpixel);
				nextPixel(&x, &y, width);
				if(nbPixelSuite-1 != i){
					cacheIndex = (cacheIndex+1)%64;
					cache[cacheIndex] = currentpixel;
				}
			}
			
			nbPixelSuite = 0;
		
		//This condition means if the 2 strong bite are on 0, we have a EVA_BLK_INDEX bloc
		}else if((bloc >> 6) == 0){
			
			//We need to check in teh cache the color
			currentpixel = cache[bloc];
			ppmWrite(uncompressed, x, y, currentpixel);
			nextPixel(&x, &y, width);
			
		//This condition means if the byte look like 01xxxxxx, we have a EVA_BLK_DIFF bloc
		}else if((bloc >> 6) == 1){
			
			//We need to calculate the color from the difference give by the bloc
			redP = red(previouspixel) -2 + ((bloc&0x30)>>4);
			
			greenP = (green(previouspixel)-2) + ((bloc&0x0C)>>2);
			
			blueP = (blue(previouspixel)-2) + (bloc&0x03);
			
			currentpixel = pixel(redP, greenP, blueP);
			ppmWrite(uncompressed, x, y, currentpixel);
			nextPixel(&x, &y, width);
		
		//This condition means if the byte look like 10xxxxxx, we have a EVA_BLK_LUMA bloc
		}else if((bloc >> 6) == 2){
		
			//We need to put the 8th bit on 0
			bloc -= 128; //(64 = 01000000)
			
			diffG = bloc-32;
			
			//We need to rad the next bloc to get diffR and diffB
			read1blocks(compress, &bloc);
			diffR = (bloc >> 4) + diffG - 8;
			diffB = (bloc & 0xF) + diffG - 8;
			
			//We need to calculate the color from the difference give by the bloc
			redP = diffR + red(previouspixel);
			greenP = diffG + green(previouspixel);
			blueP = diffB + blue(previouspixel);
			
			currentpixel = pixel(redP, greenP, blueP);
			ppmWrite(uncompressed, x, y, currentpixel);
			nextPixel(&x, &y, width);
			
		}else{
			printf("[Error] An error occur while reading the compressed file : unknow bloc.\n End of program\n");
			exit(-1);
		
		}
		
		//We put the current pixel in the cache
		cacheIndex = (cacheIndex+1)%64;
		cache[cacheIndex] = currentpixel;
		previouspixel = currentpixel;
	}
	
	ppmSave(uncompressed, outputfile);
	int compressedsize = ftell(compress);
	fclose(compress);
	
	FILE* temp = fopen(outputfile, "rb");
	if(temp == NULL){
		printf("An error occure. End of program\n");
		exit(-1);
	}
	int uncompressedsize = size(temp);
	
	fclose(temp);
	
	end = timer();
	printf("\nCompressing file took %f seconds",(end-start)/1000000.0);
	printf("\nCompression ratio : uncompressed is %f%% the size of the compressed image.\n",(uncompressedsize/(compressedsize/1.0))*100);
	
}
