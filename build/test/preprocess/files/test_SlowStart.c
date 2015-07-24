#include "unity.h"
#include "mock_congestionWindow.h"
#include "mock_Timer.h"
#include "mock_Packet.h"
#include "SlowStart.h"
void setUp(void){}

void tearDown(void){}



void test_InitWindow_offset_and_size(){

    Cwnd cwnd;

   cwndInitWindow(&cwnd);



    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)13, UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)14, UNITY_DISPLAY_STYLE_INT);

}



void test_InitTCPState(){

    TCP_state state;

    initTCPState(&state);



    UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)21, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_and_in_sequence(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);





  cwndGetBeginningOffset_CMockExpectAndReturn(51, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(52, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(53, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(56, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(57, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(58, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)60, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)61, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)62, UNITY_DISPLAY_STYLE_INT);







  cwndGetDataBlock_CMockExpectAndReturn(66, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(67, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(70, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(71, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(74, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(75, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(76, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)78, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)79, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)80, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(82, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(83, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(86, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(87, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(90, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(91, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(92, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)94, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)95, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)96, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(98, &cwnd, 250, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(99, &packet, &state.ptrBlock, 300);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(102, &cwnd, 300, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(103, &packet, &state.ptrBlock, 350);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(106, &cwnd, 350, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(107, &packet, &receiveData, 200);

  cwndIncrementWindow_CMockExpectAndReturn(108, &cwnd, 200, 250);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)110, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)111, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)112, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_but_dup_ack(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);



  cwndGetBeginningOffset_CMockExpectAndReturn(136, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(137, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(138, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(141, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(142, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(143, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)145, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)146, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)147, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(149, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(150, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(152, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(153, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(156, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(157, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(158, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(161, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(162, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(164, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(165, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(169, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(170, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)172, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)173, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((CongestionAvoidance)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)174, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_but_over_ssthress(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);



  cwndGetBeginningOffset_CMockExpectAndReturn(204, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(205, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(206, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(209, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(210, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(211, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)213, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)214, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)215, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(217, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(218, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(221, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(222, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(225, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(226, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(227, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)229, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)230, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)231, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(233, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(234, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(237, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(238, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(241, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(242, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(243, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)245, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)246, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)247, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(249, &cwnd, 250, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(250, &packet, &state.ptrBlock, 300);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(253, &cwnd, 300, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(254, &packet, &state.ptrBlock, 350);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(257, &cwnd, 350, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(258, &packet, &receiveData, 200);

  cwndIncrementWindow_CMockExpectAndReturn(259, &cwnd, 200, 250);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)261, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)262, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)263, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(265, &cwnd, 350, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(266, &packet, &state.ptrBlock, 400);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(269, &cwnd, 400, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(270, &packet, &state.ptrBlock, 450);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(273, &cwnd, 450, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(274, &packet, &receiveData, 250);

  cwndIncrementWindow_CMockExpectAndReturn(275, &cwnd, 250, 350);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)277, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((350)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)278, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((CongestionAvoidance)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)279, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_3_dup_ack(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);



  cwndGetBeginningOffset_CMockExpectAndReturn(306, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(307, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(308, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(311, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(312, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(313, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)315, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)316, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)317, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(319, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(320, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(322, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(323, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(326, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(327, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(328, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(331, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(332, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(334, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(335, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(339, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(340, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)342, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)343, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((CongestionAvoidance)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)344, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(346, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(347, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(350, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(351, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)353, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)354, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((FastRetransmit)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)355, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_2_dup_ack(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);



  cwndGetBeginningOffset_CMockExpectAndReturn(381, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(382, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(383, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(386, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(387, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(388, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)390, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)391, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)392, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(394, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(395, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(397, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(398, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(401, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(402, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(403, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(406, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(407, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(409, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(410, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(414, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(415, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)417, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)418, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((CongestionAvoidance)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)419, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(421, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(422, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(425, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(426, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);





}
