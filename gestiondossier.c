#include <dirent.h>    //We use the dirent library to manage the directory the user was told to create
#include <stdio.h>
#include <string.h>

void main(){
    char* lastperiod=NULL;
    int antiperiods=0;
    DIR* d;
    struct dirent *dir;
    d=opendir("ppm");   //Opening the ppm directory, where the user placed the files they want to modify.
    if(d){              //This if statement makes sure it exists. 
        while((dir=readdir(d))!=NULL){
            if(antiperiods!=2){
                antiperiods++;     //We use this if statement to skip the two hidden entries . and .., which are in the ppm file no matter what.
                continue;
            }
            lastperiod=strrchr(dir->d_name,'.');   //For every file in our directory, the lastperiod string becomes the extension of the file. We use strrchr to get the last occurence of . in the file's name.
            if(lastperiod==NULL){                  //The pointer being NULL usually happens when the user created a directory inside ppm
                printf("\nFile %s could not be treated. Please make sure it is a ppm file.", dir->d_name);
                continue;
            }
            else if(strcmp(lastperiod,".ppm")!=0){    //We get to this if statement if the user placed other types of files in ppm
                printf("\nFile %s could not be treated. Please make sure it is a ppm file.", dir->d_name);
            }
            else{
                printf("\n%s",dir->d_name);      //Treatement of the ppm files
            }
        }
        closedir(d);
    }
}
