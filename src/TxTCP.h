#ifndef TXTCP_H
#define TXTCP_H

#include <stdint.h>
#include "Timer.h"

#define SMSS 50  // (SMSS) SENDER MAXIMUM SEGMENT SIZE
#define ssthres 250 // slow start threshold

#define sessionCWND session->cwnd
#define sessionState session->tcpState


extern uint8_t Buffer[1000];
extern uint8_t *receiveData;

typedef enum{
	SlowStart,
	SlowStartWaitACK,
  CongestionAvoidance,
	FastRetransmit,
  FastRecovery,
  TimeOut
}State;

typedef struct{
	State state;
  uint8_t *ptrBlock;
}TCP_state;

typedef struct{
	uint32_t offset;
	uint32_t size;
  uint32_t dupACKFlag;
  uint32_t ssthresh;
  uint32_t recover;
  uint32_t flightSize;
  uint32_t lostPacket;
}Cwnd; // CONGESTION WINDOW

typedef struct{
  TCP_state *tcpState;
  Cwnd *cwnd;
  Timer *timer;
  uint32_t offset;
  uint32_t requestedSize;
  uint32_t dupAckCounter;
}TCPSession;

typedef struct{
  uint32_t srcIpAddr;
  uint32_t destIpAddr;
  uint32_t SeqNum;
  uint32_t AckNum;
  uint8_t data;
}Packet;

void cwndInitWindow(TCPSession *session);
void initTCPState(TCPSession *session);
void initPacket(Packet *packet);
uint32_t TxTCPSM(TCPSession *session, Packet *packet);

#endif // TXTCP_H
