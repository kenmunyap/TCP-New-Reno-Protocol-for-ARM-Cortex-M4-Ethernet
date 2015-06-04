#include <stdio.h>
#include "slow_start.h"

int counterRTT(){
	int i = 0;
	for(i=0;i<=500;i++);
}

int returnACK(){
	if(getData() == 1){
		return 1;
	}else{
		return 0;
	}	
}

int senderSlowStart(){
	int sque_num = 0
	int cwnd = 1;
	int totalCWNDSize = 65000;
	int cwndSize = 500;
	
	if(sendData() == 1){
		if(returnACK() == 1){
				sque_num = cwndSize;
				cwnd = cwnd*2;
				cwndSize = (500*cwnd) + 1;
		}else{
				sque_num;
		}
	}else{
		printf("data not send");
	}
}

