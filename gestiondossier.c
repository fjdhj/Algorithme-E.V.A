#include "header.h"
#include <dirent.h>    //We use the dirent library to manage the directory the user was told to create
#include <errno.h>

#ifdef __linux__
	#include<sys/stat.h>
#endif

char folderExist(const char* path){
	DIR* dir = opendir(path);
	if(dir){
		closedir(dir);
		return 1; //The folder exist
	}else if(ENOENT == errno){
		return 0; //The folder do not exist
	}else{
		perror("[ERROR] An unknown error occured. Can't access to the given folder.\n");
		exit(0);
	}
}

int folderChildNumber(const char* path){
	if(folderExist(path) == 0){
		printf("[ERROR] Can't read the content of the given folder : the directory do not exist.\n");
		exit(0);
	}
	
	DIR* dir = opendir(path);
	if(dir == NULL){
		perror("[ERROR] An unknown error occured. Can't access to the given folder.\n");
		exit(0);
	}
	
	int count = 0;
	struct dirent* content = NULL;
	
	while((content = readdir(dir)) != NULL){
		count++;
		#ifdef __linux__
		if(content->d_name[0] == '.' && (strlen(content->d_name) == 1 || (strlen(content->d_name) == 2 && content->d_name[1] == '.') )){
			count--;
		}
		#endif
		
	}
	
	closedir(dir);
	return count;
}

int folderChildName(const char* path, char** elementNameTab, int tabSize){
	if(folderExist(path) == 0){
		printf("[ERROR] Can't read the content of the given folder : the directory do not exist.\n");
		exit(0);
	}
	
	if(folderChildNumber(path) == 0){
		printf("[ERROR] Can't get the list of child : the directory is empty.\n");
		exit(0);
	}
	
	DIR* dir = opendir(path);
	if(dir == NULL){
		perror("[ERROR] An unknown error occured. Can't access to the given folder.\n");
		exit(0);
	}
	
	int count = 0;
	struct dirent* content = NULL;
	
	while((content = readdir(dir)) != NULL && count < tabSize){
		#ifdef __linux__
		if(  !(content->d_name[0] == '.' && (strlen(content->d_name) == 1 || ( strlen(content->d_name) == 2 && content->d_name[1] == '.' ) ))  ){
			elementNameTab[count] = content->d_name;
			count++;
		}
		#else
			elementNameTab[count] = content->d_name;
			count++;
		#endif
		
	}
	
	closedir(dir);
	
	if(content != NULL){
		return -1;
	}else{
		return tabSize-count;
	}
}

void createFolder(const char* path){
	#ifdef __linux__
		mkdir(path, 0754); 
	#else
		_mkdir(path);
	#endif
}

/*void main(){
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
}*/
