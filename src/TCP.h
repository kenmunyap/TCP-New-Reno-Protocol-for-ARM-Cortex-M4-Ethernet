#ifndef TCP_H
#define TCP_H

char buffer[10000];

typedef struct{
	int sendbase;
	int nextseqnum[100000];
	char dataBuffer[500];
	int cwnd;
	int value;
}DataBlock;

typedef enum{
	SendData,
	Acknowledge,
	TimeOut,
	IncreaseCW,
}SlowStartState;

typedef struct{
	SlowStartState state_SS;
}SS_state;


void initialSequnceNumber(DataBlock *data);
void slowStart(SS_state *state,DataBlock *data);

#endif // TCP_H
