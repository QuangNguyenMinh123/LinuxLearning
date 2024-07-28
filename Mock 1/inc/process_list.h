#ifndef _PROCESS_LIST_H
#define _PROCESS_LIST_H
#include <pthread.h>
/*******************************************************************/

/*******************************************************************/
typedef struct intSharedMem{
    int value;
    int fileId;
    struct intSharedMem *self;
} intSharedMem;

typedef struct ProcessListType{
    int processId;
    char Ip[16];        /* 16 = INET_ADDRSTRLEN */
    int port;
    int socketId;
    int fileId;
    char fileName[20];
    int shared_mem_idx;
    struct ProcessListType *self;
}ProcessListType;
/*******************************************************************/
void process_list_init(int serverSock);
ProcessListType process_list_new(int processId, char Ip[], int port, int socketId);
ProcessListType* process_list_find(int processId);
int process_list_connectionCount(void);
void process_list_closeAll(void);
void process_list_closeSharedMemX(int x);
int process_list_findSharedmemByPid(int pid);
ProcessListType* open_shared_memProcess(char *File, int option);
intSharedMem* open_shared_memInt (char *File, int option);
void process_list_Disconnect(int x);
/*******************************************************************/
#endif