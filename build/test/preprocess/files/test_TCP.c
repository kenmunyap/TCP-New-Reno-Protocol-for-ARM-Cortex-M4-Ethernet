#include "unity.h"
#include "mock_timeOutTimer.h"
#include "mock_sequenceNumber.h"
#include "mock_congestionWindow.h"
#include "mock_ack.h"
#include "mock_Data.h"
#include "TCP.h"


void setUp(void){}



void tearDown(void){}



void test_successful_transmit_1st_packet(){

 SS_state state;

 DataBlock data;



 sequenceNumber_CMockExpectAndReturn(17, 0);

 sequenceNumber_CMockExpectAndReturn(18, 0);

 initialSequnceNumber(&data);



 initalState(&state);



 congestionWindow_CMockExpectAndReturn(23, 1);

 countTimer_CMockExpectAndReturn(24, 0);

 sendData2RCV_CMockExpectAndReturn(25, 1);

 sequenceNumber_CMockExpectAndReturn(26, 0);

 countTimer_CMockExpectAndReturn(27, 50);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((Acknowledge)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)29, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((data.nextseqnum[0])), (((void *)0)), (_U_UINT)30, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)31, UNITY_DISPLAY_STYLE_INT);



 congestionWindow_CMockExpectAndReturn(33, 1);

 recieveRxACK_CMockExpectAndReturn(34, 500);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((SendData)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)36, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)37, UNITY_DISPLAY_STYLE_INT);



}



void test_successful_transmit_2_packet(){

 SS_state state;

 DataBlock data;



 sequenceNumber_CMockExpectAndReturn(45, 0);

 sequenceNumber_CMockExpectAndReturn(46, 0);

 initialSequnceNumber(&data);



 initalState(&state);



 congestionWindow_CMockExpectAndReturn(51, 1);

 countTimer_CMockExpectAndReturn(52, 0);

 sendData2RCV_CMockExpectAndReturn(53, 1);

 sequenceNumber_CMockExpectAndReturn(54, 0);

 countTimer_CMockExpectAndReturn(55, 50);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((Acknowledge)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)57, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((data.nextseqnum[0])), (((void *)0)), (_U_UINT)58, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)59, UNITY_DISPLAY_STYLE_INT);



 congestionWindow_CMockExpectAndReturn(61, 1);

 recieveRxACK_CMockExpectAndReturn(62, 500);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((SendData)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)64, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)65, UNITY_DISPLAY_STYLE_INT);



 congestionWindow_CMockExpectAndReturn(67, 2);

 countTimer_CMockExpectAndReturn(68, 100);

 sendData2RCV_CMockExpectAndReturn(69, 1);

 sequenceNumber_CMockExpectAndReturn(70, 500);

 countTimer_CMockExpectAndReturn(71, 150);

 sendData2RCV_CMockExpectAndReturn(72, 1);

 sequenceNumber_CMockExpectAndReturn(73, 1000);

 countTimer_CMockExpectAndReturn(74, 200);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((Acknowledge)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)76, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((data.nextseqnum[0])), (((void *)0)), (_U_UINT)77, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1500)), (_U_SINT)((data.nextseqnum[1])), (((void *)0)), (_U_UINT)78, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)79, UNITY_DISPLAY_STYLE_INT);



 congestionWindow_CMockExpectAndReturn(81, 2);

 recieveRxACK_CMockExpectAndReturn(82, 1000);

 recieveRxACK_CMockExpectAndReturn(83, 1500);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((SendData)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)85, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1500)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)86, UNITY_DISPLAY_STYLE_INT);

}



void test_1st_and_2nd_pack_successful_send_without_time_out_3rd_time_out(){



 SS_state state;

 DataBlock data;



 sequenceNumber_CMockExpectAndReturn(94, 0);

 sequenceNumber_CMockExpectAndReturn(95, 0);

 initialSequnceNumber(&data);



 initalState(&state);



 congestionWindow_CMockExpectAndReturn(100, 1);

 countTimer_CMockExpectAndReturn(101, 0);

 sendData2RCV_CMockExpectAndReturn(102, 1);

 sequenceNumber_CMockExpectAndReturn(103, 0);

 countTimer_CMockExpectAndReturn(104, 50);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((Acknowledge)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)106, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((data.nextseqnum[0])), (((void *)0)), (_U_UINT)107, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)108, UNITY_DISPLAY_STYLE_INT);



 congestionWindow_CMockExpectAndReturn(110, 1);

 recieveRxACK_CMockExpectAndReturn(111, 500);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((SendData)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)113, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)114, UNITY_DISPLAY_STYLE_INT);



 congestionWindow_CMockExpectAndReturn(116, 2);

 countTimer_CMockExpectAndReturn(117, 100);

 sendData2RCV_CMockExpectAndReturn(118, 1);

 sequenceNumber_CMockExpectAndReturn(119, 500);

 countTimer_CMockExpectAndReturn(120, 150);

 sendData2RCV_CMockExpectAndReturn(121, 1);

 sequenceNumber_CMockExpectAndReturn(122, 1000);

 countTimer_CMockExpectAndReturn(123, 200);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((Acknowledge)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)125, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((data.nextseqnum[0])), (((void *)0)), (_U_UINT)126, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1500)), (_U_SINT)((data.nextseqnum[1])), (((void *)0)), (_U_UINT)127, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)128, UNITY_DISPLAY_STYLE_INT);



 congestionWindow_CMockExpectAndReturn(130, 2);

 recieveRxACK_CMockExpectAndReturn(131, 1000);

 recieveRxACK_CMockExpectAndReturn(132, 1500);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((SendData)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)134, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1500)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)135, UNITY_DISPLAY_STYLE_INT);



 congestionWindow_CMockExpectAndReturn(137, 3);

 countTimer_CMockExpectAndReturn(138, 300);

 sendData2RCV_CMockExpectAndReturn(139, 1);

 sequenceNumber_CMockExpectAndReturn(140, 1500);

 countTimer_CMockExpectAndReturn(141, 500);

 sendData2RCV_CMockExpectAndReturn(142, 1);

 sequenceNumber_CMockExpectAndReturn(143, 2000);

 countTimer_CMockExpectAndReturn(144, 600);

 sendData2RCV_CMockExpectAndReturn(145, 1);

 sequenceNumber_CMockExpectAndReturn(146, 2500);

 countTimer_CMockExpectAndReturn(147, 5500);

 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((TimeOut)), (_U_SINT)((state.state_SS)), (((void *)0)), (_U_UINT)149, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((2000)), (_U_SINT)((data.nextseqnum[0])), (((void *)0)), (_U_UINT)150, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((2500)), (_U_SINT)((data.nextseqnum[1])), (((void *)0)), (_U_UINT)151, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((3000)), (_U_SINT)((data.nextseqnum[2])), (((void *)0)), (_U_UINT)152, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1500)), (_U_SINT)((data.sendbase)), (((void *)0)), (_U_UINT)153, UNITY_DISPLAY_STYLE_INT);



 slowStart(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((1500)), (_U_SINT)((data.nextseqnum[0])), (((void *)0)), (_U_UINT)156, UNITY_DISPLAY_STYLE_INT);

}
