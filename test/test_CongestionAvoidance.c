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












/*
     Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
      50| -------------------> | 
     100| -------------------> | 
        | <------------------  | ack 100
     150| -------------------> | 
     200| -------------------> | 
        | <------------------  | dup ack 100
        | <------------------  | dup ack 100
        | <------------------  | ack 150
     250| ------------------>  | 
        | <------------------  | ack 200
     300| ------------------>  |
        | <-----------------   | ack 250
     350| ------------------>  | 
     400| ------------------>  |
*/
// void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_2_dup_ack_and_increment_one_rtt_for_one_full_ack(){
  // Cwnd cwnd;
	// TCP_state state;
	// uint32_t size;
  // Packet packet = {.srcIpAddr = 1};
  
  // cwndInitWindow(&cwnd);
	// initTCPState(&state);
  
  // cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  // TxTCPSM(&state,&cwnd,&packet); 

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(50,cwnd.offset);
  // TEST_ASSERT_EQUAL(100,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  // TxTCPSM(&state,&cwnd,&packet); 
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,100,150);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  // TxTCPSM(&state,&cwnd,&packet); 
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,200,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  //dup 1 time goes to congestion avoidance
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(100,cwnd.offset);
  // TEST_ASSERT_EQUAL(150,cwnd.size);
  // TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,300);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,300,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,200);
  // TxTCPSM(&state,&cwnd,&packet);

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,300,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,350);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,150,200);
  // TxTCPSM(&state,&cwnd,&packet);
// }

/*
     Sender                Reciever
      0 | -------------------> | 
        | <------------------  | 50(sequenceNumber) ACKed  
            increment 100
     50 | -------------------> | send pack 50 (ACKed)
     100| -------------------> | send pack 100(ACKed)
            increment 100
     150| -------------------> | send pack 150(ACKed)
     200| -------------------> | send pack 200(ACKed)
            increment 100
     250| -------------------> | send pack 250(ACKed)
     300| -------------------> | send pack 300
            increment 100
     350| -------------------> | send pack 350
     400| -------------------> | send pack 400
            increment 100
     450| -------------------> | send pack 450
     500| -------------------> | send pack 500  
        
        
*/

// void test_TxTCPSM_CongestionAvoidance_with_not_exited_RoundTripTime_but_over_ssthress(){
  // Cwnd cwnd;
	// TCP_state state;
	// uint32_t size;
  // Packet packet = {.srcIpAddr = 1};
  
  // cwndInitWindow(&cwnd);
	// initTCPState(&state);
  
  // cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  // TxTCPSM(&state,&cwnd,&packet); 

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(50,cwnd.offset);
  // TEST_ASSERT_EQUAL(100,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
 
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,100,150);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(100,cwnd.offset);
  // TEST_ASSERT_EQUAL(150,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);  
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,200,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,150,200);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(150,cwnd.offset);
  // TEST_ASSERT_EQUAL(200,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,300);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,300,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,350);
  // TxTCPSM(&state,&cwnd,&packet);

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,200);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,200,250);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(200,cwnd.offset);
  // TEST_ASSERT_EQUAL(250,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);  

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,400);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,400,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,450);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,450,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,250,300);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(250,cwnd.offset);
  // TEST_ASSERT_EQUAL(300,cwnd.size);
  // TEST_ASSERT_EQUAL(CongestionAvoidance,state.state); 

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,450,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,500);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,500,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,550);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,300);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,400);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,450);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,500);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,550);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,300,350);
  // TxTCPSM(&state,&cwnd,&packet);
// }

/*
*    Sender                Reciever
*     0 | -------------------> | 
*       | <------------------  | 50(sequenceNumber) ACKed  
*           increment 100
*    50 | -------------------> | send pack 50 (ACKed)
*    100| -------------------> | send pack 100(ACKed)
*           increment 100
*    150| -------------------> | send pack 150(ACKed)
*    200| -------------------> | send pack 200(ACKed)
*           increment 100
*    250| -------------------> | send pack 250
*    300| -------------------> | send pack 300
*           increment 100
*    350| -------------------> | send pack 350
*    400| -------------------> | send pack 400
*           increment 100
*    450| -------------------> | send pack 450
*    500| -------------------> | send pack 500    
*       |     SSTRESH          |
*     
*/

// void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_and_at_Congestion_avoidance_state_have_3_dupACK(){
  // Cwnd cwnd;
	// TCP_state state;
	// uint32_t size;
  // Packet packet = {.srcIpAddr = 1};
  
  // cwndInitWindow(&cwnd);
	// initTCPState(&state);

  // cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  // TxTCPSM(&state,&cwnd,&packet); 

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(50,cwnd.offset);
  // TEST_ASSERT_EQUAL(100,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
 
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,100,150);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(100,cwnd.offset);
  // TEST_ASSERT_EQUAL(150,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);  
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,200,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,150,200);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(150,cwnd.offset);
  // TEST_ASSERT_EQUAL(200,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,300);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,300,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,350);
  // TxTCPSM(&state,&cwnd,&packet);

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,200);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,200,250);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(200,cwnd.offset);
  // TEST_ASSERT_EQUAL(250,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);  

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,400);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,400,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,450);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,450,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,250,300);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(250,cwnd.offset);
  // TEST_ASSERT_EQUAL(300,cwnd.size);
  // TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,450,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,500);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,500,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,550);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(FastRetransmit,state.state);
// }

/*
*    Sender                Reciever
*     0 | -------------------> | 
*       | <------------------  | 50(sequenceNumber) ACKed  
*           increment 100
*    50 | -------------------> | send pack 50 (ACKed)
*    100| -------------------> | send pack 100(ACKed)
*           increment 100
*    150| -------------------> | send pack 150(ACKed)
*    200| -------------------> | send pack 200(ACKed)
*           increment 100
*    250| -------------------> | send pack 250(skiped)
*    300| -------------------> | send pack 300(ACKed)
*           increment 100
*    350| -------------------> | send pack 350
*    400| -------------------> | send pack 400
*           increment 100
*    450| -------------------> | send pack 450
*    500| -------------------> | send pack 500    
*       |     SSTRESH          |
*     
*/

// void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_not_in_sequence_ack(){
  // Cwnd cwnd;
	// TCP_state state;
	// uint32_t size;
  // Packet packet = {.srcIpAddr = 1};
  
  // cwndInitWindow(&cwnd);
	// initTCPState(&state);

  // cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  // TxTCPSM(&state,&cwnd,&packet); 

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(50,cwnd.offset);
  // TEST_ASSERT_EQUAL(100,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
 
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,100,150);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(100,cwnd.offset);
  // TEST_ASSERT_EQUAL(150,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);  
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,200,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,150,200);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(150,cwnd.offset);
  // TEST_ASSERT_EQUAL(200,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,300);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,300,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,350);
  // TxTCPSM(&state,&cwnd,&packet);

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,200);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,200,250);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(200,cwnd.offset);
  // TEST_ASSERT_EQUAL(250,cwnd.size);
  // TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);  

  // cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,400);
  // TxTCPSM(&state,&cwnd,&packet); 
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,400,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,450);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,450,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,250,300);
  // TxTCPSM(&state,&cwnd,&packet);
  // TEST_ASSERT_EQUAL(250,cwnd.offset);
  // TEST_ASSERT_EQUAL(300,cwnd.size);
  // TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,450,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,500);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,500,50,&state.ptrBlock,50);
  // sendDataPacket_Expect(&packet,&state.ptrBlock,550);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,400);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,450);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,500);
  // TxTCPSM(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  // getDataPacket_ExpectAndReturn(&packet,&receiveData,550);
  // cwndIncrementWindow_ExpectAndReturn(&cwnd,300,350);
  // TxTCPSM(&state,&cwnd,&packet);
// }


