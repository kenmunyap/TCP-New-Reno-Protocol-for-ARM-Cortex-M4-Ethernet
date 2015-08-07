#ifndef Timer_H
#define Timer_H

#include <stdint.h>

typedef struct{
  double sendTimer;
  double ackTimer;
  double R;
  double SRTT;
  double RTTVAR;
  double RTO;
}Timer;

double initRTOTime(Timer *timer);
double updateRTOTime(Timer *timer);

#endif // Timer_H