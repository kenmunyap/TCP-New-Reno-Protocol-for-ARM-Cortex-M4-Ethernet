#include "unity.h"
#include "SlowStart.h"
#include "mock_congestionWindow.h"
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
void test_TxTCPSM_should_init_and_send_the_first_data_increase_WindowSize_after_ACK(void){
  Cwnd Window;
  TCP_state state;

  cwndInitWindow(&Window);
  initTCPState(&state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  
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
void test_TxTCPSM_should_increase_WindowSize_after_ACK_and_offset_moved_to_100(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  TxTCPSM(&state,&Window,&packet);
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
void test_TxTCPSM_should_increase_WindowSize_after_third_ACK_and_offset_moved_to_150(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  cwndIncrementWindow_ExpectAndReturn(&Window,150,200);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(150,Window.offset);
  TEST_ASSERT_EQUAL(200,Window.size);
}

void test_TxTCPSM_for_ACK_out_of_order_case(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
  //printf("\n START TEST");
  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150); // receive pck 150
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(150,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(150,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(150,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,200);
  cwndIncrementWindow_ExpectAndReturn(&Window,150,200);//
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(200,Window.offset);
  TEST_ASSERT_EQUAL(200,Window.size);
  //printf("\n END TEST");
}

//=================================
//  For not enough window size case (TxTCPSM)
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
void test_TxTCPSM_should_return_if_the_availableSize_not_enough(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,0);
  //not available size thus no Send the packet
  TxTCPSM(&state,&Window,&packet);
  
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
void test_TxTCPSM_should_return_if_the_availableSize_not_enough_case_2(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);
    
  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // Duplicate ACK Thus WindowSize no increase
  TxTCPSM(&state,&Window,&packet);
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
void xtest_TxTCPSM_should_return_if_the_availableSize_not_enough_case_3(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  
  TCP_state state;
  initTCPState(&state);

  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&Window,&packet);
  
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&Window,&packet);

  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // Duplicate ACK Thus WindowSize no increase
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size); // The Window size still 100
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
}

/////////////////////////////////////////
// Test for multiple ACK fast retransmit
/////////////////////////////////////////

void test_TxTCPSM_should_return_if_3_ACK_received_case(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  //printf("TEST START \n");
  TCP_state state;
  initTCPState(&state);
  
  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,50);
  TEST_ASSERT_EQUAL(SlowStart,state.state);
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50); // Duplicate ACK Thus WindowSize no increase
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(FastRetransmit,state.state);

  sendDataPacket_Expect(&packet,&state.ptrBlock,50); // resend packet 50 (fast retransmit)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  //printf("TEST END \n");
}

void test_TxTCPSM_for_congestion_advoidance_state_after_retransmit(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  //printf("TEST START \n");
  TCP_state state;
  initTCPState(&state);
  
  cwndGetBeginningOffset_ExpectAndReturn(&Window,0);
  cwndGetDataBlock_ExpectAndReturn(&Window,0,50,&state.ptrBlock,50);
  TEST_ASSERT_EQUAL(SlowStart,state.state);
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,50);                // send 50
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(0,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);           // ack 50
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);               // send 100
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);               // send 150
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);          // ack 100
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100); // duplicate ACK 100 (first dupACK)
  //cwndIncrementWindow_ExpectAndReturn(&Window,150,200);  // no incremet
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200);               // send 200
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250);               // send 250
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,300);               // send 300
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100); // duplicate ACK 100 (second dupACK)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100); // duplicate ACK 100 (third dupACK)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(FastRetransmit,state.state);
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);      // Resend packet 100 (fast retransmit)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);  // ACK 300
  cwndIncrementWindow_ExpectAndReturn(&Window,50,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size); //windowSize 100
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  //================================================================
  // Continue sending to increase windowsize and move offset
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,350);               // send 350
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,350,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,400);               // send 400
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,400,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);           // ACK 350
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(350,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size); //windowSize 150
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,400,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,400);           // ACK 400
  cwndIncrementWindow_ExpectAndReturn(&Window,150,200);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(400,Window.offset);
  TEST_ASSERT_EQUAL(200,Window.size); //windowSize 200
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  //printf("TEST END \n");
}












