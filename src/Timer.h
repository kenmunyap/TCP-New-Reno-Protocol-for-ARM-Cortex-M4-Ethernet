#ifndef Timer_H
#define Timer_H

#include <stdint.h>

typedef struct{
 uint32_t sendTime;
 uint32_t ackTime;
 uint32_t RTTVAR;
 uint32_t SRTT;
 uint32_t timeOut;
}Timer;

// uint32_t initEstimatedTime(Timer *timer);
// uint32_t roundTT();
// uint32_t RTO();  // Retransmission time out


#endif // Timer_H