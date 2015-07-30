#include "unity.h"
#include "SlowStart.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}
Packet packet;

//Window set to 100 and offset to 300 after fast retransmit when receiving 3 dupACK

void xtest_TxTCPSM_for_fast_recovery_for_duplicate_ack(void){
  //printf("TEST START \n");
    
  Cwnd Window  = {.offset = 300, .size = 100 };
  TCP_state state = { .state = FastRecovery };
  
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);  // ACK 300 
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(250,Window.size); // + 3*SMSS
  TEST_ASSERT_EQUAL(50,Window.ssthresh);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,350);       // send 350
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(275,Window.size);
  TEST_ASSERT_EQUAL(125,Window.ssthresh);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);

  //printf("TEST END \n\n");
}
void xtest_TxTCPSM_for_fast_recovery_for_non_duplicate_ack(void){
  //printf("TEST START \n");
    
  Cwnd Window  = {.offset = 300, .size = 100 };
  TCP_state state = { .state = FastRecovery };
  
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(100,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);  // ACK 350 
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size); // + 3*SMSS
  TEST_ASSERT_EQUAL(50,Window.ssthresh);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,400);  // ACK 400
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(300,Window.offset);
  TEST_ASSERT_EQUAL(50,Window.size);
  TEST_ASSERT_EQUAL(50,Window.ssthresh);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);

  //printf("TEST END \n\n");
}

void test_TxTCPSM_for_fast_recovery_start_from_begining_with_offset_0_and_congestion_window_size_50_for_dupack_case(void){
  printf("TEST START \n");
  
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
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);      // Resend packet 100 (fast retransmit)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);  // ACK 150
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset); //offset changed back after resend the lost packet
  TEST_ASSERT_EQUAL(275,Window.size);   //windowSize 225 >> 150/2 + 150 then 225 += SMSS
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  //================================================================
  //Continue sending to increase windowsize and move offset
  
  // cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,350);               // send 350
  // TxTCPSM(&state,&Window,&packet);
  // TEST_ASSERT_EQUAL(300,Window.offset);
  // TEST_ASSERT_EQUAL(287,Window.size);
  // TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  // cwndGetDataBlock_ExpectAndReturn(&Window,350,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,400);               // send 400
  // TxTCPSM(&state,&Window,&packet);
  // TEST_ASSERT_EQUAL(350,Window.offset);
  // TEST_ASSERT_EQUAL(293,Window.size);
  // TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  // cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,150);           // ACK 350
  // TxTCPSM(&state,&Window,&packet);
  // TEST_ASSERT_EQUAL(100,Window.offset);
  // TEST_ASSERT_EQUAL(275,Window.size); //windowSize 150
  // TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  // cwndGetDataBlock_ExpectAndReturn(&Window,400,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,400);           // ACK 400
  // cwndIncrementWindow_ExpectAndReturn(&Window,150,200);
  // TxTCPSM(&state,&Window,&packet);
  // TEST_ASSERT_EQUAL(400,Window.offset);
  // TEST_ASSERT_EQUAL(348,Window.size); //windowSize 200
  // TEST_ASSERT_EQUAL(FastRecovery,state.state);
}

void test_TxTCPSM_for_fast_recovery_start_from_begining_with_offset_0_and_congestion_window_size_50_for_non_dupack_case(void){
  printf("TEST START \n");
  
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
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);      // Resend packet 100 (fast retransmit)
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(100,Window.offset);
  TEST_ASSERT_EQUAL(150,Window.size);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&Window,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);  // ACK 150
  TxTCPSM(&state,&Window,&packet);
  TEST_ASSERT_EQUAL(150,Window.offset); //offset changed back after resend the lost packet
  TEST_ASSERT_EQUAL(75,Window.size);   //windowSize 225 >> 150/2 + 150 then 225 += SMSS
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
}








