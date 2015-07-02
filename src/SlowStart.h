#ifndef SlowStart_H
#define SlowStart_H

#include <stdint.h>

#define MSS 50
#define ssthres 20000

uint8_t Buffer[1000];
extern uint8_t *Block;
extern uint32_t returnSlowStartflag;

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
	uint32_t maximumOffsetSize;
	uint32_t lastByteSend;
	uint32_t selectedOffSet;
}Cwnd;

typedef struct{
  uint32_t srcIpAddr;
  // uint32_t destIpAddr;
  // uint32_t seqNum;
  // uint32_t AckNum;
  // uint8_t data;
}Packet;

void cwndInitWindow(Cwnd *cwnd);
void initTCPState(TCP_state *state);
void initPacket(Packet *packet);
uint32_t TxData(TCP_state *state, Cwnd *cwnd, Packet *packet);
#endif // SlowStart_H
