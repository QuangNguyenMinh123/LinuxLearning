#ifndef _PROCESS_LIST_H
#define _PROCESS_LIST_H
#include <pthread.h>
/*******************************************************************/
#define bool                            char
#define TRUE                            1
#define FALSE                           0
/*******************************************************************/
typedef struct intSharedMem{
    int value;
    int fileId;
    struct intSharedMem *self;
} intSharedMem;

typedef struct ProcessListType{
    int pre;
    int processId;
    char Ip[16];        /* 16 = INET_ADDRSTRLEN */
    int port;
    int socketId;
    int fileId;
    int Idx;
    bool newData;
    float temp;
    int next;
}ProcessListType;
/*******************************************************************/
void process_list_init(int serverSock);
ProcessListType process_list_new(int processId, char* Ip, int port, int socketId);
ProcessListType* process_list_find(int processId);
int process_list_connectionCount(void);
void process_list_closeAll(void);
int process_list_findIdxByPid(int pid);
ProcessListType* open_shared_memProcess(void);
intSharedMem* open_shared_memInt (char *File, int option);
void process_list_Disconnect(int x);
float process_list_readDataFromNode(int nodex);
/*******************************************************************/
#endif