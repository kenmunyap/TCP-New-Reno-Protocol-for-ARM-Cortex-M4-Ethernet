#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}
Packet packet;
/**
 *   0 _____       _____ 0         _____ 0
 * 50 |____|      |////| 50       |////|
 *    |    |      |____| 100      |////|  100 (dupACK 50)
 *    |    |      |____| 150      |____|  150
 *    |    |      |    |          |____|  200
 *    |    |  =>  |    |    =>    |    |  250
 *    |    |      |    |          |    |
 *    |    |      |    |          |    |
 *    |    |      |    |          |    |
 *    |____|      |____|          |____|
 *
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   after the ACK increament the window to 100
 *   offset moved to 50, 2nd ACK comes but is Duplicate
 *   moved to Congestion Avoidance cases
 */
void test_TxTCPSM_should_go_into_the_Congestion_avoidance_state_after_receive_dupack(void){
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
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);     //Entered the CongestionAvoidance case

  //printf("TEST END \n");
}
/**
 *   0 _____       _____ 0         _____ 0             _____ 0
 * 50 |____|      |////| 50       |////|              |////|
 *    |    |      |____| 100      |////|  100 (dup50) |////|      (dupACK 50)
 *    |    |      |____| 150      |____|  150         |////| 150  (dupACK 50)
 *    |    |      |    |          |____|  200         |____| 200
 *    |    |  =>  |    |    =>    |    |  250      => |____| 250
 *    |    |      |    |          |    |              |____| 300
 *    |    |      |    |          |    |              |    |
 *    |    |      |    |          |    |              |    |
 *    |____|      |____|          |____|              |____|
 *
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   after the ACK increament the window to 100
 *   offset moved to 50, 2nd ACK comes but is Duplicate
 *   moved to Congestion Avoidance cases
 *   3rd ACK comes, but still dupACK (2nd dupACK)
 *   Do nothing and still in the Congestion Avoidance cases
 */
void test_TxTCPSM_Congestion_avoidance_should_remain_in_the_case_after_2nd_dupack(void){
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50); // 1st Duplicate ACK
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);   // 2nd dupacket
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);       // Still in CongestionAvoidance case

  //printf("TEST END \n");
}
/**
 *   0 _____       _____ 0         _____ 0             _____ 0                      _____ 0
 * 50 |____|      |////| 50       |////|              |////|                       |////|
 *    |    |      |____| 100      |////|  100 (dup50) |////|      (dupACK 50)      |////|      (dupACK 50)
 *    |    |      |____| 150      |____|  150         |////| 150  (dupACK 50)      |////| 150  (dupACK 50)
 *    |    |      |    |          |____|  200         |____| 200  (success)        |////| 200  (success)
 *    |    |  =>  |    |    =>    |    |  250      => |____| 250               =>  |____| 250
 *    |    |      |    |          |    |              |    | 300                   |____| 300
 *    |    |      |    |          |    |              |    |                       |    |
 *    |    |      |    |          |    |              |    |                       |    |
 *    |____|      |____|          |____|              |____|                       |____|
 *
 *   1st congestion Window
 *   with size of 50 and offset 0
 *   after the ACK increament the window to 100
 *   offset moved to 50, 2nd ACK comes but is Duplicate
 *   moved to Congestion Avoidance cases
 *   3rd ACK comes, but still dupACK (2nd dupACK)
 *   continue sending
 */
void test_TxTCPSM_Congestion_avoidance_should_remain_in_the_case_after_2nd_dupack_and_continue_send(void){
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50); // 1st Duplicate ACK
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);   // 2nd dupacket
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);       // Still in CongestionAvoidance case

  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,200);   // succesfully receive
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(200,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,300);        // send 300
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(200,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,350);        // send 350
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(200,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  //printf("TEST END \n");
}
/**
 *   0 _____
 * 50 |////| ACK 50 Success
 *    |////| ACK 50 (duplicate) 1st time
 *    |////| ACK 50 (duplicate) 2nd time
 *    |////| ACK 50 (duplicate) 3rd time
 *    |____| 
 *    |____|
 *    |    |
 *    |    |
 *    |____|
 *
 *    After the third duplicate ACK of 50 received it moved to fast retransmit state
 */
void test_TxTCPSM_Congestion_avoidance_should_go_to_fastRetransmit_state_if_3_ACK_received(void){
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50); // 1st Duplicate ACK
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);   // 2nd dupacket
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);   // 3rd dupacket
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(FastRetransmit,state.state);    //entered the fast retransmit state

  //printf("TEST END \n");
}
/**
 *    Enter CongestionAvoidance cases
 *    Should not increment the window size when the RTT is not met
 *    the received ACK still no complete for 1 round trip time
 */
void test_TxTCPSM_Congestion_avoidance_should_Not_increment_window_in_congestion_avoidance(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  // printf("TEST START \n");
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50); // 1st Duplicate ACK enter CongestionAvoidance
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(150,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  // printf("TEST END \n");
}
/**
 *    Enter CongestionAvoidance cases
 *    Should increment the window size for ssthresh case
 */
void test_TxTCPSM_Congestion_avoidance_should_increment_window_1by1_after_receive_all_the_ack_with_1_RTT(void){
  Cwnd Window;
  cwndInitWindow(&Window);
  //printf("TEST START \n");
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
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100); // Send 100
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);

  cwndGetDataBlock_ExpectAndReturn(&Window,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150); // Send 150
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(50,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&Window,100,150);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200); // Send 200
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);

  cwndGetDataBlock_ExpectAndReturn(&Window,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250); // Send 250
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);

  cwndGetDataBlock_ExpectAndReturn(&Window,250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,300); // Send 300
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);

  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150); // ACK 150
  cwndIncrementWindow_ExpectAndReturn(&Window,150,200);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(150,Window.offset);
  TEST_ASSERT_EQUAL(200,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,200); // ACK 200
  cwndIncrementWindow_ExpectAndReturn(&Window,200,250);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(200,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250); // ACK 250
  cwndIncrementWindow_ExpectAndReturn(&Window,250,300);
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(250,Window.offset);
  TEST_ASSERT_EQUAL(300,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  //printf("TEST END \n");
}



