#include "unity.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "SlowStart.h"
void setUp(void){}

void tearDown(void){}

void test_rement_size_and_offset(void){



 Cwnd cwnd = {.size = 1150, .offset = 1150};

 TCP_state state = {.state = SlowStartWaitACK};

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndGetDataBlock_CMockExpectAndReturn(113, &cwnd, 1150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(114, &packet, &state.ptrBlock, 1200);

  TxTCPSM(&state,&cwnd,&packet);





}
