#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "mock_Timer.h"
void setUp(void){}
void tearDown(void){}



//Congestion Avoidance 
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
        | <------------------  | dup ack 100
        |    Fast Retransmit   |
*/
void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_3_dup_ack(){
  Cwnd cwnd;
	TCP_state state;
	uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  cwndInitWindow(&cwnd);
	initTCPState(&state);
  
  cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&cwnd,&packet); 

  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
  TEST_ASSERT_EQUAL(100,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&cwnd,&packet); 
  cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,100,150);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  TxTCPSM(&state,&cwnd,&packet); 
  cwndGetDataBlock_ExpectAndReturn(&cwnd,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  TxTCPSM(&state,&cwnd,&packet); 
  
  //dup 1 time goes to congestion avoidance
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(100,cwnd.offset);
  TEST_ASSERT_EQUAL(150,cwnd.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(100,cwnd.offset);
  TEST_ASSERT_EQUAL(150,cwnd.size);
  TEST_ASSERT_EQUAL(FastRetransmit,state.state);
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
void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_2_dup_ack_and_increment_one_rtt_for_one_full_ack(){
  Cwnd cwnd;
	TCP_state state;
	uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  cwndInitWindow(&cwnd);
	initTCPState(&state);
  
  cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&cwnd,&packet); 

  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
  TEST_ASSERT_EQUAL(100,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&cwnd,&packet); 
  cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,100,150);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  TxTCPSM(&state,&cwnd,&packet); 
  cwndGetDataBlock_ExpectAndReturn(&cwnd,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  TxTCPSM(&state,&cwnd,&packet); 
  
  //dup 1 time goes to congestion avoidance
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(100,cwnd.offset);
  TEST_ASSERT_EQUAL(150,cwnd.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,300);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,300,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,200);
  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_ExpectAndReturn(&cwnd,300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,350);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,150,200);
  TxTCPSM(&state,&cwnd,&packet);
}

/*
     Sender                Reciever
      0 | -------------------> | 
        | <------------------  | 50(sequenceNumber) ACKed  
            increment 100
     50 | -------------------> | send pack 50 (done)
     100| -------------------> | send pack 100(done)
            increment 100
     150| -------------------> | send pack 150(done)
     200| -------------------> | send pack 200(done)
            increment 100
     250| -------------------> | send pack 250(done)
     300| -------------------> | send pack 300
            increment 100
     350| -------------------> | send pack 350
     400| -------------------> | send pack 400
            increment 100
     450| -------------------> | send pack 450
     500| -------------------> | send pack 500  
        
        
*/

void test_TxTCPSM_CongestionAvoidance_with_not_exited_RoundTripTime_but_over_ssthress(){
  Cwnd cwnd;
	TCP_state state;
	uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  cwndInitWindow(&cwnd);
	initTCPState(&state);
  
  cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,50);
  TxTCPSM(&state,&cwnd,&packet); 

  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
  TEST_ASSERT_EQUAL(100,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
 
  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,100);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,150);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,100,150);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(100,cwnd.offset);
  TEST_ASSERT_EQUAL(150,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);  
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,200);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,250);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,150,200);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(150,cwnd.offset);
  TEST_ASSERT_EQUAL(200,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,300);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,350);
  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,200);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,200,250);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(200,cwnd.offset);
  TEST_ASSERT_EQUAL(250,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);  

  cwndGetDataBlock_ExpectAndReturn(&cwnd,350,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,400);
  TxTCPSM(&state,&cwnd,&packet); 
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,400,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,450);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,450,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,250,300);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(250,cwnd.offset);
  TEST_ASSERT_EQUAL(300,cwnd.size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state); 

  cwndGetDataBlock_ExpectAndReturn(&cwnd,450,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,500);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,500,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,550);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,300);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,350);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,400);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,450);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,500);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,550);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,300,350);
  TxTCPSM(&state,&cwnd,&packet);
}


 


