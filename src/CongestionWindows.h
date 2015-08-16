#ifndef CONGESTION_WINDOW_H
#define CONGESTION_WINDOW_H

#include "TxTCP.h"

uint32_t cwndGetBeginningOffsets(Cwnd *cwnd);
uint32_t cwndIncrementWindows(Cwnd *cwnd, uint32_t size);
uint32_t cwndGetDataBlocks(Cwnd *cwnd, uint32_t offset, uint32_t requestedSize, uint8_t **block);
uint32_t slideWindow(TCPSession *session);
#endif // CONGESTION_WINDOW_H