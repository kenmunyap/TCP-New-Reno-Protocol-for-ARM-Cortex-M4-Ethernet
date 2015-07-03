#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "congestionWindow.h"
#include "returnACK.h"
#include "Packet.h"

uint32_t TxTCP2(TCP_state *state, Cwnd *cwnd){
  uint32_t offset;
  uint32_t requestedSize;
  uint32_t availableSize;
  uint32_t tempSize;
  
	switch(state->state){
		case SlowStart:
     //Starting of slowStart
      offset = cwndGetBeginningOffset(cwnd);
      if(returnSlowStartflag == 1) requestedSize = offset;
      else requestedSize = offset + MSS;
      
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&Block);
      if(availableSize) state->state = SlowStartWaitACK;
      //else break;
      
    case SlowStartWaitACK:
    //after ACK received
      if(requestedSize <= availableSize){
        tempSize = cwndIncrementWindow(cwnd,requestedSize);
        cwnd->size = tempSize;
      }
      else{
        if(cwnd->offset == offset && cwnd->size == tempSize){
          returnSlowStartflag = 1;
        }
      }
      cwnd->offset = offset + MSS;
      Block = (uint8_t *)(offset + MSS);
      state->state = SlowStart;
    
    break;
	}
}
