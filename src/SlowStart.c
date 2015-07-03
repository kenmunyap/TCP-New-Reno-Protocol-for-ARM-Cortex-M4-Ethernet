#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "congestionWindow.h"
#include "Packet.h"

uint8_t *Block;
uint8_t *receiveData;


void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = MSS;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
}

uint32_t TxData(TCP_state *state, Cwnd *cwnd, Packet *packet){
  static uint32_t offset;
  static uint32_t currentWindowSize;
  static uint32_t requestedSize;
  static uint32_t nextSeqNo;
  static uint32_t availableSize;
  static uint8_t *getAddress; 
  static uint32_t sequenceNumber;
  static uint32_t counter = 0;

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
        printf("\nnot available");
        state->state = SlowStart;
      }
    break;
    
    case SlowStartWaitACK:
      requestedSize = MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&Block);
      if(availableSize != 0){
        availableSize = availableSize + offset;
        go:
        if(offset != availableSize){
          sendDataPacket(packet,&Block,offset);
          offset++;
          goto go;
        }else{
          state->state = SlowStartWaitACK;
        }
      }else{
        sequenceNumber = getDataPacket(packet,&receiveData);
        nextSeqNo = cwnd->offset+MSS;
        currentWindowSize = cwnd->offset+MSS;
        if(sequenceNumber == nextSeqNo){
          cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
          cwnd->offset = sequenceNumber;
          state->state = SlowStartWaitACK;
        }else{
          counter = counter+1;
          if(counter == 3){ 
            printf("\ngoes to fast retransmit");
            counter = 0;
          }else{
            state->state = SlowStartWaitACK;
          }
        }
      }
    break;
	}
}
