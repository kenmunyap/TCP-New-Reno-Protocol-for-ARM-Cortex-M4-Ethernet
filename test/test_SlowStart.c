#include "unity.h"
#include "SlowStart.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
void setUp(void){}
void tearDown(void){}

/*
      Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
     100| -------------------> |
     150| -------------------> |
 */
void test_get_and_send_1_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){

	Cwnd cwnd;
	TCP_state state;
	uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  int i;
  
  // initialize window and state;
	cwndInitWindow(&cwnd);
	TEST_ASSERT_EQUAL(0,cwnd.offset);
	TEST_ASSERT_EQUAL(50,cwnd.size);
	
	initTCPState(&state);
	TEST_ASSERT_EQUAL(SlowStart,state.state);

  //start sending
  cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&Block,50);
  for(i=0;i<50;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  //wait ACK and Receive ACK
  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
  TEST_ASSERT_EQUAL(100,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
}


/*
      Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
      50| -------------------> | 
     100| -------------------> |
        | <------------------- |100(sequenceNumber) ACKed
     150| -------------------> |
     200| -------------------> |
        | <------------------  |150(sequenceNumber) ACKed
     250| -------------------> |
     300| -------------------> |
 */
void test_get_and_send_3_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){

	Cwnd cwnd;
	TCP_state state;
	uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  int i;
 
//=================================================
/* 1st Pack Send And CWND increase 2*/
//=================================================
  // initialize window and state;
	cwndInitWindow(&cwnd);
	TEST_ASSERT_EQUAL(0,cwnd.offset);
	TEST_ASSERT_EQUAL(50,cwnd.size);
	
	initTCPState(&state);
	TEST_ASSERT_EQUAL(SlowStart,state.state);

  //start sending
  cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&Block,50);
  for(i=0;i<50;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  //wait ACK and Receive ACK
  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
  TEST_ASSERT_EQUAL(100,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
//================================================================
/* 2nd and 3rd Pack Send And return 2nd ACK with increase 2 CWND*/
//================================================================

  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&Block,50);
  for(i=50;i<100;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&Block,50);
  for(i=100;i<150;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,100,150);
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(100,cwnd.offset);
  TEST_ASSERT_EQUAL(150,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
//================================================================
/* 4th and 5th Pack Send And return 3rd ACK with increase 2 CWND*/
//================================================================

  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&Block,50);
  for(i=150;i<200;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,200,50,&Block,50);
  for(i=200;i<250;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,250,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,150);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,150,200);
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(200,cwnd.size); 
}

/*
      Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
      50| -------------------> | 
     100| -------------------> |
        |     x--------------- |100(sequenceNumber) ACKed
        | <------------------  | 50(sequenceNumber) dupACKed
        | <------------------  | 50(sequenceNumber) dupACKed
        | <------------------  | 50(sequenceNumber) dupACKed
        |    fast retransmit   | 
 */
void test_get_and_send_3_segment_but_2nd_packet_fail_to_ACK(void){

	Cwnd cwnd;
	TCP_state state;
	uint32_t size;
  Packet packet;
  int i;
 
//=================================================
/* 1st Pack Send And CWND increase 2*/
//=================================================
  // initialize window and state;
	cwndInitWindow(&cwnd);
	TEST_ASSERT_EQUAL(0,cwnd.offset);
	TEST_ASSERT_EQUAL(50,cwnd.size);
	
	initTCPState(&state);
	TEST_ASSERT_EQUAL(SlowStart,state.state);

  //start sending
  cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&Block,50);
  for(i=0;i<50;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  //wait ACK and Receive ACK
  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
  TEST_ASSERT_EQUAL(100,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
//================================================================
/* 2nd and 3rd Pack Send And Fail to return 2nd ACK*/
//================================================================

  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&Block,50);
  for(i=50;i<100;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&Block,50);
  for(i=100;i<150;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  TxData(&state,&cwnd,&packet);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  TxData(&state,&cwnd,&packet);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  TxData(&state,&cwnd,&packet);
}

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
void test_get_and_send_3_segment_but_2nd_packet_fail_with_2_dup_ACK_3rd_receive_100ACK(void){

	Cwnd cwnd;
	TCP_state state;
	uint32_t size;
  Packet packet;
  int i;
 
//=================================================
/* 1st Pack Send And CWND increase 2*/
//=================================================
  // initialize window and state;
	cwndInitWindow(&cwnd);
	TEST_ASSERT_EQUAL(0,cwnd.offset);
	TEST_ASSERT_EQUAL(50,cwnd.size);
	
	initTCPState(&state);
	TEST_ASSERT_EQUAL(SlowStart,state.state);

  //start sending
  cwndGetBeginningOffset_ExpectAndReturn(&cwnd,0);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,0,50,&Block,50);
  for(i=0;i<50;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  //wait ACK and Receive ACK
  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,50,100);
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(50,cwnd.offset);
  TEST_ASSERT_EQUAL(100,cwnd.size);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
//================================================================
/* 2nd and 3rd Pack Send And dupACK 50 2 time and 3rd ACKed 100*/
//================================================================

  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&Block,50);
  for(i=50;i<100;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,100,50,&Block,50);
  for(i=100;i<150;i++){
    sendDataPacket_Expect(&packet,&Block,i);
  }
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(SlowStartWaitACK,state.state);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  TxData(&state,&cwnd,&packet);
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,50);
  TxData(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,150,50,&Block,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,100);
  cwndIncrementWindow_ExpectAndReturn(&cwnd,100,50);
  TxData(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(100,cwnd.offset);
  TEST_ASSERT_EQUAL(50,cwnd.size);  
}


/*
      Sender                Reciever
      0 | -------------------> |
        | <------------------  | 50(sequenceNumber) ACKed
     100| -------------------> | //send 100 1st
      50| -------------------> | //late send 50
        | <------------------- |100(sequenceNumber) ACKed
     150| -------------------> |
     200| -------------------> |
        | <------------------  |150(sequenceNumber) ACKed
     250| -------------------> |
     300| -------------------> |
 */

 
 
 


