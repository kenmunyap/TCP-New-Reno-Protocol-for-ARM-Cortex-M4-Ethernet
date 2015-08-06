#include <stdio.h>
#include <stdint.h>
#include "TxTCP.h"
#include "congestionWindow.h"
#include "Packet.h"
#include "Timer.h"
#include "TCPhelper.h"

uint32_t min(uint32_t valueA, uint32_t valueB){
  return valueA < valueB ? valueA : valueB;
}

uint32_t max(uint32_t valueA, uint32_t valueB){
  return valueA > valueB ? valueA : valueB;
}

void checkCAorSSBySSTHRESH(TCP_state *state,Cwnd *cwnd){
  if(cwnd->size <= (cwnd->ssthresh = ssthres)){
    state->state = SlowStartWaitACK;
  }else{
    state->state = CongestionAvoidance;
  }
}

void incCACounter(uint32_t counter,TCP_state *state,Cwnd *cwnd,uint32_t currentWindowSize,uint32_t ackNo){
  if(counter == 0){
    cwnd->size = cwndIncrementWindow(cwnd,currentWindowSize);
    cwnd->offset = ackNo;
    state->state = CongestionAvoidance;
  }else{
    cwnd->size = currentWindowSize;
    cwnd->offset = ackNo;
    state->state = CongestionAvoidance;
  }
}
void retransmit(TCP_state *state, Cwnd *cwnd, Packet *packet, uint32_t lostPacket, uint32_t offset){
  uint32_t newFlightSize;
  cwnd->flightSize = offset - cwnd->offset;
  newFlightSize = roundOffValue(cwnd->flightSize);
  cwnd->ssthresh = max(newFlightSize/2, 2*SMSS);
  cwnd->lostPacket = cwnd->lostPacket+SMSS;
  sendDataPacket(packet,&state->ptrBlock,cwnd->lostPacket);
  cwnd->size = cwnd->ssthresh + 3*SMSS;
  cwnd->recover = cwnd->size+cwnd->offset;
}

uint32_t sendPacket(TCP_state *state, Packet *packet, uint32_t availableSize , uint32_t offset){
  availableSize = offset + availableSize;
  sendDataPacket(packet,&state->ptrBlock,availableSize);
  offset = offset + SMSS;

  return offset;
}

uint32_t duplicatePacketCount(Cwnd *cwnd, TCP_state *state, uint32_t dupAckCounter,uint32_t ackNo){
  dupAckCounter += 1;
  if(dupAckCounter >= 3){
    dupAckCounter = 0;
    cwnd->lostPacket = ackNo;
    state->state = FastRetransmit;
  }
  return dupAckCounter;
}

uint32_t roundOffFlightSize(Cwnd *cwnd){
  if(cwnd->flightSize%10 != 0){
    cwnd->flightSize = cwnd->flightSize - 25;
  }
}

uint32_t roundOffValue(uint32_t valueToRoundOff){
  if(valueToRoundOff%10 != 0){
    valueToRoundOff = valueToRoundOff - 25;
    return valueToRoundOff;
  }
}
