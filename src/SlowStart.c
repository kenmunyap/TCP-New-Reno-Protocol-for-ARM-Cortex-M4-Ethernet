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

Cwnd *TxTCP(TCP_state *state, Cwnd *cwnd){
  uint32_t offset;
  uint32_t requestedSize;
  uint32_t availableSize;
  
	switch(state->state){
		case SlowStart:
      
      offset = cwndGetBeginningOffset(cwnd);
      requestedSize = offset + MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,Block);
      
      printf("TEST cwnd->offset : %d\n",cwnd->offset);
      printf("TEST cwnd->size   : %d\n",cwnd->size);
      printf("TEST offset       : %d\n",offset);
      printf("TEST requestedSize: %d\n",requestedSize);
      printf("TEST availableSize: %d\n",availableSize);
      printf("TEST Block        : %d\n\n",Block);
      
      if(requestedSize <= availableSize){
        cwnd->offset = cwndIncrementWindow(cwnd,requestedSize);
        printf("TEST cwnd->offset : %d\n",cwnd->offset);
        printf("TEST cwnd->size   : %d\n",cwnd->size);
        printf("TEST offset       : %d\n",offset);
        printf("TEST requestedSize: %d\n",requestedSize);
        printf("TEST availableSize: %d\n",availableSize);
        printf("TEST Block        : %d\n\n",Block);
      }
      else return NULL;

		break;
    
    case SlowStartWaitACK:
    break;
	}
  return cwnd;
}

