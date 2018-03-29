// (c) Partha Dasgupta 2009
// permission to use and distribute granted.
//Project by
//Narendra kumar Sampath kumar
//Santosh Mandya Jayaram
#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

// change size according to threads

int threadsize = 0;
int x=0;
int *arr;
int *threadNum;

semaphore_t *mutex;
semaphore_t *mutexprint;

void initializeArray(){
    for( int i = 0; i < threadsize ; i++)
    {
	   arr[i] = 0;
    }
}
 
 //func1 runs first with three semaphore 
//sets V for other three semaphoree



void func1(int *index){
    while(1){
        P(&mutex[*index]);
        arr[*index]+=1;
        V(&mutexprint[*index]);
    }
}

//checks  if three semaphore are set 
//if set go and print the values
//set the other three semaphores
void func2(void){
    //printf("func2");
    while(1){
        for(int l=0;l<threadsize;l++)
            P(&mutexprint[l]);
            
            printf("\nThread - Parent:\t");
            for(int i=0;i<threadsize;i++)
                printf(" %d",arr[i]);
        for(int l=0;l<threadsize;l++)
            V(&mutex[l]);
    }
}
int main(int argc, char *arg[])
{
    //if no argument is passed, default number of children is 3
    if(argc<2)
        threadsize = 3;
    else
        threadsize = atoi(arg[1]);

    //printf("Threadsize: %d",threadsize);

    //allocating the size dynamically to handle n children
    arr = (int *)malloc(sizeof(int)*threadsize);
    threadNum = (int *)malloc(sizeof(int)*threadsize);
    mutex = (semaphore_t *)malloc(sizeof(semaphore_t)*threadsize);
    mutexprint = (semaphore_t *)malloc(sizeof(semaphore_t)*threadsize);

    // initialize the array elements to 0
    
    initializeArray();

    // initialize the semaphore

    
    for(int k=0;k<threadsize;k++){
        init_sem(&mutex[k],1); //this is used to increment
        init_sem(&mutexprint[k],0);//this is used to print
    }


    for(int j=0;j<threadsize;j++)
        threadNum[j]=j;

    int index=0;
    for(int i=0;i<threadsize;i++)
        start_thread(func1,&threadNum[i]);
    func2();
   
   return 0;
}




