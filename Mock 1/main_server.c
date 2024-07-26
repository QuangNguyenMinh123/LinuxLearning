#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h> 
#include <unistd.h>
#include "log.h"
/*******************************************************************/
#define BUFFER_SIZE                 256
/*******************************************************************/
pthread_t thread;

float temp;
int run = 0;
int acc = 0;
char buffer[1024] = {0};

int logFileId;
int serverSock;
int chToPaPipe[2], paToChPipe[2];
pthread_t threadConnect, threadData, threadStorage;
/*******************************************************************/
void *Receive(void *args)
{
    while (acc <= 0);
    while (1)
    {
        recv(acc, &temp, sizeof(temp), 0);
        if (temp > 0);
        {
            printf("Temp = %f\n",temp);
            sprintf(buffer, "Temp = %f\n",temp);
            log_write(logFileId, buffer);
        }
        
    }
}

void *threadConnecFunc(void *args)
{

}

void *threadDataFunc(void *args)
{

}

void *threadStorageFunc(void *args)
{

}
/*******************************************************************/
int main()
{
    int childPid;
	char buffer1[256], buffer2[256];
    /* For server connection */
    struct sockaddr_in my_addr, peer_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	/* Change this ip address according to your machine */
	my_addr.sin_addr.s_addr = inet_addr("192.168.0.106");
	my_addr.sin_port = htons(12000);
    /* Create log file */
    logFileId = log_open();
    /* Initialize socket */
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock < 0)
    {
        log_write(logFileId, "Error in creating server socket\n");
        return -1;
    }
	else
    {
        log_write(logFileId, "Server is created\n");
    }
    /* Create pipe */
    if (pipe(chToPaPipe) < 0)
    {
        log_write(logFileId,"chToPaPipe pipe is created unsuccessfully\n");
        return -1;
    }
    else
    {
        log_write(logFileId,"chToPaPipe pipe is created successfully\n");
    }
    if (pipe(paToChPipe) < 0)
    {
        log_write(logFileId,"paToChPipe pipe is created unsuccessfully\n");
        return -1;
    }
    else
    {
        log_write(logFileId,"paToChPipe pipe is created successfully\n");
    }

    /* Create child process*/
    log_write(logFileId, "Creating child process\n");
    childPid = fork();

    printf("Child PID = %i",childPid);
    if (childPid < 0)
    {
        /* Failed */
        log_write(logFileId, "fork() failed\n");
        return -1;
    }
    else if (childPid = 0)
    {
        /* Child; log process */
        /* Establish pipe to parent process */
        log_write(logFileId, "Child process\n");
        while(1);
    }
    else
    {
        int i = 65;
        printf("%s\n","2234234");
        /* Parent: main */
        /* Establish pipe to child process */
        if (close(chToPaPipe[1]) == -1)
            log_write(logFileId, "close(chToPaPipe[1]) failed\n");
        if (close(paToChPipe[0]) == -1)
            log_write(logFileId, "close(paToChPipe[0]) failed\n");
        /* Create thread */
        if (pthread_create(&threadConnect, NULL, threadConnecFunc, NULL) == 0)
            log_write(logFileId,"threadConnect is created\n");
        if (pthread_create(&threadData, NULL, threadDataFunc, NULL) == 0)
            log_write(logFileId,"threadData is created\n");
        if (pthread_create(&threadStorage, NULL, threadStorageFunc, NULL) == 0)
            log_write(logFileId,"threadStorage is created\n");
        
        pthread_join(threadConnect,NULL);
        /* wait */
        pthread_join(threadData,NULL);
        pthread_join(threadStorage,NULL);
        close(chToPaPipe[0]);
    }
	if (bind(serverSock, (struct sockaddr*) &my_addr, sizeof(my_addr)) == 0)
		printf("Binded Correctly\n");
	else
		printf("Unable to bind\n");
		
	if (listen(serverSock, 3) == 0)
		printf("Listening ...\n");
	else
		printf("Unable to listen\n");
	
	socklen_t addr_size;
	addr_size = sizeof(struct sockaddr_in);
	
	// Ip character array will store the ip address of client
	char *ip;
	if (pthread_create(&thread,NULL,&Receive,NULL) == 0)
    {
        printf("Receive thread is created\n");
    }
	// while loop is iterated infinitely to 
	// accept infinite connection one by one
	while (1)
	{
		acc = accept(serverSock, (struct sockaddr*) &peer_addr, &addr_size);
		printf("Connection Established\n");
		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN);
	
		// "ntohs(peer_addr.sin_port)" function is 
		// for finding port number of client
		printf("connection established with IP : %s and PORT : %d\n", 
											ip, ntohs(peer_addr.sin_port));

		//recv(acc, &temp, sizeof(temp), 0);
		//printf("Client : %f\n", temp);
		strcpy(buffer1, "Hello");
		send(acc, buffer1, 256, 0);
        run = 1;
	} 
	return 0;
}
