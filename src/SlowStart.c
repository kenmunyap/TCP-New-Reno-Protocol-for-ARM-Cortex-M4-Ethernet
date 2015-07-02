#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "congestionWindow.h"
#include "Packet.h"

uint8_t *Block;
uint32_t returnSlowStartflag = 0;

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = MSS;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
}

uint32_t TxData(TCP_state *state, Cwnd *cwnd){
  static Packet *packet;
  static uint32_t offset;
  uint32_t requestedSize;
  static uint32_t tempSize;
  static uint32_t availableSize;
  static uint8_t *getAddress; 
  static uint32_t sequenceNumber;

  switch(state->state){
		case SlowStart:
      offset = cwndGetBeginningOffset(cwnd);
      requestedSize = offset + MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&Block); 
      if(availableSize != 0){
        offset = availableSize;
          here:
        if(availableSize != 0){
          sendDataPacket(packet,&Block,availableSize);
          availableSize--;
          goto here;
        }else{
          state->state = SlowStartWaitACK;
        }
      }else{
        printf("not available");
        state->state = SlowStart;
      }
    break;
    
    case SlowStartWaitACK:
      requestedSize = offset + MSS;
      availableSize = cwndGetDataBlock(cwnd,offset,requestedSize,&Block);
      tempSize = cwnd->offset + MSS;
      printf("tempSize: %d");
      if(availableSize != 0){
        // sendDataPacket(packet,availableSize);
        availableSize--;
      }else{
        // sequenceNumber = getDataPacket();
        // if(sequenceNumber == tempSize){
          // cwnd->size = cwndIncrementWindow(cwnd,requestedSize);
          // cwnd->offset = sequenceNumber;
          // state->state = SlowStartWaitACK;
        // }else{
          // printf("goes to fast retransmit");
        // }
      }
    break;
	}
}
>>>>>>> 3cc8d5ed6f8029e13b8e2b04a923f44589dd6ba1
