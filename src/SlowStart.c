#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "congestionWindow.h"
#include "Packet.h"

uint8_t *receiveData;

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = MSS;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
  state->ptrBlock = NULL;
}

uint32_t TxTCPSM(TCP_state *state, Cwnd *cwnd, Packet *packet){
  static uint32_t offset;
  static uint32_t currentWindowSize;
  static uint32_t requestedSize;
  static uint32_t ackNo;
  static uint32_t availableSize;
  static uint8_t *getAddress; 
  static uint32_t sequenceNumber;
  static uint32_t counter = 0;

  switch(state->state){
		case SlowStart:
      offset = cwndGetBeginningOffset(cwnd);
      requestedSize = offset + MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock); 
      if(availableSize != 0){
        sendDataPacket(packet,&state->ptrBlock,availableSize);
        state->state = SlowStartWaitACK;
        offset = offset+MSS;
      }else{
        printf("\n not available");
        state->state = SlowStart;
      }
    break;
    
    case SlowStartWaitACK:
      requestedSize = MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock);
      if(availableSize != 0){
        availableSize = offset + availableSize;
        sendDataPacket(packet,&state->ptrBlock,availableSize);
        state->state = SlowStartWaitACK;
        offset = offset+MSS;
      }else{
        sequenceNumber = getDataPacket(packet,&receiveData);
        ackNo = cwnd->offset+MSS;
        currentWindowSize = cwnd->offset+MSS;
        if(sequenceNumber >= ackNo){
          cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
          cwnd->offset = sequenceNumber;
          state->state = SlowStartWaitACK;
          counter = 0;
        }else{
          state->state = CongestionAvoidance;
        }
      }
    break;
    
	}
}
