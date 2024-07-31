#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "process_list.h"

/*******************************************************************/
#define node(x)                 buffPtr[x]

/*******************************************************************/
/* ls -l /dev/shm to check shared mem*/
#define MAX_NODE                100
#define MEM_SIZE                sizeof(ConnectionType) * MAX_NODE
#define FILE_SHARED_MEM_NAME    "data"
#define HEAD                    0
#define TAIL                    MAX_NODE
/*******************************************************************/
int serverSockSave = 0;
ConnectionType* buffPtr;
int nodeCount = 0;
int nodeIdx = 0;
int checkInit = FALSE;
/*******************************************************************/

/*******************************************************************/
void connect(int nodex, int nodey)
{
    if (checkInit == TRUE)
    {
        node(nodex).next = nodey;
        node(nodey).pre = nodex;
    }
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
    ConnectionType *data = (ConnectionType *)mmap(0, MEM_SIZE,
                     PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    data->self = data;
    buffPtr = data;
    close(shm_fd);
    nodeCount = 0;
    node(HEAD).pre = -1;
    node(MAX_NODE).next = -1;
    nodeIdx = 1;
    nodeCount = 0;
    connect(HEAD, TAIL);
    checkInit = TRUE;
}

ConnectionType process_list_new(char* Ip, int port, int socketId)
{
    sprintf(node(nodeIdx).Ip, "%s",Ip);
    node(nodeIdx).port = port;
    node(nodeIdx).socketId = socketId;
    node(nodeIdx).Idx = nodeIdx;
    node(nodeIdx).Connected = TRUE;
    node(nodeIdx).newData = FALSE;
    connect(node(TAIL).pre,nodeIdx);
    connect(nodeIdx, TAIL);
    nodeIdx++;
    if (nodeIdx > MAX_NODE - 1)
        printf("Out of mem\n");
    nodeCount++;
    int flags = fcntl(socketId, F_GETFL, 0);
    fcntl(socketId, F_SETFL, flags | O_NONBLOCK);
    return node(nodeIdx-1);
}

bool process_list_checkConnect(int nodex)
{
    return node(nodex).Connected;
}

void process_list_Disconnect(int nodex)
{
    if (node(nodex).Connected == TRUE)
    {
        connect(node(nodex).pre, node(nodex).next);
        node(nodex).Connected = FALSE;
        printf("Disconnect from IP %s, port %d\n", node(nodex).Ip, node(nodex).port);
        nodeCount --;
        printf("Remaining connection: %d\n", nodeCount);
    }
}

int process_list_readDataFromNode(int nodex, float *buff)
{
    float save = -999.99;
    int cnt = read( node(nodex).socketId, &save, sizeof(float));
    *buff = save;
    return cnt;
}

void process_list_WriteData(int nodex, float data)
{
    node(nodex).temp = data;
    node(nodex).newData = TRUE;
}

int process_list_ReadData(int nodex, float *data)
{
    if (node(nodex).newData == TRUE)
    {
        node(nodex).newData = FALSE;
        *data = node(nodex).temp;
        return 1;
    }
    return  0;
}

void process_list_closeAll(void)
{
    /* unmap shared mem */
    for (int i = 1; i < process_list_connectionIdx(); i ++)
    {
        printf("closing i = %d\n",i);
        process_list_Disconnect(i);
    }
    munmap(node(HEAD).self, MEM_SIZE);
    shm_unlink(FILE_SHARED_MEM_NAME);
    /* close server */
    close(serverSockSave);
    // printf("Cleared count\n");
    printf("\nServer is manually shutdown\n");
}
/*******************************************************************/