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
	uint32_t size;
	uint32_t requestedSize;
	uint32_t offset;
	switch(state->state){
		case SlowStart:	
		if(cwnd->offset == 0){
			offset = cwnd->offset;
			requestedSize = cwnd->size;
			cwndGetDataBlock(cwnd,offset,requestedSize,Block);
			cwnd->lastByteSend = requestedSize;
			cwnd->selectedOffSet = offset;
			state->state=SlowStartWaitACK;
		}
		break;
		
		case SlowStartWaitACK:
			offset = cwnd->selectedOffSet;
			requestedSize = cwnd->lastByteSend;
			if(cwndGetDataBlock(cwnd,offset,requestedSize,Block) == -1){
				if(returnACK() == cwnd->lastByteSend){	
					if(cwnd->size < ssthres){
						size = requestedSize - offset;
						cwnd->size = cwndIncrementWindow(cwnd,size);
						cwnd->offset = returnACK();
						state->state = SlowStartWaitACK;
					}else{
						state->state = SlowStart;
					}
				}
			}else{
				
			}
			
		break;
    
	}
}

