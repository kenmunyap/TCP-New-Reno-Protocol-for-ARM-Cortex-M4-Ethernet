#ifndef Packet_H
#define Packet_H

#include "TxTCP.h"

uint32_t getDataPacket(Packet *packet,uint8_t **receiveBlock);
void sendDataPacket(Packet *packet, uint8_t **block,uint32_t size);

#endif // Packet_H