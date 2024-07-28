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
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include "log.h"
#include "process_list.h"
/*******************************************************************/
#define bool                            char
#define TRUE                            1
#define FALSE                           0
#define SO_REUSEPORT                    15
#define BUFF_SIZE                       256
#define PORT                            10000
#define READ_PIPE_IDX                   0
#define WRITE_PIPE_IDX                  1
/*******************************************************************/

/*******************************************************************/
float temp;
int acc = 0;
char buffer[1024] = {0};

pthread_mutex_t mutexCheckConnect = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condCheckConnect = PTHREAD_COND_INITIALIZER;
int logFileId;
int serverSock;
int chToPaPipe[2], paToChPipe[2];
pthread_t threadParent_Data, threadParent_Storage;
pthread_t threadChild_Receive, threadChild_CheckConnect;
socklen_t addr_size;
/* For server connection */
struct sockaddr_in my_addr, peer_addr;
char buffer1[BUFF_SIZE];
int childPid;
/*******************************************************************/
void signalHandler_INT()
{
    printf("This is Ctrl + C, pid = %d\n", getpid());
    /* close all socket */
    process_list_closeSharedMemX(process_list_findSharedmemByPid(getpid()));
    kill(getpid(), 9);
}

void signalHandler_CHLD()
{
    printf("Child is finish, child pid = %d\n",getpid());
    wait(NULL);
}


void *threadChild_ReceiveFunc(ProcessListType* arg)
{
    int check = 1;
    while (check > 0)
    {
        check = read( arg->socketId, &temp, sizeof(buffer));
        printf("Host receives from IP %s, port %d: temp = %f\n", arg->Ip, arg->port, temp);
    }
    printf("Disconnected from IP %s, port %d\n", arg->Ip, arg->port);
}

void *threadParent_DataFunc(void *args)
{
    /* Handling data manager*/
    // while (1)
    // {
    // }
}

void *threadParent_StorageFunc(void *args)
{
    // while (1)
    // {
    // }
}
/*******************************************************************/
int main()
{
    memset(buffer1, 0, sizeof(buffer1));
    logFileId = log_open();
    /* Create pipe */
    signal(SIGCHLD, signalHandler_CHLD);
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
        
        childPid = getpid();
        printf("log pid: %d\n",getpid());
        /* Establish pipe to parent process */
        if (close(chToPaPipe[0]) == -1)
            log_write(logFileId, "close(chToPaPipe[0]) failed\n");
        if (close(paToChPipe[1]) == -1)
            log_write(logFileId, "close(paToChPipe[1]) failed\n");
        while (i == 0)
            read(paToChPipe[READ_PIPE_IDX], &i, sizeof(i));
        if (i < 3) /* Failed */
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
        childPid = getpid();
        printf("parent pid: %d\n",getpid());
        /* Variable for socket */
        my_addr.sin_family = AF_INET;
        /* Change this ip address according to your machine */
        my_addr.sin_addr.s_addr = INADDR_ANY; /* inet_addr("192.168.0.104")   , INADDR_ANY */
        my_addr.sin_port = htons(PORT);
        /* Initialize socket */
        serverSock = socket(AF_INET, SOCK_STREAM, 0);
        process_list_init(serverSock);
        if (serverSock < 0)
        {
            log_write(logFileId, "Error in creating server socket\n");
            return -1;
        }
        else
        {
            log_write(logFileId, "Server is created\n");
        }
        int opt = 1;
        if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            printf("Failed to set SO_REUSEADDR option\n");
            return -1;
        }

        if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        {
            printf("Failed to set SO_REUSEPORT option\n");
            return -1;
        }
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
        /* Create thread */
        if (pthread_create(&threadParent_Data, NULL, threadParent_DataFunc, NULL) == 0)
            i++;
        if (pthread_create(&threadParent_Storage, NULL, threadParent_StorageFunc, NULL) == 0)
            i++;
        i++;                                              /* Redundent check */
        write(paToChPipe[WRITE_PIPE_IDX], &i, sizeof(i)); /* inform child to continue */
        if (i < 3)                                        /* FAIL */
            return -1;
        while (1)
        {
            /* Thread connection */
            acc = accept(serverSock, (struct sockaddr *)&peer_addr, &addr_size);
            if (acc >= 0)
            {
                int newChildPid;
                newChildPid = fork();
                if (newChildPid == 0)
                {
                    signal(SIGINT, signalHandler_INT);
                    char ip[16];
                    ProcessListType newProcess;
                    /* Cancle parent's thread */
                    pthread_cancel(threadParent_Data);
                    pthread_cancel(threadParent_Storage);
                    /* New child */
                    printf("New Connection Established\n");
                    /* New process */
                    inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN);
                    printf("Connection established with IP : %s and PORT: %d, processId = %d\n",
                                ip, ntohs(peer_addr.sin_port), getpid());
                    newProcess = process_list_new(getpid(), ip, ntohs(peer_addr.sin_port), acc);
                    printf("Connection count = %d\n",process_list_connectionCount());
                    fflush(stdout);
                    /* Create new thread */
                    if (pthread_create(&threadChild_Receive, NULL, threadChild_ReceiveFunc, &newProcess) == 0)
                        printf("Create receive successfully\n");
                    pthread_join(threadChild_Receive, NULL);
                }
            }
            else
            {
                break;
            }
        }
        printf("%s\n", "end of parent");
        
        /* wait */
        pthread_join(threadParent_Data, NULL);
        pthread_join(threadParent_Storage, NULL);
        close(chToPaPipe[0]);
    }
    return 0;
}
    