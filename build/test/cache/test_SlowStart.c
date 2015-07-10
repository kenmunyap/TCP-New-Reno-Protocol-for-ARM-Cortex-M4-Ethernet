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

  int i;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)36, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)37, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)40, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(43, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(44, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(45, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)47, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(50, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(51, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(52, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)54, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)55, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)56, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(61, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(62, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)64, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(66, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(67, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)69, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(71, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(72, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(73, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)76, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)77, UNITY_DISPLAY_STYLE_INT);











  cwndGetDataBlock_CMockExpectAndReturn(83, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(84, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(87, &cwnd, 200, 50, &state.ptrBlock, 50);



  sendDataPacket_CMockExpect(89, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(92, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(93, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(94, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)96, UNITY_DISPLAY_STYLE_INT);

}

void test_get_and_send_3_segment_but_2nd_packet_fail_to_ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;

  int i;











 cwndInitWindow(&cwnd);

  initTCPState(&state);





  cwndGetBeginningOffset_CMockExpectAndReturn(128, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(129, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(130, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(134, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(135, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(136, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);











  cwndGetDataBlock_CMockExpectAndReturn(143, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(144, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(147, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(148, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(151, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(152, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(154, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(155, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(157, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(158, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

}

void test_get_and_send_3_segment_but_2nd_packet_fail_with_2_dup_ACK_3rd_receive_100ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)187, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)188, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)191, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(194, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(195, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(196, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)198, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(200, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(201, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(202, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)204, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)205, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)206, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(211, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(212, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)214, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(216, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(217, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)219, UNITY_DISPLAY_STYLE_INT);

}
