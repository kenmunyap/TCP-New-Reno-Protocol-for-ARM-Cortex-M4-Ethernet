#include "unity.h"
#include "SlowStart.h"
#include "mock_CongestionWindow.h"

void setUp(void){}
void tearDown(void){}

// void test_module_generator_needs_to_be_implemented(void)
// {
	// Cwnd data;
	// TCP_state state;
	// cwndInitWindow(&data);
	// TEST_ASSERT_EQUAL(0,data.offset);
	// TEST_ASSERT_EQUAL(0,data.size);
	// initTCPState(&state);
	// TEST_ASSERT_EQUAL(SlowStart,state.state);
	// cwndIncrementWindow_ExpectAndReturn(&data,50,50);
	// Buffer[0] = 'a';
	// TCP(&state,&data);
	// TEST_ASSERT_EQUAL(50,data.size);

	
// }

//====================================================================
//====================================================================
//====================================================================

// By CL
void test_cwndInitWindow_should_init_a_window_with_default_data(void){
	
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
}

void test_initTCPState_should_go_to_the_slow_start_state(void){
	
  TCP_state state;
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(SlowStart,state.state);
}

/**
 *   0  _____   
 *  50 |____|  
 *     |    |  
 *     |    |
 *     |____|
 *    
 *   1st congestion Window
 *   with size of 50 and offset 0
 */
void test_TxTCP_should_init_and_send_the_first_data(void){
  Cwnd *returned;
  
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStart,state.state);
  Block = 0;
  
  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&(*Block),50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);

  TxTCP(&state,&Window);
  
  cwndGetBeginningOffset_ExpectAndReturn(&Window,50);
  cwndGetDataBlock_ExpectAndReturn(&Window,50,100,&(*Block),100);
  cwndIncrementWindow_ExpectAndReturn(&Window,100,200);
  
  TxTCP(&state,&Window);
  
  cwndGetBeginningOffset_ExpectAndReturn(&Window,100);
  cwndGetDataBlock_ExpectAndReturn(&Window,100,150,&(*Block),100);
  //cwndIncrementWindow_ExpectAndReturn(&Window,200,400);
  
  returned = TxTCP(&state,&Window);
  TEST_ASSERT_NULL(returned);
  printf("======================\n");
  printf("TEST cwnd->offset : %d\n",Window.offset);
  printf("TEST cwnd->size   : %d\n",Window.size);
  printf("TEST Block        : %d\n",Block);
}













