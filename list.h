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
 * Display the given CharList
*/
void displayCharList(CharList* list);

/**
 * return the size of the given CharList
*/
int getSize(CharList* list);

/**
 * Add an element at the given index in the CharList
 * If the index is negatif, the function start from the end
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
