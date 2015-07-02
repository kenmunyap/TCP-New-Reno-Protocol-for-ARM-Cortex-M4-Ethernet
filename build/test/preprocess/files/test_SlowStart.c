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

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)76, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)77, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)80, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(83, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(84, &cwnd, 0, 50, &Block, 50);

  for(i=0;i<50;i++){

    sendDataPacket_CMockExpect(86, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)89, UNITY_DISPLAY_STYLE_INT);



  cwndGetDataBlock_CMockExpectAndReturn(91, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(92, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(93, &cwnd, 50, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)95, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)96, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStartWaitACK)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)97, UNITY_DISPLAY_STYLE_INT);









  cwndGetDataBlock_CMockExpectAndReturn(102, &cwnd, 50, 50, &Block, 50);

  TxData(&state,&cwnd,&packet);



}
