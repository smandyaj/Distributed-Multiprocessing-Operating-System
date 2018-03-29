#include "sem.h"

typedef struct port{
	int **msgs;
	int in,out;
	Semaphore_t *full,*empty,*mutex;
} Port;

Port port[100];

void initialize(){
	for(int i=0;i<100;i++){
		//allocate memory to hold N message (here N  = 10)
		port[i].msgs = (int **)malloc(10*sizeof(int *));
		//each message is an array of 10 chars
		for(int j=0;j<10;j++)
			port[i].msgs[j] = (int *)malloc(20*sizeof(int));
		port[i].in = 0;
		port[i].out = 0;
		port[i].full = CreateSem(0); 
		port[i].empty = CreateSem(10); 
		port[i].mutex = CreateSem(1);

	}
}

void send(Port *p,int* msg)
{   
    P(p->empty);
    P(p->mutex); 
        for(int i=0;i<10;i++) {
            p->msgs[p->in][i]=msg[i];
        }
        p->in=(p->in+1)%10;
    V(p->mutex);
    V(p->full);    
}
void receive(Port *p,int* msg)
{
    P(p->full);
    P(p->mutex); 
        for(int i=0;i<10;i++) {
            msg[i]=p->msgs[p->out][i];
        }
        p->out=(p->out+1)%10;
    V(p->mutex);
    V(p->empty);    
}


