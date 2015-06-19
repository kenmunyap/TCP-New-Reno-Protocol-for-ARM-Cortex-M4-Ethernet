#include <stdio.h>
#include "TCP.h"
#include "Data.h"
#include "sequenceNumber.h"
#include "timeOutTimer.h"
#include "ack.h"
#include "congestionWindow.h"


void initialSequnceNumber(DataBlock *data){
	data->sendbase = sequenceNumber();
	data->nextseqnum[0] = sequenceNumber();
}

void initalState(SS_state *state){
	state->state_SS = SendData;
}


void slowStart(SS_state *state,DataBlock *data){
	int i;
	int j;
	int k;
	int counter;
	int checkDupACK;
		switch(state->state_SS){
			case SendData:
					j = congestionWindow();
					for(i=0;i<j;i++){
						if(countTimer() < 5000){
							sendData2RCV();
							data->nextseqnum[i] = sequenceNumber() + (sizeof(data->dataBuffer)/sizeof(char));
						}else{
							state->state_SS = TimeOut;
						}
					}
					
					if(countTimer() > 5000){
						state->state_SS = TimeOut;	
					}else{
						state->state_SS = Acknowledge;
					}
			break;
			
			case TimeOut:
				data->nextseqnum[0] = data->sendbase;
				printf("fast retransmit\n");
			break;
			
			case Acknowledge:
				j = congestionWindow();
				for(i=0;i<j;i++){
					if(recieveRxACK() == data->nextseqnum[i]){
						data->sendbase = data->nextseqnum[i];
					}else{
						if(recieveRxACK() == data->nextseqnum[i]){
							counter += 1;
							if(counter == 3){
								printf("fast retransmit\n");
							}
						}else{
							printf("nth");
						}
					}
				}
				state->state_SS = SendData;
			break;
		}
}


			/*
			case SendData:
				sendData2RCV();			
				data->nextseqnum = (sizeof(data->dataBuffer)/sizeof(char)) + data->sendbase;
				if(countTimer() > 5000){
					state->state_SS = TimeOut;
				}else{
					state->state_SS = Acknowledge;
				}
			break;
			
			case TimeOut:
				data->nextseqnum = data->sendbase;
				printf("fast retransmit\n");
			break;
			
			case Acknowledge:
				checkDupACK = recieveRxACK();
				if(recieveRxACK() > data->sendbase){
					if(j == i){
						data->sendbase = data->nextseqnum;
						state->state_SS = IncreaseCW;
					}else{
						data->sendbase = data->nextseqnum;	
					}
				}else{
					if(recieveRxACK() <= checkDupACK){ 
						counter += 1;
						if(counter == 3){
							printf("fast retransmit");
						}else{
							data->sendbase = data->nextseqnum;
						}
					}
				}
			break;
			*/