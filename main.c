#include"header.h"
#include <string.h>

#define MODE_CHOICE_TEXT "\nPlease choose an operating mode\n1) Compression\n2) Decompression\nPossibles answer are 1, 2 : "

int getChar(int minVal, int maxVal, char* message){
	int readChar = 0; //Contain a char when fgetc is use
	int tmp = 0;      //A temp var, use for store data a short amount of time
	char check = 0;    //Use to make a check in a if or a while
	
	do{
		printf("%s", message);
		
		readChar = fgetc(stdin);
		
		//If the input is juste \n, we need to put a default value to tmp
		if(readChar!='\n'){
			tmp = fgetc(stdin);
		}else{
			tmp = '\n';
		}
		
		//Check if the answer has an error (not valide char or to many char)
		if( !(minVal <= readChar && readChar <= maxVal && (tmp == '\n' || tmp == EOF)) ){
			printf("\n/!\\Please enter a valid answer/!\\\n\n\n");
			check = 0;
			
			//We need to clear the buffer of fgetc if we have other char
			while(tmp != '\n' && tmp != EOF){
				tmp=fgetc(stdin);
			}
		}else{
			check = 1;
		}
		
	}while(check == 0);
	
	return readChar;
}


/*TODO : - ajouter a DEFAULT_PPM_FOLDER le chemin jusqu'a l'executable, autrement le dossier se situra au l'endroit d'ou est executer la fonction
	 - Regarder pour le warning a la comilation (possiblement includ mis en commentaire plus haut dans gestiondedossier.c)
	 - changer les autorisations pas défaut du dossier créer
*/
int main(int argc, char** argv){

	//Put the program relative path in a var
	int progNameSize = 0;
	int len = strlen(argv[0])	;
	while(argv[0][len-progNameSize] != '/' && argv[0][len-progNameSize] != '\\'){
		progNameSize++;
	}
	progNameSize--;
	
	int start = 0;
	if(argv[0][0] == '.' && argv[0][1] == '/'){
		start = 2;
	}
	
	CharTab path;
	path.size = len-progNameSize-start;
	path.tab = malloc(sizeof(char) * path.size);
	if(path.tab == NULL){
		printf("Can't get the program path, memory error.");
		exit(0);
	}
	
	for(int i = start; i < path.size+start; i++){
		path.tab[i-start] = argv[0][i];
	}
	
	//Put the default ppm file relative path in a var
	CharTab ppmFolderPath;
	ppmFolderPath.size = path.size+strlen(DEFAULT_PPM_FOLDER_NAME);
	ppmFolderPath.tab = malloc(sizeof(char) * path.size);
	if(ppmFolderPath.tab == NULL){
		printf("Can't get the default folder path, memory error.");
		exit(0);
	}
	strcat(ppmFolderPath.tab, path.tab);
	strcat(ppmFolderPath.tab, DEFAULT_PPM_FOLDER_NAME);
	
	printf("EVA algorithm configuration interface\n");
	
	char input = getChar('1', '2', MODE_CHOICE_TEXT); //Getting the user input
	
	//Compression mode
	if(input ==  '1'){
		if(folderExist(ppmFolderPath.tab) == 0){
			printf("Folder %s do no existe creating it\n", ppmFolderPath.tab);
			createFolder(ppmFolderPath.tab);
		}
		
	}else{
	
	}
	
	free(path.tab);
	return 0;
}
