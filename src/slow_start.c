#include <stdio.h>
#include <stdlib.h>
#include "slow_start.h"
#include "congestionWindow.h"


int txSlowStart(DataBlock *data){
	int i;
	if(data->index == 0){
			increaseCongestionWindow();
			sendData();
			sequnceNumber();
	}else if(data->index == recieveRxACK()){
		for(i=0;i<=increaseCongestionWindow();i++){
			sendData();
			sequnceNumber();
		}	
		data->index = (sequnceNumber/(i+1))+1;
	}else{
		
	}

	
}




