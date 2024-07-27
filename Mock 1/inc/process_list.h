#ifndef _PROCESS_LIST_H
#define _PROCESS_LIST_H
#include <pthread.h>
/*******************************************************************/
#define CLIENT_MAX              100
#define SHARED_MEM_SIZE         (CLIENT_MAX * sizeof(ProcessListType))
#define SERVER_PROCESS_FILE     "SERVER_PROCESS"
/*******************************************************************/
typedef struct ProcessListType{
    struct ProcessListType *pre;
    int processId;
    char Ip[16];        /* 16 = INET_ADDRSTRLEN */
    int port;
    int socketId;
    struct ProcessListType *next;
}ProcessListType;
/*******************************************************************/
void process_list_init(void);
void process_list_add(ProcessListType *process);
void process_list_remove(int processId);
ProcessListType* process_list_find(int processId);
/*******************************************************************/
#endif