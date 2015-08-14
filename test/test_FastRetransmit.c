#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "linkedList.h"

void setUp(void){}
void tearDown(void){}


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
        |    retransmit 100    |
*/
void test_TxTCPSM_Fast_Retransmit_with_retransmit_sequnceNumber_100(){
  
  TCPSession session = {.offset = 250, .dupAckCounter = 0};
  Cwnd cwnd = {.size = 150, .offset = 100, .lostPacket = 100}; 
  TCP_state state = {.state = FastRetransmit};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,150);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  TEST_ASSERT_EQUAL(100,session.cwnd->ssthresh); 
  TEST_ASSERT_EQUAL(350,session.cwnd->recover); 
  TEST_ASSERT_EQUAL(250,session.cwnd->size); 
}

/*
*    Sender                Reciever
*     0 | -------------------> | 
*       | <------------------  | 50(sequenceNumber) ACKed  
     50 | -------------------> | 
    100 | -------------------> | 
        | <------------------- | 100 
    150 | -------------------> |  
    200 | -------------------> |  
        | <------------------- | 150 
    250 | -------------------> |  
    300 | -------------------> |    
        | <------------------- | 200 
    350 | -------------------> |  
    400 | -------------------> |    
        | <------------------- | 250  
    450 | -------------------> |  
    500 | -------------------> |     
        | <------------------- | 250    
        | <------------------- | 250      
        | <------------------- | 250    
*       |     SSTRESH          |
*retran | -------------------> | (300)
*/  

void test_TxTCPSM_Fast_Retransmit_sequnceNumber_200(){
  TCPSession session = {.offset = 500, .dupAckCounter = 0};
  Cwnd cwnd = {.size = 250, .offset = 300, .lostPacket = 250}; 
  TCP_state state = {.state = FastRetransmit};
  session.cwnd = &cwnd;
  session.tcpState = &state;
  uint32_t size;
  Packet packet = {.srcIpAddr = 1};
  
  sendDataPacket_Expect(&packet,&session.tcpState->ptrBlock,300);
  TxTCPSM(&session,&packet);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  TEST_ASSERT_EQUAL(100,session.cwnd->ssthresh); 
  TEST_ASSERT_EQUAL(550,session.cwnd->recover); 
  TEST_ASSERT_EQUAL(250,session.cwnd->size); 
  
}




