#ifndef _PROCESS_LIST_H
#define _PROCESS_LIST_H
#include <pthread.h>
/*******************************************************************/
typedef struct ProcessListType{
    struct ProcessListType *pre;
    int processId;
    char Ip[20];
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