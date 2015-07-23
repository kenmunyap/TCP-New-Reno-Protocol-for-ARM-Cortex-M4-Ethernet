#include "unity.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "SlowStart.h"
void setUp(void){}

void tearDown(void){}

void test_rement_size_and_offsetqwq(void){



 Cwnd cwnd = {.size = 1150, .offset = 1150};

 TCP_state state = {.state = SlowStartWaitACK};

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};





 cwndInitWindow(&cwnd);

 initTCPState(&state);





  cwndGetBeginningOffset_CMockExpectAndReturn(168, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(169, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(170, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)172, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(175, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(176, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(177, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)179, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)180, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)181, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(183, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(184, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)186, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(188, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(189, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)191, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(193, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(194, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(197, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(198, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(201, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(202, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(205, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(206, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(209, &cwnd, 200, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(210, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(211, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);









}
