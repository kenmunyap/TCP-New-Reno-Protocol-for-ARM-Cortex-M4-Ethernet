#include "unity.h"
#include "TCPhelper.h"
#include "TxTCP.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "linkedList.h"

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
  TCPSession session = {.dupAckCounter = 0};
  TCP_state tcpState = {.state = CongestionAvoidance};
  Cwnd cwnd;
  session.cwnd = &cwnd;
  session.tcpState = &tcpState;

  uint32_t ackNo = 50;

  duplicatePacketCount(&session,ackNo);
  TEST_ASSERT_EQUAL(1,session.dupAckCounter);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);

  duplicatePacketCount(&session,ackNo);
  TEST_ASSERT_EQUAL(2,session.dupAckCounter);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);

  duplicatePacketCount(&session,ackNo);
  TEST_ASSERT_EQUAL(0,session.dupAckCounter);
  TEST_ASSERT_EQUAL(FastRetransmit,session.tcpState->state); // state changed
}

void test_duplicatePacketCount_should_change_the_state(void)
{
  TCPSession session = {.dupAckCounter = 1};
  TCP_state tcpState = { .state = CongestionAvoidance };
  session.tcpState = &tcpState;
  
  uint32_t ackNo = 50;

  duplicatePacketCount(&session,ackNo);
  TEST_ASSERT_EQUAL(2,session.dupAckCounter);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); // state changed
}

void test_sendPacket_should_return_the_offset_after_send(void)
{
  TCPSession session = { .offset = 0};
  Packet packet;
  uint32_t availableSize = SMSS;

  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,50);
  sendPacket(&session,&packet,availableSize);
  TEST_ASSERT_EQUAL(50,session.offset);

  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,100);
  sendPacket(&session,&packet,availableSize);
  TEST_ASSERT_EQUAL(100,session.offset);
}

void test_retransmit_should_change_the_cwnd_data_and_the_state(void)
{
  TCPSession session;
  TCP_state state = { .state = CongestionAvoidance };
  Cwnd cwnd = {.offset = 0, .size = 50, .ssthresh = 1000,.lostPacket = 50};
  session.tcpState = &state;
  session.cwnd = &cwnd;
  Packet packet;
  session.offset = 0;
  uint32_t lostPacket = 50;

  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,100);
  retransmit(&session,&packet);
  TEST_ASSERT_EQUAL(0,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->ssthresh);
  TEST_ASSERT_EQUAL(250,session.cwnd->size);
}

void test_retransmit_should_change_the_cwnd_data_and_the_state_with_different_data(void)
{ 
  TCPSession session;
  TCP_state state = { .state = CongestionAvoidance };
  Cwnd cwnd = {.offset = 100, .size = 200, .ssthresh = 1000,.lostPacket = 50};
  session.tcpState = &state;
  session.cwnd = &cwnd;
  Packet packet;
  session.offset = 300;

  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,100);
  retransmit(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->ssthresh);
  TEST_ASSERT_EQUAL(250,session.cwnd->size);
}

void test_incCACounter_counter_equal_0_increment(void){
  TCPSession session;
  TCP_state tcpState;
  Cwnd cwnd;
  session.cwnd = &cwnd;
  session.tcpState = &tcpState;
  uint32_t counter;
  uint32_t currentWindowSize;
  uint32_t ackNo;

  counter = 0;
  currentWindowSize = 100;
  ackNo = 50;
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,currentWindowSize,150);
  incCACounter(counter,&session,currentWindowSize,ackNo);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
}

void test_incCACounter_counter_equal_1_not_increment(void){
  TCPSession session;
  TCP_state tcpState;
  Cwnd cwnd;
  session.cwnd = &cwnd;
  session.tcpState = &tcpState;
  uint32_t counter;
  uint32_t currentWindowSize;
  uint32_t ackNo;

  counter = 1;
  currentWindowSize = 100;
  ackNo = 50;
  incCACounter(counter,&session,currentWindowSize,ackNo);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
}

void test_checkCAorSSBySSTHRESH_cwnd_size_smaller_than_ssthresh(void){
  TCPSession session;
  TCP_state tcpState;
  Cwnd cwnd = { .size = 200};
  session.cwnd = &cwnd;
  session.tcpState = &tcpState;
  
  checkCAorSSBySSTHRESH(&session);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);
}

void test_checkCAorSSBySSTHRESH_cwnd_size_smaller_and_equal_than_ssthresh(void){
  TCPSession session;
  TCP_state tcpState;
  Cwnd cwnd = { .size = 250};
  session.cwnd = &cwnd;
  session.tcpState = &tcpState;
  
  checkCAorSSBySSTHRESH(&session);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);
}

void test_checkCAorSSBySSTHRESH_cwnd_size_larger_than_ssthresh(void){
  TCPSession session;
  TCP_state tcpState;
  Cwnd cwnd = { .size = 300};
  session.cwnd = &cwnd;
  session.tcpState = &tcpState;

  checkCAorSSBySSTHRESH(&session);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
}

void test_checkSSIsACKNoEqualSequenceNumber_acknowledge_number_is_equal_to_sequence_number(){
  TCPSession session;
  TCP_state tcpState;
  Cwnd cwnd;
  session.cwnd = &cwnd;
  session.tcpState = &tcpState;
  
  uint32_t ackNo = 50;
  uint32_t sequenceNumber = 50;
  uint32_t currentWindowSize = 50;
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,50,100);
  checkSSisACKNoEqualSequenceNumber(ackNo,sequenceNumber,currentWindowSize,&session);
}

void test_checkSSIsACKNoEqualSequenceNumber_acknowledge_number_is_not_equal_to_sequence_number(){
  TCPSession session;
  TCP_state state;
  Cwnd cwnd;
  session.cwnd = &cwnd;
  session.tcpState = &state;
  
  uint32_t ackNo = 100;
  uint32_t sequenceNumber = 150;
  uint32_t currentWindowSize = 50;
  checkSSisACKNoEqualSequenceNumber(ackNo,sequenceNumber,currentWindowSize,&session);
  TEST_ASSERT_EQUAL(1,session.dupAckCounter);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
}

void test_checkCAisACKNoEqualSequenceNumber_acknowledge_number_is_not_equal_to_sequence_number(){
  TCPSession session = {.dupAckCounter = 1};
  TCP_state state = {.state = CongestionAvoidance};
  Cwnd cwnd = {.size = 150, .offset=100};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  
  uint32_t ackNo = 100;
  uint32_t sequenceNumber = 150;
  uint32_t currentWindowSize = 150;
  uint32_t counter = 1;
  checkCAisACKNoEqualSequenceNumber(ackNo,sequenceNumber,currentWindowSize,counter,&session);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
}

void test_checkCAisACKNoEqualSequenceNumber_acknowledge_number_is_equal_to_sequence_number_but_counter_is_3_not_increment(){
  TCPSession session;
  TCP_state state = {.state = CongestionAvoidance};
  Cwnd cwnd = {.size = 150, .offset=100};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  
  uint32_t ackNo = 150;
  uint32_t sequenceNumber = 150;
  uint32_t currentWindowSize = 150;
  uint32_t counter = 3;
  checkCAisACKNoEqualSequenceNumber(ackNo,sequenceNumber,currentWindowSize,counter,&session);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
}

void test_checkCAisACKNoEqualSequenceNumber_acknowledge_number_is_equal_to_sequence_number_but_counter_is_1_and_increment(){
  TCPSession session;
  TCP_state state = {.state = CongestionAvoidance};
  Cwnd cwnd = {.size = 150, .offset=100};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  
  uint32_t ackNo = 150;
  uint32_t sequenceNumber = 150;
  uint32_t currentWindowSize = 150;
  uint32_t counter = 1;
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,currentWindowSize,200);
  checkCAisACKNoEqualSequenceNumber(ackNo,sequenceNumber,currentWindowSize,counter,&session);

  // TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
}


void test_roundOffValue_with_the_data_175_should_rounddown_to_150(void){
 uint32_t returnResult;
 returnResult = roundOffValue(175);
 TEST_ASSERT_EQUAL(150,returnResult);
}

void test_roundOffValue_with_the_data_225_should_rounddown_to_200(void){
 uint32_t returnResult;
 returnResult = roundOffValue(225);
 TEST_ASSERT_EQUAL(200,returnResult);
}
void test_roundOffValue_with_the_data_150_should_remain_as_150(void){
 uint32_t returnResult;
 returnResult = roundOffValue(150);
 TEST_ASSERT_EQUAL(150,returnResult);
}