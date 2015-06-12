/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "mock_congestionWindow.h"

typedef struct _CMOCK_increaseCongestionWindow_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int ReturnVal;
  int CallOrder;

} CMOCK_increaseCongestionWindow_CALL_INSTANCE;

typedef struct _CMOCK_decreaseCongestionWindow_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int ReturnVal;
  int CallOrder;

} CMOCK_decreaseCongestionWindow_CALL_INSTANCE;

static struct mock_congestionWindowInstance
{
  int increaseCongestionWindow_IgnoreBool;
  int increaseCongestionWindow_FinalReturn;
  CMOCK_increaseCongestionWindow_CALLBACK increaseCongestionWindow_CallbackFunctionPointer;
  int increaseCongestionWindow_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE increaseCongestionWindow_CallInstance;
  int decreaseCongestionWindow_IgnoreBool;
  int decreaseCongestionWindow_FinalReturn;
  CMOCK_decreaseCongestionWindow_CALLBACK decreaseCongestionWindow_CallbackFunctionPointer;
  int decreaseCongestionWindow_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE decreaseCongestionWindow_CallInstance;
} Mock;

extern jmp_buf AbortFrame;
extern int GlobalExpectCount;
extern int GlobalVerifyOrder;

void mock_congestionWindow_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  if (Mock.increaseCongestionWindow_IgnoreBool)
    Mock.increaseCongestionWindow_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.increaseCongestionWindow_CallInstance, cmock_line, "Function 'increaseCongestionWindow' called less times than expected.");
  if (Mock.increaseCongestionWindow_CallbackFunctionPointer != NULL)
    Mock.increaseCongestionWindow_CallInstance = CMOCK_GUTS_NONE;
  if (Mock.decreaseCongestionWindow_IgnoreBool)
    Mock.decreaseCongestionWindow_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.decreaseCongestionWindow_CallInstance, cmock_line, "Function 'decreaseCongestionWindow' called less times than expected.");
  if (Mock.decreaseCongestionWindow_CallbackFunctionPointer != NULL)
    Mock.decreaseCongestionWindow_CallInstance = CMOCK_GUTS_NONE;
}

void mock_congestionWindow_Init(void)
{
  mock_congestionWindow_Destroy();
}

void mock_congestionWindow_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.increaseCongestionWindow_CallbackFunctionPointer = NULL;
  Mock.increaseCongestionWindow_CallbackCalls = 0;
  Mock.decreaseCongestionWindow_CallbackFunctionPointer = NULL;
  Mock.decreaseCongestionWindow_CallbackCalls = 0;
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
}

int increaseCongestionWindow(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_increaseCongestionWindow_CALL_INSTANCE* cmock_call_instance = (CMOCK_increaseCongestionWindow_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.increaseCongestionWindow_CallInstance);
  Mock.increaseCongestionWindow_CallInstance = CMock_Guts_MemNext(Mock.increaseCongestionWindow_CallInstance);
  if (Mock.increaseCongestionWindow_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.increaseCongestionWindow_FinalReturn;
    Mock.increaseCongestionWindow_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (Mock.increaseCongestionWindow_CallbackFunctionPointer != NULL)
  {
    return Mock.increaseCongestionWindow_CallbackFunctionPointer(Mock.increaseCongestionWindow_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'increaseCongestionWindow' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'increaseCongestionWindow' called earlier than expected.");
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'increaseCongestionWindow' called later than expected.");
  return cmock_call_instance->ReturnVal;
}

void increaseCongestionWindow_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_increaseCongestionWindow_CALL_INSTANCE));
  CMOCK_increaseCongestionWindow_CALL_INSTANCE* cmock_call_instance = (CMOCK_increaseCongestionWindow_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.increaseCongestionWindow_CallInstance = CMock_Guts_MemChain(Mock.increaseCongestionWindow_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.increaseCongestionWindow_IgnoreBool = (int)1;
}

void increaseCongestionWindow_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_increaseCongestionWindow_CALL_INSTANCE));
  CMOCK_increaseCongestionWindow_CALL_INSTANCE* cmock_call_instance = (CMOCK_increaseCongestionWindow_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.increaseCongestionWindow_CallInstance = CMock_Guts_MemChain(Mock.increaseCongestionWindow_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void increaseCongestionWindow_StubWithCallback(CMOCK_increaseCongestionWindow_CALLBACK Callback)
{
  Mock.increaseCongestionWindow_CallbackFunctionPointer = Callback;
}

int decreaseCongestionWindow(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_decreaseCongestionWindow_CALL_INSTANCE* cmock_call_instance = (CMOCK_decreaseCongestionWindow_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.decreaseCongestionWindow_CallInstance);
  Mock.decreaseCongestionWindow_CallInstance = CMock_Guts_MemNext(Mock.decreaseCongestionWindow_CallInstance);
  if (Mock.decreaseCongestionWindow_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.decreaseCongestionWindow_FinalReturn;
    Mock.decreaseCongestionWindow_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (Mock.decreaseCongestionWindow_CallbackFunctionPointer != NULL)
  {
    return Mock.decreaseCongestionWindow_CallbackFunctionPointer(Mock.decreaseCongestionWindow_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'decreaseCongestionWindow' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'decreaseCongestionWindow' called earlier than expected.");
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'decreaseCongestionWindow' called later than expected.");
  return cmock_call_instance->ReturnVal;
}

void decreaseCongestionWindow_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_decreaseCongestionWindow_CALL_INSTANCE));
  CMOCK_decreaseCongestionWindow_CALL_INSTANCE* cmock_call_instance = (CMOCK_decreaseCongestionWindow_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.decreaseCongestionWindow_CallInstance = CMock_Guts_MemChain(Mock.decreaseCongestionWindow_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.decreaseCongestionWindow_IgnoreBool = (int)1;
}

void decreaseCongestionWindow_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_decreaseCongestionWindow_CALL_INSTANCE));
  CMOCK_decreaseCongestionWindow_CALL_INSTANCE* cmock_call_instance = (CMOCK_decreaseCongestionWindow_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.decreaseCongestionWindow_CallInstance = CMock_Guts_MemChain(Mock.decreaseCongestionWindow_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void decreaseCongestionWindow_StubWithCallback(CMOCK_decreaseCongestionWindow_CALLBACK Callback)
{
  Mock.decreaseCongestionWindow_CallbackFunctionPointer = Callback;
}
