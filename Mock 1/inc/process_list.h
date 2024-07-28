#ifndef _PROCESS_LIST_H
#define _PROCESS_LIST_H
#include <pthread.h>
/*******************************************************************/

/*******************************************************************/
typedef struct ProcessListType{
    struct ProcessListType *pre;
    int processId;
    char Ip[16];        /* 16 = INET_ADDRSTRLEN */
    int port;
    int socketId;
    int fileId;
    char fileName[15];
    struct ProcessListType *self;
    struct ProcessListType *next;

}ProcessListType;
/*******************************************************************/
void process_list_init(int serverSock);
void process_list_new(int processId, char Ip[], int port, int socketId);
void process_list_remove(int processId);
ProcessListType* process_list_find(int processId);
int process_list_connectionCount(void);
void process_list_closeAll(void);
/*******************************************************************/
#endif