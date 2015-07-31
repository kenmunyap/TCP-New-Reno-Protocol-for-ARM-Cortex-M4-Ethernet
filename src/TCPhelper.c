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

void checkSSthresh(TCP_state *state,Cwnd *cwnd){
  if(cwnd->size <= (cwnd->ssthresh = ssthres)){
    state->state = SlowStartWaitACK;
  }else{
    state->state = CongestionAvoidance;
  } 
}

void retransmit(TCP_state *state, Cwnd *cwnd, Packet *packet, uint32_t lostPacket, uint32_t offset){
  static uint32_t flightSize;
  flightSize = offset - cwnd->offset;
  cwnd->ssthresh = max(flightSize/2, 2*SMSS);
  sendDataPacket(packet,&state->ptrBlock,lostPacket);
  cwnd->size = cwnd->ssthresh + 3*SMSS;
  state->state = FastRecovery;
}

uint32_t sendPacket(TCP_state *state, Packet *packet, uint32_t availableSize , uint32_t offset){
  availableSize = offset + availableSize;
  sendDataPacket(packet,&state->ptrBlock,availableSize);
  offset = offset + SMSS;
  
  return offset;
}

uint32_t duplicatePacketCount(TCP_state *state, uint32_t dupAckCounter){
  dupAckCounter += 1;
  if(dupAckCounter >= 3){
    dupAckCounter = 0;
    state->state = FastRetransmit;
  }
  return dupAckCounter;
}