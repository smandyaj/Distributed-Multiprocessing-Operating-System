#include "q.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
//points to the readyQ and current thread under execution
TCB_t* ReadyQ;
TCB_t* Curr_Thread;
int counter = 0;

void start_thread(void (*function)(void))
{
	//allocate a stack (via malloc) of a certain size (choose 8192)
	void *stackP = (void *)malloc(8192);		
	//allocate a TCB (via malloc)
	TCB_t* new_thread = NewItem(++counter); 
	//call init_TCB with appropriate arguments
	init_TCB(new_thread, function,stackP,8192);
	//Add a thread_id (use a counter)
	new_thread->thread_id = counter;
	//call addQ to add this TCB into the “ReadyQ” which is a global header pointer
	AddQueue(&ReadyQ,&new_thread);
}

void run()
{   
	// real code
	Curr_Thread = DelQueue(&ReadyQ);
	ucontext_t parent;     // get a place to store the main context, for faking
	getcontext(&parent);   // magic sauce
	swapcontext(&parent, &(Curr_Thread->context));  // start the first thread
}

 
void yield()
{
	// similar to run()
	TCB_t* Prev_Thread;
	AddQueue(&ReadyQ, &Curr_Thread);
	Prev_Thread = Curr_Thread;
	Curr_Thread = DelQueue(&ReadyQ);
	//swap the context, from Prev_Thread to the thread pointed to Curr_Thread
	swapcontext(&Prev_Thread->context,&(Curr_Thread->context));
}
