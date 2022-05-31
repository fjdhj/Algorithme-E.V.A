#include <sys/time.h>
//#include "ppm_lib.c"
#include "list.h"
#include <string.h>
#define EVA_BLK_SAME 0xC0
#define EVA_BLK_INDEX 0x0
#define EVA_BLK_DIFF 0x40
#define EVA_BLK_LUMA 0x8000
#define EVA_BLK_RGB 0xFE000000
#define EVA_BLK_RGB_READER 0xFE

#define DEFAULT_PPM_FOLDER_NAME "ppm"

#ifdef __linux__
	#define DELIMITER '/'
#else
	#define DELIMITER '\\'
#endif

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

/* Clear the buffer of stdin with fgetc
*/
void clearStdinBuffer();



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
 * If the directory do not exist, the program is also close 
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

/* Put elements name in the given elementTab
 * If an unknow error occurr, close the program
 * If the directory do not exist or is empty, the program is also close
 * 
 * path : the path to the folder
 * elementNameTab : a tab for storing the child name
 * tabSize : the size of elementNameTab
 *
 * return : -1 if some file miss in elementTab, the size remain in elementNameTab otherwise
*/
int folderChildName(const char* path, CharList** elementNameTab, int tabSize);

/* ________________________________
Prototype fonction.c function
________________________________ */

/*
This function returns the endianness setting of the machine it is being executed on.
Takes no parameter.
Returns either int 0 for little endian or int 1 for big endian.
*/
int getEndianness();

/*
This functions returns int value to the power of int p. 
Follows p^0=1.
*/
int power(int value, int p);

/*
This function reverses the order of bytes of an int. 0xABCDEFGH becomes 0xGHEFCDAB.
Takes the int a as its sole parameter.
Returns an int which is a with reversed bytes order.
*/
unsigned int reverseint(unsigned int a);

/*
This function reverses the order of bytes of a short. 0xABCD becomes 0xCDAB.
Takes the short a as its sole parameter.
Returns a short which is a with reversed bytes order.
*/
unsigned short reverseshort(unsigned short a);

/*
This function writes 4 bytes into a binary file, taking into account endianness.
FILE* file is the stream to the affected file. 
Int val represents the 4 bytes written.
Int endianness is the endianness value defined by a call of getEndianness().
Does not return anything.
*/
void write4blocks(FILE* file,unsigned int val,int endianness);

/*
This function reads 4 bytes from a binary file, taking into account endianness.
FILE* file is the stream to the affected file. 
Int* val represents the 4 bytes read.
Int endianness is the endianness value defined by a call of getEndianness().
Does not return anything.
*/
void read4blocks(FILE* file, unsigned int* val, int endianness);           

/*
This function writes 2 bytes into a binary file, taking into account endianness.
FILE* file is the stream to the affected file. 
Short val represents the 2 bytes written.
Int endianness is the endianness value defined by a call of getEndianness().
Does not return anything.
*/
void write2blocks(FILE* file,unsigned short val,int endianness);

/*
This function read 2 bytes from a binary file, taking into account endianness.
FILE* file is the stream to the affected file. 
Short* val represents the 2 bytes read.
Int endianness is the endianness value defined by a call of getEndianness().
Does not return anything.
*/
void read2blocks(FILE* file, unsigned short* val, int endianness); 

/*
This function writes a byte into a binary file.
FILE* file is the stream to the affected file. 
char val represents the byte written.
Does not return anything.
*/
void write1blocks(FILE* file,char val);

/*
This function writes a byte into a binary file.
FILE* file is the stream to the affected file. 
char* val represents the byte written.
Does not return anything.
*/
void read1blocks(FILE* file, unsigned char*	 val);

/*
This function returns the time elapsed since January 1,1970 in microseconds (unsigned long). 
Here we are using it to get the number of seconds that passes between start and end of (de)compression.
Takes no parameter.
*/
unsigned long timer();

/*
This function returns the size of a file in bytes (long int).
FILE* f is the stream to the desired file.
*/
long int size(FILE* f);

/*
This function compresses a .ppm image using EVA. It creates a binary file which name will be specified by the user. 
It takes into account the endianness of the machine it is used on.
Created binary file's 16 first bytes are respectively the width,height,range and colors variables of the original image as they are defined in ppm_lib.h. 
Remaining bytes are the various blocks used by EVA.
Image used and created file will be found at [filename].
char* filename is the name of the to be compressed PPM image. 
Int endianness is the endianness value defined by a call of getEndianness().
char* outputfile is the name of the binary outuput file.
Does not return anything.
*/
void compression(char* filename,int endianness, char* outputfile);

/*
This function decompresses a fille using EVA. It creates a ppm image which name will be specified by the user.
It takes into account the endianness of the machine it is used on.
The 16 first bytes are respectively the width,height,range and colors variables of the original image as they are defined in ppm_lib.h.
Remaining bytes are the various blocks used by EVA.
File used will be found at [filename]
char* filename is the name of the file to be decompressed. 
Int endianness is the endianness value defined by a call of getEndianness().
char* outputfile is the name of the ppm outuput file.
Does not return anything.
*/
void decompression(char* filename, int endianness, char* outputfile);
