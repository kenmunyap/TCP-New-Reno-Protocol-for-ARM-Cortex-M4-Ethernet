#ifndef SlowStart_H
#define SlowStart_H

#include <stdint.h>

#define MSS 50

typedef enum{
	sendData,
	waitACK,
}State;

typedef struct{
	State state;
}SS_state;

typedef struct{
	int offset;
	int size;
}Cwnd;

#endif // SlowStart_H
