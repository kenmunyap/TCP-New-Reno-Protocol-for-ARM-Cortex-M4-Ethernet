#include "SlowStart.h"








uint32_t cwndGetBeginningOffset(Cwnd *cwnd);

uint32_t cwndIncrementWindow(Cwnd *cwnd, uint32_t size);

uint32_t cwndGetDataBlock(Cwnd *cwnd, uint32_t offset, uint32_t requestedSize, uint8_t **block);
