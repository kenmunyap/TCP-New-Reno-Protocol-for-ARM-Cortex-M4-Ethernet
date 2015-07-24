#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "congestionWindow.h"
#include "Packet.h"
#include "Timer.h"

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
  static uint32_t lostPacket;
  
  switch(state->state){
    case SlowStart:
      offset = cwndGetBeginningOffset(cwnd);
      requestedSize = offset + MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock); 
      if(availableSize != 0){
        // if(roundTT() < 50){
          sendDataPacket(packet,&state->ptrBlock,availableSize);
          state->state = SlowStartWaitACK;
          offset = offset+MSS;
        // }else{
          // state->state = FastRetransmit;
        // }
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
              ackNo = getDataPacket(packet,&receiveData);
              sequenceNumber = cwnd->offset+MSS;
              currentWindowSize = cwnd->size;
              if(ackNo >= sequenceNumber){
                cwnd->offset = ackNo;
                cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
                if(cwnd->size <= (cwnd->ssthresh = ssthres)){
                  state->state = SlowStartWaitACK;
                }else{
                  state->state = CongestionAvoidance;
                }
              }else{
                dupAckCounter = 1;
                lostPacket = ackNo;
                state->state = CongestionAvoidance;
              }
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
            if(cwnd->size == cwnd->offset){
              cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
              cwnd->offset = ackNo;
              state->state = CongestionAvoidance;
            }else{
              cwnd->offset = ackNo;
              state->state = CongestionAvoidance;
            }
          }else if(ackNo == cwnd->offset){
              dupAckCounter += 1;
              if(dupAckCounter >= 3){
                dupAckCounter = 0;
                state->state = FastRetransmit;
              }
            }
          }
        // }else{
          // state->state = FastRetransmit;
        // }
    break;

    case FastRetransmit:
      sendDataPacket(packet,&state->ptrBlock,lostPacket);
      cwnd->size = MSS;
      state->state = SlowStartWaitACK;
    break;
    
    case FastRecovery:
    
    break;
}
}
