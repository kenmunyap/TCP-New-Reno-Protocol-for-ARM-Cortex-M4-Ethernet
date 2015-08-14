#include <stdio.h>
#include <malloc.h>
#include "linkedList.h"


linkedList *create_linkedList(){
		linkedList *list = malloc(sizeof(linkedList));
		list->head = NULL;
		list->tail = NULL;
		list->length = 0;
		
		return list;
}

void List_addLast(linkedList *list,Element *element){
		if(list->head == NULL && list-> tail == NULL){
      list->head = element;
			list->tail = element;
		}else{
      list->tail->next = element;
      list->tail = element;
		}
    element->next = NULL;
		list->length++;
}

Element *List_removeFirst(linkedList *list){
    Element *element; 
    int length;
    if(list->head == NULL && list->tail == NULL){
        return NULL;
    }else if(list->head == list->tail){
      list->head = NULL;
      list->tail = NULL;
      list->length--;
    }else{
      list->head = list->head->next;
      list->length--;
    }
    return element;
}

	