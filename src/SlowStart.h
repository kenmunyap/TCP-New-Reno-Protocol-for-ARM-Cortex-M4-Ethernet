#ifndef SlowStart_H
#define SlowStart_H

#include <stdint.h>

#define MSS 50

typedef enum{
	SlowStart,
	WaitACK,
}State;

typedef struct{
	State state;
}TCP_state;

typedef struct{
	int offset;
	int size;
}Cwnd;

#endif // SlowStart_H
