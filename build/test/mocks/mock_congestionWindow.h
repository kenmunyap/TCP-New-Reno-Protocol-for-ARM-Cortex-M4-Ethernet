/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_CONGESTIONWINDOW_H
#define _MOCK_CONGESTIONWINDOW_H

#include "congestionWindow.h"

void mock_congestionWindow_Init(void);
void mock_congestionWindow_Destroy(void);
void mock_congestionWindow_Verify(void);




#define cwndGetBeginningOffset_IgnoreAndReturn(cmock_retval) cwndGetBeginningOffset_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void cwndGetBeginningOffset_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
#define cwndGetBeginningOffset_ExpectAndReturn(cwnd, cmock_retval) cwndGetBeginningOffset_CMockExpectAndReturn(__LINE__, cwnd, cmock_retval)
void cwndGetBeginningOffset_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, Cwnd* cwnd, uint32_t cmock_to_return);
typedef uint32_t (* CMOCK_cwndGetBeginningOffset_CALLBACK)(Cwnd* cwnd, int cmock_num_calls);
void cwndGetBeginningOffset_StubWithCallback(CMOCK_cwndGetBeginningOffset_CALLBACK Callback);
#define cwndIncrementWindow_IgnoreAndReturn(cmock_retval) cwndIncrementWindow_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void cwndIncrementWindow_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
#define cwndIncrementWindow_ExpectAndReturn(cwnd, size, cmock_retval) cwndIncrementWindow_CMockExpectAndReturn(__LINE__, cwnd, size, cmock_retval)
void cwndIncrementWindow_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, Cwnd* cwnd, uint32_t size, uint32_t cmock_to_return);
typedef uint32_t (* CMOCK_cwndIncrementWindow_CALLBACK)(Cwnd* cwnd, uint32_t size, int cmock_num_calls);
void cwndIncrementWindow_StubWithCallback(CMOCK_cwndIncrementWindow_CALLBACK Callback);
#define cwndGetDataBlock_IgnoreAndReturn(cmock_retval) cwndGetDataBlock_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void cwndGetDataBlock_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
#define cwndGetDataBlock_ExpectAndReturn(cwnd, offset, requestedSize, block, cmock_retval) cwndGetDataBlock_CMockExpectAndReturn(__LINE__, cwnd, offset, requestedSize, block, cmock_retval)
void cwndGetDataBlock_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, Cwnd* cwnd, uint32_t offset, uint32_t requestedSize, uint8_t** block, uint32_t cmock_to_return);
typedef uint32_t (* CMOCK_cwndGetDataBlock_CALLBACK)(Cwnd* cwnd, uint32_t offset, uint32_t requestedSize, uint8_t** block, int cmock_num_calls);
void cwndGetDataBlock_StubWithCallback(CMOCK_cwndGetDataBlock_CALLBACK Callback);
#define cwndGetSSthresh_IgnoreAndReturn(cmock_retval) cwndGetSSthresh_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void cwndGetSSthresh_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint32_t cmock_to_return);
#define cwndGetSSthresh_ExpectAndReturn(cwnd, cmock_retval) cwndGetSSthresh_CMockExpectAndReturn(__LINE__, cwnd, cmock_retval)
void cwndGetSSthresh_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, Cwnd* cwnd, uint32_t cmock_to_return);
typedef uint32_t (* CMOCK_cwndGetSSthresh_CALLBACK)(Cwnd* cwnd, int cmock_num_calls);
void cwndGetSSthresh_StubWithCallback(CMOCK_cwndGetSSthresh_CALLBACK Callback);

#endif
