#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "CongestionWindow.h"

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = 0;
}

void initTCPState(TCP_state *state){
	state->state = SlowStart;
}

uint32_t TCP(TCP_state *state,Cwnd *cwnd){
	switch(state->state){
		case SlowStart:

		break;
	}
}

