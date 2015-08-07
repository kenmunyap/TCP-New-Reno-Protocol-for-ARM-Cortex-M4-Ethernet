#ifndef TCPhelper_H
#define TCPhelper_H

#include "TxTCP.h"

void sendPacket(TCPSession *session, Packet *packet, uint32_t availableSize);
void retransmit(TCPSession *session, Packet *packet, uint32_t lostPacket);
void checkCAorSSBySSTHRESH(TCPSession *session);
void incCACounter(uint32_t counter,TCPSession *session,uint32_t currentWindowSize,uint32_t ackNo);
void duplicatePacketCount(TCPSession *session,uint32_t ackNo);
uint32_t roundOffValue(uint32_t valueToRoundOff);
uint32_t min(uint32_t valueA, uint32_t valueB);
uint32_t max(uint32_t valueA, uint32_t valueB);
double floatMax(double valueA, double valueB);

#endif // TCPhelper_H
