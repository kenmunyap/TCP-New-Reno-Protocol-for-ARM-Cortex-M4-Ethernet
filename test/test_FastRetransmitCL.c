#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}
Packet packet;

/////////////////////////////////////////
// Test for multiple ACK fast retransmit
/////////////////////////////////////////
/**
 *   0 _____
 * 50 |////| ACK 50 Success
 *    |////| ACK 50 (duplicate) 1st time
 *    |////| ACK 50 (duplicate) 2nd time
 *    |////| ACK 50 (duplicate) 3rd time
 *    |____| ---> Resend the lostPacket
 *    |____|
 *    |    |
 *    |    |
 *    |____|
 *
 *    After the third duplicate ACK of 50 received it moved to fast retransmit state
 *    The fast retransmit case should resend the lostpack after 3 time duplicate acknowledgemet
 */
void test_TxTCPSM_for_fast_retransmit_resend_the_lost_packet_after_3_ACK_from_congestion_avoidance(void){
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

  sendDataPacket_Expect(&packet,&state.ptrBlock,150);      // Resend packet 100 (fast retransmit)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  //printf("TEST END \n");
}
/**
 * 0     _____
 * 50   |////| ACK 50 Success
 * 100  |////| ACK 100 Success
 * 150  |////| ACK 100 (duplicate) 1st time
 * 200  |////| ACK 100 (duplicate) 2nd time
 * 250  |____| ACK 100 (duplicate) 3rd time---> Resend the lostPacket
 * 300  |____| ACK 300
 * 350  |    |
 *      |    |
 *      |____|
 *
 *    After the third duplicate ACK of 50 received it moved to fast retransmit state
 *    The fast retransmit case should resend the lostpack after 3 time duplicate acknowledgemet
 *    Then it should moved to fast recovery state
 */
void test_TxTCPSM_for_fast_retransmit_after_resend_the_lost_packet_and_should_move_to_fast_recovery_state(void){
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

  sendDataPacket_Expect(&packet,&state.ptrBlock,150);      // Resend packet 100 (fast retransmit)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);  // ACK 300
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(300,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  //printf("TEST END \n");
}
