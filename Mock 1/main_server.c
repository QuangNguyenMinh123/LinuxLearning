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
#include <pthread.h>
#include <sys/select.h>
#include <poll.h>
#include "log.h"
#include "process_list.h"
/*******************************************************************/
#define SO_REUSEPORT                    15
#define BUFF_SIZE                       256
#define PORT                            10000
#define READ_PIPE_IDX                   0
#define WRITE_PIPE_IDX                  1
/*******************************************************************/

/*******************************************************************/
float temp;
int socketId = 0;
char buffer[1024] = {0};

pthread_mutex_t mutexPass = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conPass = PTHREAD_COND_INITIALIZER;
pthread_cond_t condCheckConnect = PTHREAD_COND_INITIALIZER;
int logFileId;
int serverSock;
int chToPaPipe[2], paToChPipe[2];
pthread_t threadParent_Data, threadParent_Storage;
socklen_t addr_size;
/* For server connection */
struct sockaddr_in my_addr, peer_addr;
char buffer1[BUFF_SIZE];
bool terminate = FALSE;
int logPid, parentPid;
int nodeIdxPassing;
ConnectionType* threadStorage;
float avgTemp = 0, avgSum = 0;
/*******************************************************************/
void signalHandler_INT()
{
    if (parentPid != getpid())
    {
        printf("this pid:%d\n",getpid());
        kill(getpid(), 9);
    }
    else
    {
        terminate = TRUE;
    }
        
}

void signalHandler_CHLD()
{
    printf("Child is finish, child pid = %d\n",getpid());
    wait(NULL);
}

void *threadParent_DataFunc(int * nodeIdx)
{
    /* Handling data manager*/
    int cnt = 0;
    float data = 0;
    int checkCnt;
    while (terminate == FALSE)
    {
        pthread_mutex_lock(&mutexPass);
        pthread_cond_wait(&conPass, &mutexPass);
        checkCnt = process_list_ReadData(*nodeIdx, &data);
        pthread_mutex_unlock(&mutexPass);
        if (checkCnt == 1)
        {
            avgSum += data;
            cnt++;
            avgTemp = avgSum / cnt;
            printf("temp read: %f\tavg temp: %f\n", data, avgTemp);
        }
    }
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
    int childPid;
    memset(buffer1, 0, sizeof(buffer1));
    logFileId = log_open();
    signal(SIGCHLD, signalHandler_CHLD);
    signal(SIGINT, signalHandler_INT);
    /* Create pipe */
    parentPid = getpid();
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
        printf("log pid = %d\n", getpid());
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
        // while (1)
        // {
        //     /* wait for command */
        // }
    }
    else if (childPid > 0)
    {
        /* Parent: main */
        int i = 0;
        printf("parent pid = %d\n", getpid());
        childPid = getpid();
        pthread_mutex_init(&mutexPass, NULL);
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
        if (pthread_create(&threadParent_Data, NULL, threadParent_DataFunc, &nodeIdxPassing) == 0)
            i++;
        if (pthread_create(&threadParent_Storage, NULL, threadParent_StorageFunc, threadStorage) == 0)
            i++;
        i++;                                              /* Redundent check */
        write(paToChPipe[WRITE_PIPE_IDX], &i, sizeof(i)); /* inform child to continue */
        if (i < 3)                                        /* FAIL */
            return -1;
        struct pollfd pollfds;
        pollfds.fd = serverSock;
        pollfds.events = POLLIN | POLLPRI;
        int useClient = 0;
        while (terminate == FALSE)
        {
            /* Thread connection */
            int pollResult = poll(&pollfds, useClient + 1, 1);
            if (pollResult > 0)
            {
                if (pollfds.revents & POLLIN)
                {
                    char ip[16];
                    socketId = accept(serverSock, (struct sockaddr *)&peer_addr, &addr_size);
                    printf("New Connection Established\n");
                    inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN);
                    printf("Connection established with IP : %s and PORT: %d\n",
                                ip, ntohs(peer_addr.sin_port));
                    process_list_new(ip, ntohs(peer_addr.sin_port), socketId);
                    printf("Connection count = %d\n",process_list_connectionCount());
                    fflush(stdout);
                    useClient++;
                }
                else
                {
                    for (int i = 1; i < process_list_connectionIdx(); i ++)
                    {
                        float data;
                        if (process_list_checkConnect(i) == TRUE)
                        {
                            int buffCnt = process_list_readDataFromNode(i, &data);
                            if (buffCnt  <= 0)
                            {
                                pollfds.revents = 0;
                                useClient--;
                                process_list_Disconnect(i);
                            }
                            else
                            {
                                process_list_WriteData(i, data);
                                pthread_mutex_lock(&mutexPass);
                                nodeIdxPassing = i;
                                pthread_cond_signal(&conPass);
                                pthread_mutex_unlock(&mutexPass);
                            }
                        }
                    }
                }
            }
        }
        /* wait */
        pthread_join(threadParent_Data, NULL);
        pthread_join(threadParent_Storage, NULL);
        close(chToPaPipe[0]);
        printf("%s\n", "end of parent");
        process_list_closeAll();
        
    }
    return 0;
}
    