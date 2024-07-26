#ifndef _THREAD_LIST_H
#define _THREAD_LIST_H
#include <pthread.h>
/*******************************************************************/
typedef struct ProcessListType{
    struct ProcessListType *pre;
    int processId;
    struct ProcessListType *next;
}ProcessListType;
/*******************************************************************/
void thread_list_init(void);
void thread_list_add(ProcessListType *process);
void thread_list_remove(int processId);
ProcessListType* thread_list_find(int processId);
/*******************************************************************/
#endif