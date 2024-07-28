#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "process_list.h"
/*******************************************************************/

/*******************************************************************/
/* ls -l /dev/shm to check shared mem*/
#define SHARED_MEM_COUNT        "shared_idx"
#define SHARED_MEM_COUNT        "shared_count"
#define START_COUNT_VALUE       5000
/*******************************************************************/
intSharedMem *count = 0;
intSharedMem *Idx = 0;
int serverSockSave = 0;
/*******************************************************************/

/*******************************************************************/
intSharedMem* open_shared_memInt (char *File, int option)
{
    int shm_fd = shm_open(File, option, 0666);
    if (shm_fd < 0) {
        return NULL;
    }
    ftruncate(shm_fd, sizeof(intSharedMem));
    intSharedMem *data = (intSharedMem *)mmap(0, sizeof(intSharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    data->fileId = shm_fd;
    close(shm_fd);
    return data;
}

ProcessListType* open_shared_memProcess (char *File, int option)
{ 
    int shm_fd = shm_open(File, option, 0666);
    if (shm_fd < 0) {
        return NULL;
    }
    ftruncate(shm_fd, sizeof(ProcessListType));
    ProcessListType *data = (ProcessListType *)mmap(0, sizeof(ProcessListType), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    data->fileId = shm_fd;
    close(shm_fd);
    return data;       
}

int process_list_connectionCount(void)
{
    return (open_shared_memInt("shared_count", O_RDWR))->value;
}

int process_list_connectionIdx(void)
{
    return (open_shared_memInt("shared_idx", O_RDWR))->value;
}

void process_list_init(int serverSock)
{
    serverSockSave = serverSock;
    /* FOR COUNT */
    count = open_shared_memInt("shared_count", O_CREAT | O_RDWR);
    count->value = 0;
    /* FOR IDX */
    Idx = open_shared_memInt("shared_idx", O_CREAT | O_RDWR);
    Idx->value = 0;
}

ProcessListType process_list_new(int processId, char Ip[], int port, int socketId)
{
    char *newSharedMemory = (char *) malloc (15 * sizeof(char));
    ProcessListType* newProcess;
    int* saveCount =  &open_shared_memInt("shared_count", O_RDWR)->value;
    *saveCount = *saveCount + 1;
    int* saveIdx =  &open_shared_memInt("shared_idx", O_RDWR)->value;
    *saveIdx = *saveIdx + 1;
    sprintf(newSharedMemory,"shared_mem_%d",process_list_connectionIdx());
    newProcess = open_shared_memProcess(newSharedMemory, O_CREAT | O_RDWR);
    newProcess->shared_mem_idx = *saveIdx;
    sprintf(newProcess->fileName,newSharedMemory);
    strcpy(newProcess->Ip, Ip);
    newProcess->processId = processId;
    newProcess->port = port;
    newProcess->socketId = socketId;
    newProcess->self = newProcess;
    free(newSharedMemory);
    return *newProcess;
}

void process_list_Disconnect(int x)
{
    char fileName[15];
    sprintf(fileName,"shared_mem_%d",x);
    if (x <= 0)
        return 0;
    int shm_fd = shm_open(fileName, O_RDWR, 0666);
    if (shm_fd < 0) {
        return ;
    }
    ftruncate(shm_fd, sizeof(ProcessListType));
    ProcessListType *data = (ProcessListType *)mmap(0, sizeof(ProcessListType), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    data->processId = -1;
    int* saveCount =  &open_shared_memInt("shared_count", O_RDWR)->value;
    *saveCount = *saveCount - 1;
    close(shm_fd);
}

void process_list_closeSharedMemX(int x)
{
    char fileName[15];
    int* saveCount;
    sprintf(fileName,"shared_mem_%d",x);
    if (x <= 0)
        return;
    /* open shared mem */
    int shm_fd = shm_open(fileName, O_RDWR, 0666);
    if (shm_fd < 0) {
        return ;
    }
    ftruncate(shm_fd, sizeof(ProcessListType));
    ProcessListType *data = (ProcessListType *)mmap(0, sizeof(ProcessListType), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    munmap(data->fileId, sizeof(ProcessListType));
    close(data->fileId);
    close(data->socketId);
    close(shm_fd);
    shm_unlink(fileName);
    saveCount = &open_shared_memInt("shared_count", O_RDWR)->value;
    *saveCount = *saveCount - 1;
}

int process_list_findSharedmemByPid(int pid)
{
    int i;
    int maxIdx = process_list_connectionIdx();
    char fileBuff[15];
    for (i = 1; i <= maxIdx; i ++)
    {
        sprintf(fileBuff,"shared_mem_%d",i);
        if (open_shared_memProcess(fileBuff, O_RDWR) != NULL)
            if (open_shared_memProcess(fileBuff, O_RDWR)->processId == pid)
            {
                return open_shared_memProcess(fileBuff, O_RDWR)->shared_mem_idx;
            }
    }
    return -1;
}

void process_list_closeAll(void)
{
    /* close server */
    close(serverSockSave);
    /* unmap shared mem */
    munmap(open_shared_memInt("shared_count", O_RDWR)->fileId, sizeof(intSharedMem));
    shm_unlink("shared_count");
    /* unmap shared mem */
    munmap(open_shared_memInt("shared_idx", O_RDWR)->fileId, sizeof(intSharedMem));
    shm_unlink("shared_idx");
    // printf("Cleared count\n");
    printf("\nServer is manually shutdown\n");
}
/*******************************************************************/