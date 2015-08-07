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
void test_TxTCPSM_for_fast_recovery_start_from_begining_with_offset_0_and_congestion_window_size_50_for_dupack_case(void){
  Cwnd Window;
  cwndInitWindow(&Window);
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
void test_TxTCPSM_for_fast_recovery_start_from_begining_with_offset_0_and_congestion_window_size_50_for_non_dupack_case(void){
  Cwnd Window;
  cwndInitWindow(&Window);
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);  // ACK 150
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(300,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
}

void test_fastReCovery_all_start_from_slow_start(void){
  // printf("TEST START \n");

  Cwnd Window;
  cwndInitWindow(&Window);
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100); // duplicate ACK 100 (first dupACK) no increment
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100); // duplicate ACK 100 (second dupACK)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
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

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);  //still duplicate, cwnd += SMSS
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(300,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);  // non duplicate
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(350,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,200);  // non duplicate
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(400,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);  // non duplicate
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(450,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);  // non duplicate
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(500,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  cwndGetDataBlock_ExpectAndReturn(&Window,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);  // non duplicate
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(550,Window.size);
  // TEST_ASSERT_EQUAL(CongestionAvoidance,state.state); // recover complete ; come out of fast recovery case

  // printf("TEST END \n\n");
}


