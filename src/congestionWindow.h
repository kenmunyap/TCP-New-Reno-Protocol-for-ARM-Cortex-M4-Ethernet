#ifndef CONGESTION_WINDOW_H
#define CONGESTION_WINDOW_H

#include "SlowStart.h"

uint32_t cwndGetBeginningOffset(Cwnd *cwnd);
int cwndIncrementWindow(Cwnd *cwnd,uint32_t size);
uint32_t cwndGetDataBlock(Cwnd *cwnd, uint32_t offset, uint32_t requestedSize, char **block);

#endif // CONGESTION_WINDOW_H