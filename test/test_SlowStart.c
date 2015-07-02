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
  
  //wait ACK
  cwndGetDataBlock_ExpectAndReturn(&cwnd,50,50,&Block,0);
  
  TxData(&state,&cwnd,&packet);
  
  // cwndGetDataBlock_ExpectAndReturn(&cwnd,50,100,&Block,0);
  // TxData(&state,&cwnd);
  
}


