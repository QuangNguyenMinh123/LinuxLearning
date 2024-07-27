#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "process_list.h"
/*******************************************************************/
#define CREAT_SHARED_MEM

#define SHARED_MEM_COUNT        "shared_count"
#define SHARED_MEM_KEY          "shared_key"
#define SHARED_MEM_HEAD         "shared_head"
#define SHARED_MEM_TAIL         "shared_tail"
#define START_COUNT_VALUE       5000
/*******************************************************************/
ProcessListType* head;
ProcessListType* tail;
int *key = 0;
int *count = 0;
static inline int* open_shared_memInt (char *File, int option);
/*******************************************************************/
static void connect(ProcessListType *a, ProcessListType *b)
{
    a->next = b;
    b->pre = a;
}
/*******************************************************************/
static inline int* open_shared_memInt (char *File, int option)
{
    int shm_fd = shm_open(File, option, 0666);
    if (shm_fd < 0) {
        printf("shm_open() is failed\n");
        return NULL;
    }
    ftruncate(shm_fd, sizeof(int));
    int *data = (int *)mmap(0, sizeof(int), option, MAP_SHARED, shm_fd, 0);
    return data;         
}
int process_list_connectionCount(void)
{
    return *(open_shared_memInt("shared_count", O_RDWR));
}

void process_list_init(void)
{
    /* for key */
    key = open_shared_memInt("shared_count", O_CREAT | O_RDWR);
    *key = START_COUNT_VALUE;
    /* FOR COUNT */
    *(open_shared_memInt("shared_count", O_CREAT | O_RDWR)) = 0;
    /* for head */
    int mem_id = shmget(*key,sizeof(ProcessListType),IPC_CREAT|0666);
    if (mem_id<0) {
        perror("head error shmget");
        return NULL;
    }
    *key ++;
    head = (ProcessListType*)shmat(mem_id,(void*)0,0);
    /* for tail */
    mem_id = shmget(*key,sizeof(ProcessListType),IPC_CREAT|0666);
    if (mem_id<0) {
        perror("error shmget");
    }
    *key ++;
    tail = (ProcessListType*)shmat(mem_id,(void*)0,0);
    /* head to tail*/
    connect(head, tail);
    head->pre = NULL;
    tail->next = NULL;
}

void process_list_new(int processId, char Ip[], int port, int socketId)
{
    ProcessListType* newProcess;
    int* save;
    int mem_id = shmget(*key,sizeof(ProcessListType),IPC_CREAT|0666);
    if (mem_id<0) {
        perror("error shmget for new process");
    }
    newProcess = (ProcessListType*)shmat(mem_id,(void*)0,0);
    strcpy(newProcess->Ip, Ip);
    newProcess->processId = processId;
    newProcess->port = port;
    newProcess->socketId = socketId;
    connect(tail->pre, newProcess);
    connect(newProcess,tail);
    save = open_shared_memInt("shared_count", O_RDWR);
    *save = *save + 1;
}

void process_list_remove(int processId)
{
    
}

ProcessListType* process_list_find(int processId)
{
    
    return NULL;
}
/*******************************************************************/