#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"

void setUp(void){}
void tearDown(void){}

void test_InitWindow_offset_and_size(){
    TCPSession session;
    Cwnd cwnd;
    session.cwnd = &cwnd;
   
  	cwndInitWindow(&session);
    
    TEST_ASSERT_EQUAL(0, session.cwnd->offset);
    TEST_ASSERT_EQUAL(50,session.cwnd->size);
}

void test_InitTCPState(){
    TCPSession session;
    TCP_state state;
    session.tcpState = &state;
    
    initTCPState(&session);
    
    TEST_ASSERT_EQUAL(SlowStart,session.tcpState->state);
}

/*
     Sender                Reciever
      0 | -------------------> |
*/
void test_TxTCPSM_SlowStart_initial_start_sending_packet_0(){
  TCPSession session;
  Cwnd cwnd;
  TCP_state state;
  session.cwnd = &cwnd;
  session.tcpState = &state;
	uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  cwndInitWindow(&session);
	initTCPState(&session);

  cwndGetBeginningOffset_ExpectAndReturn(session.cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,0,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,50);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(0,session.cwnd->offset);
  TEST_ASSERT_EQUAL(50,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state);  
}

/*
     Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
      50| -------------------> | send
     100| -------------------> | send
*/
void test_TxTCPSM_SlowStart_send_3packet_successfully(){
  TCPSession session;
  Cwnd cwnd;
  TCP_state state;
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
 
  
  cwndInitWindow(&session);
	initTCPState(&session);
  
  cwndGetBeginningOffset_ExpectAndReturn(session.cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,0,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,50);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(0,session.cwnd->offset);
  TEST_ASSERT_EQUAL(50,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 

  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 50, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,50,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,100);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150);
  TxTCPSM(&session,&packet);
}
  
/*
     Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
      50| -------------------> | send
     100| -------------------> | send
        | <------------------  |100(acked)
     150| ------------------>  |
     200| ------------------>  |
        | <-----------------   |100(dupAcked)
*/
void test_TxTCPSM_SlowStart_send_3packet_successfully_1st_and_2nd_successfully_ack_3rd_duplicate(){
  TCPSession session;
  Cwnd cwnd;
  TCP_state state;
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
 
  
  cwndInitWindow(&session);
	initTCPState(&session);
  
  cwndGetBeginningOffset_ExpectAndReturn(session.cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,0,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,50);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(0,session.cwnd->offset);
  TEST_ASSERT_EQUAL(50,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 

  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 50, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,50,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,100);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 150, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,100,150);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 150,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,200);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 200,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,250);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(100,session.cwnd->offset);
  TEST_ASSERT_EQUAL(150,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
}
  
  
/*
     Sender                Reciever
       | <------------------- |150(acked)
    250| -------------------> |
    300| -------------------> |
       | <------------------  |200(acked)
    350| ------------------>  |
    400| ------------------>  |
       | size 300 > ssthresh  | (goes to CongestionAvoidance)
    
*/
void test_TxTCPSM_SlowStart_send_from_150_but_congestion_window_size_larger_than_ssthresh_220(){
  TCPSession session = {.offset = 250};
  Cwnd cwnd = {.size = 150}; 
  TCP_state state = {.state = SlowStartWaitACK };
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250, session.requestedSize = 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,150,200);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(150,session.cwnd->offset);
  TEST_ASSERT_EQUAL(200,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 250,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,300);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 300,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,350);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 350, session.requestedSize = 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,200);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,200,250);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(200,session.cwnd->offset);
  TEST_ASSERT_EQUAL(250,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 350,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,400);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 400,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,450);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 450, session.requestedSize = 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,250,300);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(250,session.cwnd->offset);
  TEST_ASSERT_EQUAL(300,session.cwnd->size);
  TEST_ASSERT_EQUAL(CongestionAvoidance,session.tcpState->state); 
}
  
  
/*
     Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
      50| -------------------> | send
     100| -------------------> | send
        | <------------------  |150(acked)
     150| ------------------>  |
     200| ------------------>  |
     250| ------------------>  |
     300| ------------------>  |
        
*/
void test_TxTCPSM_SlowStart_send_3packet_successfully_but_ack_skipped_100_and_acked_150(){
  TCPSession session;
  Cwnd cwnd;
  TCP_state state;
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
 
  
  cwndInitWindow(&session);
	initTCPState(&session);
  
  cwndGetBeginningOffset_ExpectAndReturn(session.cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,0,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,50);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(0,session.cwnd->offset);
  TEST_ASSERT_EQUAL(50,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 

  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 50, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,50,100);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(50,session.cwnd->offset);
  TEST_ASSERT_EQUAL(100,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,50,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,100);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd,100,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150);
  TxTCPSM(&session,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(session.cwnd, 150, 50, &state.ptrBlock, 0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  cwndIncrementWindow_ExpectAndReturn(session.cwnd,100,200);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(150,session.cwnd->offset);
  TEST_ASSERT_EQUAL(200,session.cwnd->size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,session.tcpState->state); 
}




/*
    Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber)
      50| -------------------> | 
     100| ----------x          |
        | <------------------- |100(sequenceNumber) ACKed
        | <------------------  | 50(sequenceNumber) dupACKed
        | <------------------  | 50(sequenceNumber) dupACKed
        | <------------------  | 50(sequenceNumber) dupACKed/z
        |    fast retransmit   | 
 */


/*
      Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
      50| -------------------> | 
     100| -------------------> |
        |     x--------------- |100(sequenceNumber) ACKed     
        | <------------------  | 50(sequenceNumber) dupACKed
        | <------------------  | 50(sequenceNumber) dupACKed
        | <------------------  |100(sequenceNumber) ACKed
     150| -------------------> | 
 */



/*
      Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
      50| -------------------> | 
     100| -------------------> | 
        | <------------------- |150(sequenceNumber) ACKed
        | <------------------  |100(sequenceNumber) ACKed
     150| -------------------> |
     200| -------------------> |
     250| -------------------> |
     300| -------------------> |
 */

 


