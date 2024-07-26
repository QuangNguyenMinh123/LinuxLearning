#ifndef _THREAD_LIST_H
#define _THREAD_LIST_H
#include <pthread.h>
/*******************************************************************/
typedef struct ThreadListType{
    ThreadListType *pre;
    pthread_t thread;
    ThreadListType *next;
}ThreadListType;
/*******************************************************************/
void thread_list_init(void);
void thread_list_add(ThreadListType *thread);
void thread_list_remove(pthread_t thread);
ThreadListType* thread_list_find(pthread_t thread);
/*******************************************************************/
#endif