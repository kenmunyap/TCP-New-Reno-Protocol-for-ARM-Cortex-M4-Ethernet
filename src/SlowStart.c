#include <stdio.h>
#include <stdint.h>
#include "SlowStart.h"
#include "CongestionWindow.h"

void cwndInitWindow(Cwnd *cwnd){
	cwnd->offset = 0;
	cwnd->size = 0;
}

uint32_t SlowStart(SS_state *state,Cwnd *cwnd){
	int ackValue;
	int value;
	switch(state->state){
		case sendData:
			// cwndIncrementWindow(cwnd,);
		break;
	}
}

