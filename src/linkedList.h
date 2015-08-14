#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

// typedef struct Element_t Element;
typedef struct Element_t{
	void *next;
	int data;
}Element;
//Struct of linkedList
typedef struct{
	Element *head;
	Element *tail;
  int length;
}linkedList;

//Struct of Element;



linkedList *create_linkedList();
void List_addLast(linkedList *list,Element *element);
Element *List_removeFirst(linkedList *list);


//Function prototype
// linkedList *create_linkedList();
// void List_addLast(linkedList *list,Element *element);
// void List_addFirst(linkedList *list,Element *element);
// Element *List_removeLast(linkedList *list);
// Element *List_removeFirst(linkedList *list);

#endif //__LINKED_LIST_H__