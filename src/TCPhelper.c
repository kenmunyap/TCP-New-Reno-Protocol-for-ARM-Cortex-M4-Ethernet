#include <stdio.h>
#include <stdint.h>
#include "TxTCP.h"
#include "congestionWindow.h"
#include "Packet.h"
#include "Timer.h"
#include "TCPhelper.h"

/******************************************************************************
 * minimum
 *
 *  Operation:
 *          To check value A or value B is smaller
 *
 *  Input:
 *         value A and value B
 *          
 *  Return:
 *          return value A / value B
 ******************************************************************************/
uint32_t min(uint32_t valueA, uint32_t valueB){
  return valueA < valueB ? valueA : valueB;
}

/******************************************************************************
 * maximum
 *
 *  Operation:
 *          To check value A or value B is larger
 *
 *  Input:
 *         value A and value B
 *          
 *  Return:
 *          return value A / value B
 ******************************************************************************/
uint32_t max(uint32_t valueA, uint32_t valueB){
  return valueA > valueB ? valueA : valueB;
}

/******************************************************************************
 * floating point with maximum value
 *
 *  Operation:
 *          To check value A and value B in double and get the maximum value
 *
 *  Input:
 *         value A and value B
 *          
 *  Return:
 *          return value A / value B
 ******************************************************************************/
double floatMax(double valueA, double valueB){
  return valueA > valueB ? valueA : valueB;
}

/******************************************************************************
 * check CA or SS Using SSTHRESH
 *
 *  Operation:
 *          check Congestion Avoidance and Slow Start by using 
 *          cwnd->size is smaller or larger than SSTHRESH 
 *
 *  Input:
 *         cwnd->size and cwnd->ssthresh
 *          
 *  Return:
 *          no return
 ******************************************************************************/
void checkCAorSSBySSTHRESH(TCPSession *session){
  if(sessionCWND->size <= (sessionCWND->ssthresh = ssthres)){
    sessionState->state = SlowStartWaitACK;
  }else{
    sessionState->state = CongestionAvoidance;
  }
}

/******************************************************************************
 * increment Congestion Avoidance Counter
 *
 *  Operation:
 *      Increment Congestion Window by using counter
*       (mean: every RTT increment 1) 
 *
 *
 *  Input:
 *         TCPSession, currentWindowSize and ackNo
 *          
 *  Return:
 *          no return
 ******************************************************************************/
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

/******************************************************************************
 * retransmit
 *
 *  Operation:
 *        To perform retransmition of missing packet  
 *
 *  Input:
 *         cwnd->size and cwnd->ssthresh
 *          
 *  Return:
 *          no return
 ******************************************************************************/
void retransmit(TCPSession *session, Packet *packet){
  uint32_t newFlightSize;
  sessionCWND->flightSize = session->offset - sessionCWND->offset;
  newFlightSize = roundOffValue(sessionCWND->flightSize);
  sessionCWND->ssthresh = max(newFlightSize/2, 2*SMSS);
  sessionCWND->lostPacket = sessionCWND->lostPacket+SMSS;
  sendDataPacket(packet,&sessionState->ptrBlock,sessionCWND->lostPacket);
  sessionCWND->size = sessionCWND->ssthresh + 3*SMSS;
  sessionCWND->recover = sessionCWND->size+sessionCWND->offset;
}

/******************************************************************************
 * send Packet
 *
 *  Operation:
 *        To perform send Packet 
 *
 *  Input:
 *        TCPSession,Packet and availableSize
 *          
 *  Return:
 *        no return
 ******************************************************************************/
void sendPacket(TCPSession *session, Packet *packet, uint32_t availableSize){
  availableSize = session->offset + availableSize;
  sendDataPacket(packet,&sessionState->ptrBlock,availableSize);
  session->offset = session->offset + SMSS;
}

/******************************************************************************
 * duplicate Packet Counter
 *
 *  Operation:
 *        To check the times of duplicate acknowledgement
 *
 *  Input:
 *         TCPSession and ackNo
 *          
 *  Return:
 *          no return
 ******************************************************************************/
void duplicatePacketCount(TCPSession *session,uint32_t ackNo){
  session->dupAckCounter += 1;
  if(session->dupAckCounter >= 3){
    session->dupAckCounter = 0;
    sessionCWND->lostPacket = ackNo;
    sessionState->state = FastRetransmit;
  }
}

/******************************************************************************
 * check SlowStart (ackNo == SequenceNumber /ackNo != SequenceNumber )
 *
 *  Operation:
 *     To perform ackNo is equal or not equal SequenceNumber in Slow Start State 
 *
 *  Input:
 *     ackNo, sequenceNumber, currentWindowSize and TCPSession
 *          
 *  Return:
 *     no return
 ******************************************************************************/
void checkSSisACKNoEqualSequenceNumber(uint32_t ackNo,uint32_t sequenceNumber,uint32_t currentWindowSize,TCPSession *session){
    if(ackNo >= sequenceNumber){      
      sessionCWND->size = cwndIncrementWindow(sessionCWND,currentWindowSize);
      sessionCWND->offset = ackNo;
      checkCAorSSBySSTHRESH(session);
    }else{
      session->dupAckCounter = 1;
      sessionState->state = CongestionAvoidance;
    }
}

/******************************************************************************
 * check Congestion Avoidance (ackNo == SequenceNumber /ackNo != SequenceNumber)
 *
 *  Operation:
 *        To perform ackNo = SequenceNumber in Slow Start State 
 *
 *  Input:
 *        ackNo, sequenceNumber, currentWindowSize and TCPSession
 *          
 *  Return:
 *        counter
 ******************************************************************************/
uint32_t checkCAisACKNoEqualSequenceNumber(uint32_t ackNo,uint32_t sequenceNumber,uint32_t currentWindowSize,uint32_t counter,TCPSession *session){
    static uint32_t checkCounter;
    
    if(ackNo >= sequenceNumber){
      checkCounter = ((ackNo-sequenceNumber)+SMSS)/SMSS;
      session->dupAckCounter = 0;
      counter = counter-checkCounter;
      incCACounter(counter,session,currentWindowSize,ackNo);
    }else if(ackNo == sessionCWND->offset){
      counter = 0;
      duplicatePacketCount(session,ackNo);
    }
    return counter;
}

/******************************************************************************
 * round off value
 *
 *  Operation:
 *        To perform round off value to smaller value
 *
 *  Input:
 *       valueToRoundOff
 *          
 *  Return:
 *       valueToRoundOff
 ******************************************************************************/
uint32_t roundOffValue(uint32_t valueToRoundOff){
  if(valueToRoundOff%10 != 0){
    valueToRoundOff = valueToRoundOff - 25;
    return valueToRoundOff;
  }
}

