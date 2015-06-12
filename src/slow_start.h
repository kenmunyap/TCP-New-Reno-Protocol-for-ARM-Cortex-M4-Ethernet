#ifndef slow_start_H
#define slow_start_H


typedef struct{
		int cwnd;
		int seq_num;
		int ssthres;
		int packet_num;
		int ack;
		int index;
}DataBlock;



char txSlowStart(DataBlock *data);
char txData(DataBlock *ssData, char *buffer);
int returnACK();
int counterRTT();


#endif // slow_start_H
