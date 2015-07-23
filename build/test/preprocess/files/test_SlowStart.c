#include "unity.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "SlowStart.h"
void setUp(void){}

void tearDown(void){}

void test_get_and_send_3_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};











 cwndInitWindow(&cwnd);



 initTCPState(&state);





  cwndGetBeginningOffset_CMockExpectAndReturn(39, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(40, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(41, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)43, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(46, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(47, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(48, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)50, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)51, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)52, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(57, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(58, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)60, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(62, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(63, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)65, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(67, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(68, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(69, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)71, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)72, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)73, UNITY_DISPLAY_STYLE_INT);











  cwndGetDataBlock_CMockExpectAndReturn(79, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(80, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(83, &cwnd, 200, 50, &state.ptrBlock, 50);



  sendDataPacket_CMockExpect(85, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(88, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(89, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(90, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)92, UNITY_DISPLAY_STYLE_INT);

}

void test_rement_size_and_offset(void){



 Cwnd cwnd = {.size = 1150, .offset = 1150};

 TCP_state state = {.state = SlowStartWaitACK};

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



}
