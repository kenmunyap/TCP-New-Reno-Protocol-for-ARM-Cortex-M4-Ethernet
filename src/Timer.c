#include <stdio.h>
#include <stdint.h>
#include "Timer.h"
#include "TCPhelper.h"

#define G  2;
#define alpha 1/4;
#define beta 1/8;

void initRTOTime(Timer *timer){
  timer->SRTT = timer->ackTimer - timer->sendTimer; 
  timer->RTTVAR = timer->SRTT/2;
  timer->RTO = timer->SRTT + max(G,4*RTTVAR);
}

void updateRTOTime(Timer *timer){
  
}
