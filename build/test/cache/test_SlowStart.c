#include "unity.h"
#include "mock_returnACK.h"
#include "mock_congestionWindow.h"
#include "SlowStart.h"
void setUp(void){}

void tearDown(void){}





void test_get_and_send_1_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){



 Cwnd cwnd;

 TCP_state state;

 uint32_t size;



 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)16, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)17, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)20, UNITY_DISPLAY_STYLE_INT);

 Block = 0;



 cwndGetDataBlock_CMockExpectAndReturn(23, &cwnd, 0, 50, &(*Block), 0);

 TxTCP(&state,&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((&(*Block))), (((void *)0)), (_U_UINT)25, UNITY_DISPLAY_STYLE_INT);



 cwndGetDataBlock_CMockExpectAndReturn(27, &cwnd, 0, 50, &(*Block), -1);

 returnACK_CMockExpectAndReturn(28, 50);

 size = 50;

 cwndIncrementWindow_CMockExpectAndReturn(30, &cwnd, size, 100);

 returnACK_CMockExpectAndReturn(31, 50);

 TxTCP(&state,&cwnd);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)33, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)34, UNITY_DISPLAY_STYLE_INT);

}



void test_get_and_send_3_segment_of_data_to_receiver_after_return_ack_increment_size_and_offset(void){

 Cwnd cwnd;

 TCP_state state;

 uint32_t size;



 cwndInitWindow(&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)43, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)44, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)47, UNITY_DISPLAY_STYLE_INT);





 cwndGetDataBlock_CMockExpectAndReturn(50, &cwnd, 0, 50, &(*(Block = 0)), 0);

 TxTCP(&state,&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((&(*Block))), (((void *)0)), (_U_UINT)52, UNITY_DISPLAY_STYLE_INT);



 cwndGetDataBlock_CMockExpectAndReturn(54, &cwnd, 0, 50, &(*Block), -1);

 returnACK_CMockExpectAndReturn(55, 50);

 size = 50;

 cwndIncrementWindow_CMockExpectAndReturn(57, &cwnd, size, 100);

 returnACK_CMockExpectAndReturn(58, 50);

 TxTCP(&state,&cwnd);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)60, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)61, UNITY_DISPLAY_STYLE_INT);





 cwndGetDataBlock_CMockExpectAndReturn(64, &cwnd, 50, 100, &(*Block = 0), 50);

 TxTCP(&state,&cwnd);



}

void test_cwndInitWindow_should_init_a_window_with_default_data(void){



  Cwnd Window;

  cwndInitWindow(&Window);



  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Window.offset)), (((void *)0)), (_U_UINT)86, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((Window.size)), (((void *)0)), (_U_UINT)87, UNITY_DISPLAY_STYLE_INT);

}



void test_initTCPState_should_go_to_the_slow_start_state(void){



  TCP_state state;

  initTCPState(&state);



  UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)95, UNITY_DISPLAY_STYLE_INT);

}
