#include "unity.h"
#include "linkedList.h"
#include <malloc.h>


void setUp(){}
void tearDown(){}

void test_create_linkedlist(){
  linkedList *list;
  list = create_linkedList();
  TEST_ASSERT_NULL(list->head);
  TEST_ASSERT_NULL(list->tail);
}

void test_List_addLast_should_add_last_third_element(){

	linkedList *list;
	
	// initialize the list
	list = create_linkedList();
	
	//Test fixture
  
  Element elemArray1 = {.next = NULL, .data = 2};
	Element elemArray0 = {.next = NULL, .data = 1};
  Element elemArrayA = {.next = NULL, .data = 4};
  Element elemArrayB = {.next = NULL, .data = 7};  
	// list->head = &elemArray0;
	// list->tail = &elemArray0;
	// list->length = 1;
	
  
  List_addLast(list,&elemArray0);
  TEST_ASSERT_NULL(elemArray0.next);
  TEST_ASSERT_EQUAL_PTR(&elemArray0,list->head);
  TEST_ASSERT_EQUAL_PTR(&elemArray0,list->tail);
  TEST_ASSERT_EQUAL(1,elemArray0.data);
  TEST_ASSERT_EQUAL(1,list->length);
  
  List_addLast(list,&elemArray1);
  TEST_ASSERT_NULL(elemArray1.next);
  TEST_ASSERT_EQUAL_PTR(&elemArray0,list->head);
  TEST_ASSERT_EQUAL_PTR(&elemArray1,list->tail);
  TEST_ASSERT_EQUAL(1,elemArray0.data);
  TEST_ASSERT_EQUAL(2,elemArray1.data);
  TEST_ASSERT_EQUAL(2,list->length);
  
  List_removeFirst(list);
  TEST_ASSERT_NULL(elemArray1.next);
  TEST_ASSERT_EQUAL_PTR(&elemArray1,list->head);
  TEST_ASSERT_EQUAL_PTR(&elemArray1,list->tail);
  TEST_ASSERT_EQUAL(2,elemArray1.data);
  TEST_ASSERT_EQUAL(1,list->length);
}







	