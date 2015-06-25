#include "unity.h"
#include "mock_CongestionWindow.h"
#include "SlowStart.h"


void setUp(void){}

void tearDown(void){}

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

void test_TxTCP_should_init_and_send_the_first_data(void){

  Cwnd *returned;



  Cwnd Window;

  cwndInitWindow(&Window);



  TCP_state state;

  initTCPState(&state);



  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((Window.offset)), (((void *)0)), (_U_UINT)66, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50)), (_U_SINT)((Window.size)), (((void *)0)), (_U_UINT)67, UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((SlowStart)), (_U_SINT)((state.state)), (((void *)0)), (_U_UINT)68, UNITY_DISPLAY_STYLE_INT);

  Block = 0;



  cwndGetBeginningOffset_CMockExpectAndReturn(71, &Window, 0);

  cwndGetDataBlock_CMockExpectAndReturn(72, &Window, 0, 50, &(*Block), 50);

  cwndIncrementWindow_CMockExpectAndReturn(73, &Window, 50, 100);



  TxTCP(&state,&Window);



  cwndGetBeginningOffset_CMockExpectAndReturn(77, &Window, 50);

  cwndGetDataBlock_CMockExpectAndReturn(78, &Window, 50, 100, &(*Block), 100);

  cwndIncrementWindow_CMockExpectAndReturn(79, &Window, 100, 200);



  TxTCP(&state,&Window);



  cwndGetBeginningOffset_CMockExpectAndReturn(83, &Window, 100);

  cwndGetDataBlock_CMockExpectAndReturn(84, &Window, 100, 150, &(*Block), 100);





  returned = TxTCP(&state,&Window);

  if ((((returned)) == ((void *)0))) {} else {UnityFail( (" Expected NULL"), (_U_UINT)(_U_UINT)(_U_UINT)88);;};

  printf("======================\n");

  printf("TEST cwnd->offset : %d\n",Window.offset);

  printf("TEST cwnd->size   : %d\n",Window.size);

  printf("TEST Block        : %d\n",Block);

}
