#include "unity.h"
#include "TxTCP.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}
Packet packet;

//Window set to 100 and offset to 300 after fast retransmit when receiving 3 dupACK

void test_TxTCPSM_for_fast_recovery_for_duplicate_ack(void){
  printf("TEST START \n");
    
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

  printf("TEST END \n");
}
void test_TxTCPSM_for_fast_recovery_for_non_duplicate_ack(void){
  
  printf("TEST START \n");
    
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

  printf("TEST END \n");
  
}