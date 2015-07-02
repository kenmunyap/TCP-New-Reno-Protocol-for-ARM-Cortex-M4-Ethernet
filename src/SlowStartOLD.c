#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "congestionWindow.h"
#include "returnACK.h"
#include "Packet.h"

uint8_t *Block;
uint32_t returnSlowStartflag = 0;

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = MSS;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
}
// Merging
// uint32_t TxData(TCP_state *state, Cwnd *cwnd){
  // static uint32_t offset;
  // static uint32_t requestedSize;
  // static uint32_t tempSize;
  // static uint32_t availableSize;
  // static uint8_t *getAddress;
  // static Packet *packet;
  
  // switch(state->state){
		// case SlowStart:
      // offset = cwndGetBeginningOffset(cwnd);
      // requestedSize = offset + MSS;
      // availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&getAddress);
      // if(availableSize){
        // state->state = SlowStartWaitACK;
      // }
    // break;
    
    // case SlowStartWaitACK:
      // availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&getAddress);
      // if(!availableSize){
        // packet = getDataPacket();
        // if(packet->seqNum == requestedSize)
        // {
          // cwnd->size = cwndIncrementWindow(cwnd,requestedSize);
        // }
        // else{
          // state->state = SlowStart;
        // }
      // }
    // break;
	// }
// }

//=============================================

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

uint32_t TxTCP(TCP_state *state, Cwnd *cwnd){
	uint32_t size;
	uint32_t requestedSize;
	uint32_t offset;
	switch(state->state){
		case SlowStart:	
		if(cwnd->offset == 0){
			offset = cwnd->offset;
			requestedSize = cwnd->size;
			cwndGetDataBlock(cwnd,offset,requestedSize,&Block);
			cwnd->lastByteSend = requestedSize;
			cwnd->selectedOffSet = offset;
			state->state=SlowStartWaitACK;
		}
		break;
		
		case SlowStartWaitACK:
			offset = cwnd->selectedOffSet;
			requestedSize = cwnd->lastByteSend;
			if(cwndGetDataBlock(cwnd,offset,requestedSize,&Block) == -1){
				if(returnACK() == cwnd->lastByteSend){	
					if(cwnd->size < ssthres){
						size = requestedSize - offset;
						cwnd->size = cwndIncrementWindow(cwnd,size);
						cwnd->offset = returnACK();
					}else{
						state->state = SlowStart;
					}
				}
			}else{
				
			}
			
		break;
    
	}
}
