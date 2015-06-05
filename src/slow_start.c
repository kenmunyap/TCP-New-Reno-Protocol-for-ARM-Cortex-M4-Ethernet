#include <stdio.h>
#include "slow_start.h"

int returnACK(){
	if(getData() == 1){
		return 1;
	}else{
		return 0;
	}	
}

int counterTimeOut(){
	int i = 0;
	int counter = 0;
	for(i=0;i<500;i++){
		counter = counter+1;
	}
	return counter;
}

int senderSlowStart(DataBlock *ssData){
	ssData->cwnd = 1;
	ssData->packet_num = 1;
	
	if(sendData() == 1){
		if(counterTimeOut() <= 500){
			if(returnACK() == 1){
				ssData->cwnd = ssData->cwnd+1;
				ssData->packet_num = ssData->packet_num*2;
			}else{
				printf("Does Not Return Any Acknowlegde");
			}
		}else{
			printf("TimeOut Of Data Occur");
		}
	}
}

