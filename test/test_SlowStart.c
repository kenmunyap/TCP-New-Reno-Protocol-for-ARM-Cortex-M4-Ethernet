#include "unity.h"
#include "SlowStart.h"
#include "mock_CongestionWindow.h"

void setUp(void){}
void tearDown(void){}


void test_module_generator_needs_to_be_implemented(void){
	int size = 50;
	int offset = 0;
	Cwnd data;
	TCP_state state;
	cwndInitWindow(&data);
	TEST_ASSERT_EQUAL(0,data.offset);
	TEST_ASSERT_EQUAL(50,data.size);
	initTCPState(&state);
	TEST_ASSERT_EQUAL(SlowStart,state.state);
	TxTCP(&state,&data,size,offset);
	// TEST_ASSERT_EQUAL(50,data.size);
}

<<<<<<< HEAD


=======
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
>>>>>>> a4181e80062f6d390b75858f99da7bf406fee684
