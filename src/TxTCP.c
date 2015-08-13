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
          checkSSisACKNoEqualSequenceNumber(ackNo,sequenceNumber,currentWindowSize,session);
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
          counter = checkCAisACKNoEqualSequenceNumber(ackNo,sequenceNumber,currentWindowSize,counter,session);
      }
    break;

    case FastRetransmit:
      retransmit(session,packet);
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
        currentWindowSize = sessionCWND->size;
        if(ackNo == sessionCWND->recover){
          printf("Fully ACK");
          sessionCWND->size = min(sessionCWND->ssthresh, sessionCWND->flightSize+SMSS);
          sessionCWND->offset = sessionCWND->recover;
          sessionState->state = CongestionAvoidance;
        }else{
          printf("Partial ACK");
          sessionCWND->lostPacket = ackNo + SMSS;
          sendDataPacket(packet,&sessionState->ptrBlock,sessionCWND->lostPacket);
          sessionCWND->offset = ackNo;
          sessionCWND->size = cwndIncrementWindow(sessionCWND,currentWindowSize);
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
