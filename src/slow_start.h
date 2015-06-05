#ifndef slow_start_H
#define slow_start_H


typedef struct{
	int cwnd;
	int seq_num;
	int ssthres;
	int packet_num;
}DataBlock;


int senderSlowStart();
int returnACK();
int counterRTT();


#endif // slow_start_H
