#include "unity.h"
#include "TCP.h"
#include "mock_Data.h"
#include "mock_sequenceNumber.h"
#include "mock_timeOutTimer.h"
#include "mock_ack.h"
#include "mock_congestionWindow.h"

void setUp(void){}

void tearDown(void){}

void test_successful_transmit_1st_packet(){
	SS_state state;
	DataBlock data;
	
	sequenceNumber_ExpectAndReturn(0);
	sequenceNumber_ExpectAndReturn(0);
	initialSequnceNumber(&data);
	
	initalState(&state);
	
	congestionWindow_ExpectAndReturn(1);
	countTimer_ExpectAndReturn(0);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(0);
	countTimer_ExpectAndReturn(50);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(Acknowledge,state.state_SS);
	TEST_ASSERT_EQUAL(500,data.nextseqnum[0]);
	TEST_ASSERT_EQUAL(0,data.sendbase);
	
	congestionWindow_ExpectAndReturn(1);
	recieveRxACK_ExpectAndReturn(500);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(SendData,state.state_SS);
	TEST_ASSERT_EQUAL(500,data.sendbase);
	
}

void test_successful_transmit_2_packet(){
	SS_state state;
	DataBlock data;
	
	sequenceNumber_ExpectAndReturn(0);
	sequenceNumber_ExpectAndReturn(0);
	initialSequnceNumber(&data);
	
	initalState(&state);

	congestionWindow_ExpectAndReturn(1);
	countTimer_ExpectAndReturn(0);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(0);
	countTimer_ExpectAndReturn(50);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(Acknowledge,state.state_SS);
	TEST_ASSERT_EQUAL(500,data.nextseqnum[0]);
	TEST_ASSERT_EQUAL(0,data.sendbase);
	
	congestionWindow_ExpectAndReturn(1);
	recieveRxACK_ExpectAndReturn(500);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(SendData,state.state_SS);
	TEST_ASSERT_EQUAL(500,data.sendbase);
	
	congestionWindow_ExpectAndReturn(2);
	countTimer_ExpectAndReturn(100);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(500);
	countTimer_ExpectAndReturn(150);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(1000);
	countTimer_ExpectAndReturn(200);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(Acknowledge,state.state_SS);
	TEST_ASSERT_EQUAL(1000,data.nextseqnum[0]);
	TEST_ASSERT_EQUAL(1500,data.nextseqnum[1]);
	TEST_ASSERT_EQUAL(500,data.sendbase);
	
	congestionWindow_ExpectAndReturn(2);
	recieveRxACK_ExpectAndReturn(1000);
	recieveRxACK_ExpectAndReturn(1500);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(SendData,state.state_SS);
	TEST_ASSERT_EQUAL(1500,data.sendbase);
}

void test_1st_and_2nd_pack_successful_send_without_time_out_3rd_time_out(){

	SS_state state;
	DataBlock data;
	
	sequenceNumber_ExpectAndReturn(0);
	sequenceNumber_ExpectAndReturn(0);
	initialSequnceNumber(&data);
	
	initalState(&state);

	congestionWindow_ExpectAndReturn(1);
	countTimer_ExpectAndReturn(0);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(0);
	countTimer_ExpectAndReturn(50);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(Acknowledge,state.state_SS);
	TEST_ASSERT_EQUAL(500,data.nextseqnum[0]);
	TEST_ASSERT_EQUAL(0,data.sendbase);
	
	congestionWindow_ExpectAndReturn(1);
	recieveRxACK_ExpectAndReturn(500);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(SendData,state.state_SS);
	TEST_ASSERT_EQUAL(500,data.sendbase);
	
	congestionWindow_ExpectAndReturn(2);
	countTimer_ExpectAndReturn(100);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(500);
	countTimer_ExpectAndReturn(150);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(1000);
	countTimer_ExpectAndReturn(200);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(Acknowledge,state.state_SS);
	TEST_ASSERT_EQUAL(1000,data.nextseqnum[0]);
	TEST_ASSERT_EQUAL(1500,data.nextseqnum[1]);
	TEST_ASSERT_EQUAL(500,data.sendbase);
	
	congestionWindow_ExpectAndReturn(2);
	recieveRxACK_ExpectAndReturn(1000);
	recieveRxACK_ExpectAndReturn(1500);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(SendData,state.state_SS);
	TEST_ASSERT_EQUAL(1500,data.sendbase);
	
	congestionWindow_ExpectAndReturn(3);
	countTimer_ExpectAndReturn(300);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(1500);
	countTimer_ExpectAndReturn(500);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(2000);
	countTimer_ExpectAndReturn(600);
	sendData2RCV_ExpectAndReturn(1);
	sequenceNumber_ExpectAndReturn(2500);
	countTimer_ExpectAndReturn(5500);
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(TimeOut,state.state_SS);
	TEST_ASSERT_EQUAL(2000,data.nextseqnum[0]);
	TEST_ASSERT_EQUAL(2500,data.nextseqnum[1]);
	TEST_ASSERT_EQUAL(3000,data.nextseqnum[2]);
	TEST_ASSERT_EQUAL(1500,data.sendbase);
	
	slowStart(&state,&data);
	TEST_ASSERT_EQUAL(1500,data.nextseqnum[0]);
}


	
	



