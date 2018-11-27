// socket server example, handles multiple clients using threads

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

struct queue
{
	char string[2000];
	struct queue *next;
};

struct args21
{
	char string[2000];
	int sock1;
	int sock2;
}

static int file_number=0;
static int curr_file_number=0;
struct queue *top1=NULL;
struct queue *end1=NULL;
struct queue *top2=NULL;
struct queue *end2=NULL;
struct queue *top3=NULL;
struct queue *end3=NULL;
struct queue *top4=NULL;
struct queue *end4=NULL;
struct queue *top5=NULL;
struct queue *end5=NULL;
void enqueue(char *re,int i)
{
	switch(i)
	{
		case 1:
			if(top1==NULL)
			{
				top1=(struct queue *)malloc(sizeof(struct queue));
				top1->string=re;
				end1=top1;
				top1->next=NULL;
			}
			else
			{
				struct queue *temp;
				temp=malloc(sizeof(struct queue));
				temp->string=re;
				end1->next=temp;
				temp->next=NULL;
			}
			break;
		case 2:
			if(top2==NULL)
			{
				top2=(struct queue *)malloc(sizeof(struct queue));
				top2->string=re;
				end2=top2;
				top2->next=NULL;
			}
			else
			{
				struct queue *temp;
				temp=malloc(sizeof(struct queue));
				temp->string=re;
				end2->next=temp;
				temp->next=NULL;
			}
			break;
		case 3:
			if(top3==NULL)
			{
				top3=(struct queue *)malloc(sizeof(struct queue));
				top3->string=re;
				end3=top3;
				top3->next=NULL;
			}
			else
			{
				struct queue *temp;
				temp=malloc(sizeof(struct queue));
				temp->string=re;
				end3->next=temp;
				temp->next=NULL;
			}
			break;
		default:
			break;
	}
}

struct queue* dequeue(int i)
{
	struct queue *temp;
	switch(i)
	{
		case 1:
			temp=top1;
			top1=top1->next;
			break;
		case 2:
			temp=top2;
			top2=top2->next;
			break;
		case 3:
			temp=top3;
			top3=top3->next;
			break;
		default:
			break;
	}
	return temp;
}

//the thread function
void *connection_handler(void *);

int numbers_rq=0;
int numbers_rs=0;

int main(int argc , char *argv[])
{
	int socket_desc1,socket_desc2,socket_desc3,socket_desc4,socket_desc5 , client_sock , c , *new_sock,clientsock1,clientsock2,clientsock3,clientsock4,clientsock5;
	struct sockaddr_in server1,server2,server3,server4,server5 , client,client_1,client_2,client_3,client_4,client_5;

	//Create socket
	socket_desc1 = socket(AF_INET , SOCK_STREAM , 0);
	socket_desc2 = socket(AF_INET , SOCK_STREAM , 0);
	socket_desc3 = socket(AF_INET , SOCK_STREAM , 0);
	socket_desc4 = socket(AF_INET , SOCK_STREAM , 0);
	socket_desc5 = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc1 == -1)
	{
		printf("Could not create socket");
	}
	if (socket_desc2 == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	//Prepare the sockaddr_in structure
	server1.sin_family = AF_INET;
	server1.sin_addr.s_addr = INADDR_ANY;
	server1.sin_port = htons( 2221 );

	server2.sin_family = AF_INET;
	server2.sin_addr.s_addr = INADDR_ANY;
	server2.sin_port = htons( 2222 );

	server3.sin_family = AF_INET;
	server3.sin_addr.s_addr = INADDR_ANY;
	server3.sin_port = htons( 2223 );

	server4.sin_family = AF_INET;
	server4.sin_addr.s_addr = INADDR_ANY;
	server4.sin_port = htons( 2224 );

	server5.sin_family = AF_INET;
	server5.sin_addr.s_addr = INADDR_ANY;
	server5.sin_port = htons( 2225 );

	client_1.sin_family=AF_INET;
	client_1.sin_addr.s_addr=inet_addr("192.168.1.1");
	client_1.sin_port=htons(2225);

	client_2.sin_family=AF_INET;
	client_2.sin_addr.s_addr=inet_addr("192.168.1.2");
	client_2.sin_port=htons(2225);

	client_3.sin_family=AF_INET;
	client_3.sin_addr.s_addr=inet_addr("192.168.1.3");
	client_3.sin_port=htons(2225);

	client_4.sin_family=AF_INET;
	client_4.sin_addr.s_addr=inet_addr("192.168.1.4");
	client_4.sin_port=htons(2225);

	client_5.sin_family=AF_INET;
	client_5.sin_addr.s_addr=inet_addr("192.168.1.5");
	client_5.sin_port=htons(2225);

	//Bind
	if( bind(socket_desc1,(struct sockaddr *)&server1 , sizeof(server1)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	if( bind(socket_desc2,(struct sockaddr *)&server2 , sizeof(server2)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	if( bind(socket_desc3,(struct sockaddr *)&server3 , sizeof(server3)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	if( bind(socket_desc4,(struct sockaddr *)&server4 , sizeof(server4)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	if( bind(socket_desc5,(struct sockaddr *)&server5 , sizeof(server5)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");

	//Listen
	listen(socket_desc1 , 1);
	listen(socket_desc2 , 1);
	listen(socket_desc3 , 1);
	listen(socket_desc4 , 1);
	listen(socket_desc5 , 1);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

        c=sizeof(struct sockaddr_in);

	pthread_t tcrq_1,tcrq_2,tcrq_3,tcrs_4,tcrs_5;

	clientsock1=accept(socket_desc1,(struct sockaddr*)&client1,(socklen_t*)&c);
	pthread_create(&tcrq_1,NULL,connection_handler_for_client_1_request,socket_desc1);

	numbers_rq++;

	clientsock2=accept(socket_desc2,(struct sockaddr*)&client2,(socklen_t*)&c);
	pthread_create(&tcrq_2,NULL,connection_handler_for_client_2_request,socket_desc2);

	number_rq++;

	clientsock3=accept(socket_desc3,(struct sockaddr*)&client3,(socklen_t*)&c);
	pthread_create(&tcrq_3,NULL,connection_handler_for_client_3_request,socket_desc3);

	number_rq++;

	clientsock4=accept(socket_desc4,(struct sockaddr*)&client4,(socklen_t*)&c);
	pthread_create(&tcrs_1,NULL,connection_handler_for_client_1_request,socket_desc4);

	numbers_rs++;

	clientsock5=accept(socket_desc5,(struct sockaddr*)&client5,(socklen_t*)&c);
	pthread_create(&tcrs_2,NULL,connection_handler_for_client_1_request,socket_desc5);

	numbers_rs++;

	return 0;
}
/*
  This will handle connection for each client
  */

int i=0;
void *connection_handler_for_client_request_run(void *arg)
{
	while(1)
	{
		struct args21 *temp=(struct args21 *)arg;
		i++;
		if(i%2==0)
		{
			enqueue(temp->string,4);
			write(temp->sock1,temp->string,strlen(temp->string));
		}
		else
		{
			enqueue(temp->string,5);
			write(temp->sock2,temp->string,strlen(temp->string));
		}
	}
}

void *connection_handler_for_client_1_request(void *arg)
{
	struct sockaddr_in server_1=*(struct sockaddr_in *)arg;
	char buffer_1[2000];
	while(1)
	{
		if(recv(server_1,buffer_1,2000,0)>0)
		{
			enqueue(buffer_1,1);
		}
		memset(buffer_1,0,strlen(buffer_1));
	}
}
void *connection_handler_for_client_2_request(void *arg)
{
	char buffer_2[2000];
	while(1)
	{
		if(recv(client_2,buffer_2,2000,0)>0)
		{
			enqueue(client_2,2);
		}
		memset(buffer_2,0,strlen(buffer_2));
	}
}
void *connection_handler_for_client_3_request(void *arg)
{
	char buffer_3[2000];
	while(1)
	{
		if(recv(client_3,buffer_3,2000,0)>0)
		{
			enqueue(client_3,3);
		}
		memset(buffer_3,0,strlen(buffer_3));
	}
}

void *connection_handler_for_client_response(void *arg)
{
	
}

int r1=0;
int r2=0;

void *connection_handler_for_client_1_response(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int *)socket_desc;
	int n;

	char filename[2000],client_message[2000];

	while(1)
	{  
		FILE * fp;
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		recv(sock,filename,2000,0);
		sprintf(client_message,"%s_%d_1",filename,r1);
		r1++;
		fp = fopen(client_message, "a");
		memset(client_message,0,strlen(client_message));
		while(1)
		{        
			//Receive a reply from the server
			if( recv(sock,client_message,2000,0) < 0)
			{
				puts("recv failed");
				break;
			}
			if(strcmp(server_reply,"!@#$%^&*()")>0)
				break;
			fprintf(fp,"%s",client_message);
			memset(server_reply,0,strlen(server_reply));
		}
	    	fclose(fp);
		system("rm temp.txt");
	    	if (line)
			free(line);
	}
	close(sock);

	if(n==0)
	{
		puts("Client Disconnected");
	}
	else
	{
		perror("recv failed");
	}
	return 0;
}
void *connection_handler_for_client_2_response(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int *)socket_desc;
	int n;

	char client_message[2000];

	while(1)
	{  
		FILE * fp;
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		sprintf(client_message,"2_%d",r2);
		r2++;
		fp = fopen(client_message, "a");

		while(1)
		{        
			//Receive a reply from the server
			if( recv(sock,client_message,2000,0) < 0)
			{
				puts("recv failed");
				break;
			}
			if(strcmp(server_reply,"!@#$%^&*()")>0)
				break;
			fprintf(fp,"%s",client_message);
			memset(server_reply,0,strlen(server_reply));
		}
		send(sock,"!@#$%^&*()",10,0);
	    	fclose(fp);
		system("rm temp.txt");
	    	if (line)
			free(line);
	}
	close(sock);

	if(n==0)
	{
		puts("Client Disconnected");
	}
	else
	{
		perror("recv failed");
	}
	return 0;
}
