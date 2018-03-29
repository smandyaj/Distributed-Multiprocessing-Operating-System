/*
Submitted by
Santosh Mandya Jayaram - 1211225922
Narendra Kumar Sampath Kumar - 1211068609
*/
#define _BSD_SOURCE || _XOPEN_SOURCE >= 500 ||_XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED|| /* Since glibc 2.12: */ _POSIX_C_SOURCE >= 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msgs.h"
#include <ucontext.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void convert_int_to_char(char* result,int *msg);
void convert_char_to_int(char* c,int *msg) ;

char *strreverse(char *str)
{
      	char *s1, *s2;
		if (! str || ! *str) return str;
      	for (s1 = str, s2 = str + strlen(str) - 1; s2 > s1; ++s1, --s2)
      	{ *s1 ^= *s2; *s2 ^= *s1; *s1 ^= *s2; }
      return str;
}

void server() {    
    printf("\n Server listening..");
    int client_count[100],connection_count=0,file_count[100];
    for(int i=0;i<100;i++) client_count[i]=0;
    char filename[100][100];
    
    while(1){
        int msg[10];
        receive(&port[0],msg);
      	if(client_count[msg[0]]!=3)
        	printf("\nServer received message from port %d\n",0);
		if(client_count[msg[0]]==0) 
		{		
			client_count[msg[0]]++;
			char result[100]="";
			convert_int_to_char(result,msg);
			int sendMessage[10];
			sendMessage[0]=msg[0];
			for(int i=1;i<10;i++) sendMessage[i]=0;
			if(strlen(result)>15) 
			{
                printf("Filename Size More than 15 characters for client %d",msg[0]);
				convert_char_to_int("decline",sendMessage);
				send(&port[msg[0]],sendMessage);		
			}
			else 
			{
				strcat(result, ".server");
				printf("\nThe filename being created is %s",result);
				strcpy(filename[msg[0]],result);
				printf("\nFileName: %s",filename[msg[0]]);
				connection_count++;
				printf("Total Connection Count till now: %d",connection_count);
	 			if(connection_count>3) {
					convert_char_to_int("decline",sendMessage);
				}
				else {
					convert_char_to_int("accept",sendMessage);			
				}
				
				send(&port[msg[0]],sendMessage);
			}
		}
		else 
		{
			char status[100]="";
			convert_int_to_char(status,msg);
			FILE* server_file;
			if(strcmp(status,"done")!=0) {
				if(client_count[msg[0]]==1) {
					client_count[msg[0]]++; 
					server_file = fopen(filename[msg[0]], "w+");
				}
				else {
					server_file = fopen(filename[msg[0]], "a");
				}
				for(int i=1;i<10;i++){                        			
					fprintf(server_file,"%c",msg[i]);
					file_count[msg[0]]++;
					
         		}
				fclose(server_file);
				int sendMessage[10];
				sendMessage[0]=msg[0];
				if(file_count[msg[0]]>1024) {
					convert_char_to_int("decline",sendMessage);
				}
				else 
				{
					convert_char_to_int("received",sendMessage);
				}
				send(&port[msg[0]],sendMessage);		

			}
			else {
				printf("\nConnection Closed for Client %d",msg[0]);
				client_count[msg[0]]=3;
				connection_count--;
				int sendMessage[10];
				sendMessage[0]=msg[0];
				for(int i=1;i<10;i++) sendMessage[i]=0;
				convert_char_to_int("closed",sendMessage);
				send(&port[msg[0]],sendMessage);
			}
			  
		}        
    }
}


void client(char* filename) {    
		printf("\n Client sending..");    
		printf("\n%s",filename);
		int val[10],msg[10],receivedMessage[10];
        int client_port=getThreadId()-1;
		printf("\nClient_port: %d",client_port);
        msg[0]=client_port;  
   		convert_char_to_int(filename,msg);
       	printf("\nClient Sent Message");    
        send(&port[0],msg);
		receive(&port[client_port],receivedMessage);
		char message[100];
		convert_int_to_char(message,receivedMessage);
		if(strcmp(message,"accept")==0) 
		{
			printf("\nAccepted");
			FILE* file = fopen(filename, "rb");
			char buf[9];
			int nread;
			if (file) 
			{
    			while ((nread = fread(buf, 1, sizeof(buf), file)) > 0) 
    			{
				 	msg[0]=client_port;  
					for(int i=1;i<10;i++) msg[i]=0;
					printf("\nClient Message: ");
					for(int i=1;i<=9;i++) {
						msg[i]=buf[i-1];
						printf("%c",msg[i]) ;
				}
				printf("\n") ;
				send(&port[0],msg);
				receive(&port[client_port],receivedMessage);
				char ack[100]="";
				convert_int_to_char(ack,receivedMessage);
				memset(buf,' ',sizeof(buf));
				if(strcmp(ack,"received")==0) {
					continue;
				}
				else {
					printf("File size More than 1 MB for client %d",client_port);
					break;
				}
			}
			if (ferror(file)) {
        		/* deal with error */
    			}
    			fclose(file);
			}
			
		} 
		else 
		{
		printf("\nDeclined");
		}
		msg[0]=client_port;  
		for(int i=1;i<10;i++) {
			msg[i]=0;	
		}
		printf("\n");
  	
		convert_char_to_int("done",msg);
		send(&port[0],msg);
		receive(&port[client_port],receivedMessage);
		char ack[100]="";
		convert_int_to_char(ack,receivedMessage);
		if(strcmp(ack,"closed")==0) printf("\nClient %d Closed",client_port);
		while(1) 
		{
		yield();
		}

}



void convert_int_to_char(char* result,int *msg) {
	for(int i=1;i<10;i++) 
	{
		int a=msg[i];
		if(a==0) break;
		int j=0;char string[10]="";
		while(a>0) 
		{
			int rem=a%100; string[j]=(char)rem+45;
			j++; a=a/100;
		}
		char *str=strreverse(string);
		if(strcmp(result,"")==0) 
				strcpy(result,string); 
		else
				strcat(result, string);
	}
}

void convert_char_to_int(char* c,int *msg) {
	int count=0,j=1;
	for(int i=1;i<10;i++)  msg[i]=0;
	for(int i=0;i<strlen(c);i++) {
		int t=c[i]-45;
		if(c[i]!='\0'&&count==4) {
			count=0; j++;
		}
		if(c[i]!='\0'&&count<=3) {
			msg[j]=msg[j]*100+t; 	count++;
		}
		
	}
}

int main(int argc, char *argv[])
{   ReadyQ=newQueue();
    initialize();
    start_thread(&server,NULL);
    int client_count = atoi(argv[1]);
    printf("\n Client count %d ",client_count);
    for(int i=2;i<client_count+2;i++){        
            printf("\n Client filename %s ",argv[i]);   
            start_thread(&client,argv[i]);
    }
    run();
    return 0;
}

