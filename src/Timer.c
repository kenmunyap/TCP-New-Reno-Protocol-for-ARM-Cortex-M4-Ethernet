#include <stdio.h>
#include <stdint.h>
#include "Timer.h"
#include "TCPhelper.h"

#define G 3
#define K 4
#define alpha 0.125   // 1/8
#define beta 0.25     // 1/4

double initRTOTime(Timer *timer){
  timer->R = timer->sendTimer - timer->ackTimer;
  timer->SRTT = timer->R;
  timer->RTTVAR = timer->SRTT/2;
  timer->RTO = timer->SRTT + max(G,K*timer->RTTVAR);
}

double updateRTOTime(Timer *timer){
  double RTTValue;
  
  timer->SRTT = ((1 - alpha) * timer->SRTT) + (alpha * timer->R);
  RTTValue = (timer->SRTT - timer->R);
  if(RTTValue){
    RTTValue = RTTValue*-1;
  }  
  timer->RTTVAR = ((1 - beta) * timer->RTTVAR) + (beta * RTTValue);
  timer->RTO = timer->SRTT + floatMax(G,K*timer->RTTVAR);
}