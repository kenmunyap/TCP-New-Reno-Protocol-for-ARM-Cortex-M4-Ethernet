/* AUTOGENERATED FILE. DO NOT EDIT. */

//=======Test Runner Used To Run Each Test Below=====
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      CMock_Init(); \
      setUp(); \
      TestFunc(); \
      CMock_Verify(); \
  } \
  CMock_Destroy(); \
  if (TEST_PROTECT() && !TEST_IS_IGNORED) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

//=======Automagically Detected Files To Include=====
#include "unity.h"
#include "cmock.h"
#include <setjmp.h>
#include <stdio.h>
#include "mock_Packet.h"
#include "mock_Timer.h"
#include "mock_congestionWindow.h"

int GlobalExpectCount;
int GlobalVerifyOrder;
char* GlobalOrderError;

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_InitWindow_offset_and_size(void);
extern void test_InitTCPState(void);
extern void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_and_in_sequence(void);
extern void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_but_dup_ack(void);
extern void test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_but_over_ssthress(void);
extern void test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_3_dup_ack(void);


//=======Mock Management=====
static void CMock_Init(void)
{
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
  GlobalOrderError = NULL;
  mock_Packet_Init();
  mock_Timer_Init();
  mock_congestionWindow_Init();
}
static void CMock_Verify(void)
{
  mock_Packet_Verify();
  mock_Timer_Verify();
  mock_congestionWindow_Verify();
}
static void CMock_Destroy(void)
{
  mock_Packet_Destroy();
  mock_Timer_Destroy();
  mock_congestionWindow_Destroy();
}

//=======Test Reset Option=====
void resetTest()
{
  CMock_Verify();
  CMock_Destroy();
  tearDown();
  CMock_Init();
  setUp();
}


//=======MAIN=====
int main(void)
{
  Unity.TestFile = "test_SlowStart.c";
  UnityBegin();
  RUN_TEST(test_InitWindow_offset_and_size, 11);
  RUN_TEST(test_InitTCPState, 19);
  RUN_TEST(test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_and_in_sequence, 43);
  RUN_TEST(test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_but_dup_ack, 129);
  RUN_TEST(test_TxTCPSM_SlowStart_with_not_exited_RoundTripTime_but_over_ssthress, 197);
  RUN_TEST(test_TxTCPSM_Congestion_Avoidance_with_not_exited_RoundTripTime_but_with_3_dup_ack, 299);

  return (UnityEnd());
}
