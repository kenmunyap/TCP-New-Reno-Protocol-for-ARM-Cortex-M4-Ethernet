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
  static uint32_t dupAckCounter;
  static uint32_t thresholdValue;
  
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
        state->state = SlowStart;
      }
    break;
    
    case SlowStartWaitACK:
      if(cwnd->size <= ssthres){
        requestedSize = MSS;
        availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock);
          if(availableSize != 0){
            availableSize = offset + availableSize;
            sendDataPacket(packet,&state->ptrBlock,availableSize);
            state->state = SlowStartWaitACK;
            offset = offset+MSS;
          }else{
            ackNo = getDataPacket(packet,&receiveData);
            sequenceNumber = cwnd->offset+MSS;
            currentWindowSize = cwnd->size;
            if(ackNo >= sequenceNumber){
              cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
              cwnd->offset = ackNo;
              state->state = SlowStartWaitACK;
            }else{
              dupAckCounter = 1;
              state->state = CongestionAvoidance;
            }
          }
      }else{
        state->state = CongestionAvoidance;
      }
    break;
    
    case CongestionAvoidance:
        availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock);
        if(availableSize != 0){
          availableSize = offset + availableSize;
          sendDataPacket(packet,&state->ptrBlock,availableSize);
          state->state = CongestionAvoidance;
          offset = offset+MSS;
        }else{
          ackNo = getDataPacket(packet,&receiveData);
          sequenceNumber = cwnd->offset+MSS;
          currentWindowSize = cwnd->size;
          if(ackNo == sequenceNumber){
            dupAckCounter = 0;
            cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
            cwnd->offset = ackNo;
            state->state = SlowStartWaitACK;
          }else if(ackNo == cwnd->offset){
            dupAckCounter += 1;
            if(dupAckCounter >= 3){
              dupAckCounter = 0;
              state->state = FastRetransmit;
            }
          }
        }
    break;

    case FastRetransmit:
      sequenceNumber = sequenceNumber + MSS;
      sendDataPacket(packet,&state->ptrBlock,sequenceNumber);
      cwnd->size = MSS;
    break;
}
}

//after CA -> cwnd->size/2
//cwnd->size +=  (MSS*MSS)/cwnd->size if(MSS = 50), size = 100   mean 0 - 100 range


