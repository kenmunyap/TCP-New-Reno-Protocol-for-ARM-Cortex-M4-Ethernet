/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "mock_delay.h"

typedef struct _CMOCK_delayRTT_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int ReturnVal;
  int CallOrder;

} CMOCK_delayRTT_CALL_INSTANCE;

static struct mock_delayInstance
{
  int delayRTT_IgnoreBool;
  int delayRTT_FinalReturn;
  CMOCK_delayRTT_CALLBACK delayRTT_CallbackFunctionPointer;
  int delayRTT_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE delayRTT_CallInstance;
} Mock;

extern jmp_buf AbortFrame;
extern int GlobalExpectCount;
extern int GlobalVerifyOrder;

void mock_delay_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  if (Mock.delayRTT_IgnoreBool)
    Mock.delayRTT_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.delayRTT_CallInstance, cmock_line, "Function 'delayRTT' called less times than expected.");
  if (Mock.delayRTT_CallbackFunctionPointer != NULL)
    Mock.delayRTT_CallInstance = CMOCK_GUTS_NONE;
}

void mock_delay_Init(void)
{
  mock_delay_Destroy();
}

void mock_delay_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.delayRTT_CallbackFunctionPointer = NULL;
  Mock.delayRTT_CallbackCalls = 0;
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
}

int delayRTT(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_delayRTT_CALL_INSTANCE* cmock_call_instance = (CMOCK_delayRTT_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.delayRTT_CallInstance);
  Mock.delayRTT_CallInstance = CMock_Guts_MemNext(Mock.delayRTT_CallInstance);
  if (Mock.delayRTT_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.delayRTT_FinalReturn;
    Mock.delayRTT_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (Mock.delayRTT_CallbackFunctionPointer != NULL)
  {
    return Mock.delayRTT_CallbackFunctionPointer(Mock.delayRTT_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'delayRTT' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'delayRTT' called earlier than expected.");
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'delayRTT' called later than expected.");
  return cmock_call_instance->ReturnVal;
}

void delayRTT_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_delayRTT_CALL_INSTANCE));
  CMOCK_delayRTT_CALL_INSTANCE* cmock_call_instance = (CMOCK_delayRTT_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.delayRTT_CallInstance = CMock_Guts_MemChain(Mock.delayRTT_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.delayRTT_IgnoreBool = (int)1;
}

void delayRTT_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_delayRTT_CALL_INSTANCE));
  CMOCK_delayRTT_CALL_INSTANCE* cmock_call_instance = (CMOCK_delayRTT_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.delayRTT_CallInstance = CMock_Guts_MemChain(Mock.delayRTT_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void delayRTT_StubWithCallback(CMOCK_delayRTT_CALLBACK Callback)
{
  Mock.delayRTT_CallbackFunctionPointer = Callback;
}
