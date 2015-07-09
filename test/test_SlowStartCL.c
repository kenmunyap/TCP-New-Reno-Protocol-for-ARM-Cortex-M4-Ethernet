#include "unity.h"
#include "SlowStart.h"
#include "mock_congestionWindow.h"
#include "mock_returnACK.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}
Packet packet;

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
/*  Mock Fucntion  
*
*   uint32_t cwndGetBeginningOffset(Cwnd *cwnd);
*   uint32_t cwndIncrementWindow(Cwnd *cwnd, uint32_t size);
*   uint32_t cwndGetDataBlock(Cwnd *cwnd, uint32_t offset, uint32_t requestedSize, uint8_t **block);
*
*/

///////////////////////
// NEW function test
///////////////////////
/**
 *   0  _____      _____ 0
 *  50 |____|     |////| 50
 *     |    |  => |____| 100
 *     |    |     |____| 150
 *     |    |     |    |
 *     |    |     |    |
 *     |____|     |____|
 *    
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   after the ACK increament the window size to 100
 *   offset moved to 50
 */
void test_TxData_should_init_and_send_the_first_data_increase_WindowSize_after_ACK(void){
  Cwnd Window;
  TCP_state state;

  cwndInitWindow(&Window);
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStart,state.state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,50);
  txTcpSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  txTcpSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
}
/**
 *   0 _____       _____ 0         _____  0
 * 50 |____|      |////| 50       |////|
 *    |    |      |____| 100      |////|  100
 *    |    |      |____| 150      |____|  150
 *    |    |      |    |          |____|  200
 *    |    |  =>  |    |    =>    |____|  250
 *    |    |      |    |          |    |
 *    |    |      |    |          |    |
 *    |____|      |____|          |____|
 *    
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   after the ACK increament the window to 100
 *   offset moved to 50, 2nd ACK comes and increase window size by 50, to 150
 *   offset moved to 100
 */
void test_TxData_should_increase_WindowSize_after_ACK_and_offset_moved_to_100(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStart,state.state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,50);
  txTcpSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,150);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
}

/**
 *   0 _____       _____ 0         _____ 0         _____ 0
 * 50 |____|      |////| 50       |////|          |////|
 *    |    |      |____| 100      |////|  100     |////|
 *    |    |      |____| 150      |____|  150     |////| 150
 *    |    |      |    |          |____|  200     |____| 200
 *    |    |  =>  |    |    =>    |____|  250  => |____| 250
 *    |    |      |    |          |    |          |____| 300
 *    |    |      |    |          |    |          |____| 350
 *    |    |      |    |          |    |          |    |
 *    |____|      |____|          |____|          |____|
 *
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   after the ACK increament the window to 100
 *   offset moved to 50, 2nd ACK comes and increase window size by 50, to 150
 *   offset moved to 100, 3rd ACK comes and increase window size by 50, to 200
 *   offset moved to 150
 */
void test_TxData_should_increase_WindowSize_after_third_ACK_and_offset_moved_to_150(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStart,state.state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,50);
  txTcpSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,150);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,200);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,250);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  cwndIncrementWindow_ExpectAndReturn(&Window,150,200);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(150,Window.offset);
  TEST_ASSERT_EQUAL(200,Window.size);
}

//=================================
//  For not enough window size case (txTcpSM)
//=================================
/**
 *   0  _____      _____ 0
 *  50 |____|     |////| 50
 *     |    |  => |____| 100
 *     |    |     |    | 
 *     |    |     |    |
 *     |    |     |    |
 *     |____|     |____|
 *    
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   //fail to increase windowSize
 *   offset remain 50
 */
void test_TxData_should_return_if_the_availableSize_not_enough(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStart,state.state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&Block,0);
  //not available size thus no Send the packet
  txTcpSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
}

/**
 *   0 _____       _____ 0         _____  0
 * 50 |____|      |////| 50       |////|
 *    |    |      |____| 100      |////|  100
 *    |    |      |____| 150      |____|  150
 *    |    |      |    |          |____|  200
 *    |    |  =>  |    |    =>    |    |  250
 *    |    |      |    |          |    |
 *    |    |      |    |          |    |
 *    |____|      |____|          |____|
 *    
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   after the ACK increament the window to 100
 *   offset moved to 50, 2nd ACK comes but available size not enough
 *   offset moved to 100
 *    * First time able to increase window size, but second time not
 */
void test_TxData_should_return_if_the_availableSize_not_enough_case_2(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStart,state.state);
  
  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,50);
  txTcpSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,150);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // Duplicate ACK Thus WindowSize no increase
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size); // The window Size still 100
}


/**
 *   0 _____       _____ 0         _____ 0         _____ 0
 * 50 |____|      |////| 50       |////|          |////|
 *    |    |      |____| 100      |////|  100     |////|
 *    |    |      |____| 150      |____|  150     |////| 150
 *    |    |      |    |          |____|  200     |____| 200
 *    |    |  =>  |    |    =>    |    |  250  => |____| 250
 *    |    |      |    |          |    |          |____| 300
 *    |    |      |    |          |    |          |    | 
 *    |    |      |    |          |    |          |    |
 *    |____|      |____|          |____|          |____|
 *
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   after the ACK increament the window to 100
 *   offset moved to 50, 2nd ACK comes but available size not enough
 *   offset moved to 100, 3rd ACK comes and increase window size by 50, to 150
 *   offset moved to 150
 *    * First time able to increase window size, but second time not
 *    * window no increase, 3rd time ACK then window only increase by 50
 */
void test_TxData_should_return_if_the_availableSize_not_enough_case_3(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStart,state.state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,50);
  txTcpSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,150);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // Duplicate ACK Thus WindowSize no increase
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size); // The Window size still 100
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,200);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,250);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
}

/////////////////////////////////////////
// Test for multiple ACK fast retransmit
/////////////////////////////////////////
void test_TxData_should_return_if_3_ACK_received(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStart,state.state);
  printf("\n=====================================");
  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,50);
  txTcpSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,100);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,150);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // Duplicate ACK Thus WindowSize no increase
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size); // The Window size still 100
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,200);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&Block,50);
  sendDataPacket_Expect(&packet,&Block,250);
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  //cwndIncrementWindow_ExpectAndReturn(&Window,100,150); // no increment
  sendDataPacket_Expect(&packet,&Block,100); // resend packet 100 (fast recovery)
  txTcpSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
}
