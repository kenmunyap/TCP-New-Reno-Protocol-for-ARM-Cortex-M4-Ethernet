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

  cwndGetDataBlock_CMockExpectAndReturn(44, &cwnd, 0, 50, &Block, 50);

  sendDataPacket_CMockExpect(45, &packet, &Block, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)47, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(50, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(51, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(52, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)54, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)55, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)56, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(61, &cwnd, 50, 50, &Block, 50);

  sendDataPacket_CMockExpect(62, &packet, &Block, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)64, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(66, &cwnd, 100, 50, &Block, 50);

  sendDataPacket_CMockExpect(67, &packet, &Block, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)69, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(71, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(72, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(73, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)76, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)77, UNITY_DISPLAY_STYLE_INT);











  cwndGetDataBlock_CMockExpectAndReturn(83, &cwnd, 150, 50, &Block, 50);

  sendDataPacket_CMockExpect(84, &packet, &Block, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(87, &cwnd, 200, 50, &Block, 50);



  sendDataPacket_CMockExpect(89, &packet, &Block, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(92, &cwnd, 250, 50, &Block, 0);

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

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)125, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)126, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)129, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(132, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(133, &cwnd, 0, 50, &Block, 50);

  sendDataPacket_CMockExpect(134, &packet, &Block, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)136, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(138, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(139, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(140, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)142, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)143, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)144, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(149, &cwnd, 50, 50, &Block, 50);



  sendDataPacket_CMockExpect(151, &packet, &Block, 100);



  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)154, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(156, &cwnd, 100, 50, &Block, 50);



  sendDataPacket_CMockExpect(158, &packet, &Block, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)160, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(162, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(163, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(165, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(166, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(168, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(169, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

}

void test_get_and_send_3_segment_but_2nd_packet_fail_with_2_dup_ACK_3rd_receive_100ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)198, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)199, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)202, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(205, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(206, &cwnd, 0, 50, &Block, 50);

  sendDataPacket_CMockExpect(207, &packet, &Block, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)209, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(211, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(212, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(213, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)215, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)216, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)217, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(222, &cwnd, 50, 50, &Block, 50);

  sendDataPacket_CMockExpect(223, &packet, &Block, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)225, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(227, &cwnd, 100, 50, &Block, 50);

  sendDataPacket_CMockExpect(228, &packet, &Block, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)230, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(232, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(233, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(235, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(236, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(239, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(240, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(241, &cwnd, 100, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)243, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)244, UNITY_DISPLAY_STYLE_INT);

}
