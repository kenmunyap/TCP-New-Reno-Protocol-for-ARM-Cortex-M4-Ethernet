#include <stdio.h>
#include <stdlib.h>
#include "slow_start.h"
#include "congestionWindow.h"
#include "delay.h"
#include "getData.h"


char txSlowStart(DataBlock *data){	
	int i;
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
					decreaseCongestionWindow();	
				}
		}else{
			decreaseCongestionWindow();
		}
	}else{
		
	}	
}






