#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "process_list.h"
/*******************************************************************/
#define node(x)                 open_shared_memProcess()[x]
#define nodeCount               node(0).processId
#define nodeIdx                 node(0).Idx

/*******************************************************************/
/* ls -l /dev/shm to check shared mem*/
#define MAX_NODE                100
#define MEM_SIZE                sizeof(ProcessListType) * MAX_NODE
#define FILE_SHARED_MEM_NAME    "data"
#define HEAD                    0
#define TAIL                    MAX_NODE
/*******************************************************************/
int serverSockSave = 0;
/*******************************************************************/

/*******************************************************************/
ProcessListType* open_shared_memProcess(void)
{ 
   int shm_fd = shm_open(FILE_SHARED_MEM_NAME, O_RDWR, 0666);
    if (shm_fd < 0) {
        return NULL;
    }
    ftruncate(shm_fd, MEM_SIZE);
    ftruncate(shm_fd, MEM_SIZE);
    ProcessListType *data = (ProcessListType *)mmap(0, MEM_SIZE,
                     PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    data->fileId = shm_fd;
    close(shm_fd);
    return data;      
}

void connect(int nodex, int nodey)
{
    node(nodex).next = nodey;
    node(nodey).pre = nodex;
}

int process_list_connectionCount(void)
{
    return nodeCount;
}

int process_list_connectionIdx(void)
{
    return nodeIdx;
}

void process_list_init(int serverSock)
{
    serverSockSave = serverSock;
    int shm_fd = shm_open(FILE_SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        return;
    }
    ftruncate(shm_fd, MEM_SIZE);
    ProcessListType *data = (ProcessListType *)mmap(0, MEM_SIZE,
                     PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    data->fileId = shm_fd;
    close(shm_fd);
    node(0).processId = 0;
    node(0).pre = -1;
    node(MAX_NODE).next = -1;
    nodeIdx = 1;
    nodeCount = 0;
    connect(HEAD, TAIL);
}

ProcessListType process_list_new(int processId, char* Ip, int port, int socketId)
{
    sprintf(node(nodeIdx).Ip, "%s",Ip);
    node(nodeIdx).processId = processId;
    node(nodeIdx).port = port;
    node(nodeIdx).socketId = socketId;
    node(nodeIdx).Idx = nodeIdx;
    node(nodeIdx).newData = TRUE;
    connect(node(TAIL).pre,nodeIdx);
    connect(nodeIdx, TAIL);
    nodeIdx++;
    if (nodeIdx > MAX_NODE - 1)
        printf("out of mem\n");
    nodeCount++;
    return node(nodeIdx-1);
}

void process_list_Disconnect(int nodex)
{
    connect(node(nodex).pre, node(nodex).next);
    nodeCount --;
}

int process_list_findIdxByPid(int pid)
{
    int ptr = 0;
    while (ptr != -1)
    {
        if (node(ptr).processId == pid)
            return ptr;
        else
        {
            ptr = node(ptr).next;
        }
    }
    return -1;

}

float process_list_readDataFromNode(int nodex)
{
    float data;
    int check = read( node(nodex).socketId, &data, sizeof(data));
    if (check > 0)
    {
        if (node(nodex).newData == TRUE)
            printf("Host receives from IP %s, port %d: temp = %f\n", 
            node(nodex).Ip, node(nodex).port, node(nodex).temp);
        else
        {
            printf("FALSE\n");
            return -1.0;
        }
    }
    else
    {
        return -1.0;
    }
}

void process_list_closeAll(void)
{
    /* close server */
    close(serverSockSave);
    /* unmap shared mem */
    munmap(node(HEAD).fileId, MEM_SIZE);
    shm_unlink(FILE_SHARED_MEM_NAME);
    // printf("Cleared count\n");
    printf("\nServer is manually shutdown\n");
}
/*******************************************************************/