#include <stdio.h>
#include <stdlib.h>
#include "slow_start.h"
#include "congestionWindow.h"
#include "delay.h"
#include "getData.h"


char txSlowStart(DataBlock *data){	
	int i = 0;
	data->index = sequenceNumber();
	if(data->index == 0){
		if(delayRTT() == 500){
			for(i=0;i<increaseCongestionWindow();i++){
				sendData();
				data->nextSeqNum = sequenceNumber();
			}
				if(recieveRxACK() == data->nextSeqNum){
					increaseCongestionWindow();
					data->index = data->nextSeqNum;
				}else{
					printf("does not get ACK");
					decreaseCongestionWindow();	
				}
		}else{
			printf("Time Out");
			decreaseCongestionWindow();
		}
	}else{
		if(delayRTT() == 500){
			for(i=0;i<increaseCongestionWindow();i++){
				sendData();
				data->nextSeqNum = sequenceNumber();
				printf("a");
				if( i+1 == increaseCongestionWindow()){				
					if(recieveRxACK() == data->nextSeqNum){
						increaseCongestionWindow();
						data->index = data->nextSeqNum;
					}else{
						printf("does not get ACK1");
						decreaseCongestionWindow();	
					}
				}else{
					if(recieveRxACK() == data->nextSeqNum){
						data->index = data->nextSeqNum;
					}else{
						printf("does not get ACK2");
						decreaseCongestionWindow();
					}
				}
	
			}
		}else{
			printf("Time Out");
			decreaseCongestionWindow();
		}
	}	
}






