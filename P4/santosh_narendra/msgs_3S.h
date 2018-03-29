#include "sem.h"

typedef struct port{
	char **msgs;
	int in,out;
	
} Port;

Port port[100];
Semaphore_t *full[10],*empty[10],*mutex[10];
void initialize(){
	for(int i=0;i<100;i++){
		//allocate memory to hold N message (here N  = 10)
		port[i].msgs = (char **)malloc(10*sizeof(char *));
		//each message is an array of 10 chars
		for(int j=0;j<10;j++)
			port[i].msgs[j] = (char *)malloc(20*sizeof(char));
		port[i].in = 0;
		port[i].out = 0;
		if(i<10){
			full[i] = CreateSem(0); 
			empty[i] = CreateSem(10); 
			mutex[i] = CreateSem(1);
		}

	}
}

int map_index(int port_index){
	int start = 0;
	int end = 9;
		
	for(int i=0;i<10;i++){
		if(port_index>=start && port_index<=end){
			return i;
		}
		start = start+10;
		end = end+10;
	}
}

void send(int pindex, char *message){
	int mutIndex = map_index(pindex);
	//printf("send: %d",mutIndex);
	P(empty[mutIndex]);
	P(mutex[mutIndex]);
	//printf("IN send");
	strcpy(port[pindex].msgs[port[pindex].in],message);
	//printf("IN send2");
	port[pindex].in = ((port[pindex].in)+1)%10;
	//printf("IN send3");
	V(mutex[mutIndex]);
	V(full[mutIndex]);
	//printf("IN send4");
}

void receive(int pindex, char *message){
	int mutIndex = map_index(pindex);
	//printf("receive: %d",mutIndex);
	P(full[mutIndex]);
	P(mutex[mutIndex]);
	//printf("IN receive");
	strcpy(message,port[pindex].msgs[port[pindex].out]);
	port[pindex].out = ((port[pindex].out)+1)%10;
	V(mutex[mutIndex]);
	V(empty[mutIndex]);
}


