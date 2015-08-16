#include <stdio.h>
#include <stdint.h>
#include "TxTCP.h"
#include "CongestionWindows.h"
#include "Packet.h"

uint8_t Buffer[1000];
#define Val 'A'

uint32_t cwndGetBeginningOffsets(Cwnd *cwnd){
  return cwnd->offset;
}

uint32_t cwndIncrementWindows(Cwnd *cwnd, uint32_t size){
  
  uint32_t windowSize;
  
  windowSize = cwnd->size + SMSS;
  
  return windowSize;
}

uint32_t cwndGetDataBlocks(Cwnd *cwnd, uint32_t offset, uint32_t requestedSize, uint8_t **block){
  uint32_t i;
  uint32_t counter = 0;
  uint32_t availableSize;
  uint32_t Offset = cwnd->offset;
  uint32_t Size = cwnd->size;
  
  *block = Buffer;
  
  if (((Size - requestedSize) < Size) && (Offset - offset == 0)){
    for(i = 0 ; i < requestedSize ; i++){
      Buffer[i] = Val+1;
      if(Buffer[i]){
        counter++;
      }
      if(counter == requestedSize){
        availableSize = requestedSize;
      }
    }
    return availableSize;
  }
  return 0;
}

uint32_t slideWindow(TCPSession *session){
  uint32_t availableSize;
  uint32_t sendDataSize;
  sendDataSize= session->offset - sessionCWND->offset;
  if(sendDataSize > sessionCWND->size){
    return 0;
  }else{
    return availableSize = session->requestedSize;
  }
}

