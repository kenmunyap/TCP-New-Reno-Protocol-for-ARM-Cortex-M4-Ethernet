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

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((cwnd.offset)), (((void *)0)), (_U_UINT)15, UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((cwnd.size)), (((void *)0)), (_U_UINT)16, UNITY_DISPLAY_STYLE_INT);



 initTCPState(&state);

 UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)19, UNITY_DISPLAY_STYLE_INT);

 Block = 0;



 cwndGetDataBlock_CMockExpectAndReturn(22, &cwnd, 0, 50, &(*Block), 0);

 TxTCP(&state,&cwnd);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((&(*Block))), (((void *)0)), (_U_UINT)24, UNITY_DISPLAY_STYLE_INT);



 cwndGetDataBlock_CMockExpectAndReturn(26, &cwnd, 0, 50, &(*Block), -1);

 returnACK_CMockExpectAndReturn(27, 50);

 size = 50;

 cwndIncrementWindow_CMockExpectAndReturn(29, &cwnd, size, 100);

 returnACK_CMockExpectAndReturn(30, 50);

 TxTCP(&state,&cwnd);







}
