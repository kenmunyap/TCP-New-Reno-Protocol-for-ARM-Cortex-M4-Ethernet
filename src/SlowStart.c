#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "CongestionWindow.h"

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = MSS;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
}

uint32_t TxTCP(TCP_state *state,Cwnd *cwnd){
	switch(state->state){
		case SlowStart:	
			// if(Buffer[0] != 0){
				// cwndGetDataBlock(cwnd,offset,requestedSize,block);
			// }
			// else{
				// state->state = SlowStart;
			// }
		break;
	}
}

