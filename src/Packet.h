#ifndef Packet_H
#define Packet_H

#include "SlowStart.h"

int getDataPacket(Packet *packet);
void sendDataPacket(Packet *packet, uint8_t **block,int size);

#endif // Packet_H