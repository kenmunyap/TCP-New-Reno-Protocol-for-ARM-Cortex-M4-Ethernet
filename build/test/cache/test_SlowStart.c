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

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((data.offset)), (((void *)0)), (_U_UINT)14, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((data.size)), (((void *)0)), (_U_UINT)15, UNITY_DISPLAY_STYLE_INT);

 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)17, UNITY_DISPLAY_STYLE_INT);

 cwndIncrementWindow_CMockExpectAndReturn(18, &data, 50, 50);

 Buffer[0] = 'a';

 TCP(&state,&data);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((data.size)), (((void *)0)), (_U_UINT)21, UNITY_DISPLAY_STYLE_INT);





}
