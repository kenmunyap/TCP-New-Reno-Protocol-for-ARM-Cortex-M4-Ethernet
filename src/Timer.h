#ifndef Timer_H
#define Timer_H

#include <stdint.h>

typedef struct{
  sendTime;
  ackTime;
  RTTVAR;
  SRTT;
  timeOut;
}Timer;

uint32_t initEstimatedTime(Timer *timer);
uint32_t roundTT();
uint32_t RTO();  // Retransmission time out


#endif // Timer_H