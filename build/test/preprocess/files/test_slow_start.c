#include "unity.h"
#include "slow_start.h"
#include "mock_getData.h"
#include "mock_delay.h"
#include "mock_congestionWindow.h"


void setUp(void){}



void tearDown(void){}



void test_TCP_successful_send_all_data_using_slow_start(void){

 DataBlock data;



 sequenceNumber_CMockIgnoreAndReturn(14, 0);

 delayRTT_CMockIgnoreAndReturn(15, 500);

 increaseCongestionWindow_CMockIgnoreAndReturn(16, 1);

 sendData_CMockIgnoreAndReturn(17, 1);

 sequenceNumber_CMockIgnoreAndReturn(18, 501);

 recieveRxACK_CMockIgnoreAndReturn(19, 501);

 increaseCongestionWindow_CMockIgnoreAndReturn(20, 2);

 txSlowStart(&data);

 UnityAssertEqualNumber((_U_SINT)((data.index)), (_U_SINT)((501)), (((void *)0)), (_U_UINT)22, UNITY_DISPLAY_STYLE_INT);

 sendData_CMockIgnoreAndReturn(23, 2);

 sequenceNumber_CMockIgnoreAndReturn(24, 1001);

 recieveRxACK_CMockIgnoreAndReturn(25, 1001);

 sendData_CMockIgnoreAndReturn(26, 3);

 sequenceNumber_CMockIgnoreAndReturn(27, 1501);

 recieveRxACK_CMockIgnoreAndReturn(28, 1501);

 txSlowStart(&data);

}
