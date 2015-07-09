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

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)35, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)36, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)39, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(42, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(43, &cwnd, 0, 50, &Block, 50);

  sendDataPacket_CMockExpect(44, &packet, &Block, 50);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)46, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(49, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(50, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(51, &cwnd, 50, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)53, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)54, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)55, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(60, &cwnd, 50, 50, &Block, 50);

  sendDataPacket_CMockExpect(61, &packet, &Block, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)63, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(65, &cwnd, 100, 50, &Block, 50);

  sendDataPacket_CMockExpect(66, &packet, &Block, 150);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)68, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(70, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(71, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(72, &cwnd, 100, 150);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)74, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)76, UNITY_DISPLAY_STYLE_INT);











  cwndGetDataBlock_CMockExpectAndReturn(82, &cwnd, 150, 50, &Block, 50);

  sendDataPacket_CMockExpect(83, &packet, &Block, 200);

  TxData(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(86, &cwnd, 200, 50, &Block, 50);



  sendDataPacket_CMockExpect(88, &packet, &Block, 250);

  TxData(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(91, &cwnd, 250, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(92, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(93, &cwnd, 150, 200);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)95, UNITY_DISPLAY_STYLE_INT);

}

void test_get_and_send_3_segment_but_2nd_packet_fail_to_ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;

  int i;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)124, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)125, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)128, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(131, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(132, &cwnd, 0, 50, &Block, 50);

  sendDataPacket_CMockExpect(133, &packet, &Block, 50);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)135, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(137, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(138, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(139, &cwnd, 50, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)141, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)142, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)143, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(148, &cwnd, 50, 50, &Block, 50);



  sendDataPacket_CMockExpect(150, &packet, &Block, 100);



  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)153, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(155, &cwnd, 100, 50, &Block, 50);



  sendDataPacket_CMockExpect(157, &packet, &Block, 150);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)159, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(161, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(162, &packet, &receiveData, 50);

  TxData(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(164, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(165, &packet, &receiveData, 50);

  TxData(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(167, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(168, &packet, &receiveData, 50);

  TxData(&state,&cwnd,&packet);

}

void test_get_and_send_3_segment_but_2nd_packet_fail_with_2_dup_ACK_3rd_receive_100ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)197, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)198, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)201, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(204, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(205, &cwnd, 0, 50, &Block, 50);

  sendDataPacket_CMockExpect(206, &packet, &Block, 50);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)208, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(210, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(211, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(212, &cwnd, 50, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)214, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)215, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)216, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(221, &cwnd, 50, 50, &Block, 50);

  sendDataPacket_CMockExpect(222, &packet, &Block, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)224, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(226, &cwnd, 100, 50, &Block, 50);

  sendDataPacket_CMockExpect(227, &packet, &Block, 150);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)229, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(231, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(232, &packet, &receiveData, 50);

  TxData(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(234, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(235, &packet, &receiveData, 50);

  TxData(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(238, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(239, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(240, &cwnd, 100, 50);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)242, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)243, UNITY_DISPLAY_STYLE_INT);

}
