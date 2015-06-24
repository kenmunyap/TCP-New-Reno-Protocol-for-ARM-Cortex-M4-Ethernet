#include "unity.h"
#include "SlowStart.h"
#include "mock_CongestionWindow.h"

void setUp(void){}

void tearDown(void){}

void test_module_generator_needs_to_be_implemented(void)
{
	Cwnd data;
	TCP_state state;
	cwndInitWindow(&data);
	TEST_ASSERT_EQUAL(0,data.offset);
	TEST_ASSERT_EQUAL(0,data.size);
	initTCPState(&state);
	TEST_ASSERT_EQUAL(SlowStart,state.state);
	cwndIncrementWindow_ExpectAndReturn(&data,50,50);
	Buffer[0] = 'a';
	TCP(&state,&data);
	TEST_ASSERT_EQUAL(50,data.size);

	
}
