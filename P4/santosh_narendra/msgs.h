#include "sem.h"

typedef struct port{
	char **msgs;
	int in,out;
	Semaphore_t *full,*empty,*mutex;
} Port;

Port port[100];

void initialize(){
	for(int i=0;i<100;i++){
		//allocate memory to hold N message (here N  = 10)
		port[i].msgs = (char **)malloc(10*sizeof(char *));
		//each message is an array of 10 chars
		for(int j=0;j<10;j++)
			port[i].msgs[j] = (char *)malloc(20*sizeof(char));
		port[i].in = 0;
		port[i].out = 0;
		port[i].full = CreateSem(0); 
		port[i].empty = CreateSem(10); 
		port[i].mutex = CreateSem(1);

	}
}

void send(Port *p, char *message){
	P(p->empty);
	P(p->mutex);
	strcpy(p->msgs[p->in],message);
	p->in = ((p->in)+1)%10;
	V(p->mutex);
	V(p->full);
}

void receive(Port *p, char *message){
	P(p->full);
	P(p->mutex);
	strcpy(message,p->msgs[p->out]);
	p->out = ((p->out)+1)%10;
	V(p->mutex);
	V(p->empty);
}


