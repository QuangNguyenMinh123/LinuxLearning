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
#include <pthread.h>
#include <sys/select.h>
#include <poll.h>
#include <math.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "log.h"
#include "process_list.h"
#include "sql.h"
/*******************************************************************/
#define SO_REUSEPORT                    15
#define BUFF_SIZE                       256
#define PORT                            10000
#define READ_PIPE_IDX                   0
#define WRITE_PIPE_IDX                  1
#define NOTHING                         1000
#define NEW_CONNECTION                  10005
#define NEW_DATA                        1100
#define DISCONNECT                      1122
/*******************************************************************/

/*******************************************************************/
float temp;
int socketId = 0;
pthread_mutex_t mutexData = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conData = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexStorage = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conStorage = PTHREAD_COND_INITIALIZER;
int logFileId;
int serverSock;
int chToPaPipe[2], paToChPipe[2];
pthread_t threadParent_Data, threadParent_Storage;
socklen_t addr_size;
/* For server connection */
struct sockaddr_in my_addr, peer_addr;
bool terminate = FALSE;
int parentPid = 0;
int nodeIdxPassing;
ConnectionType* threadStorage;
float avgTemp = 0, avgSum = 0;
sem_t* sem_PaToCh;
sem_t* sem_ChToPa;
/*******************************************************************/
void signalHandler_INT()
{
    if (parentPid != getgid())
    {
        printf("this pid:%d\n",getpid());
        terminate = TRUE;
        pthread_cond_signal(&conData);
        pthread_mutex_unlock(&mutexData);
    }
}

void signalHandler_CHLD()
{
    wait(NULL);
}

// void signalHandler_PIPE()
// {
//     printf("Broken pipe\n");
// }


void *threadParent_DataFunc(int * args)
{
    /* Handling data manager*/
    int cnt = 0;
    float data = 0;
    int checkCnt;
    int value;
    while (terminate == FALSE)
    {
        pthread_mutex_lock(&mutexData);
        pthread_cond_wait(&conData, &mutexData);
        checkCnt = process_list_ReadData(*args, &data);
        // printf("args = %d\tdata = %f\n",*args,data);
        pthread_mutex_unlock(&mutexData);
        /* Signaling Datamanager thread to continue */
        pthread_mutex_lock(&mutexStorage);
        pthread_cond_signal(&conStorage);
        pthread_mutex_unlock(&mutexStorage);
        if (checkCnt == 1)
        {
            avgSum += data;
            cnt++;
            avgTemp = avgSum / cnt;
        }

    }
}

void *threadParent_StorageFunc(int *args)
{
    sql_init(logFileId);
    int save;
    while (terminate == FALSE)
    {
        pthread_mutex_lock(&mutexStorage);
        pthread_cond_wait(&conStorage, &mutexStorage);
        save = *args;
        if (save > 0)
            sql_insert(process_list_node(save));
        pthread_mutex_unlock(&mutexStorage);
    }
}
/*******************************************************************/
int main()
{
    int childPid;
    logFileId = log_open();
    signal(SIGINT, signalHandler_INT);
    //sigaction(SIGPIPE, &(struct sigaction){signalHandler_PIPE}, NULL);
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
    /* Create semaphore */
    
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
            read(paToChPipe[READ_PIPE_IDX], &i, sizeof(int));
        if (i < 3) /* Failed */
        {
            printf("FAILED\n");
            return -1;
        }
        log_write(logFileId, "Thread handlers is created\n");

        char buffer[200] = {0};
        char Ip[INET_ADDRSTRLEN] = {0};
        int port;
        float temp;
        int checkCnt = 0;
        int Command = NOTHING;
        while (terminate == FALSE)
        {
            Command = NOTHING;
            checkCnt = read(paToChPipe[READ_PIPE_IDX], &Command, sizeof(int));       /* wait for parent send node*/
            if (checkCnt > 0)
            {
                if (Command == NEW_CONNECTION)
                {
                    read(paToChPipe[READ_PIPE_IDX], Ip, INET_ADDRSTRLEN * sizeof(char));
                    read(paToChPipe[READ_PIPE_IDX], &port, sizeof(int));
                    sprintf(buffer,"Connection established with IP %s and PORT: %d\n",Ip,port);
                    log_write(logFileId,buffer);
                    fflush(stdout);
                }
                else if (Command == DISCONNECT)
                {
                    read(paToChPipe[READ_PIPE_IDX], Ip, INET_ADDRSTRLEN * sizeof(char));
                    read(paToChPipe[READ_PIPE_IDX], &port, sizeof(int));
                    sprintf(buffer, "Disconnect from IP %s, port %d\n", Ip, port);
                    log_write(logFileId,buffer);
                }
                else if (Command == NEW_DATA)
                {
                    read(paToChPipe[READ_PIPE_IDX], Ip, INET_ADDRSTRLEN * sizeof(char));
                    read(paToChPipe[READ_PIPE_IDX], &port, sizeof(int));
                    read(paToChPipe[READ_PIPE_IDX], &temp, sizeof(float));
                    int int1 = temp;
                    float tempFrac = temp - int1;
                    int int2 = trunc(tempFrac * 10000);
                    sprintf(buffer, "IP: %s, port: %d, temp: %d.%04d\n", Ip, port, int1, int2);
                    log_write(logFileId,buffer);
                }
            }
        }
        printf("end of log\n");
    }
    else if (childPid > 0)
    {
        /* Parent: main */
        if (1)
        {
            int i = 0;
            signal(SIGCHLD, signalHandler_CHLD);
            parentPid = getpid();
            pthread_mutex_init(&mutexData, NULL);
            pthread_cond_init(&conData, NULL);
            pthread_mutex_init(&mutexStorage, NULL);
            pthread_cond_init(&conStorage, NULL);
            if (pthread_cond_init(&conData, NULL) != 0)
                printf("Error while creating cond variable\n");
            /* Variable for socket */
            my_addr.sin_family = AF_INET;
            /* Change this IP address according to your machine */
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
            if (listen(serverSock, 1) == 0)
                printf("Listening ...\n");
            else
                printf("Unable to listen\n");

            addr_size = sizeof(struct sockaddr_in);
            /* Create thread */
            if (pthread_create(&threadParent_Data, NULL, &threadParent_DataFunc, &nodeIdxPassing) == 0)
                i++;
            if (pthread_create(&threadParent_Storage, NULL, &threadParent_StorageFunc, &nodeIdxPassing) == 0)
                i++;
            i++;                                              /* Redundent check */
            write(paToChPipe[WRITE_PIPE_IDX], &i, sizeof(i)); /* inform child to continue */
            if (i < 3)                                        /* FAIL */
            {
                printf("FAILED\n");
                return -1;
            }
        }
        struct pollfd pollfds;
        pollfds.fd = serverSock;
        pollfds.events = POLLIN | POLLPRI;
        int useClient = 0;
        int Command = NOTHING;
        while (terminate == FALSE)
        {
            /* Thread connection */
            int pollResult = poll(&pollfds, useClient + 1, 1);
            if (pollResult > 0)
            {
                if (pollfds.revents & POLLIN)
                {
                    char Ip[INET_ADDRSTRLEN];
                    ConnectionType Dum;
                    socketId = accept(serverSock, (struct sockaddr *)&peer_addr, &addr_size);
                    printf("New Connection Established\n");
                    inet_ntop(AF_INET, &(peer_addr.sin_addr), Ip, INET_ADDRSTRLEN);
                    printf("Connection established with IP: %s and PORT: %d\n", Ip, ntohs(peer_addr.sin_port));
                    Dum = process_list_new(Ip, ntohs(peer_addr.sin_port), socketId);
                    printf("Connection count = %d\n",process_list_connectionCount());
                    useClient++;
                    sql_newnode(&Dum);
                    /* Prepare for data */
                    Command = NEW_CONNECTION;
                    /* Write to child process */
                    pthread_mutex_lock(&mutexData);
                    write(paToChPipe[WRITE_PIPE_IDX], &Command, sizeof(int));
                    write(paToChPipe[WRITE_PIPE_IDX], Dum.Ip, INET_ADDRSTRLEN * sizeof(char));
                    write(paToChPipe[WRITE_PIPE_IDX], &Dum.port, sizeof(int));
                    pthread_mutex_unlock(&mutexData);
                }
                else
                {
                    for (int cnt = 1; cnt < process_list_connectionIdx(); cnt ++)
                    {
                        float data;
                        if (process_list_checkConnect(cnt) == TRUE)
                        {
                            int buffCnt = process_list_readDataFromNode(cnt, &data);
                            if (data > -500 || buffCnt == 0)
                            {
                                if (buffCnt < -1)       /* Nodata*/
                                {
                                    buffCnt = 1;
                                }
                                else if (buffCnt == 0)  /* Disconnected */
                                {
                                    pollfds.revents = 0;
                                    useClient--;
                                    process_list_Disconnect(cnt);
                                    sql_disconnect(process_list_node(cnt));
                                    /* Prepare data */
                                    Command = DISCONNECT;
                                    /* Send to child process */
                                    pthread_mutex_lock(&mutexData);
                                    write(paToChPipe[WRITE_PIPE_IDX], &Command, sizeof(int));
                                    write(paToChPipe[WRITE_PIPE_IDX], process_list_node(cnt)->Ip, 
                                                            INET_ADDRSTRLEN * sizeof(char));
                                    write(paToChPipe[WRITE_PIPE_IDX], &process_list_node(cnt)->port, 
                                                            sizeof(int));
                                    pthread_mutex_unlock(&mutexData);
                                }
                                else
                                {
                                    pthread_mutex_lock(&mutexData);
                                    process_list_WriteData(cnt, data);
                                    nodeIdxPassing = cnt;
                                    /* Prepare data */
                                    Command = NEW_DATA;
                                    /* Send to child process */
                                    write(paToChPipe[WRITE_PIPE_IDX], &Command, sizeof(int));
                                    write(paToChPipe[WRITE_PIPE_IDX], &process_list_node(cnt)->Ip, INET_ADDRSTRLEN * sizeof(char));
                                    write(paToChPipe[WRITE_PIPE_IDX], &process_list_node(cnt)->port, sizeof(int));
                                    write(paToChPipe[WRITE_PIPE_IDX], &process_list_node(cnt)->temp, sizeof(float));
                                    pthread_cond_signal(&conData);
                                    pthread_mutex_unlock(&mutexData);
                                }
                            }
                        }
                    }
                }
                pollResult = 0;
            }
        }
        /* wait */
        pthread_mutex_destroy(&mutexData);
        pthread_cond_destroy(&conData);
        pthread_mutex_destroy(&mutexStorage);
        pthread_cond_destroy(&conStorage);
        pthread_join(threadParent_Data, NULL);
        pthread_join(threadParent_Storage, NULL);
        close(chToPaPipe[0]);
        sql_deinit();
        printf("%s\n", "end of parent");
        process_list_closeAll();
        log_close(logFileId);
    }
    return 0;
}
    