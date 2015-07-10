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

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)46, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(49, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(50, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(51, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)53, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)54, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)55, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(60, &cwnd, 50, 50, &Block, 50);

  sendDataPacket_CMockExpect(61, &packet, &Block, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)63, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(65, &cwnd, 100, 50, &Block, 50);

  sendDataPacket_CMockExpect(66, &packet, &Block, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)68, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(70, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(71, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(72, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)74, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)75, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)76, UNITY_DISPLAY_STYLE_INT);











  cwndGetDataBlock_CMockExpectAndReturn(82, &cwnd, 150, 50, &Block, 50);

  sendDataPacket_CMockExpect(83, &packet, &Block, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(86, &cwnd, 200, 50, &Block, 50);



  sendDataPacket_CMockExpect(88, &packet, &Block, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(91, &cwnd, 250, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(92, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(93, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)95, UNITY_DISPLAY_STYLE_INT);

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

  sendDataPacket_CMockExpect(150, &packet, &Block, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)152, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(154, &cwnd, 100, 50, &Block, 50);

  sendDataPacket_CMockExpect(155, &packet, &Block, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)157, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(159, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(160, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(162, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(163, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(165, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(166, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

}

void test_get_and_send_3_segment_but_2nd_packet_fail_with_2_dup_ACK_3rd_receive_100ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)195, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)196, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)199, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(202, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(203, &cwnd, 0, 50, &Block, 50);

  sendDataPacket_CMockExpect(204, &packet, &Block, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)206, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(208, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(209, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(210, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)212, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)213, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)214, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(219, &cwnd, 50, 50, &Block, 50);

  sendDataPacket_CMockExpect(220, &packet, &Block, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)222, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(224, &cwnd, 100, 50, &Block, 50);

  sendDataPacket_CMockExpect(225, &packet, &Block, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)227, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(229, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(230, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(232, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(233, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(236, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(237, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(238, &cwnd, 100, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)240, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)241, UNITY_DISPLAY_STYLE_INT);

}

void test_gepacket_fail_with_2_dup_ACK_3rd_receive_100ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)271, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)272, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)275, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(278, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(279, &cwnd, 0, 50, &Block, 50);

  sendDataPacket_CMockExpect(280, &packet, &Block, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)282, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(284, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(285, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(286, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)288, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)289, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)290, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(295, &cwnd, 50, 50, &Block, 50);

  sendDataPacket_CMockExpect(296, &packet, &Block, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)298, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(300, &cwnd, 100, 50, &Block, 50);

  sendDataPacket_CMockExpect(301, &packet, &Block, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)303, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(305, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(306, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(307, &cwnd, 100, 200);

  TxTCPSM(&state,&cwnd,&packet);

 UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)309, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)310, UNITY_DISPLAY_STYLE_INT);

}
