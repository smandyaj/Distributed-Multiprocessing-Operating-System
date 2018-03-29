
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msgs_3S.h"
#include <ucontext.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
/*
Project Done by
Narendra kumar Sampath kumar
Santosh Mandya Jayaram
*/
void server() { 
    printf("\n Server Started");    

    while(1){
        char msg[10];
        //server always recieve message at port 0
        receive(77,msg);
        printf("\n Server received message from client: %d\t",(int)msg[0]);
        //msg[0] contains port Information
        for(int i=1;i<10;i++){            
            printf("%d ", msg[i]);
        }        
        char b[10];
        for(int i=0;i<10;i++){            
            b[i]=65+i;
        }
        //sending message to the recieved the client port which is in msg[0]
	    //printf("\nServer Sent Message"); 

        send((int)msg[0],b);       
    }
}
void client1() {    
    printf("\n Client 1 Started");    
    while(1){       
        char msg[10];
        int client_port=getTId()-1;
        //make the first index as clientport holder
        msg[0]=client_port;  
        for(char i=1;i<10;i++){
            msg[i]=96+i;
        }
        //printf("\nClient %d sent message",(int)msg[0]);    
        send(77,msg);
      //  printf("SEND DONE");
        char b[10];
        receive(client_port,b);
        printf("\n Client %d received message:\t",client_port);
        for(int i=0;i<10;i++){            
            printf("%d ", b[i]);
        }  
     }
}

void client2() {    
    printf("\n Client 2 Started");    
    while(1){       
        char msg[10];
        int client_port=getTId()-1;
        //make the first index as clientport holder
        msg[0]=client_port;  
        for(char i=1;i<10;i++){
            msg[i]=100+i;
        }
        //printf("\nClient %d sent message",(int)msg[0]);    
        send(77,msg);
      //  printf("SEND DONE");
        char b[10];
        receive(client_port,b);
        printf("\n Client %d received message:\t",client_port);
        for(int i=0;i<10;i++){            
            printf("%d ", b[i]);
        }  
     }
}

void client3() {    
    printf("\n Client 3 Started");    
    while(1){       
        char msg[10];
        int client_port=getTId()-1;
        //make the first index as clientport holder
        msg[0]=client_port;  
        for(char i=1;i<10;i++){
            msg[i]=107+i;
        }
        //printf("\nClient %d sent message",(int)msg[0]);    
        send(77,msg);
      //  printf("SEND DONE");
        char b[10];
        receive(client_port,b);
        printf("\n Client %d received message:\t",client_port);
        for(int i=0;i<10;i++){            
            printf("%d ", b[i]);
        }  
     }
}



int main()
{   ReadyQ=newQueue();
    initialize();
    start_thread(&server);
    start_thread(&client1);
    start_thread(&client2);
    start_thread(&client3);

    run();
    return 0;
}


