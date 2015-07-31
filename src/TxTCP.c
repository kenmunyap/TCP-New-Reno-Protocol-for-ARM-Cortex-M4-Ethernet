#include <stdio.h>
#include <stdint.h>
#include "TxTCP.h"
#include "congestionWindow.h"
#include "Packet.h"
#include "Timer.h"

uint8_t *receiveData;

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = SMSS;
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
  static uint32_t counterTime;
  static uint32_t counter = 0;
  static uint32_t tempCwndSize;
  
  switch(state->state){
    case SlowStart:
      offset = cwndGetBeginningOffset(cwnd);
      requestedSize = offset + SMSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock); 
      if(availableSize != 0){
          offset = sendPacket(state,packet,availableSize,offset);
          state->state = SlowStartWaitACK;
      }
    break;
    
    case SlowStartWaitACK:
      requestedSize = SMSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock);
        if(availableSize != 0){
          offset = sendPacket(state,packet,availableSize,offset);
          state->state = SlowStartWaitACK;
        }else{
          ackNo = getDataPacket(packet,&receiveData);
          sequenceNumber = cwnd->offset+SMSS;
          currentWindowSize = cwnd->size;
          if(ackNo >= sequenceNumber){
            cwnd->offset = ackNo;
            cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
            checkSSthresh(state,cwnd);
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
        offset = sendPacket(state,packet,availableSize,offset);
        state->state = CongestionAvoidance;
      }else{
        ackNo = getDataPacket(packet,&receiveData);
        sequenceNumber = cwnd->offset+SMSS;
        currentWindowSize = cwnd->size;
        if (!counter) counter  = cwnd->size/SMSS;
        if(ackNo >= sequenceNumber){
          dupAckCounter = 0;
          counter --; 
          // if(counter == 0){
            // cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
            // cwnd->offset = ackNo;
            // state->state = CongestionAvoidance;
          // }else{
            // cwnd->offset = ackNo;
            // state->state = CongestionAvoidance;
          // }
        }else if(ackNo == cwnd->offset){
            dupAckCounter = duplicatePacketCount(state,dupAckCounter);
          }
        }
    break;
    
    case FastRetransmit:
      retransmit(state,cwnd,packet,lostPacket,offset);
    break;
    
    case FastRecovery:
       
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock);
      if(availableSize != 0){
        offset = sendPacket(state,packet,availableSize,offset);
        tempCwndSize -= SMSS;
        if(tempCwndSize == 0) state->state = CongestionAvoidance;
      }else{
        ackNo = getDataPacket(packet,&receiveData);
        sequenceNumber = cwnd->offset+SMSS;
        currentWindowSize = cwnd->size;
        tempCwndSize = cwnd->size;
        
        if(ackNo == sequenceNumber){
          printf("non-dupACK case\n");
          cwnd->size = cwnd->ssthresh;
          cwnd->offset = ackNo;
          state->state = FastRecovery;
        }
        else if(ackNo == cwnd->offset){
          printf("dupACK case\n");
          cwnd->size += SMSS;
          tempCwndSize += SMSS;
          state->state = FastRecovery;
        }
      }
    break;
    
    default:
      cwnd->ssthresh = cwnd->size / 2;
      cwnd->size = SMSS;
      state->state = SlowStart;
    break;
  }
}

// SlowStart left timeout 
// Congestion Avoidance timeout
// Fast Retransmit
// Fast Recovery
// Mock SendPacket
// Increment
// GetPacket
// CwndGetPacket
//if(timeout() == 50){
//    ssthress = cwnd->size/2 
//}


// timeout then  ssthresh = cwndsize/2 then goes bak to slow start
// duplicate 3 time = cwndsize/2 then goes to fast recover
// program still can send after exceed offset using mock
