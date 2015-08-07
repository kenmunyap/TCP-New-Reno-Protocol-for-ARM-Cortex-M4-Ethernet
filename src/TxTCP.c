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
  static uint32_t counterTime;
  static uint32_t counter = 0;
  static uint32_t tempCwndSize;
  static uint32_t recover;
  static uint32_t checkCounter;

  switch(state->state){
    case SlowStart:
      offset = cwndGetBeginningOffset(cwnd);
      cwnd->recover = offset;
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
            checkCAorSSBySSTHRESH(state,cwnd);
          }else{
            dupAckCounter = 1;
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
            checkCounter = ((ackNo-sequenceNumber)+SMSS)/SMSS;
            dupAckCounter = 0;
            counter = counter-checkCounter; 
            incCACounter(counter,state,cwnd,currentWindowSize,ackNo);
          }else if(ackNo == cwnd->offset){
            dupAckCounter = duplicatePacketCount(cwnd,state,dupAckCounter,ackNo);
          }
        }
    break;

    case FastRetransmit:
      retransmit(state,cwnd,packet,cwnd->lostPacket,offset);
      state->state = FastRecovery;
    break;

    case FastRecovery:
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&state->ptrBlock);
      cwnd->recover = cwnd->size+cwnd->offset;
      if(availableSize != 0){
        offset = sendPacket(state,packet,availableSize,offset);
        state->state = FastRecovery;
      }else{
        ackNo = getDataPacket(packet,&receiveData); 
        if(ackNo == cwnd->recover){
          cwnd->size = cwnd->size+SMSS;
          cwnd->offset = cwnd->recover;
          state->state = CongestionAvoidance;
        }
        else{
          cwnd->size += SMSS;
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
