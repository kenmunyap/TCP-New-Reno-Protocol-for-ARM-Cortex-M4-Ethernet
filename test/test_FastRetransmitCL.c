#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "linkedList.h"

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
  TCPSession session;
  Cwnd Window;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;

  cwndInitWindow(&session);
  initTCPState(&session);
  TEST_ASSERT_EQUAL(SlowStart,session.tcpState->state);

  cwndGetBeginningOffset_ExpectAndReturn(session.cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,0,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,50);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(0,session.cwnd->offset);
  TEST_ASSERT_EQUAL(50,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,50,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,50,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,100); //send 100
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150); //send 150
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,150,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);    // ack 100
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,100,150);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,150,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);  // duplicate ACK 100 (first dupACK)
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,200); //send 200
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,250); //send 250
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,300); //send 300
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,300,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);  // duplicate ACK 100 (second dupACK)
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,300,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);  // duplicate ACK 100 (third dupACK)
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRetransmit,session.tcpState->state);

  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150); //send 300      // Resend packet 100 (fast retransmit)
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(250,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);

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
  TCPSession session;
  Cwnd Window;
  TCP_state state;
  session.cwnd = &Window;
  session.tcpState = &state;

  cwndInitWindow(&session);
  initTCPState(&session);
  TEST_ASSERT_EQUAL(SlowStart,session.tcpState->state);

  cwndGetBeginningOffset_ExpectAndReturn(session.cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,0,50,&state.ptrBlock,50);

  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,50);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(0,session.cwnd->offset);
  TEST_ASSERT_EQUAL(50,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,50,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,50,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,100); //send 100
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150); //send 150
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,150,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);    // ack 100
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,100,150);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,150,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);  // duplicate ACK 100 (first dupACK)
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,200); //send 200
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,250); //send 250
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,300); //send 300
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,300,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);  // duplicate ACK 100 (second dupACK)
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,300,50,&state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);  // duplicate ACK 100 (third dupACK)
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRetransmit,session.tcpState->state);

  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150); // Resend packet 150 (fast retransmit)
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(250,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,350); //send 350
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,350,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,400); //send 400
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,400,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,450); //send 450
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,450,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,500); //send 500
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,500,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,550); //send 550
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(350,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
}
