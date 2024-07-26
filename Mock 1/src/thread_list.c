#include <stdio.h>
#include <stdlib.h>
#include "thread_list.h"
/*******************************************************************/

/*******************************************************************/
ThreadListType head, tail;
/*******************************************************************/
void thread_list_init(void)
{
    head.next = &tail;
    tail.pre = &head;
    tail.next = NULL;
    head.pre = NULL;
}
void thread_list_add(ThreadListType *thread)
{
    if (head.next == &tail)
    {
        head.next = thread;
        thread->pre = &head;
        thread->next = &tail;
        tail.pre = thread;
    }
    else
    {
        (tail.pre)->next = thread;
        thread->pre = tail.pre;
        thread->next = &tail;
        tail.pre = thread;
    }
}
void thread_list_remove(pthread_t thread)
{
    ThreadListType *ptr = head.next;
    /* Linked list is empty*/
    if (ptr == &tail)
        return;
    /* else */
    while (ptr != &tail)
    {
        if (pthread_equal(ptr->thread, thread))
        {
            ptr->pre->next = ptr ->next;
            ptr->next->pre = ptr->pre;
            return;
        }
        else
            ptr = ptr->next;
    }
}
ThreadListType* thread_list_find(pthread_t thread)
{
    ThreadListType *ptr = head.next;
    /* Linked list is empty*/
    if (ptr == &tail)
        return NULL;
    while (ptr != &tail)
    {
        if (pthread_equal(ptr->thread, thread))
        {
            return ptr;
        }
        else
            ptr = ptr->next;
    }
    return NULL;
}
/*******************************************************************/