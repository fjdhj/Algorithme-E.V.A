/*!
 * \file ppm_lib.c
 */

//======================================================
// INCLUDES
//======================================================
#include "ppm_lib.h"

PPM_IMG* ppmNew(int w, int h, int rng, int nbColors){
    PPM_IMG* img;
    // check if the range is below 256
    if(rng<1 || rng>255){
        printf("[ERROR] in newImage : range is not between 1 and 255!\n");
        exit(-1);
    }
    // check if the structure is not 0
    img = malloc(sizeof(PPM_IMG));
    if(img == 0){
        printf("[ERROR] in newImage : null structure (bad malloc) !\n");
        exit(-1);
    }  
    // check if the number of colors is either 1 or 3
    if(nbColors != 1 && nbColors != 3){
        printf("[ERROR] in newImage : the number of colors is not correct (%d) !\n", nbColors);
        exit(-1);
    }
    // allocate memory
    int size = (w*h*nbColors)+HEADER_SIZE;
    char* pBuff = malloc( size );
    if( pBuff == 0 ){
        printf("[ERROR] in newImage : buffer has not be allocated !\n");
        exit(-1);
    }
    // init structure
    img->W        = w;
    img->H        = h;
    img->range    = rng;
    img->nbColors = nbColors;
    img->hdrSize  = 0;
    img->dataSize = 0;
    // store buffer address
    img->pHeader  = pBuff;
    // init buffer with black
    memset(pBuff, 0, size);
    // create header (
    int headerType = img->nbColors == 3 ? 6: 5;
    sprintf(img->pHeader, "P%d\n%d %d\n%d\n",headerType,img->W,img->H,img->range);
    // set the number of characters in the header
    img->hdrSize = strlen(img->pHeader);
    // Set the data buffer address and size
    img->pData = (img->pHeader)+(img->hdrSize);
    img->dataSize = img->W * img->H * img->nbColors;
    // display info
    //printf("Created an image structure : \n");
    //ppmDisplay(img);
    return img;
}

PPM_IMG* ppmOpen(char* path){
    // local variables
    char  locBuf[255];    
    FILE* pFile;
    PPM_IMG* img;
    int W        = 0;
    int H        = 0;
    int range    = 0;
    int nbColors = 0;
    // check if the path is not null
    if(path == 0){
        printf("[ERROR] in openImage : the file path is null ! \n");
        exit(-1);
    }
    // open file in reading mode
    pFile = fopen(path,"rb");
    if(pFile == 0){
        printf("[ERROR] in openImage : the requested file has not been found (%s) !\n", path);
        exit(-1);
    }
    else{
        //printf("Successfully opened the file '%s'.\n", path);    
    }
    // Read the first line from the file and compare to the type
    do{
        fgets(locBuf, 250, pFile);
    } while (locBuf[0] == '#');
    // If the file is PPM_IMG P5 or P6 format
    if( strcmp(locBuf,"P5\n") == 0 ){
        nbColors = 1;
    }
    else if( strcmp(locBuf,"P6\n") == 0 ){
        nbColors = 3;
    }
    // check if the number of colors is correct
    if( nbColors == 1 || nbColors == 3 ) {
        // Read the second line containnig at least dimensions
        do{
            fgets(locBuf, 250, pFile);
        } while (locBuf[0] == '#');
        // Get dimensions + range
        sscanf(locBuf, "%d %d %d\n", &W, &H, &range);
        // check if we retrieved them
        if(range == 0){
            // try to get range only
            do{
                fgets(locBuf, 250, pFile);
                // printf("%s\n",locBuf);
            } while (locBuf[0] == '#');
            sscanf(locBuf, "%d\n", &range);
        }
        // check we have everything to create a new image
        if(W>0 && W<=5000 && H>0 && H<=5000 && range>0 && range<=255){
                // Create new PPM_IMG header from the file
                img = ppmNew( W, H, range, nbColors);
                // Read the following data and fill the data buffer
                fread( img->pData, img->dataSize, 1, pFile );
                // Close the input file
                fclose(pFile);
        }
        else{
            printf("[ERROR] the file seems to be corrupted : impossible to retrieve dimensions and/or dynamic range!\n");
            exit(-1);
        }
    }
    else{
        printf("[ERROR] the file is neither a GRAY PPM_IMG (P5) nor a COLOR PPM_IMG (P6) file !\n");
        exit(-1);
    }
    return img;
}

int ppmRead(PPM_IMG* img, int x, int y){
    // check if the structure is not 0
    if(img == 0){
        printf("[ERROR] in getPixel : null structure !\n");
        exit(-1);
    }    
    // check if the x and y are in the scope of the picture
    if(x<0 || x >= img->W || y<0 || y >= img->H){
        printf("[ERROR] in getPixel : position does not fit requirements (x=%d / y=%d / W=%d / H=%d)!\n",x,y,img->W, img->H);
        exit(-1);
    }    
    // Get index of the current pixel
    int index = (y*img->W + x)*img->nbColors;
    // check the index is in the buffer size
    if(index >= img->dataSize){
        printf("[ERROR] in getPixel : the index exceeds the data buffer size (index=%d / dataSize=%d)\n", index, img->dataSize);
        exit(-1);
    }
    unsigned char* pDat = img->pData;
    // Get byte value (either Gray or Red component)
    unsigned char r = pDat[index];
    unsigned char g = 0;
    unsigned char b = 0;
    int value = r & 0xFF;
    // update value if color mode
    if(img->nbColors == 3){
        g = pDat[index+1];
        b = pDat[index+2];
        value = pixel(r,g,b);
    }
    // return pixel value (either 1 or 3 bytes)
    return value;
}

void ppmWrite(PPM_IMG* img, int x, int y, int value){
    int rng1 = img->range + 1;
    int rng3 = rng1 * rng1 * rng1; 
    // check if the structure is not 0
    if(img == 0){
        printf("[ERROR] in setPixel : null structure !\n");
        exit(-1);
    }    
    // check if the x and y are in the scope of the picture
    if(x<0 || x >= img->W || y<0 || y >= img->H){
        printf("[ERROR] in getPixel : position does not fit requirements (x=%d / y=%d / w=%d / h=%d) !\n", x, y, img->W, img->H);
        exit(-1);
    }
    // Check the value is in the range
    if(   (value < 0)
       || ( (value>=rng1) && (img->nbColors == 1))
       || ( (value>=rng3) && (img->nbColors == 3)) 
       ){
        printf("[ERROR] in setPixel : value is out of range (x=%d / y=%d / value=%d(0x%x) / range=%d / nbColors=%d) ! \n", x, y, value, value, img->range, img->nbColors);
        exit(-1);
    }
    // Get pixel index
    int index = (y*img->W + x)*img->nbColors;
    unsigned char* pDat = img->pData;
    // Get each component
    unsigned char r = red(value);
    unsigned char g = green(value);
    unsigned char b = blue(value);
    // Write in color mode
    if(img->nbColors == 3){
        pDat[index] = r;
        pDat[index+1] = g;
        pDat[index+2] = b;
    }
    else if (img->nbColors ==1){
        // or write in gray mode
        pDat[index] = b;
    }
}

void ppmDisplay(PPM_IMG* img){
    printf("----- Image information -----\n");
    printf("Width       = %d \n", ppmGetWidth (img) );
    printf("Height      = %d \n", ppmGetHeight(img) );
    printf("Nb Colors   = %d \n", ppmGetColors(img) );
    printf("Range       = %d \n", ppmGetRange (img) );
    printf("Header size = %d \n", img->hdrSize      );
    printf("Data size   = %d \n", img->dataSize     );
    printf("\n\n");
}

void ppmSave(PPM_IMG* img, char* path){
    FILE* pFile;
    // check img is not null
    if(img == NULL){
       printf("[ERROR] in ppmSave : null image structure pointer !\n");
        exit(-1);
    }
    // open file in writing mode
    pFile = fopen(path,"wb");
    if(pFile == 0){
        printf("[ERROR] in saveImage : the requested file has not been opened in write mode (%s) !\n", path);
        exit(-1);
    }
    // write header
    fwrite(img->pHeader, img->hdrSize,  1, pFile);
    // write data
    fwrite(img->pData,   img->dataSize, 1, pFile);
    // save file
    fclose(pFile);
    // display successfull message
    //printf("The image '%s' has been successfully saved. \n", path);
    //ppmDisplay(img);
}

void ppmClose(PPM_IMG* img){
    if(img == NULL){
       printf("[ERROR] in ppmClose : null image structure pointer !\n");
        exit(-1);
    }
    // Free buffer allocation
    free(img->pHeader);
    // Set the whole structure to zero
    memset( img, 0x00, sizeof(PPM_IMG) );    
    // free image structure
    free(img);
}

int ppmGetWidth (PPM_IMG* img){
    if(img == NULL){
       printf("[ERROR] in ppmGetWidth : null image structure pointer !\n");
        exit(-1);
    }
    return img->W;    
}

int ppmGetHeight(PPM_IMG* img){
    if(img == NULL){
       printf("[ERROR] in ppmGetHeight : null image structure pointer !\n");
        exit(-1);
    }
    return img->H;    
}

int ppmGetColors(PPM_IMG* img){
    if(img == NULL){
       printf("[ERROR] in ppmGetColors : null image structure pointer !\n");
        exit(-1);
    }
    return img->nbColors;    
}

int ppmGetRange (PPM_IMG* img){
    if(img == NULL){
       printf("[ERROR] in ppmGetRange : null image structure pointer !\n");
        exit(-1);
    }
    return img->range;    
}

