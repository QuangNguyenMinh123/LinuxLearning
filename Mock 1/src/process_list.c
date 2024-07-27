#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include "process_list.h"
/*******************************************************************/
#define SHARED_MEM_COUNT        "shared_count"
#define START_COUNT_VALUE       5000
/*******************************************************************/
ProcessListType* head;
ProcessListType* tail;
int *key = 0;
/*******************************************************************/
static void connect(ProcessListType *a, ProcessListType *b)
{
    a->next = b;
    b->pre = a;
}
/*******************************************************************/
void process_list_init(void)
{
    /* for count */
    int shm_fd = shm_open(SHARED_MEM_COUNT, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        printf("shm_open() is failed\n");
        return;
    }
    ftruncate(shm_fd, sizeof(int));
    key = (int *)mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, 
                            MAP_SHARED, shm_fd, 0);
    *key = START_COUNT_VALUE; 
    int mem_id = shmget(*key,sizeof(ProcessListType),IPC_CREAT|0666);
    if (mem_id<0) {
        perror("head error shmget");
    }
    head = (ProcessListType*)shmat(mem_id,(void*)0,0);
    mem_id = shmget(*key,sizeof(ProcessListType),IPC_CREAT|0666);
    if (mem_id<0) {
        perror("error shmget");
    }
    tail = (ProcessListType*)shmat(mem_id,(void*)0,0);
    connect(head, tail);
    head->pre = NULL;
    tail->next = NULL;
}

void process_list_add(ProcessListType *process)
{
    
}
void process_list_remove(int processId)
{
    
}
ProcessListType* process_list_find(int processId)
{
    
    return NULL;
}
/*******************************************************************/