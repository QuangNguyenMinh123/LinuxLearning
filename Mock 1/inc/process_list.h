#ifndef _PROCESS_LIST_H
#define _PROCESS_LIST_H
#include <pthread.h>
/*******************************************************************/

/*******************************************************************/
typedef struct ProcessListType{
    int processId;
    char Ip[16];        /* 16 = INET_ADDRSTRLEN */
    int port;
    int socketId;
    int fileId;
    int shared_mem_count;
    struct ProcessListType *self;
}ProcessListType;
/*******************************************************************/
void process_list_init(int serverSock);
ProcessListType process_list_new(int processId, char Ip[], int port, int socketId);
void process_list_remove(int processId);
ProcessListType* process_list_find(int processId);
int process_list_connectionCount(void);
void process_list_closeAll(void);
void process_list_closeSharedMemX(int x);
/*******************************************************************/
#endif