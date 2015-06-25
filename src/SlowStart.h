#ifndef SlowStart_H
#define SlowStart_H

#include <stdint.h>

#define MSS 50

uint8_t Buffer[1000];
extern uint8_t **Block;

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
}Cwnd;

void cwndInitWindow(Cwnd *cwnd);
void initTCPState(TCP_state *state);
Cwnd *TxTCP(TCP_state *state, Cwnd *cwnd);

#endif // SlowStart_H
