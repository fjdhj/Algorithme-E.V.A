#include "liste.h"

CharList* createCharList(){
	CharList* list = malloc(sizeof(CharList));
	if(list == NULL){
		printf("[ERROR] A memory error occured. Can't create a new CharList. Program Exit.");
		exit(-1);
	}
	list->next = NULL;
	return list;
}

void displayCharList(CharList* list){
	printf("{");
	CharList* p1 = list;
	
	while(p1 != NULL){
		id(p1->suivant == NULL){
			printf("%c}", p1->val);
		}else{
			printf("%c, ",p1->val);
		}
		p1 = p1 -> suivant;
	}
}

void getSize(CharList* list){
	CharList p1 = list;
	int count = 0;
	while(p1 != NULL){
		count++;
		p1 = p1-> next;
	}
}

void addCharList(int index, char val, CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't add a element to NULL. Program Exit.");
		exit(-1);
	}
	if(index == 0){
		CharList* p2 = createCharList();
		p2->val = val;
		p2->next = list;
		list=p2;
		
	}else{
		if(index < 0){
			index += getSize(list);
		}else{
			index--;
		}
	
		CharList p1 = list;
		for(int i = 0; i < index; i++){
			p1 = p1-> next;
			if(p1 == NULL){
				printf("[ERROR] A memory error occured. Can't access to the index %d in the CharList. Program Exit.");
				exit(-1);
			}
		}
	
	
		CharList* p2 = createCharList();
		p2->val = val;
		p2->next = p1->next;
		p1->next = p2;
	}
}

void appendCharList(char val, CharList* list){
	addCharList(-1, val, list);
}

void removeFirstCharList(CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't remove a element from NULL. Program Exit.");
		exit(-1);
	}
	CharList* p1 = list;
	list = list->next;
	free(p1);
}

void removeLastCharList(CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't remove a element from NULL. Program Exit.");
		exit(-1);
	}
	CharList* p1 = list;
	while(p1->next->next != NULL){
		p1 = p1->next;
	}
	free(p1->next)
}

void destroyCharList(CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't remove a element from NULL. Program Exit.");
		exit(-1);
	}
	
	while(list != NULL){
		removeFirstCharList(list);
	}
}
