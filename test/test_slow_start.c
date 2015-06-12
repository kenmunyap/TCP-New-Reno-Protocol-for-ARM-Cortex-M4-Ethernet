#include "unity.h"
#include "slow_start.h"
#include "mock_getData.h"
#include "mock_congestionWindow.h"
#include "mock_delay.h"

void setUp(void){}

void tearDown(void){}

void test_TCP_successful_send_all_data_using_slow_start(void){
	DataBlock data;
	
	sequenceNumber_IgnoreAndReturn(0);
	delayRTT_IgnoreAndReturn(500);
	increaseCongestionWindow_IgnoreAndReturn(1);
	sendData_IgnoreAndReturn(1);
	sequenceNumber_IgnoreAndReturn(501);
	recieveRxACK_IgnoreAndReturn(501);
	increaseCongestionWindow_IgnoreAndReturn(2);
	txSlowStart(&data);
	TEST_ASSERT_EQUAL(data.index,501);
	sendData_IgnoreAndReturn(2);
	// sequenceNumber_IgnoreAndReturn(1001);
	// recieveRxACK_IgnoreAndReturn(1001);
	// sendData_IgnoreAndReturn(3);
	// sequenceNumber_IgnoreAndReturn(1501);
	// recieveRxACK_IgnoreAndReturn(1501);
	txSlowStart(&data);
}
