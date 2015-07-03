#include "unity.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "SlowStart.h"
void setUp(void){}

void tearDown(void){}

void test_get_and_send_1_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};

  int i;





 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)25, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)26, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)29, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(32, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(33, &cwnd, 0, 50, &Block, 50);

  for(i=0;i<50;i++){

    sendDataPacket_CMockExpect(35, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)38, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(41, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(42, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(43, &cwnd, 50, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)45, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)46, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)47, UNITY_DISPLAY_STYLE_INT);

}

void test_get_and_send_3_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};

  int i;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)77, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)78, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)81, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(84, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(85, &cwnd, 0, 50, &Block, 50);

  for(i=0;i<50;i++){

    sendDataPacket_CMockExpect(87, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)90, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(92, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(93, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(94, &cwnd, 50, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)96, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)97, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)98, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(103, &cwnd, 50, 50, &Block, 50);

  for(i=50;i<100;i++){

    sendDataPacket_CMockExpect(105, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)108, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(110, &cwnd, 100, 50, &Block, 50);

  for(i=100;i<150;i++){

    sendDataPacket_CMockExpect(112, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)115, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(117, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(118, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(119, &cwnd, 100, 150);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)121, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)122, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)123, UNITY_DISPLAY_STYLE_INT);











  cwndGetDataBlock_CMockExpectAndReturn(129, &cwnd, 150, 50, &Block, 50);

  for(i=150;i<200;i++){

    sendDataPacket_CMockExpect(131, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(135, &cwnd, 200, 50, &Block, 50);

  for(i=200;i<250;i++){

    sendDataPacket_CMockExpect(137, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(141, &cwnd, 250, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(142, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(143, &cwnd, 150, 200);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)145, UNITY_DISPLAY_STYLE_INT);

}

void test_get_and_send_3_segment_but_2nd_packet_fail_to_ACK(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet;

  int i;











 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)174, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)175, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)178, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(181, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(182, &cwnd, 0, 50, &Block, 50);

  for(i=0;i<50;i++){

    sendDataPacket_CMockExpect(184, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)187, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(189, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(190, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(191, &cwnd, 50, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)193, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)194, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)195, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(200, &cwnd, 50, 50, &Block, 50);

  for(i=50;i<100;i++){

    sendDataPacket_CMockExpect(202, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)205, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(207, &cwnd, 100, 50, &Block, 50);

  for(i=100;i<150;i++){

    sendDataPacket_CMockExpect(209, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)212, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(214, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(215, &packet, &receiveData, 50);

  TxData(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(217, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(218, &packet, &receiveData, 50);

  TxData(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(220, &cwnd, 150, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(221, &packet, &receiveData, 50);

  TxData(&state,&cwnd,&packet);

}
