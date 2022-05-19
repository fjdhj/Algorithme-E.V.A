#include"header.h"

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
			if(tmp != '\n' && tmp != EOF){
				clearStdinBuffer();
			}
		}else{
			check = 1;
		}
		
	}while(check == 0);
	
	return readChar;
}

void clearStdinBuffer(){
	int tmp = fgetc(stdin);
	while(tmp != '\n' && tmp != EOF){
		tmp=fgetc(stdin);
	}
}

/* TODO : vérifier si les élémen de PPM sont des fichiers ou des dossiers
 * 		demander a l'utilisateur le nom du fichier de sorti
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
		printf("\nCan't get the program path, memory error.\n");
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
		printf("\nCan't get the default folder path, memory error.\n");
		exit(0);
	}
	strcat(ppmFolderPath.tab, path.tab);
	strcat(ppmFolderPath.tab, DEFAULT_PPM_FOLDER_NAME);
	
	
	
	//Start intecteraction with user
	printf("EVA algorithm configuration interface\n");
	
	char input = getChar('1', '2', MODE_CHOICE_TEXT); //Getting the user input compression on decompression
	
	//Compression mode
	if(input ==  '1'){
		if(folderExist(ppmFolderPath.tab) == 0){
			printf("Folder %s do no existe creating it\n", ppmFolderPath.tab);
			createFolder(ppmFolderPath.tab);
		}
		
		printf("You can add ppm file in the %s directory\nWhen you are ready, press return ", ppmFolderPath.tab);
		clearStdinBuffer(); //Clear the stdin buffer, but if buffer is already empty, wait for an input
		
		//Getting the number of element inside the directory
		int ppmFileNb = folderChildNumber(ppmFolderPath.tab);
		printf("There is %d element in %s\n", ppmFileNb, ppmFolderPath.tab);
		
		if(ppmFileNb == 0){
			printf("The folder %s is empty, please add file.\nEnd of programme.\n", ppmFolderPath.tab);
			free(path.tab);
			free(ppmFolderPath.tab);
			exit(0);
		}
		
		//Getting the list of file name in the directory
		char** elementNameTab = malloc(sizeof(char*)*ppmFileNb);
		if(elementNameTab == NULL){
			printf("\nCan't get the file list, memory error.\n");
			free(path.tab);
			free(ppmFolderPath.tab);
			exit(0);
		}
		
		int result = folderChildName(ppmFolderPath.tab, elementNameTab, ppmFileNb);
		if(result == -1){
			printf("[ERROR] Can't get all the child in %s because the tab is to tiny.\nEnd of program.\n", ppmFolderPath.tab);
			free(path.tab);
			free(ppmFolderPath.tab);
			free(elementNameTab);
			exit(0);
		}else if(result != 0){
			printf("[DEBUG] I don't know why but the tab is to big, there are %d empty cels. But we don't care because the programe work well !\n", result);
			elementNameTab -= result;
		}
		
		int endianness = getEndianness();
		char* inFile = NULL, outFile = NULL;
		for(int i = 0; i < ppmFileNb; i++){
			/*char buff[500] = {0};
			strcat(buff, ppmFolderPath.tab);
			strcat(buff, "/");
			strcat(buff, elementNameTab[i]);*/
			
			
			compression(buff, endianness, NULL);
		}
		
		
		
		free(elementNameTab);
	}else{
	
	}
	
	free(path.tab);
	free(ppmFolderPath.tab);
	return 0;
}
