#include "unity.h"
#include "mock_CongestionWindow.h"
#include "SlowStart.h"


void setUp(void){}



void tearDown(void){}







void test_module_generator_needs_to_be_implemented(void){

 int size = 50;

 int offset = 0;

 Cwnd data;

 TCP_state state;

 cwndInitWindow(&data);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((data.offset)), (((void *)0)), (_U_UINT)17, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((data.size)), (((void *)0)), (_U_UINT)18, UNITY_DISPLAY_STYLE_INT);

 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)20, UNITY_DISPLAY_STYLE_INT);

 TxTCP(&state,&data,size,offset);



}
