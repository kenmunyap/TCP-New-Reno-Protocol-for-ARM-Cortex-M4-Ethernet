#include "unity.h"
#include "TxTCP.h"
#include "TCPhelper.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
void setUp(void){}
void tearDown(void){}


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
    500 | -------------------> | 500    
        | <------------------- | 250    
        | <------------------- | 250      
        | <------------------- | 250    
*       |     SSTRESH          |
*retran | -------------------> | (100)
*/  

void test_TxTCPSM_Fast_Recover_200_and_get_550_ack_that_is_fully_ack(){
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(FastRetransmit,state.state);
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,300);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  TEST_ASSERT_EQUAL(150,cwnd.ssthresh);
  TEST_ASSERT_EQUAL(300,cwnd.size);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,550);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(CongestionAvoidance,state.state);
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
    500 | -------------------> | 500    
        | <------------------- | 250    
        | <------------------- | 250      
        | <------------------- | 250    
*       |     SSTRESH          |
*retran | -------------------> | (100)
        | <------------------  |250
    600 | -------------------> |
*/  

void test_TxTCPSM_Fast_Recover_200_but_still_duplicate_250(){
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
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  TxTCPSM(&state,&cwnd,&packet);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(FastRetransmit,state.state);
  
  sendDataPacket_Expect(&packet,&state.ptrBlock,300);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(FastRecovery,state.state);
  TEST_ASSERT_EQUAL(150,cwnd.ssthresh);
  TEST_ASSERT_EQUAL(300,cwnd.size);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,0);
  getDataPacket_ExpectAndReturn(&packet,&receiveData,250);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(350,cwnd.size);
  
  cwndGetDataBlock_ExpectAndReturn(&cwnd,550,50,&state.ptrBlock,50);
  sendDataPacket_Expect(&packet,&state.ptrBlock,600);
  TxTCPSM(&state,&cwnd,&packet);
  TEST_ASSERT_EQUAL(350,cwnd.size);
  TEST_ASSERT_EQUAL(600,cwnd.recover);

  
}
 
 


