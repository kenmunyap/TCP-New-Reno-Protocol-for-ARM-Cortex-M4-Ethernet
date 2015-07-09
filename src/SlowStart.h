#ifndef SlowStart_H
#define SlowStart_H

#include <stdint.h>

#define MSS 50
#define ssthres 20000

uint8_t Buffer[1000];
extern uint8_t *Block;
extern uint8_t *receiveData;

typedef enum{
	SlowStart,
	SlowStartWaitACK,
}State;

typedef struct{
	State state;
}TCP_state;

typedef struct{
	uint32_t offset;
	uint32_t size;
  uint32_t dupACKFlag;
}Cwnd;

typedef struct{
  uint32_t srcIpAddr;
  uint32_t destIpAddr;
  uint32_t SeqNum;
  uint32_t AckNum;
  uint8_t data;
}Packet;

void cwndInitWindow(Cwnd *cwnd);
void initTCPState(TCP_state *state);
void initPacket(Packet *packet);
uint32_t txTcpSM(TCP_state *state, Cwnd *cwnd, Packet *packet);
void fastRetransmit(Cwnd *cwnd, Packet *packet, uint32_t size);

#endif // SlowStart_H
