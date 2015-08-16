#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "linkedList.h"

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
                50  | -------------------> | 
                100 | -------------------> |
                    | <------------------  | ack 100
       (done)   150 | -------------------> | 
                200 | -------------------> | 
                    | <------------------  | dup ack100 (ack 150)
                    | <------------------  | dup ack100
                    | <------------------  | dup ack100
                    | -------------------> | retransmit 100
                250 | -------------------> | -formula extra 
       (done)   300 | -------------------> | -formula extra  
                350 | -------------------> | 
                400 | -------------------> | -partial ack add 1 packet
                    | -------------------> | retransmit 150
                    | <------------------- | ack 300
                450 | -------------------> | 
                500 | -------------------> |
                550 | -------------------> |
                600 | -------------------> |
                650 | -------------------> |
*/  
void test_TxTCPSM_Fast_Recover_200_and_get_550_ack_that_is_partial_ack(){
  TCPSession session = {.offset = 250, .requestedSize = 50};
  Cwnd cwnd = {.size = 250, .offset = 100 ,.ssthresh = 100,.flightSize = 150}; 
  TCP_state state = {.state = FastRecovery};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,300);
  TxTCPSM(&session,&packet);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,  300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,350);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 350,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,200);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,250,300);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(150,session.cwnd->offset);
  TEST_ASSERT_EQUAL(350,session.cwnd->recover);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(150,session.cwnd->flightSize);
  TEST_ASSERT_EQUAL(FastRecovery,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 350,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,400);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 400,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,450);
  TxTCPSM(&session,&packet);

  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 450,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,350);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,300,350);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 450,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,500);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 500,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,550);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,600);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 600,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,650);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,650);
  TxTCPSM(&session,&packet);
}

/*
*                 Sender                Reciever 
                50  | -------------------> | 
                100 | -------------------> |
                    | <------------------  | ack 100
                150 | -------------------> | 
                200 | -------------------> | 
                    | <------------------  | dup ack100 (ack 150)
                    | <------------------  | dup ack100
                    | <------------------  | dup ack100
                    | -------------------> | retransmit 100
                250 | -------------------> | -formula extra 
                300 | -------------------> | -formula extra
                350 | -------------------> | dup add 
                400 | -------------------> | dup add
                450 | -------------------> | dup add
*/  
void test_TxTCPSM_Fast_Recover_200_and_get_dupACK_5_times(){
  TCPSession session = {.offset = 250, .requestedSize = 50};
  Cwnd cwnd = {.size = 250, .offset = 100 ,.ssthresh = 100,.flightSize = 150}; 
  TCP_state state = {.state = FastRecovery};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,300);
  TxTCPSM(&session,&packet);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,  300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,350);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 350,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,250,300);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,  350,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,400);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 400,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,300,350);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,  400,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,450);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 450,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,350,400);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,  450,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,500);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(500,session.cwnd->recover);  
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(400,session.cwnd->size);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 500,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,500);
  TxTCPSM(&session,&packet);
}




  


 


