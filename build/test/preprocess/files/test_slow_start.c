#include "unity.h"
#include "slow_start.h"
#include "mock_getData.h"


void setUp(void){}



void tearDown(void){}



void test_TCP_successful_send_all_data_using_slow_start(void){





 sendData_CMockIgnoreAndReturn(12, 1);

 getData_CMockIgnoreAndReturn(13, 1);

 senderSlowStart();

 sendData_CMockIgnoreAndReturn(15, 1);

 getData_CMockIgnoreAndReturn(16, 1);

 senderSlowStart();

}
