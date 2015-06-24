#include "unity.h"
#include "mock_CongestionWindow.h"
#include "SlowStart.h"


void setUp(void){}

void tearDown(void){}



void test_module_generator_needs_to_be_implemented(void)

{

 Cwnd data;

 TCP_state state;

 cwndInitWindow(&data);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((data.offset)), (((void *)0)), (_U_UINT)13, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((data.size)), (((void *)0)), (_U_UINT)14, UNITY_DISPLAY_STYLE_INT);

 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)16, UNITY_DISPLAY_STYLE_INT);

 cwndIncrementWindow_CMockExpectAndReturn(17, &data, 50, 50);

 Buffer[0] = 'a';

 TCP(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((data.size)), (((void *)0)), (_U_UINT)20, UNITY_DISPLAY_STYLE_INT);





}













void test_cwndInitWindow_should_init_a_window_with_default_data(void){



  Cwnd Window;

  cwndInitWindow(&Window);



  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Window.offset)), (((void *)0)), (_U_UINT)35, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((Window.size)), (((void *)0)), (_U_UINT)36, UNITY_DISPLAY_STYLE_INT);

}



void test_initTCPState_should_go_to_the_slow_start_state(void){



  TCP_state state;

  initTCPState(&state);



  UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)44, UNITY_DISPLAY_STYLE_INT);

}
