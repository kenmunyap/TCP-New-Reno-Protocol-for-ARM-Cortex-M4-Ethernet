#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "congestionWindow.h"
#include "Packet.h"

uint8_t *Block;
uint8_t *receiveData;
uint32_t returnSlowStartflag = 0;

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = MSS;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
}




// Merging
uint32_t TxData(TCP_state *state, Cwnd *cwnd, Packet *packet){
  static uint32_t offset;
  uint32_t requestedSize;
  static uint32_t nextSeqNo;
  static uint32_t availableSize;
  static uint8_t *getAddress; 
  static uint32_t sequenceNumber;
  
  
  switch(state->state){
		case SlowStart:
      offset = cwndGetBeginningOffset(cwnd);
      requestedSize = offset + MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&Block); 
      if(availableSize != 0){
          here:
        if(offset != availableSize){
          sendDataPacket(packet,&Block,offset);
          offset++;
          goto here;
        }else{
          state->state = SlowStartWaitACK;
        }
      }else{
        printf("not available");
        state->state = SlowStart;
      }
    break;
    
    case SlowStartWaitACK:
      requestedSize = offset;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&Block);
      if(availableSize != 0){
        sendDataPacket(packet,&Block,offset);
        availableSize--;
      }else{
        sequenceNumber = getDataPacket(packet,&receiveData);
        nextSeqNo = cwnd->offset+MSS;
        if(sequenceNumber == nextSeqNo){
          cwnd->size = cwndIncrementWindow(cwnd,requestedSize);
          cwnd->offset = sequenceNumber;
          state->state = SlowStartWaitACK;
        }else{
          printf("goes to fast retransmit");
        }
      }
    break;
	}
}
