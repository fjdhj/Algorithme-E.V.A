#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#define DEFAULT_PPM_FOLDER_NAME "ppm"

/* ________________________________
Struct
________________________________ */
typedef struct{
	char* 	tab;
	int size;
}CharTab;


/* ________________________________
Prototype main.c function
________________________________ */

/* The default main function
*/
int main();

/* Read a char from the user, the function check tif the value is allow
 *
 * minVal : the minimal value allow
 * maxVal : the maximal value allox
 * message : the message to display
 *
 * Return an int containing the value of the char given by the user
*/
int getChar(int minVal, int maxVal, char* message);



/* ________________________________
Prototype gestiondossier.c function
________________________________ */

/* Check if the given folder exist
 * If an unknow error occurr, close the program
 * 
 * path : the path to the folder
 * 
 * return : a char containing 1 if the folder exist, 0 otherwise
*/
char folderExist(const char* path);

/* Count the amount of element in a given folder
 * If an unknow error occurr, close the program
 * If the directory is empty, the program is also close 
 * 
 * path : the path to the folder
 * 
 * return : an int containing the amount of elements in a given folder
*/
int folderChildNumber(const char* path);

/* Create a folder
 * path : where the folder need to be created
*/
void createFolder(const char* path);
