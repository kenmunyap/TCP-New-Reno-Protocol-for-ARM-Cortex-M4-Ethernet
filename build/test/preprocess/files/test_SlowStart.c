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

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)22, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)23, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)26, UNITY_DISPLAY_STYLE_INT);





  cwndGetBeginningOffset_CMockExpectAndReturn(29, &cwnd, 0);

  cwndGetDataBlock_CMockExpectAndReturn(30, &cwnd, 0, 50, &Block, 50);

  for(i=0;i<50;i++){

    sendDataPacket_CMockExpect(32, &packet, &Block, i);

  }

  TxData(&state,&cwnd,&packet);





  cwndGetDataBlock_CMockExpectAndReturn(37, &cwnd, 50, 50, &Block, 0);

  getDataPacket_CMockExpectAndReturn(38, &packet, &receiveData, 50);

  cwndIncrementWindow_CMockExpectAndReturn(39, &cwnd, 50, 100);

  TxData(&state,&cwnd,&packet);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)41, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)42, UNITY_DISPLAY_STYLE_INT);

}
