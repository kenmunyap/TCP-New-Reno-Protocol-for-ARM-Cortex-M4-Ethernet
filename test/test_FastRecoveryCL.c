#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}
Packet packet;
/**
 *   0 _____       _____ 0         _____ 0             _____ 0                      _____ 0
 * 50 |____|      |////| 50       |////|              |////|                       |////|
 *    |    |      |____| 100      |////|  100 (succs) |////| 100  (success)        |////| 100  (success)
 *    |    |      |____| 150      |____|  150(dup100) |////| 150  (dupACK 100)     |////| 150  (dupACK 100)
 *    |    |      |    |          |____|  200         |____| 200  (dupACK 100)     |////| 200  (dupACK 100)
 *    |    |  =>  |    |    =>    |    |  250      => |____| 250  (dupACK 100) =>  |____| 250  (dupACK 100) resend lostpacket
 *    |    |      |    |          |    |              |    | 300                   |____| 300  FastRecovery
 *    |    |      |    |          |    |              |    |                       |    |
 *    |    |      |    |          |    |              |    |                       |    |
 *    |____|      |____|          |____|              |____|                       |____|
 *
 *
 */
void test_TxTCPSM_for_fast_recovery_start_from_begining_with_offset_0_and_congestion_window_size_50_for_dupack_case_Partial_ACK(void){
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
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,250,300);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd,300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,350); //send 350
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(300,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,350,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,400); //send 400
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(300,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,400,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,300);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,300,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(250,session.cwnd->offset);
  TEST_ASSERT_EQUAL(350,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,400,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,450); //send 450
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(250,Window.offset);
  TEST_ASSERT_EQUAL(350,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,450,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,500); //send 500
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(250,Window.offset);
  TEST_ASSERT_EQUAL(350,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,500,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,550); //send 550
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(250,Window.offset);
  TEST_ASSERT_EQUAL(350,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,550,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,600); //send 600
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(250,Window.offset);
  TEST_ASSERT_EQUAL(350,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,600,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,600);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(600,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
}
/**
 * 0     _____
 * 50   |////| ACK 50 Success
 * 100  |////| ACK 100 Success
 * 150  |////| ACK 100 (duplicate) 1st time
 * 200  |////| ACK 100 (duplicate) 2nd time
 * 250  |____| ACK 100 (duplicate) 3rd time---> Resend the lostPacket
 * 300  |____| ACK 100 
 * 350  |    |
 *      |    |
 *      |____|
 *
 *    After the third duplicate ACK of 50 received it moved to fast retransmit state
 *    The fast retransmit case should resend the lostpack after 3 time duplicate acknowledgemet
 *    Then it should moved to fast recovery state
 */
void test_TxTCPSM_for_fast_recovery_start_from_begining_with_offset_0_and_congestion_window_size_50_for_non_dupack_case_Fully_ACK(void){
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
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,400);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(400,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,450);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,100,150);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(450,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,500);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(500,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,550);    //ack 550 all
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(550,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state);
}
