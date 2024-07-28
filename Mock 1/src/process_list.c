#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "process_list.h"
/*******************************************************************/
typedef struct intSharedMem{
    int value;
    int fileId;
    struct intSharedMem *self;
} intSharedMem;
/*******************************************************************/
/* ls -l /dev/shm to check shared mem*/

#define SHARED_MEM_COUNT        "shared_count"
#define SHARED_MEM_KEY          "shared_key"
#define SHARED_MEM_HEAD         "shared_head"
#define SHARED_MEM_TAIL         "shared_tail"
#define START_COUNT_VALUE       5000
/*******************************************************************/
ProcessListType* head;
ProcessListType* tail;
intSharedMem *key = 0;
intSharedMem *count = 0;
static inline intSharedMem* open_shared_memInt (char *File, int option);
int serverSockSave = 0;
/*******************************************************************/
static void connect(ProcessListType *a, ProcessListType *b)
{
    a->next = b;
    b->pre = a;
}
/*******************************************************************/
static inline intSharedMem* open_shared_memInt (char *File, int option)
{
    int shm_fd = shm_open(File, option, 0666);
    if (shm_fd < 0) {
        printf("shm_open() %s is failed\n",File);
        return NULL;
    }
    ftruncate(shm_fd, sizeof(intSharedMem));
    intSharedMem *data = (intSharedMem *)mmap(0, sizeof(int), option, MAP_SHARED, shm_fd, 0);
    data->fileId = shm_fd;
    return data;
}

static inline ProcessListType* open_shared_memProcess (char *File, int option)
{ 
    int shm_fd = shm_open(File, option, 0666);
    if (shm_fd < 0) {
        printf("shm_open() %s is failed\n",File);
        return NULL;
    }
    ftruncate(shm_fd, sizeof(int));
    ProcessListType *data = (ProcessListType *)mmap(0, sizeof(int), option, MAP_SHARED, shm_fd, 0);
    data->fileId = shm_fd;
    return data;       
}

int process_list_connectionCount(void)
{
    return (open_shared_memInt("shared_count", O_RDWR))->value;
}

void process_list_init(int serverSock)
{
    serverSockSave = serverSock;
    /* for key */
    key = open_shared_memInt("shared_key", O_CREAT | O_RDWR);
    key->value = START_COUNT_VALUE;
    /* FOR COUNT */
    count = open_shared_memInt("shared_count", O_CREAT | O_RDWR);
    count->value = 0;
    /* for head */
    head = open_shared_memProcess("shared_head", O_CREAT | O_RDWR);
    strcpy(head->fileName, "head");
    head->processId = -1;
    head->socketId = -1;
    head->port = -1;
    /* for tail */
    tail = open_shared_memProcess("shared_tail", O_CREAT | O_RDWR);
    strcpy(tail->fileName, "tail");
    tail->processId = -1;
    tail->socketId = -1;
    head->port = -1;
    /* head to tail*/
    connect(head, tail);
    head->pre = NULL;
    tail->next = NULL;
}

void process_list_new(int processId, char Ip[], int port, int socketId)
{
    char *newSharedMemory = (char *) malloc (15 * sizeof(char));
    ProcessListType* newProcess;
    int* save;
    save = &open_shared_memInt("shared_count", O_RDWR)->value;
    *save = *save + 1;
    sprintf(newSharedMemory,"shared_mem_%d",process_list_connectionCount());
    newProcess = open_shared_memProcess(newSharedMemory, O_CREAT | O_RDWR);
    strcpy(newProcess->fileName, newSharedMemory);
    strcpy(newProcess->Ip, Ip);
    newProcess->processId = processId;
    newProcess->port = port;
    newProcess->socketId = socketId;
    newProcess->self = newProcess;
    connect(tail->pre, newProcess);
    connect(newProcess,tail);
    free(newSharedMemory);
}

void process_list_remove(int processId)
{
    
}

ProcessListType* process_list_find(int processId)
{
    
    return NULL;
}

void process_list_closeSharedMemX(int x)
{
    char fileBuff[15];
    sprintf(fileBuff,"shared_mem_%d",x);
    munmap(open_shared_memProcess(fileBuff, O_RDWR)->self,
                 sizeof(ProcessListType));
    shm_unlink(fileBuff); 
}

void process_list_closeAll(void)
{
    char fileBuff[15];
    /* shared_mem_x */
    int i;
    /* close server */
    close(serverSockSave);
    for (i = 1; i <= open_shared_memInt("shared_count", O_RDWR)->value; i ++)
    {
        sprintf(fileBuff,"shared_mem_%d",i);
        munmap(open_shared_memProcess(fileBuff, O_RDWR)->self,
                 sizeof(ProcessListType));
        close(open_shared_memProcess(fileBuff, O_RDWR)->fileId);
        close(open_shared_memProcess(fileBuff, O_RDWR)->socketId);
        shm_unlink(fileBuff); 
    }
    // printf("Cleared all shared_mem\n");
    /* shared_head */
    munmap(open_shared_memProcess("shared_head", O_RDWR)->self,
                 sizeof(ProcessListType));
    shm_unlink("shared_head");
    // printf("Cleared shared_head\n");
    /* shared_tail */
    munmap(open_shared_memProcess("shared_tail", O_RDWR)->self,
                 sizeof(ProcessListType));
    shm_unlink("shared_tail");
    // printf("Cleared shared_tail\n");
     /* unmap shared mem */
    munmap(open_shared_memInt("shared_count", O_RDWR)->fileId
                , sizeof(int));
    munmap(open_shared_memInt("shared_key", O_RDWR)->fileId
                , sizeof(int));
    shm_unlink("shared_count");
    shm_unlink("shared_key");
    // printf("Cleared count, key\n");
    printf("Server is closed\n");
}
/*******************************************************************/