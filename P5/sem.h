#include "threads.h"
#include "stdlib.h"

typedef struct Sempahore{
	int count;
	TCB_t *SemQ;
}Semaphore_t;

Semaphore_t* CreateSem(int InputValue){
	// mallocs a semaphore structure, 
	// initializes it to the InitValue and 
	// returns the pointer to the semaphore.
	
	Semaphore_t* sem = NULL;
	sem = (Semaphore_t*)malloc(sizeof(Semaphore_t));
	sem->count = InputValue;
	sem->SemQ = newQueue();
	return sem;
}

void P(Semaphore_t *sem){
	// takes a pointer to a semaphore and performs P, 
	// i.e. decrements the semaphore, and if the value is less 
	// than zero then blocks the thread in the queue associated with the semaphore.
	
	sem->count--;
	if(sem->count < 0 ){
		//TCB_t *current = DelQueue(&ReadyQ);
		//AddQueue(&(sem->SemQ),&current);
		//swapcontext(&(current->context), &(ReadyQ->context));

		AddQueue(&(sem->SemQ),&Curr_Thread);
		TCB_t *from = Curr_Thread;
		Curr_Thread = DelQueue(&ReadyQ);
    		swapcontext(&(from->context), &(Curr_Thread->context));
	}

}

void V(Semaphore_t *sem){
	// increments the semaphore, and if the value is 0 or negative,
	// then takes a PCB out of the semaphore queue and puts it into the ReadyQ.
	// The V routine also "yields" to the next runnable thread.
	sem->count++;
	if(sem->count <= 0)
		{
			TCB_t *thread = DelQueue(&(sem->SemQ));
			AddQueue(&ReadyQ, &thread);
		}
	yield();
}


