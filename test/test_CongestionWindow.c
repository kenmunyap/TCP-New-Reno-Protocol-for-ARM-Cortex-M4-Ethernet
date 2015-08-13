#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "CongestionWindows.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}

void test_cwndGetBeginningOffsets_should_return_begining_Offset(void){
  TCPSession session;
  Cwnd Window;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t beginingOffset;

  cwndInitWindow(&session);
  
  beginingOffset = cwndGetBeginningOffsets(session.cwnd);
  
  TEST_ASSERT_EQUAL(0, beginingOffset);
}

void test_cwndIncrementWindows_should_return_the_incremented_window(void){
  TCPSession session;
  Cwnd Window;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t windowSize;

  cwndInitWindow(&session);
  
  windowSize = cwndIncrementWindows(session.cwnd,50);
  
  TEST_ASSERT_EQUAL(100, windowSize);
}

void test_cwndIncrementWindows_should_return_the_incremented_window_case2(void){
  Cwnd Window = {.size = 200, .offset = 150};
  TCPSession session;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t windowSize;
  
  windowSize = cwndIncrementWindows(session.cwnd,200);
  
  TEST_ASSERT_EQUAL(250, windowSize);
}

void test_cwndGetDataBlock_should_return_the_availableSize_of_the_block(void){
  TCPSession session;
  Cwnd Window;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t returnAvailableSize;

  cwndInitWindow(&session);
  initTCPState(&session);
  
  returnAvailableSize = cwndGetDataBlocks(session.cwnd,0,50,&state.ptrBlock);
  
  TEST_ASSERT_EQUAL(50, returnAvailableSize);
}
void test_cwndGetDataBlock_should_return_0_if_the_size_not_enough(void){
    TCPSession session;
  Cwnd Window;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t returnAvailableSize;

  cwndInitWindow(&session);
  initTCPState(&session);
  
  returnAvailableSize = cwndGetDataBlocks(session.cwnd,0,51,&state.ptrBlock);
  
  TEST_ASSERT_EQUAL(0, returnAvailableSize);
}
void test_cwndGetDataBlock_should_return_the_availableSize_if_the_offSet_is_same(void){
  TCPSession session;
  Cwnd Window;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t returnAvailableSize;

  cwndInitWindow(&session);
  initTCPState(&session);
  
  returnAvailableSize = cwndGetDataBlocks(session.cwnd,0,50,&state.ptrBlock);
  
  TEST_ASSERT_EQUAL(50, returnAvailableSize);
}
void test_cwndGetDataBlock_should_not_return_the_availableSize_if_the_offSet_is_not_same(void){
  Cwnd Window;
  TCPSession session;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t returnAvailableSize;

  cwndInitWindow(&session);
  initTCPState(&session);
  
  returnAvailableSize = cwndGetDataBlocks(session.cwnd,50,50,&state.ptrBlock);
  
  TEST_ASSERT_EQUAL(0, returnAvailableSize);
}

void test_cwndGetDataBlock_should_return_the_availableSize_with_higher_offset(void){
  Cwnd Window = {.size = 50, .offset = 200};
  TCPSession session;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t returnAvailableSize;
  
  returnAvailableSize = cwndGetDataBlocks(session.cwnd,200,50,&state.ptrBlock);
  
  TEST_ASSERT_EQUAL(50, returnAvailableSize);
}

void test_cwndGetDataBlock_should_return_the_availableSize_with_different_offset(void){
  Cwnd Window = {.size = 50, .offset = 200};
  TCPSession session;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t returnAvailableSize;
  
  returnAvailableSize = cwndGetDataBlocks(session.cwnd,150,50,&state.ptrBlock);
  
  TEST_ASSERT_EQUAL(0, returnAvailableSize);
}

void test_cwndGetDataBlock_should_return_the_availableSize_with_higher_requesting_size(void){
  Cwnd Window = {.size = 400, .offset = 200};
  TCPSession session;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t returnAvailableSize;
  
  returnAvailableSize = cwndGetDataBlocks(session.cwnd,200,300,&state.ptrBlock);
  
  TEST_ASSERT_EQUAL(300, returnAvailableSize);
}

void test_cwndGetDataBlock_should_not_return_availableSize_if_the_window_size_not_enough(void){
  Cwnd Window = {.size = 400, .offset = 200};
  TCPSession session;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;
  uint32_t returnAvailableSize;
  
  returnAvailableSize = cwndGetDataBlocks(session.cwnd,200,600,&state.ptrBlock);
  
  TEST_ASSERT_EQUAL(0, returnAvailableSize);
}





