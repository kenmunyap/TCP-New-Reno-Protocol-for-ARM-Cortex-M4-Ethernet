#include "unity.h"
#include "SlowStart.h"
#include "mock_congestionWindow.h"
#include "mock_returnACK.h"
void setUp(void){}
void tearDown(void){}


void test_module_generator_needs_to_be_implemented(void){

	Cwnd cwnd;
	TCP_state state;
	uint32_t size;

	cwndInitWindow(&cwnd);
	TEST_ASSERT_EQUAL(0,cwnd.offset);
	TEST_ASSERT_EQUAL(50,cwnd.size);
	
	initTCPState(&state);
	TEST_ASSERT_EQUAL(SlowStart,state.state);
	Block = 0;
	
	cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&(*Block),0);
	TxTCP(&state,&cwnd);
	TEST_ASSERT_EQUAL(0,&(*Block));
	
	cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&(*Block),-1);
	returnACK_ExpectAndReturn(50);
	size = MSS;
	cwndIncrementWindow_ExpectAndReturn(&cwnd,size,100);
	returnACK_ExpectAndReturn(50);
	TxTCP(&state,&cwnd);
	// TEST_ASSERT_EQUAL
	
	
}




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

