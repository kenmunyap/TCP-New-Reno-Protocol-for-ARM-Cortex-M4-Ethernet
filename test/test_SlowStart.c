#include "unity.h"
#include "SlowStart.h"
#include "mock_congestionWindow.h"
#include "mock_returnACK.h"
void setUp(void){}
void tearDown(void){}

<<<<<<< HEAD

void test_get_and_send_1_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){
=======
void test_module_generator_needs_to_be_implemented(void){
>>>>>>> 86e7e64a1629c35e607d9c1371214099e52d601e

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
	TEST_ASSERT_EQUAL(50,cwnd.offset);
	TEST_ASSERT_EQUAL(100,cwnd.size);	
}

void test_get_and_send_3_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){
	Cwnd cwnd;
	TCP_state state;
	uint32_t size;
	//1st segment
	cwndInitWindow(&cwnd);
	TEST_ASSERT_EQUAL(0,cwnd.offset);
	TEST_ASSERT_EQUAL(50,cwnd.size);
	
	initTCPState(&state);
	TEST_ASSERT_EQUAL(SlowStart,state.state);

	
	cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&(*(Block = 0)),0);
	TxTCP(&state,&cwnd);
	TEST_ASSERT_EQUAL(0,&(*Block));
	
	cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&(*Block),-1);
	returnACK_ExpectAndReturn(50);
	size = MSS;
	cwndIncrementWindow_ExpectAndReturn(&cwnd,size,100);
	returnACK_ExpectAndReturn(50);
	TxTCP(&state,&cwnd);
	TEST_ASSERT_EQUAL(50,cwnd.offset);
	TEST_ASSERT_EQUAL(100,cwnd.size);	
	
	//2nd segment
	cwndGetDataBlock_ExpectAndReturn(&cwnd,50,100,&(*Block = 0),50);
	TxTCP(&state,&cwnd);
	
<<<<<<< HEAD
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

=======
}
>>>>>>> 86e7e64a1629c35e607d9c1371214099e52d601e
