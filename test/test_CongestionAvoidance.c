#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "mock_Timer.h"

void setUp(void){}
void tearDown(void){}


/*
               Sender                Reciever
cwnd->offset-> 250| ------------------>  |
               300| ------------------>  | 
                  | <-----------------   | ack 200
               350| ------------------>  | 
               400| ------------------>  | 
                  | <-----------------   | ack 250
      offset-> 450| ------------------>  | 
               500| ------------------>  | (size 300 > ssthresh)) 
                  | <-----------------   | ack 300 counter = 6
               550| ------------------>  | 
                  | <-----------------   | ack 350 counter = 5
               600| ------------------>  |
                  | <-----------------   | ack 400 counter = 4
               650| ------------------>  |
                  | <-----------------   | ack 450 counter = 3
               700| ------------------>  | 
                  | <-----------------   | ack 500 counter = 2
               750| ------------------>  | 
                  | <-----------------   | ack 550 counter = 1
*/
void test_TxTCPSM_Congestion_Avoidance_but_successfully_increment_one_by_each_RTT(){
  
  TCPSession session = {.offset = 450};
  Cwnd cwnd = {.size = 300, .offset = 250}; 
  TCP_state state = {.state = CongestionAvoidance};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 450,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,500);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 500,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,550);
  TxTCPSM(&session,&packet);
  
  // counter = 6
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(300,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,600);
  TxTCPSM(&session,&packet);
  
  // counter = 5
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 600, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(350,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 600,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,650);
  TxTCPSM(&session,&packet);
  
  // counter = 4
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,400);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(400,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,700);
  TxTCPSM(&session,&packet);
  
  // counter = 3
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 700, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,450);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(450,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 700,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,750);
  TxTCPSM(&session,&packet);
  
  // counter = 2
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 750, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,500);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(500,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 750,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,800);
  TxTCPSM(&session,&packet);
  
  // counter = 1
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 800, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,550);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,300,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(550,session.cwnd->offset);
  TEST_ASSERT_EQUAL(350,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
}

/*
               Sender                Reciever
cwnd->offset-> 250| ------------------>  |
               300| ------------------>  | 
                  | <-----------------   | ack 200
               350| ------------------>  | 
               400| ------------------>  | 
                  | <-----------------   | ack 250
      offset-> 450| ------------------>  | 
               500| ------------------>  | (size 300 > ssthresh)) 
                  | <-----------------   | ack 300 counter = 6
               550| ------------------>  | 
                  | <-----------------   | ack 400 counter = 4
               600| ------------------>  |
               650| ------------------>  |
                  | <-----------------   | ack 450 counter = 3
               700| ------------------>  | 
                  | <-----------------   | ack 500 counter = 2
               750| ------------------>  | 
                  | <-----------------   | ack 550 counter = 1
*/
void test_TxTCPSM_Congestion_Avoidance_but_successfully_increment_one_by_each_RTT_but_skiped_350_acked_400(){
  
  TCPSession session = {.offset = 450};
  Cwnd cwnd = {.size = 300, .offset = 250}; 
  TCP_state state = {.state = CongestionAvoidance};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 450,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,500);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 500,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,550);
  TxTCPSM(&session,&packet);
  
  // counter = 6
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(300,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,600);
  TxTCPSM(&session,&packet);
  
  // skiped counter 5 and get counter = 4
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 600, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,400);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(400,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 600,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,650);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,700);
  TxTCPSM(&session,&packet);
  
  // counter = 3
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 700, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,450);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(450,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 700,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,750);
  TxTCPSM(&session,&packet);
  
  // counter = 2
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 750, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,500);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(500,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 750,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,800);
  TxTCPSM(&session,&packet);
  
  // counter = 1
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 800, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,550);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,300,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(550,session.cwnd->offset);
  TEST_ASSERT_EQUAL(350,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
}

/*
               Sender                Reciever
cwnd->offset-> 250| ------------------>  |
               300| ------------------>  | 
                  | <-----------------   | ack 200
               350| ------------------>  | 
               400| ------------------>  | 
                  | <-----------------   | ack 250
      offset-> 450| ------------------>  | 
               500| ------------------>  | (size 300 > ssthresh)) 
                  | <-----------------   | ack 300 counter = 6
               550| ------------------>  | 
                  | <-----------------   | ack 350 counter = 5
               600| ------------------>  |
                  | <-----------------   | dup 350
                  | <-----------------   | dup 350
                  | <-----------------   | dup 350
*/
void test_TxTCPSM_Congestion_Avoidance_counter_decrement_into_5_but_get_duplicate_ack_after_counter_5(){
  
  TCPSession session = {.offset = 450};
  Cwnd cwnd = {.size = 300, .offset = 250}; 
  TCP_state state = {.state = CongestionAvoidance};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 450,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,500);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 500,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,550);
  TxTCPSM(&session,&packet);
  
  // counter = 6
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(300,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 550,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,600);
  TxTCPSM(&session,&packet);
  
  // counter = 5
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 600, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(350,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 600,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,650);
  TxTCPSM(&session,&packet);
  
  //dup ACK 1
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(350,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  TEST_ASSERT_EQUAL(1,session.dupAckCounter);
  
  //dup ACK 2
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(350,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  TEST_ASSERT_EQUAL(2,session.dupAckCounter);
  
  //dup ACK 3
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 650, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(350,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRetransmit,session.tcpState->state); 
  TEST_ASSERT_EQUAL(0,session.dupAckCounter);
}


/*
     Sender                Reciever
        | <------------------  | ack 100
     150| -------------------> | 
     200| -------------------> | 
        | <------------------  | dup ack 100
        | <------------------  | dup ack 100
        | <------------------  | dup ack 100
        |    Fast Retransmit   |
*/
void test_TxTCPSM_Congestion_Avoidance_3_dup_ack_of_100(){
  TCPSession session = {.offset = 250, .dupAckCounter = 1};
  Cwnd cwnd = {.size = 150, .offset = 100}; 
  TCP_state state = {.state = CongestionAvoidance};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  TEST_ASSERT_EQUAL(2,session.dupAckCounter);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(FastRetransmit,session.tcpState->state); 
  TEST_ASSERT_EQUAL(0,session.dupAckCounter);
}

/*
     Sender                Reciever
        | <------------------  | ack 100
     150| -------------------> | 
     200| -------------------> | 
        | <------------------  | dup ack 100
        | <------------------  | dup ack 100
        | <------------------  | ack 150
*/
void test_TxTCPSM_Congestion_Avoidance_2_dup_ack_of_100_and_3rd_recover_back_into_150(){
  TCPSession session = {.offset = 250, .dupAckCounter = 1};
  Cwnd cwnd = {.size = 150, .offset = 100}; 
  TCP_state state = {.state = CongestionAvoidance};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  TEST_ASSERT_EQUAL(2,session.dupAckCounter);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(150,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
  TEST_ASSERT_EQUAL(0,session.dupAckCounter);
}


