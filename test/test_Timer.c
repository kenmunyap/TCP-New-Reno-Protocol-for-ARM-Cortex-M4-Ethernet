#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "Timer.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include <stdio.h>
#include <stdint.h>

void setUp(void){}
void tearDown(void){}

/*
* Test cases
* Example Link https://trello.com/c/MFT9k6lV/37-rtt-and-rto
*/
void test_initRTOTime_should_init_the_value_and_RTO_to_3(void){
	Timer time = {.ackTimer = 0, .sendTimer = 0};
  
  initRTOTime(&time);
  
  TEST_ASSERT_EQUAL_FLOAT(0,time.R);
  TEST_ASSERT_EQUAL_FLOAT(0,time.SRTT);
  TEST_ASSERT_EQUAL_FLOAT(0,time.RTTVAR);
  TEST_ASSERT_EQUAL_FLOAT(3,time.RTO);
}

void test_initRTOTime_should_init_the_value_with_the_sendtime_and_SampleRTT(void){
	Timer time = {.ackTimer = 1.5, .sendTimer = 3};
  
  initRTOTime(&time);
  
  TEST_ASSERT_EQUAL_FLOAT(1.5,time.R);
  TEST_ASSERT_EQUAL_FLOAT(1.5,time.SRTT);
  TEST_ASSERT_EQUAL_FLOAT(0.75,time.RTTVAR);
  TEST_ASSERT_EQUAL_FLOAT(4.5,time.RTO);
}

void test_updateRTOTime_should_update_the_RTO_when_the_R_changed(void){
	Timer time = {.ackTimer = 1.5, .sendTimer = 3};
  
  initRTOTime(&time);
  
  TEST_ASSERT_EQUAL_FLOAT(1.5,time.R);
  TEST_ASSERT_EQUAL_FLOAT(1.5,time.SRTT);
  TEST_ASSERT_EQUAL_FLOAT(0.75,time.RTTVAR);
  TEST_ASSERT_EQUAL_FLOAT(4.5,time.RTO);

  time.R = 2.5;
  
  updateRTOTime(&time);
  // printf("R       : %f\n", time.R);
  // printf("SRTT    : %f\n", time.SRTT);
  // printf("RTTVAR  : %f\n", time.RTTVAR);
  // printf("RTO     : %f\n\n", time.RTO);
  
  TEST_ASSERT_EQUAL_FLOAT(2.5,time.R);
  TEST_ASSERT_EQUAL_FLOAT(1.625,time.SRTT);
  TEST_ASSERT_EQUAL_FLOAT(0.78125,time.RTTVAR);
  TEST_ASSERT_EQUAL_FLOAT(4.75,time.RTO);
}
