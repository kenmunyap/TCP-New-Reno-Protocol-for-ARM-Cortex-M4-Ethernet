#include "unity.h"
#include "mock_returnACK.h"
#include "mock_congestionWindow.h"
#include "SlowStart.h"
void setUp(void){}

void tearDown(void){}





void test_module_generator_needs_to_be_implemented(void){



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





}

void test_cwndInitWindow_should_init_a_window_with_default_data(void){



  Cwnd Window;

  cwndInitWindow(&Window);



  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Window.offset)), (((void *)0)), (_U_UINT)50, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((Window.size)), (((void *)0)), (_U_UINT)51, UNITY_DISPLAY_STYLE_INT);

}



void test_initTCPState_should_go_to_the_slow_start_state(void){



  TCP_state state;

  initTCPState(&state);



  UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)59, UNITY_DISPLAY_STYLE_INT);

}
