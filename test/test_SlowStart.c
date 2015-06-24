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



