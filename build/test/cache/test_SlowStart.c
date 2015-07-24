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



    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)15, UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)16, UNITY_DISPLAY_STYLE_INT);

}



void test_InitTCPState(){

    TCP_state state;

    initTCPState(&state);



    UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)23, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_and_in_sequence(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);





  cwndGetBeginningOffset_CMockExpectAndReturn(53, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(54, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(55, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(58, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(59, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(60, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)62, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)63, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)64, UNITY_DISPLAY_STYLE_INT);







  cwndGetDataBlock_CMockExpectAndReturn(68, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(69, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(72, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(73, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(76, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(77, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(78, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)80, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)81, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)82, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(84, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(85, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(88, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(89, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(92, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(93, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(94, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)96, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)97, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)98, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(100, &cwnd, 250, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(101, &packet, &state.ptrBlock, 300);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(104, &cwnd, 300, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(105, &packet, &state.ptrBlock, 350);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(108, &cwnd, 350, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(109, &packet, &receiveData, 200);

  cwndIncrementWindow_CMockExpectAndReturn(110, &cwnd, 200, 250);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)112, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)113, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)114, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_but_dup_ack(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);



  cwndGetBeginningOffset_CMockExpectAndReturn(138, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(139, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(140, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(143, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(144, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(145, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)147, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)148, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)149, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(151, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(152, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(154, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(155, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(158, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(159, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(160, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(163, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(164, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(166, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(167, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(171, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(172, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)174, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)175, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((CongestionAvoidance)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)176, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_but_over_ssthress(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);



  cwndGetBeginningOffset_CMockExpectAndReturn(206, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(207, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(208, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(211, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(212, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(213, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)215, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)216, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)217, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(219, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(220, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(223, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(224, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(227, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(228, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(229, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)231, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)232, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)233, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(235, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(236, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(239, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(240, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(243, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(244, &packet, &receiveData, 150);

  cwndIncrementWindow_CMockExpectAndReturn(245, &cwnd, 150, 200);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)247, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)248, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)249, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(251, &cwnd, 250, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(252, &packet, &state.ptrBlock, 300);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(255, &cwnd, 300, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(256, &packet, &state.ptrBlock, 350);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(259, &cwnd, 350, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(260, &packet, &receiveData, 200);

  cwndIncrementWindow_CMockExpectAndReturn(261, &cwnd, 200, 250);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)263, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)264, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)265, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(267, &cwnd, 350, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(268, &packet, &state.ptrBlock, 400);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(271, &cwnd, 400, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(272, &packet, &state.ptrBlock, 450);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(275, &cwnd, 450, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(276, &packet, &receiveData, 250);

  cwndIncrementWindow_CMockExpectAndReturn(277, &cwnd, 250, 350);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)279, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((350)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)280, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((CongestionAvoidance)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)281, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_3_dup_ack(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);



  cwndGetBeginningOffset_CMockExpectAndReturn(308, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(309, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(310, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(313, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(314, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(315, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)317, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)318, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)319, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(321, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(322, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(324, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(325, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(328, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(329, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(330, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(333, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(334, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(336, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(337, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(341, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(342, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)344, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)345, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((CongestionAvoidance)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)346, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(348, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(349, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(352, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(353, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)355, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)356, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((FastRetransmit)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)357, UNITY_DISPLAY_STYLE_INT);

}

void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_2_dup_ack(){

  Cwnd cwnd;

 TCP_state state;

 uint32_t size;

  Packet packet = {.srcIpAddr = 1};



  cwndInitWindow(&cwnd);

 initTCPState(&state);



  cwndGetBeginningOffset_CMockExpectAndReturn(383, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(384, &cwnd, 0, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(385, &packet, &state.ptrBlock, 50);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(388, &cwnd, 50, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(389, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(390, &cwnd, 50, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)392, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)393, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)394, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(396, &cwnd, 50, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(397, &packet, &state.ptrBlock, 100);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(399, &cwnd, 100, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(400, &packet, &state.ptrBlock, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(403, &cwnd, 150, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(404, &packet, &receiveData, 100);

  cwndIncrementWindow_CMockExpectAndReturn(405, &cwnd, 100, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(408, &cwnd, 150, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(409, &packet, &state.ptrBlock, 200);

  TxTCPSM(&state,&cwnd,&packet);

  cwndGetDataBlock_CMockExpectAndReturn(411, &cwnd, 200, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(412, &packet, &state.ptrBlock, 250);

  TxTCPSM(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(416, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(417, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)419, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((150)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)420, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((CongestionAvoidance)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)421, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(423, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(424, &packet, &receiveData, 100);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(427, &cwnd, 250, 50, &state.ptrBlock, 0);

  getDataPacket_CMockExpectAndReturn(428, &packet, &receiveData, 150);

  TxTCPSM(&state,&cwnd,&packet);



  cwndGetDataBlock_CMockExpectAndReturn(431, &cwnd, 250, 50, &state.ptrBlock, 50);

  sendDataPacket_CMockExpect(432, &packet, &state.ptrBlock, 300);

  TxTCPSM(&state,&cwnd,&packet);





}
