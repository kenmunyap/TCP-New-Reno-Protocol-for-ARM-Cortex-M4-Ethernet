#include "unity.h"
#include "mock_congestionWindow.h"
#include "mock_Packet.h"
#include "SlowStart.h"
void setUp(void){}

void tearDown(void){}

void test_get_and_send_3_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){
<<<<<<< HEAD



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





  cwndGetDataBlock_CMockExpectAndReturn(45, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(46, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(47, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)49, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)50, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)51, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(56, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(57, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)59, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(61, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(62, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)64, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(66, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(67, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(68, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)70, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)71, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)72, UNITY_DISPLAY_STYLE_INT);











  cwndGetDataBlock_CMockExpectAndReturn(78, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(79, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(82, &cwnd, 200, 50, &state.ptrBlock, 50);



  sendDataPacket_CMockExpect(84, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(87, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(88, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(89, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)91, UNITY_DISPLAY_STYLE_INT);

}

void test_rement_size_and_offset(void){
=======
>>>>>>> 1e239bba04b69153fb30f2293d286086d15bd4d8



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};





<<<<<<< HEAD
 cwndInitWindow(&cwnd);

 initTCPState(&state);





  cwndGetBeginningOffset_CMockExpectAndReturn(115, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(116, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(117, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)119, UNITY_DISPLAY_STYLE_INT);





  cwndGetDataBlock_CMockExpectAndReturn(122, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(123, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(124, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)126, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)127, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)128, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(130, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(131, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)133, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(135, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(136, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)138, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(140, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(141, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(144, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(145, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(148, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(149, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



}





void test_rement_size_and_offsetqwq(void){



 Cwnd cwnd = {.size = 1150, .offset = 1150};

 TCP_state state = {.state = SlowStartWaitACK};

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};





 cwndInitWindow(&cwnd);
=======






 cwndInitWindow(&cwnd);


>>>>>>> 1e239bba04b69153fb30f2293d286086d15bd4d8

 initTCPState(&state);





<<<<<<< HEAD
  cwndGetBeginningOffset_CMockExpectAndReturn(167, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(168, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(169, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)171, UNITY_DISPLAY_STYLE_INT);
=======
  cwndGetBeginningOffset_CMockExpectAndReturn(39, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(40, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(41, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)43, UNITY_DISPLAY_STYLE_INT);
>>>>>>> 1e239bba04b69153fb30f2293d286086d15bd4d8





<<<<<<< HEAD
  cwndGetDataBlock_CMockExpectAndReturn(174, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(175, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(176, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)178, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)179, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)180, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(182, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(183, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)185, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(187, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(188, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)190, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(192, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(193, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(196, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(197, &packet, &receiveData, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(200, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(201, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(202, &cwnd, 100, 50);

  TxTCPSM(&state,&cwnd,&packet);




=======
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
>>>>>>> 1e239bba04b69153fb30f2293d286086d15bd4d8



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
