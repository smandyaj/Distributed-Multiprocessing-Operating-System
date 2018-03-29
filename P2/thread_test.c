/*
Project by
Santosh Mandya Jayaram
Narendra kumar Sampath kumar
*/

#include "threads.h"
#include "stdio.h"
#include "stdlib.h"

int j =0;

void function1(){
	int i;
	for(i=0;;){
		i++;
		j++;
		//sleep(1);
		printf("\n Func 1 Thread ID %d , Local %d Global %d",Curr_Thread->thread_id,i,j);
		yield();
	}
}

void function2(){
	int i;
	for(i=0;;){
		i++;
		j++;
		//sleep(1);
		printf("\n Func 2 Thread ID %d , Local %d Global %d",Curr_Thread->thread_id,i,j);
		yield();
	}
}


void function3(){
    int i;
	for(i=0;;){
		i++;
		j++;
		//sleep(1);
		printf("\n Func 3 Thread ID %d , Local %d Global %d",Curr_Thread->thread_id,i,j);
		yield();
	}
}

int main(){
	
	ReadyQ = newQueue();
	// first thread
	start_thread(&function1);
	start_thread(&function2);
	start_thread(&function3);
	run();
	return 0;
}
