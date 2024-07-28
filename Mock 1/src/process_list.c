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
#define START_COUNT_VALUE       5000
/*******************************************************************/
intSharedMem *count = 0;
static inline intSharedMem* open_shared_memInt (char *File, int option);
int serverSockSave = 0;
/*******************************************************************/

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
    /* FOR COUNT */
    count = open_shared_memInt("shared_count", O_CREAT | O_RDWR);
    count->value = 0;
}

ProcessListType process_list_new(int processId, char Ip[], int port, int socketId)
{
    char *newSharedMemory = (char *) malloc (15 * sizeof(char));
    ProcessListType* newProcess;
    int* save =  &open_shared_memInt("shared_count", O_RDWR)->value;
    *save = *save + 1;
    sprintf(newSharedMemory,"shared_mem_%d",process_list_connectionCount());
    newProcess = open_shared_memProcess(newSharedMemory, O_CREAT | O_RDWR);
    newProcess->shared_mem_count = *save;
    printf("shared mem count = %d\n",newProcess->shared_mem_count);
    strcpy(newProcess->Ip, Ip);
    newProcess->processId = processId;
    newProcess->port = port;
    newProcess->socketId = socketId;
    newProcess->self = newProcess;
    free(newSharedMemory);
    return *newProcess;
}

void process_list_remove(int processId)
{
    
}

void process_list_closeSharedMemX(int x)
{
    char fileBuff[15];
    int* save;
    sprintf(fileBuff,"shared_mem_%d",x);
    munmap(open_shared_memProcess(fileBuff, O_RDWR)->self, sizeof(ProcessListType));
    close(open_shared_memProcess(fileBuff, O_RDWR)->fileId);
    close(open_shared_memProcess(fileBuff, O_RDWR)->socketId);
    shm_unlink(fileBuff);
    save = &open_shared_memInt("shared_count", O_RDWR)->value;
    //*save = *save - 1;
}

int process_list_findSharedmemByPid(int pid)
{
    int i;
    int countSave = open_shared_memInt("shared_count", O_RDWR)->value;
    char fileBuff[15];
    printf("Finding PID: %d\n",pid);
    printf("countSave = %d\n",countSave);
    for (i = 1; i <= countSave; i ++)
    {
        sprintf(fileBuff,"shared_mem_%d",i);
        printf("File opening: %s\n",fileBuff);
        printf("PID found: %d\n",open_shared_memProcess(fileBuff, O_RDWR)->processId);
        printf("memcount: %d\n",open_shared_memProcess(fileBuff, O_RDWR)->shared_mem_count);
        if (open_shared_memProcess(fileBuff, O_RDWR)->processId == pid)
        {
            printf("found 123:%d", open_shared_memProcess(fileBuff, O_RDWR)->shared_mem_count);
            return open_shared_memProcess(fileBuff, O_RDWR)->shared_mem_count;
        }
    }
    return -1;
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
        process_list_closeSharedMemX(i);
    }
    // printf("Cleared all shared_mem\n");
     /* unmap shared mem */
    munmap(open_shared_memInt("shared_count", O_RDWR)->fileId, sizeof(int));
    shm_unlink("shared_count");
    // printf("Cleared count\n");
    printf("Server is closed\n");
}
/*******************************************************************/