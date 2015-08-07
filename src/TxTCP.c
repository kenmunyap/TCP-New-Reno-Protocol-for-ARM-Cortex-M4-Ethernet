#include <stdio.h>
#include <stdint.h>
#include "TxTCP.h"
#include "congestionWindow.h"
#include "Packet.h"
#include "Timer.h"

uint8_t *receiveData;

void cwndInitWindow(TCPSession *session){
	sessionCWND->offset = 0;
	sessionCWND->size = SMSS;
}

void initTCPState(TCPSession *session){
	sessionState->state = SlowStart;
  sessionState->ptrBlock = NULL;
}

uint32_t TxTCPSM(TCPSession *session, Packet *packet){
  static uint32_t currentWindowSize;
  static uint32_t ackNo;
  static uint32_t availableSize;
  static uint8_t *getAddress;
  static uint32_t sequenceNumber;
  static uint32_t thresholdValue;
  static uint32_t counterTime;
  static uint32_t counter = 0;
  static uint32_t tempCwndSize;
  static uint32_t recover;
  static uint32_t checkCounter;

  switch(sessionState->state){
    case SlowStart:
      session->offset = cwndGetBeginningOffset(sessionCWND);
      sessionCWND->recover = session->offset;
      session->requestedSize = session->offset + SMSS;
      availableSize = cwndGetDataBlock(sessionCWND,session->offset,session->requestedSize,&sessionState->ptrBlock);
      if(availableSize != 0){
          sendPacket(session,packet,availableSize);
          sessionState->state = SlowStartWaitACK;
      }
    break;

    case SlowStartWaitACK:
      session->requestedSize = SMSS;
      availableSize = cwndGetDataBlock(sessionCWND,session->offset,session->requestedSize,&sessionState->ptrBlock);
        if(availableSize != 0){
          sendPacket(session,packet,availableSize);
          sessionState->state = SlowStartWaitACK;
        }else{
          ackNo = getDataPacket(packet,&receiveData);
          sequenceNumber = sessionCWND->offset+SMSS;
          currentWindowSize = sessionCWND->size;
          if(ackNo >= sequenceNumber){
            sessionCWND->offset = ackNo;
            sessionCWND->size = cwndIncrementWindow(sessionCWND,currentWindowSize);
            checkCAorSSBySSTHRESH(session);
          }else{
            session->dupAckCounter = 1;
            sessionState->state = CongestionAvoidance;
          }
        }
    break;

    case CongestionAvoidance:
      session->requestedSize = SMSS;
      availableSize = cwndGetDataBlock(sessionCWND,session->offset,session->requestedSize,&sessionState->ptrBlock);
      if(availableSize != 0){
        sendPacket(session,packet,availableSize);
        sessionState->state = CongestionAvoidance;
      }else{
        ackNo = getDataPacket(packet,&receiveData);
        sequenceNumber = sessionCWND->offset+SMSS;
        currentWindowSize = sessionCWND->size;
        if (!counter) counter  = sessionCWND->size/SMSS;
          if(ackNo >= sequenceNumber){
            checkCounter = ((ackNo-sequenceNumber)+SMSS)/SMSS;
            session->dupAckCounter = 0;
            counter = counter-checkCounter;
            incCACounter(counter,session,currentWindowSize,ackNo);
          }else if(ackNo == sessionCWND->offset){
            counter = 0;
            duplicatePacketCount(session,ackNo);
          }
        }
    break;

    case FastRetransmit:
      retransmit(session,packet,sessionCWND->lostPacket);
      sessionState->state = FastRecovery;
    break;

    case FastRecovery:
      availableSize = cwndGetDataBlock(sessionCWND,session->offset,session->requestedSize,&sessionState->ptrBlock);
      sessionCWND->recover = sessionCWND->size+sessionCWND->offset;
      if(availableSize != 0){
        sendPacket(session,packet,availableSize);
        sessionState->state = FastRecovery;
      }else{
        ackNo = getDataPacket(packet,&receiveData); 
        if(ackNo == sessionCWND->recover){
          sessionCWND->size = sessionCWND->size+SMSS;
          sessionCWND->offset = sessionCWND->recover;
          sessionState->state = CongestionAvoidance;
        }
        else{
          sessionCWND->size += SMSS;
          sessionState->state = FastRecovery;
        }
      }
    break;

    default:
      sessionCWND->ssthresh = sessionCWND->size / 2;
      sessionCWND->size = SMSS;
      sessionState->state = SlowStart;
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
