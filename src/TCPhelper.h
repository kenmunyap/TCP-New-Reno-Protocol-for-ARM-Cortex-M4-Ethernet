#ifndef TCPhelper_H
#define TCPhelper_H

#include "TxTCP.h"

uint32_t sendPacket(TCP_state *state, Packet *packet, uint32_t availableSize , uint32_t offset);
void retransmit(TCP_state *state, Cwnd *cwnd, Packet *packet, uint32_t lostPacket, uint32_t offset);
void checkCAorSSBySSTHRESH(TCP_state *state,Cwnd *cwnd);
void incCACounter(uint32_t counter,TCP_state *state,Cwnd *cwnd,uint32_t currentWindowSize,uint32_t ackNo);
uint32_t duplicatePacketCount(TCP_state *state, uint32_t dupAckCounter);
uint32_t min(uint32_t valueA, uint32_t valueB);
uint32_t max(uint32_t valueA, uint32_t valueB);

#endif // TCPhelper_H
