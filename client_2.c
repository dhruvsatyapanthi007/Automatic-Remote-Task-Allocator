#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<pthread.h> //for threading , link with lpthread

struct queue
{
	char string[2000];
	struct queue *next;
};

static int file_number=0;
static int curr_file_number=0;
struct queue *tope=NULL;
struct queue *ende=NULL;
void enqueue(char *re)
{
	if(tope==NULL)
	{
		tope=(struct queue *)malloc(sizeof(struct queue));
		tope->string=re;
		ende=tope;
		tope->next=NULL;
	}
	else
	{
		struct queue *temp;
		temp=malloc(sizeof(struct queue));
		temp->string=re;
		ende->next=temp;
		temp->next=NULL;
	}
}

struct queue* dequeue(struct queue *tope)
{
	struct queue *temp;
	temp=tope;
	tope=tope->next;
	return temp;
}

void *connection_handler_for_run(void *arg)
{
	while(1)
	{
		if(tope!=NULL)
		{
			struct queue *temp;
			temp=dequeue();
			char *send;
			sprintf(send,"gcc %s.c",temp->string);
			system(send);
			sprintf(send,"./a.out>%d",file_number);
			system(send);
			file_number++;
		}
	}
}

void *connection_handler_for_send_file(void *arg)
{
	while(1)
	{
		int sock=*(int *)arg;
		if(curr_file_number<=file_number)
		{
			FILE * fp;
			char * line = NULL;
			size_t len = 0;
			ssize_t read;
			sprintf(line,"%d",curr_file_number);
			fp = fopen(line, "r");
			if (fp == NULL)
				exit(EXIT_FAILURE);

			while ((read = getline(&line, &len, fp)) != -1)
			{
				printf("Retrieved line of length %zu :\n", read);
				printf("%s", line);
				send(sock,line,strlen(line));
		    	}
			write(sock,"!@#$%^&*()",10);
		    	fclose(fp);
			system("rm ");
		    	if (line)
				free(line);
			curr_file_number++;
		}
	}
}

void *connection_handler_for_receive(void *arg)
{
	while(1)
	{
		int sock=*(int *)arg;
		char send[2000];
		if(recv(sock,send,2000,0)>0)
		{
			enqueue(send);
		}
	}
}

int main(int argc , char *argv[])
{
	int sock1,sock2;
    	struct sockaddr_in server1,server2;
    	char message[1000] , server_reply[2000];

    	//Create socket
    	sock1 = socket(AF_INET , SOCK_STREAM , 0);
    	sock2 = socket(AF_INET , SOCK_STREAM , 0);
    	if (sock1 == -1)
    	{
		printf("Could not create socket");
    	}
    	if (sock2 == -1)
    	{
		printf("Could not create socket");
    	}
    	puts("Socket No.1 created");
	puts("Socket No.2 created");

     
    	server1.sin_addr.s_addr = inet_addr("192.168.1.4");
    	server1.sin_family = AF_INET;
    	server1.sin_port = htons( 2221 );

	server2.sin_addr.s_addr = inet_addr("192.168.1.4");
    	server2.sin_family = AF_INET;
    	server2.sin_port = htons( 2222 );

	server3.sin_addr.s_addr = inet_addr("192.168.1.4");
    	server3.sin_family = AF_INET;
    	server3.sin_port = htons( 2223 );

	server4.sin_addr.s_addr = inet_addr("192.168.1.4");
    	server4.sin_family = AF_INET;
    	server4.sin_port = htons( 2224 );

	server5.sin_addr.s_addr = inet_addr("192.168.1.4");
    	server5.sin_family = AF_INET;
    	server5.sin_port = htons( 2225 );
 
    	//Connect to remote server
    	if (connect(sock1 , (struct sockaddr *)&server1 , sizeof(server1)) < 0)
    	{
		perror("connect failed. Error");
        	return 1;
    	}

	if (connect(sock2 , (struct sockaddr *)&server2 , sizeof(server2)) < 0)
    	{
		perror("connect failed. Error");
        	return 1;
    	}
     
    	puts("Connected\n");
    	puts("Connected\n");

	pthread_t thread_run,thread_send_file,thread_receive;

	pthread_create(thread_run,NULL,connection_handler_for_run,NULL);

	pthread_create(thread_send_file,connection_handler_for_send_file,sock1);

	pthread_create(thread_receive,NULL,connection_handler_for_receive,sock2);
     
	close(sock);
	return 0;
}
