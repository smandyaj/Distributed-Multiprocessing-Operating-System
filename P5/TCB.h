#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <string.h>
typedef struct TCB_t
{
	struct TCB_t* next;
	struct TCB_t* prev;
	int thread_id;
	ucontext_t context;
} TCB_t;

void init_TCB (TCB_t *tcb, void *function, void *stackP, int stack_size,char* arg)
{
	memset(tcb, '\0', sizeof(struct TCB_t)); // wash, rinse
	getcontext(&tcb->context);               // have to get parent context, else snow forms on hell
	tcb->context.uc_stack.ss_sp = stackP;
	tcb->context.uc_stack.ss_size = (size_t) stack_size;
	makecontext(&tcb->context, function, 1, arg); // context is now cooked
}
