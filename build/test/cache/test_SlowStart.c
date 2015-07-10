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

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)34, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)35, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)38, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(41, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(42, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(43, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)45, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(48, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(49, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(50, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)52, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)53, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)54, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(59, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(60, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)62, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(64, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(65, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)67, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(69, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(70, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(71, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)73, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)74, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_INT);











  cwndGetDataBlock_CMockExpectAndReturn(81, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(82, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(85, &cwnd, 200, 50, &state.ptrBlock, 50);



  sendDataPacket_CMockExpect(87, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(90, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(91, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(92, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)94, UNITY_DISPLAY_STYLE_INT);

}

void test_get_and_send_3_segment_but_2nd_packet_fail_to_ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;

  int i;











 cwndInitWindow(&cwnd);

  initTCPState(&state);





  cwndGetBeginningOffset_CMockExpectAndReturn(127, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(128, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(129, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(133, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(134, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(135, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);











  cwndGetDataBlock_CMockExpectAndReturn(142, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(143, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);



  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)146, UNITY_DISPLAY_STYLE_INT);

  cwndGetDataBlock_CMockExpectAndReturn(147, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(148, &packet, &state.ptrBlock, 150);

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

void test_gepacket_fail_with_2_dup_ACK_3rd_receive_100ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)263, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)264, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)267, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(270, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(271, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(272, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)274, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(276, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(277, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(278, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)280, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)281, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)282, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(287, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(288, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)290, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(292, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(293, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)295, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(297, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(298, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(299, &cwnd, 100, 200);

  TxTCPSM(&state,&cwnd,&packet);

 UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)301, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)302, UNITY_DISPLAY_STYLE_INT);

}
