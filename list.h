#include <stdio.h>
#include <stdlib.h>

typedef struct _charList CharList;
struct _charList{
	char val;
	CharList* next;
};

/**
 * Create a new list of CharList type
 * return the new list created
*/
CharList* createCharList();

/**
 * Convert a CharList to a char tab
 * WARNING : the free function need to be use with the char tab before the end of the program
 * 
 * list : the CharList to convert
 * return the new char tab
*/
char* convertCharListToTab(CharList* list);

/**
 * Convert a char tab to a CharList
 * 
 * tab : the char tab to convert
 * size : the size of the tab
 * return the new CharList
*/
CharList* convertTabToCharList(char* tab, int size);

/**
 * Display the given CharList
*/
void displayCharList(CharList* list);

/**
 * Like displayCharList but without space { or ,
 * It show the content like a char tab with a %s
*/
void displayTextCharList(CharList* list);

/**
 * return the size of the given CharList
*/
int getSize(CharList* list);

/**
 * Add an element at the given index in the CharList
 * If the index is negative, the function start from the end
 * 
 * index : the index of the new element
 * val : the value to put in the cells
 * list : the CharList
 * return : the new adress of the CharList if need
*/
CharList* addCharList(int index, char val, CharList* list);

/**
 * Add an element at the end of the CharList
 * 
 * val : the value ofthe element
 * list : the CharList
*/
void appendCharList(char val, CharList* list);

/**
 * Add the content of a CharList at the end an other 
 * 
 * l1 : The ChartList where the content need to be add
 * l2 : The content to add
*/
void concatenateCharList(CharList* l1, CharList* l2);

/**
 * Remove the first element in a list
 * 
 * list : the CharList
 * return : the new address of the list
*/
CharList* removeFirstCharList(CharList* list);

/**
 * Remove the last element in a list
 *
 * list : The CharList
 * return : the new address
*/
CharList* removeLastCharList(CharList* list);

/**
 * Remove all the element of a list and free the memory
 *
 *list : the list to destroy
*/
void destroyCharList(CharList* list);

/**
 * Get the element of a list at the given index
 * If the index is negative, start from the end
 * 
 * index : the index of the value we wan't to get
 * list : the CharList
 * return : the element store in list at the given index
*/
char getElementCharList(int index, CharList* list);
