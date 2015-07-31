#ifndef CONGESTION_WINDOW_H
#define CONGESTION_WINDOW_H

#include "TxTCP.h"

uint32_t cwndGetBeginningOffsets(Cwnd *cwnd);
uint32_t cwndIncrementWindows(Cwnd *cwnd, uint32_t size);
uint32_t cwndGetDataBlocks(Cwnd *cwnd, uint32_t offset, uint32_t requestedSize, uint8_t **block);
uint32_t min(uint32_t valueA, uint32_t valueB);
uint32_t max(uint32_t valueA, uint32_t valueB);

#endif // CONGESTION_WINDOW_H