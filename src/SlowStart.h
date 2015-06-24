#ifndef SlowStart_H
#define SlowStart_H

#include <stdint.h>

#define MSS 50

char Buffer[100000];
 
typedef enum{
	SlowStart,
	SlowStartWaitACK,
}State;

typedef struct{
	State state;
}TCP_state;

typedef struct{
	uint32_t offset;
	uint32_t size;
}Cwnd;

#endif // SlowStart_H
