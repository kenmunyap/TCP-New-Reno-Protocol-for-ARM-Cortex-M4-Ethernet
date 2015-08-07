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

double floatMax(double valueA, double valueB){
  return valueA > valueB ? valueA : valueB;
}

void checkCAorSSBySSTHRESH(TCPSession *session){
  if(sessionCWND->size <= (sessionCWND->ssthresh = ssthres)){
    sessionState->state = SlowStartWaitACK;
  }else{
    sessionState->state = CongestionAvoidance;
  }
}

void incCACounter(uint32_t counter,TCPSession *session,uint32_t currentWindowSize,uint32_t ackNo){
  if(counter == 0){
    sessionCWND->size = cwndIncrementWindow(sessionCWND,currentWindowSize);
    sessionCWND->offset = ackNo;
    sessionState->state = CongestionAvoidance;
  }else{
    sessionCWND->size = currentWindowSize;
    sessionCWND->offset = ackNo;
    sessionState->state = CongestionAvoidance;
  }
}

void retransmit(TCPSession *session, Packet *packet, uint32_t lostPacket){
  uint32_t newFlightSize;
  sessionCWND->flightSize = session->offset - sessionCWND->offset;
  newFlightSize = roundOffValue(sessionCWND->flightSize);
  sessionCWND->ssthresh = max(newFlightSize/2, 2*SMSS);
  sessionCWND->lostPacket = sessionCWND->lostPacket+SMSS;
  sendDataPacket(packet,&sessionState->ptrBlock,sessionCWND->lostPacket);
  sessionCWND->size = sessionCWND->ssthresh + 3*SMSS;
  sessionCWND->recover = sessionCWND->size+sessionCWND->offset;
}

void sendPacket(TCPSession *session, Packet *packet, uint32_t availableSize){
  availableSize = session->offset + availableSize;
  sendDataPacket(packet,&sessionState->ptrBlock,availableSize);
  session->offset = session->offset + SMSS;
}

void duplicatePacketCount(TCPSession *session,uint32_t ackNo){
  session->dupAckCounter += 1;
  if(session->dupAckCounter >= 3){
    session->dupAckCounter = 0;
    sessionCWND->lostPacket = ackNo;
    sessionState->state = FastRetransmit;
  }
}

uint32_t roundOffValue(uint32_t valueToRoundOff){
  if(valueToRoundOff%10 != 0){
    valueToRoundOff = valueToRoundOff - 25;
    return valueToRoundOff;
  }
}

