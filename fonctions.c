#include "ppm_lib.c"
#define EVA_BLK_SAME 192
#define EVA_BLK_INDEX 0
#define EVA_BLK_DIFF 64
#define EVA_BLK_LUMA 32768
#define EVA_BLK_RGB 4261412864


// #  &  { }  ~  [] \  |

void write(FILE* file,int val){                //Writes an int value inside a binary file. Takes the file structure and the written value as parameters.
	fwrite(&val,sizeof(int),1,file);
}

void compression(char* filename){			//Takes the name of a ppm file as a parameter, creates a new compressed binary file named "compressed/name of file/" using the EVA algorithm method.
    char* name=sprintf("compressed%s",filename);
    int EVA_BLK_DEBUG[10];
    int cache[64];
    int previouspixel,currentpixel,output;
    int i,j,width,height,range,colors;
    int samepixels=0,index,diffr,diffb,diffg,diffrg,diffbg;
    for(i=0;i<64;i++){
          cache[i]=0;
    }
    PPM_IMG* old=ppmOpen("Image.ppm");
    if(!old){
        printf("\n There was a problem.");
    }
    width=ppmGetWidth(old);
    height=ppmGetHeight(old);
    range=ppmGetRange(old);
    colors=ppmGetColors(old);
    FILE* new=fopen(name,"wb+");
    if(!new){
        printf("\n There was a problem.");
    }

    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            output=0;
            currentpixel=ppmRead(old,i,j);
            //Step 1
            if(currentpixel==previouspixel){
                if(EVA_BLK_SAME==192+63){
                    write(new,EVA_BLK_SAME-1);
                    samepixels=0;
                    output=1;
                }
                else{
                    samepixels++;
		    output=1;
                }
            }
            else if(previouspixel!=currentpixel && EVA_BLK_SAME>192){
                write(new,EVA_BLK_SAME-1);
                samepixels=0;
                output=1;
            }
            //Step 2
            else if(output==0){
                 index=(3*red(currentpixel)+5*green(currentpixel)+7*blue(currentpixel))%64;
                 if(currentpixel==cache[index]){
                    write(new,EVA_BLK_INDEX+index);
                    output=1;
                }
            }
            //Step 3
            else if(output==0){
                diffr=red(currentpixel)-red(previouspixel);
                diffg=green(currentpixel)-green(previouspixel);
                diffb=blue(currentpixel)-blue(previouspixel);
                if(diffr>=-2 && diffr<=1 && diffg>=-2 && diffg<=1 && diffb>=-2 && diffb<=1){
                    write(new,EVA_BLK_DIFF+diffb+2+4*(diffg+2)+16*(diffr+2));
                    output=1;
                }
            }
            //Step 4
            else if(output==0){
                if(diffg>=-32 && diffg<=31){
                    diffrg=diffr-diffg;
                    diffbg=diffb-diffg;
                    if(diffrg>=-8 && diffrg<=7 && diffbg>=-8 && diffbg<=7){
                        write(new,EVA_BLK_LUMA+diffbg+8+16*(diffrg+8)+256*(diffg+32));
                        output=1;
                    }
                }
            }
            //Step 5
            else if(output==0){
                write(new,EVA_BLK_RGB+blue(currentpixel)+256*green(currentpixel)+65536*red(currentpixel));
                output=1;
            }
            //Step 6
            //Step 7
            cache[j%64]=currentpixel;
            previouspixel=currentpixel;
        }
    }
    ppmClose(old);
    fclose(new);
}
