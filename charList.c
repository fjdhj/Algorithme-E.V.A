#include "list.h"

CharList* createCharList(){
	CharList* list = malloc(sizeof(CharList));
	if(list == NULL){
		printf("[ERROR] A memory error occured. Can't create a new CharList. Program Exit.\n");
		exit(-1);
	}
	list->next = NULL;
	list->val = 0;
	return list;
}

void displayCharList(CharList* list){
	printf("{");
	CharList* p1 = list;
	
	while(p1 != NULL){
		if(p1->next == NULL){
			printf("%c}", p1->val);
		}else{
			printf("%c, ",p1->val);
		}
		p1 = p1 -> next;
	}
}

int getSize(CharList* list){
	CharList* p1 = list;
	int count = 0;
	while(p1 != NULL){
		count++;
		p1 = p1-> next;
	}
	
	return count;
}

CharList* addCharList(int index, char val, CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't add a element to NULL. Program Exit.\n");
		exit(-1);
	}
	
	int size = getSize(list);
	
	if(index + size + 1 < 0 || index > size){
		printf("[ERROR] A memory error occured. Can't access to the index %d in the CharList. Program Exit.\n", index);
		exit(-1);
	}
	
	if(index == 0 || size+1 == -index){
		CharList* p2 = createCharList();
		p2->val = val;
		p2->next = list;
		list=p2;
		
	}else{
		if(index < 0){
			index += size;
		}else{
			index--;
		}
	
		CharList* p1 = list;
		for(int i = 0; i < index; i++){
			p1 = p1->next;
			if(p1 == NULL){
				printf("[ERROR] A memory error occured. Can't access to the index %d in the CharList. Program Exit.\n", index);
				exit(-1);
			}
		}
	
	
		CharList* p2 = createCharList();
		p2->val = val;
		p2->next = p1->next;
		p1->next = p2;
	}
	return list;
}

void appendCharList(char val, CharList* list){
	addCharList(-1, val, list);
}

CharList* removeFirstCharList(CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't remove a element from NULL. Program Exit.\n");
		exit(-1);
	}
	
	CharList* p1 = list;
	list = list->next;
	free(p1);
	return list;
}

CharList* removeLastCharList(CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't remove a element from NULL. Program Exit.\n");
		exit(-1);	
	}
	CharList* p1 = list;
	
	if(p1->next == NULL){
		free(p1);
		return NULL;
	}else{
		while(p1->next->next != NULL){
			p1 = p1->next;
		}
		free(p1->next);
		p1->next = NULL;
		return list;
	}
}

void destroyCharList(CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't remove a element from NULL. Program Exit.\n");
		exit(-1);
	}
	
	while(list != NULL){
		list = removeFirstCharList(list);
	}
}

char getElementCharList(int index, CharList* list){
	if(list == NULL){
		printf("[ERROR] Can't remove a element from NULL. Program Exit.\n");
		exit(-1);
	}
	
	int size = getSize(list);
	
	if(index + size < 0 || index > size){
		printf("[ERROR] A memory error occured. Can't access to the index %d in the CharList. Program Exit.\n", index);
		exit(-1);
	}
	
	CharList* p1 = list;
	if(index == 0 || size+1 == -index){
		return list -> val;
		
	}else{
		if(index < 0){
			index += size;
		}
		
		int i = 0;
		
		while(i < index){
			if(p1->next == NULL){
				printf("[ERROR] A memory error occured. Can't access to the index %d in the CharList. Program Exit.\n", index);
				exit(-1);
			}
			p1 = p1 -> next;
			i++;
		}
	}
	return p1 -> val;
}

void mainTest(){
    CharList* l=createCharList();
    l->val='o';
    displayCharList(l);
    printf("\n%d",getSize(l));
  
    appendCharList('e',l);
    displayCharList(l);
    printf("\n%d",getSize(l));

    l = addCharList(1,'x',l);
    displayCharList(l);
  
    l = addCharList(2,'y',l);
    displayCharList(l);
    printf("\n%d",getSize(l));
  
    l = addCharList(-3,'z',l);
    displayCharList(l);
    printf("\n%d",getSize(l));
    
    l = removeFirstCharList(l);
    displayCharList(l);
    printf("\n%d",getSize(l));
    
    l = addCharList(3,'u',l);
    displayCharList(l);
    printf("\n%d",getSize(l));
    
    appendCharList('r',l);
    displayCharList(l);
    printf("\n%d",getSize(l));
    
    appendCharList('v',l);
    displayCharList(l);
    printf("\n%d",getSize(l));
    
    appendCharList('t',l);
    displayCharList(l);
    printf("\n%d",getSize(l));
    
    l = removeLastCharList(l);
    displayCharList(l);
    printf("\n%d",getSize(l));
    
    printf("\n%c\n", getElementCharList(2, l));
    printf("%c\n", getElementCharList(4, l));
    printf("%c\n", getElementCharList(6, l));
    printf("%c\n", getElementCharList(-1, l));
    printf("%c\n", getElementCharList(-6, l));
    printf("%c\n", getElementCharList(-7, l));
    destroyCharList(l);

}
