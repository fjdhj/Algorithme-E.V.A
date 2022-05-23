#include "header.h"
#include "ppm_lib.c"

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
           printf("\nThere was a problem during compression.");
           exit(-1);
	}
	
	if(endianness==0){ 
		*val = reverseint(*val);						
	}
}

void write2blocks(FILE* file,unsigned short val,int endianness){            
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
			printf("\nThere was a problem during compression.");
			exit(-1);
	}
	
	if(endianness==0){ 
		*val = reverseshort(*val);
	}
							
}

void write1blocks(FILE* file,char val){               
    if(fwrite(&val,1,1,file)!=1){
        printf("\nThere was a problem during compression.");
        exit(-1);
    }
}


void read1blocks(FILE* file,char*	 val){               
	if(fread(val, 1, 1, file) != 1){
		printf("\nThere was a problem during compression.");
		exit(-1);
	}
}

void compression(char* filename,int endianness, char* outputfile){			
    char* name;
    int cache[64],cacheindex=0;
    int previouspixel,currentpixel,output;
    int i,j,width,height,range,colors;
    int samepixels,index,diffr,diffb,diffg,diffrg,diffbg;
    for(i=0;i<64;i++){
          cache[i]=0;
    }

    PPM_IMG* old = ppmOpen(filename);             
    if(!old){
        printf("\n There was a problem while opening the file.\n");
        exit(-1);
    }
    width=ppmGetWidth(old);
    height=ppmGetHeight(old);
    range=ppmGetRange(old);
    colors=ppmGetColors(old);
    
    printf("Width dec : %d, Width hexa : %X\n, Sizeof(int) : %d", width, width, sizeof(int));

    FILE* new=fopen(outputfile, "wb+");
    if(!new){
        printf("\n There was a problem.\n");
        exit(-1);
    }
    previouspixel=-1;
    samepixels=0;

    write4blocks(new,width,endianness);
    write4blocks(new,height,endianness);
    write4blocks(new,range,endianness);
    write4blocks(new,colors,endianness);

    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            output=0;
            currentpixel=ppmRead(old,j,i);
            //Step 1
            if(currentpixel==previouspixel){
                if(samepixels==63){
                    write1blocks(new,EVA_BLK_SAME+samepixels-1);
                    samepixels=0;
                    output=1;
                }
                else{
                    samepixels++;
		    output=1;
                }
            }
            if(previouspixel!=currentpixel && samepixels>0){
                write1blocks(new,EVA_BLK_SAME+samepixels-1);
                samepixels=0;
                output=0;
            }
            //Step 2
            if(output==0){
                 index=(3*red(currentpixel)+5*green(currentpixel)+7*blue(currentpixel))%64;
                 if(currentpixel==cache[index]){
                    write1blocks(new,EVA_BLK_INDEX+index);
                    output=1;
                }
            }
            //Step 3
            if(output==0){
                diffr=red(currentpixel)-red(previouspixel);
                diffg=green(currentpixel)-green(previouspixel);
                diffb=blue(currentpixel)-blue(previouspixel);
                if(((diffr>=-2 && diffr<=1) && diffg>=-2) && ((diffg<=1 && diffb>=-2) && diffb<=1)){
                    write1blocks(new,EVA_BLK_DIFF+diffb+2+4*(diffg+2)+16*(diffr+2));
                    output=1;
                }
            }
            //Step 4
            if(output==0){
                if(diffg>=-32 && diffg<=31){
                    diffrg=diffr-diffg;
                    diffbg=diffb-diffg;
                    if((diffrg>=-8 && diffrg<=7) && (diffbg>=-8 && diffbg<=7)){
                        write2blocks(new,EVA_BLK_LUMA+diffbg+8+16*(diffrg+8)+256*(diffg+32),endianness);
                        output=1;
                    }
                }
            }
            //Step 5
            if(output==0){
                write4blocks(new,EVA_BLK_RGB+blue(currentpixel)+256*green(currentpixel)+65536*red(currentpixel),endianness);
                output=1;
            }
            //Step 6
            //Step 7
            cacheindex++;
            cache[cacheindex%64]=currentpixel;
            previouspixel=currentpixel;
        }
    }
    ppmClose(old);
    fclose(new);
}

void decompression(char* filename, int endianness, char* outputfile){
	int cache[64] = {0};
	int previouspixel = 0;
	
	int width = 0, height = 0, range = 0, color = 0;
	
	FILE* compress = fopen(filename, "rb");
	if(compress == NULL){
		printf("\n There was a problem while opening the file.\n");
		exit(-1);
	}
	
	read4blocks(compress, &width, endianness);
	printf("PUTE : %x\n", width);
	read4blocks(compress, &height, endianness);
	read4blocks(compress, &range, endianness);
	read4blocks(compress, &color, endianness);        
	
	printf("Width : %d, Height : %d, Range : %d, Color : %d\n", width, height, range, color);
	
	PPM_IMG* old = ppmOpen(filename);             
	
}
