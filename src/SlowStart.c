#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "congestionWindow.h"
#include "returnACK.h"

uint8_t **Block;

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = MSS;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
}

uint32_t TxTCP(TCP_state *state, Cwnd *cwnd){
	uint32_t requestedSize;
	uint32_t offset;
	switch(state->state){
		case SlowStart:	
		if(cwnd->offset == 0){
			offset = cwnd->offset;
			requestedSize = cwnd->size;
			cwndGetDataBlock(cwnd,offset,requestedSize,Block);
			cwnd->lastByteSend = requestedSize;
			if(cwndGetDataBlock(cwnd,offset,requestedSize,Block) == -1){
				state->state = SlowStartWaitACK;
			}
		}
		break;
		
		case SlowStartWaitACK:
			if(returnACK() == cwnd->lastByteSend){
				// Increment
			}
		break;
    
	}
}

