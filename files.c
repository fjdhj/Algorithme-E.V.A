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

int folderChildName(const char* path, CharList** elementNameList, int tabSize){
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
			elementNameList[count] = convertTabToCharList(content->d_name, strlen(content->d_name));
			count++;
		}
		#else
			elementNameTab[count] = content->d_name;
			count++;
		#endif
		
		
	}
	
	#ifdef __linux__
		//On linux file system . and .. can be at the end or the start in c
		while(  content != NULL && (content->d_name[0] == '.' && (strlen(content->d_name) == 1 || ( strlen(content->d_name) == 2 && content->d_name[1] == '.' ) ))){
			content = readdir(dir);
		}
	#endif
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
