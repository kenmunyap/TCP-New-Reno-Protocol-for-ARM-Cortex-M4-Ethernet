#include "unity.h"
#include "TCPhelper.h"
#include "TxTCP.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}

void test_incCACounter_counter_equal_0_increment(void){
  Cwnd cwnd;
  TCP_state state;
  uint32_t counter;
  uint32_t currentWindowSize;
  uint32_t ackNo;
  
  counter = 0;
  currentWindowSize = 100;
  ackNo = 50;
  cwndIncrementWindow_ExpectAndReturn(&cwnd,currentWindowSize,150);
  incCACounter(counter,&state,&cwnd,currentWindowSize,ackNo);
  TEST_ASSERT_EQUAL(150,cwnd.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
}

void test_incCACounter_counter_equal_1_not_increment(void){
  Cwnd cwnd;
  TCP_state state;
  uint32_t counter;
  uint32_t currentWindowSize;
  uint32_t ackNo;
  
  counter = 1;
  currentWindowSize = 100;
  ackNo = 50;
  incCACounter(counter,&state,&cwnd,currentWindowSize,ackNo);
  TEST_ASSERT_EQUAL(100,cwnd.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
}

void test_checkCAorSSBySSTHRESH_cwnd_size_smaller_than_ssthresh(void){
  Cwnd cwnd;
  TCP_state state;
  cwnd.size = 200;
  checkCAorSSBySSTHRESH(&state,&cwnd);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
}

void test_checkCAorSSBySSTHRESH_cwnd_size_smaller_and_equal_than_ssthresh(void){
  Cwnd cwnd;
  TCP_state state;
  cwnd.size = 250;
  checkCAorSSBySSTHRESH(&state,&cwnd);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
}

void test_checkCAorSSBySSTHRESH_cwnd_size_larger_than_ssthresh(void){
  Cwnd cwnd;
  TCP_state state;
  cwnd.size = 300;
  checkCAorSSBySSTHRESH(&state,&cwnd);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
}

