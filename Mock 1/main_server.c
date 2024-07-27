#include <stdio.h>
#include <stdlib.h>
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
#define BUFF_SIZE                       256

#define READ_PIPE_IDX                   0
#define WRITE_PIPE_IDX                  1
/*******************************************************************/
pthread_t thread;
#define BUFF_SIZE                       256

#define READ_PIPE_IDX                   0
#define WRITE_PIPE_IDX                  1
/*******************************************************************/
float temp;
int acc = 0;
char buffer[1024] = {0};

int logFileId;
int serverSock;
int chToPaPipe[2], paToChPipe[2];
pthread_t threadConnect, threadData, threadStorage;
socklen_t addr_size;
/* For server connection */
struct sockaddr_in my_addr, peer_addr;
char buffer1[BUFF_SIZE];
/*******************************************************************/
void *Receive(void *args)
{
    // while (acc <= 0);
    // int check = read(acc, &temp, sizeof(temp));
    // while (check > 0)
    // {
    //     check = recv(acc, &temp, sizeof(temp), 0);
    //     log_write(logFileId, buffer);
    // }
    while (1);
}

void *threadConnecFunc(void *args)
{
}

void *threadDataFunc(void *args)
{
    /* Handling data manager*/
    while (1)
    {
    }
}

void *threadStorageFunc(void *args)
{
}
/*******************************************************************/
int main()
{
    int childPid;
    memset(buffer1, 0, sizeof(buffer1));
    /* Create log file */
    logFileId = log_open();
    /* Create pipe */
    if (pipe(chToPaPipe) < 0)
    {
        log_write(logFileId, "chToPaPipe pipe is created unsuccessfully\n");
        return -1;
    }
    else
    {
        log_write(logFileId, "chToPaPipe pipe is created successfully\n");
    }
    if (pipe(paToChPipe) < 0)
    {
        log_write(logFileId, "paToChPipe pipe is created unsuccessfully\n");
        return -1;
    }
    else
    {
        log_write(logFileId, "paToChPipe pipe is created successfully\n");
    }

    /* Create child process*/
    log_write(logFileId, "Creating child process\n");
    childPid = fork();
    if (childPid < 0)
    {
        /* Failed */
        log_write(logFileId, "fork() failed\n");
        return -1;
    }
    else 
    if (childPid == 0)
    {
        /* Child; log process */
        int i = 0;
        /* Establish pipe to parent process */
        if (close(chToPaPipe[0]) == -1)
            log_write(logFileId, "close(chToPaPipe[0]) failed\n");
        if (close(paToChPipe[1]) == -1)
            log_write(logFileId, "close(paToChPipe[1]) failed\n");
        while (i == 0)
            read(paToChPipe[READ_PIPE_IDX], &i, sizeof(i));
        if (i < 4) /* Failed */
        {
            printf("FAILED\n");
            return -1;
        }
            
        log_write(logFileId, "Thread handlers is created\n");
        while (1);
    }
    else if (childPid > 0)
    {
        /* Parent: main */
        int i = 0;
        my_addr.sin_family = AF_INET;
        /* Change this ip address according to your machine */
        my_addr.sin_addr.s_addr = INADDR_ANY; /* inet_addr("192.168.0.104") */
        my_addr.sin_port = htons(12000);
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
        /* Establish pipe to child process */
        if (close(chToPaPipe[1]) == -1)
            log_write(logFileId, "close(chToPaPipe[1]) failed\n");
        if (close(paToChPipe[0]) == -1)
            log_write(logFileId, "close(paToChPipe[0]) failed\n");
        /* Bind serverSock to my_addr */
        if (bind(serverSock, (struct sockaddr *)&my_addr, sizeof(my_addr)) == 0)
            printf("Binded Correctly\n");
        else
            printf("Unable to bind\n");
        write(paToChPipe[WRITE_PIPE_IDX], &i, sizeof(i));
        if (listen(serverSock, 3) == 0)
            printf("Listening ...\n");
        else
            printf("Unable to listen\n");

        addr_size = sizeof(struct sockaddr_in);
        if (pthread_create(&thread, NULL, &Receive, NULL) == 0)
        {
            printf("Receive thread is created\n");
        }
        /* Create thread */
        if (pthread_create(&threadConnect, NULL, threadConnecFunc, NULL) == 0)
            i++;
        if (pthread_create(&threadData, NULL, threadDataFunc, NULL) == 0)
            i++;
        if (pthread_create(&threadStorage, NULL, threadStorageFunc, NULL) == 0)
            i++;
        i++;                                              /* Redundent check */
        write(paToChPipe[WRITE_PIPE_IDX], &i, sizeof(i)); /* inform child to continue */
        if (i < 4)                                        /* FAIL */
            return -1;
        while (1)
        {
            acc = accept(serverSock, (struct sockaddr *)&peer_addr, &addr_size);
            childPid = fork();
            char ip[16];
            if (childPid == 0)
            {
                /* New child */
                printf("New Connection Established\n");
                /* New process */
                inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN);
                //process_list_add(newProcess);
                // "ntohs(peer_addr.sin_port)" function is
                // for finding port number of client
                printf("Connection established with IP : %s and PORT : %d\n",
                   ip, ntohs(peer_addr.sin_port));
                // recv(acc, &temp, sizeof(temp), 0);
                // printf("Client : %f\n", temp);
                strcpy(buffer1, "Hello");
                send(acc, buffer1, 256, 0);    // recv(acc, &temp, sizeof(temp), 0);
                // printf("Client : %f\n", temp);
                strcpy(buffer1, "Hello");
                send(acc, buffer1, 256, 0);
            }
            else
            {

            }
            
        }

        printf("%s\n", "end of parent");
        pthread_join(threadConnect, NULL);
        /* wait */
        pthread_join(threadData, NULL);
        pthread_join(threadStorage, NULL);
        close(chToPaPipe[0]);
    }

    return 0;
}
    