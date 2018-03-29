#include "stdio.h"
#include <stdlib.h>
#include "TCB.h"
#include "string.h"
// returns a pointer to a new q-element, uses memory allocation
TCB_t* NewItem(int val){
	TCB_t* item = (TCB_t*)malloc(sizeof(TCB_t));
	item->thread_id = val;
	item->next = NULL;
	item->prev = NULL;
	return item;
}

// returns a pointer to a new q-element, uses memory allocation
TCB_t* newQueue(){
	TCB_t* header = (TCB_t*)malloc(sizeof(TCB_t));
	header->thread_id = 0;
	header->next = header;
	//header->prev = header;
	return header;
}

// adds a queue item, pointed to by “item”, to the queue pointed to by head
void AddQueue(TCB_t** head, TCB_t** item){
	TCB_t* curr = (TCB_t*) *head;	
	if( curr->next != *head){
		while( curr->next != *head){
			curr = curr->next;
		}
	}
	//printf("Inserting item %d\n",(*item)->thread_id);
	curr->next = *item;
	(*item)->prev = curr;
	(*item)->next = *head;
}

// deletes an item from head and returns a pointer to the deleted item. If the queue is already empty, flag error
TCB_t* DelQueue(TCB_t** head){
	TCB_t* curr = *head;
	
	if( curr == curr->next){
	//	printf("Header wont be deleted, the Q is empty!!\n");
		return NULL;
	}
	
	TCB_t* delNode = curr->next;
	curr->next = delNode->next;
	TCB_t* temp = delNode->next;
	temp->prev = curr;
	//printf("Deleted %d from Q\n",delNode->thread_id);
	return delNode;
}

void printQueue(TCB_t** head){
	TCB_t* curr = *head;
	curr = curr->next;
	while( curr != *head){
	//	printf("Node value:%d\n", curr->thread_id);
		curr = curr->next;
	}
} 
