#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "CongestionWindow.h"

uint8_t **Block;

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = MSS;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
}

void TxTCP(TCP_state *state, Cwnd *cwnd){
  uint32_t offset;
  uint32_t requestedSize;
  uint32_t availableSize;
  
	switch(state->state){
		case SlowStart:
     //Starting of slowStart
      offset = cwndGetBeginningOffset(cwnd);
      //printf("offset: %d\n",offset);
      requestedSize = offset + MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,Block);

      if(availableSize) state->state = SlowStartWaitACK;
      else break;
    
    case SlowStartWaitACK:
    //after ACK received
      if(requestedSize <= availableSize){
        cwnd->size = cwndIncrementWindow(cwnd,requestedSize);
        cwnd->offset = offset + MSS;
        Block = (uint8_t **)(offset + MSS);
        state->state = SlowStart;
      }
    
    break;
	}
}