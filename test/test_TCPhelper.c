#include "unity.h"
#include "TCPhelper.h"
#include "TxTCP.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}

void test_min_should_return_the_smaller_value(void)
{
  uint32_t a = 10;
  uint32_t b = 5;
  uint32_t result;
  
  result = min(a,b);
  
  TEST_ASSERT_EQUAL(5,result);
}

void test_max_should_return_the_bigger_value(void)
{
  uint32_t a = 10;
  uint32_t b = 5;
  uint32_t result;
  
  result = max(a,b);
  
  TEST_ASSERT_EQUAL(10,result);
}

void test_duplicatePacketCount_should_change_the_state_if_dupack_more_than_3(void)
{
  TCP_state state = { .state = CongestionAvoidance };
  Cwnd cwnd;
  uint32_t ackNo = 50;
  uint32_t dupack = 0;
  uint32_t result;
  
  result = duplicatePacketCount(&cwnd,&state,dupack,ackNo);
  TEST_ASSERT_EQUAL(1,result);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  result = duplicatePacketCount(&cwnd,&state,result,ackNo);
  TEST_ASSERT_EQUAL(2,result);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  result = duplicatePacketCount(&cwnd,&state,result,ackNo);
  TEST_ASSERT_EQUAL(0,result);
  TEST_ASSERT_EQUAL(FastRetransmit,state.state); // state changed
}

void test_duplicatePacketCount_should_change_the_state(void)
{
  TCP_state state = { .state = CongestionAvoidance };
  Cwnd cwnd;
  uint32_t ackNo = 50;
  uint32_t dupack = 1;
  uint32_t result;
  
  result = duplicatePacketCount(&cwnd,&state,dupack,ackNo);
  TEST_ASSERT_EQUAL(2,result);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state); // state changed
}

void test_sendPacket_should_return_the_offset_after_send(void)
{
  TCP_state state;
  Packet packet;
  uint32_t offset = 0;
  uint32_t availableSize = SMSS;
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,50);   
  offset = sendPacket(&state,&packet,availableSize,offset);
  TEST_ASSERT_EQUAL(50,offset);
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);   
  offset = sendPacket(&state,&packet,availableSize,offset);
  TEST_ASSERT_EQUAL(100,offset);
}

void test_retransmit_should_change_the_cwnd_data_and_the_state(void)
{
  TCP_state state = { .state = CongestionAvoidance };
  Cwnd Window = {.offset = 0, .size = 50, .ssthresh = 1000};
  Packet packet;
  uint32_t offset = 0;
  uint32_t lostPacket = 50;
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,lostPacket);   
  retransmit(&state,&Window,&packet,lostPacket,offset);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.ssthresh);
  TEST_ASSERT_EQUAL(250,Window.size);
}

void test_retransmit_should_change_the_cwnd_data_and_the_state_with_different_data(void)
{
  TCP_state state = { .state = CongestionAvoidance };
  Cwnd Window = {.offset = 100, .size = 200, .ssthresh = 1000};
  Packet packet;
  uint32_t offset = 300;
  uint32_t lostPacket = 50;
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,lostPacket);   
  retransmit(&state,&Window,&packet,lostPacket,offset);
  
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.ssthresh);
  TEST_ASSERT_EQUAL(250,Window.size);
}

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

void test_roundOffFlightSize(void){
 Cwnd cwnd = {.flightSize = 175};
 roundOffFlightSize(&cwnd);
 TEST_ASSERT_EQUAL(150,cwnd.flightSize);
}