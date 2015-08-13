#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
void setUp(void){}
void tearDown(void){}


/*
*                 Sender                Reciever 
                400 | -------------------> | 
                    | <------------------  | ack 450
                450 | -------------------> |  
                500 | -------------------> | 
                    | <------------------  | dup 450
                    | <------------------  | dup 450
                    | -------------------> | retransmit 450
                550 | -------------------> | -formula extra 3 packet
                600 | -------------------> | -formula extra 3 packet   
                650 | -------------------> | -formula extra 3 packet
                    | <------------------- | ack 650
                    
*/  
void test_TxTCPSM_Fast_Recover_200_and_get_550_ack_that_is_fully_ack(){
  TCPSession session = {.offset = 550, .requestedSize = 50};
  Cwnd cwnd = {.size = 250, .offset = 400 ,.ssthresh = 100,.flightSize = 100}; 
  TCP_state state = {.state = FastRecovery};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,600);
  TxTCPSM(&session,&packet);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,  600,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,650);
  TxTCPSM(&session,&packet);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,700);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 700,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,650);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(650,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
}

/*
*                 Sender                Reciever 
                400 | -------------------> | 
                    | <------------------  | ack 450
                450 | -------------------> |  
                500 | -------------------> | 
                    | <------------------  | dup 450
                    | <------------------  | dup 450
                    | -------------------> | retransmit 450
                550 | -------------------> | -formula extra 3 packet
                600 | -------------------> | -formula extra 3 packet   
                650 | -------------------> | -formula extra 3 packet
                    | <------------------  | ack 450
                700 | -------------------> | -partial ack add 1 packet
*/  
void test_TxTCPSM_Fast_Recover_200_and_get_550_ack_that_is_partial_ack(){
  TCPSession session = {.offset = 550, .requestedSize = 50};
  Cwnd cwnd = {.size = 250, .offset = 400 ,.ssthresh = 100}; 
  TCP_state state = {.state = FastRecovery};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,600);
  TxTCPSM(&session,&packet);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,  600,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,650);
  TxTCPSM(&session,&packet);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,700);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 700,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,450);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(450,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state); 
  
  // cwndGetDataBlock_ExpectAndReturn(session.cwnd, 700,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,750);
  // TxTCPSM(&session,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(session.cwnd, 750,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,650);
  // TxTCPSM(&session,&packet);
  // TEST_ASSERT_EQUAL(650,session.cwnd->offset);
  // TEST_ASSERT_EQUAL(350,session.cwnd->size);
  // TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state); 
  
}


 


