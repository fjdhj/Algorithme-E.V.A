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

char* getFileName(CharList* path, char* message){
	if(path == NULL){
		path = createCharList();
	}
	
	int elementToRemove = 0;
	//Check if we need to add a file delimiter at the end
	if(getElementCharList(-1, path) != DELIMITER && getSize(path) > 1 && getElementCharList(0, path) != 0){
		appendCharList(DELIMITER, path);
		elementToRemove++;
	}
	
	printf("\n%s", message);
	
	int buff = 0;
	while( (buff = fgetc(stdin)) != EOF && buff != '\n' ){
		appendCharList((char)buff, path);
		elementToRemove++;
	}
	
	char* copy = convertCharListToTab(path);
	
	for(int i = 0; i < elementToRemove; i++){
		removeLastCharList(path);
	}
	
	return copy;
}

void clearStdinBuffer(){
	int tmp = fgetc(stdin);
	while(tmp != '\n' && tmp != EOF){
		tmp=fgetc(stdin);
	}
}

CharTab getExecutableLocation(char* arg0){
	//Get the lenght of the program name
	int progNameSize = 0;
	int len = strlen(arg0);
	while(arg0[len-progNameSize] != '/' && arg0[len-progNameSize] != '\\'){
		progNameSize++;
	}
	progNameSize--;
	
	//If we have the ./ of the execution, we need to rmeove it
	int start = 0;
	if(arg0[0] == '.' && arg0[1] == '/'){
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
		path.tab[i-start] = arg0[i];
	}
	return path;
}

/* TODO : vérifier si les élémen de PPM sont des fichiers ou des dossiers
 * 		demander a l'utilisateur le nom du fichier de sorti
*/
int main(int argc, char** argv){

	//Put the program relative path in a var
	CharTab relativeExecutablePath = getExecutableLocation(argv[0]);
	
	//Put the default ppm file relative path in a var
	CharTab ppmFolderPath;
	ppmFolderPath.size = relativeExecutablePath.size+strlen(DEFAULT_PPM_FOLDER_NAME)+1;
	ppmFolderPath.tab = malloc(sizeof(char) * relativeExecutablePath.size);
	if(ppmFolderPath.tab == NULL){
		printf("\nCan't get the default folder path, memory error.\n");
		exit(0);
	}
	strcat(ppmFolderPath.tab, relativeExecutablePath.tab);
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
			free(relativeExecutablePath.tab);
			free(ppmFolderPath.tab);
			exit(0);
		}
		
		//Getting the list of file name in the directory
		CharList** elementNameTab = malloc(sizeof(CharList*)*ppmFileNb);
		if(elementNameTab == NULL){
			printf("\nCan't get the file list, memory error.\n");
			free(relativeExecutablePath.tab);
			free(ppmFolderPath.tab);
			exit(0);
		}
		
		int result = folderChildName(ppmFolderPath.tab, elementNameTab, ppmFileNb);
		if(0>result){
			printf("[ERROR] Can't get all the child in %s because the tab is to tiny.\nEnd of program.\n", ppmFolderPath.tab);
			free(relativeExecutablePath.tab);
			free(ppmFolderPath.tab);
			
			for(int i = 0; i < ppmFileNb; i++){
				destroyCharList(elementNameTab[i]);
			}
			
			free(elementNameTab);
			exit(0);
		}else if(result != 0){
			printf("[DEBUG] I don't know why but the tab is to big, there are %d empty cels. But we don't care because the programe work well !\n", result);
			elementNameTab -= result;
		}
		
		int endianness = getEndianness();
		int sizeName = 0;

		char* out, *in;
		
		//The function fopen use relative path from executable, not execution location
		CharList* outList = convertTabToCharList(ppmFolderPath.tab, ppmFolderPath.size);
		CharList* inList = convertTabToCharList(ppmFolderPath.tab, ppmFolderPath.size);
		appendCharList(DELIMITER, inList);
		for(int i = 0; i < ppmFileNb; i++){
			sizeName = getSize(elementNameTab[i]);
			concatenateCharList(inList, elementNameTab[i]);
			
			printf("Current file : ");
			displayTextCharList(inList);
			
			out = getFileName(outList, "Please put the name for compressed file : \n");
			in = convertCharListToTab(inList);
			printf("The output file will be %s\n", out);
			printf("Opening file : %s\n", in);
			compression(in, endianness, out);
			
			
			
			for(int k = 0; k < sizeName; k++){
				removeLastCharList(inList);
			}
			
			free(out);
			free(in);		
			
		}
		
		for(int i = 0; i < ppmFileNb; i++){
				destroyCharList(elementNameTab[i]);
		}
			
		free(elementNameTab);
	}else{
		decompression("ppm/DPC", getEndianness(), "AYAYA.ppm");
	}
	
	free(relativeExecutablePath.tab);
	free(ppmFolderPath.tab);
	return 0;
}
