#include <stdio.h>
#include <stdlib.h>
#include "ppm_lib.c"

int ppmfinaltest1(PPM_IMG* original,PPM_IMG* modified){
    //Returns 1 if images are the same, else 0.
    int widthO,heightO,rangeO,colorsO;
    int widthM,heightM,rangeM,colorsM;
    unsigned long size,differentpixels;
    int i,j;
    if(!original){
        printf("\nAccessing original image was not possible.");
        return 0;
    }
    if(!modified){
        printf("\nAccessing modified image was not possible.");
        return 0;
    }
    widthO=ppmGetWidth(original);
    heightO=ppmGetHeight(original);
    rangeO=ppmGetRange(original);
    colorsO=ppmGetColors(original);
    widthM=ppmGetWidth(modified);
    heightM=ppmGetHeight(modified);
    rangeM=ppmGetRange(modified);
    colorsM=ppmGetColors(modified);
    if(heightO!=heightM || widthM!=widthO){
        printf("\nDimensions are not the same.");
        return 0;
    }
    if(rangeO!=rangeM){
        printf("\nRange is not the same.");
        return 0;
    }
    if(colorsO!=colorsM){
        printf("\nColors are not the same.");
        return 0;
    }
    size=(widthO*heightO);
    differentpixels=0;
    for(i=0;i<heightO;i++){
        for(j=0;j<widthO;j++){
            if(ppmRead(original,j,i)!=ppmRead(modified,j,i)){
                differentpixels++;
            }
        }
    }
    if(differentpixels==0){
        printf("\nImages are the same !");
        return 1;
    }
    else{
        printf("\nImages are not the same");
        printf("\nPercentage of different pixels : %f %%",((differentpixels/1.0)/size)*100);
        return 0;
    }

}


int ppmfinaltest2(PPM_IMG* original,PPM_IMG* modified){
    //Returns 1 if images are the same, else 0.
    //Shall be used when it is known that different pixels are not many
    int widthO,heightO,rangeO,colorsO;
    int widthM,heightM,rangeM,colorsM;
    unsigned long size,differentpixels;
    int i,j;
    if(!original){
        printf("\nAccessing original image was not possible.");
        return 0;
    }
    if(!modified){
        printf("\nAccessing modified image was not possible.");
        return 0;
    }
    widthO=ppmGetWidth(original);
    heightO=ppmGetHeight(original);
    rangeO=ppmGetRange(original);
    colorsO=ppmGetColors(original);
    widthM=ppmGetWidth(modified);
    heightM=ppmGetHeight(modified);
    rangeM=ppmGetRange(modified);
    colorsM=ppmGetColors(modified);
    if(heightO!=heightM || widthM!=widthO){
        printf("\nDimensions are not the same.");
        return 0;
    }
    if(rangeO!=rangeM){
        printf("\nRange is not the same.");
        return 0;
    }
    if(colorsO!=colorsM){
        printf("\nColors are not the same.");
        return 0;
    }
    size=(widthO*heightO);
    differentpixels=0;
    for(i=0;i<heightO;i++){
        for(j=0;j<widthO;j++){
            if(ppmRead(original,j,i)!=ppmRead(modified,j,i)){
                differentpixels++;
                printf("\n x : %d  y : %d are different.",j,i);
            }
        }
    }
    if(differentpixels==0){
        printf("\nImages are the same !");
        return 1;
    }
    else{
        printf("\nImages are not the same");
        printf("\nPercentage of different pixels : %f %%",((differentpixels/1.0)/size)*100);
        return 0;
    }
}
