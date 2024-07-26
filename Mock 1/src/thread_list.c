#include <stdio.h>
#include <stdlib.h>
#include "thread_list.h"
/*******************************************************************/

/*******************************************************************/
ProcessListType head, tail;
/*******************************************************************/
void thread_list_init(void)
{
    head.next = &tail;
    tail.pre = &head;
    tail.next = NULL;
    head.pre = NULL;
}
void thread_list_add(ProcessListType *process)
{
    if (head.next == &tail)
    {
        head.next = process;
        process->pre = &head;
        process->next = &tail;
        tail.pre = process;
    }
    else
    {
        (tail.pre)->next = process;
        process->pre = tail.pre;
        process->next = &tail;
        tail.pre = process;
    }
}
void thread_list_remove(int processId)
{
    ProcessListType *ptr = head.next;
    /* Linked list is empty*/
    if (ptr == &tail)
        return;
    /* else */
    while (ptr != &tail)
    {
        if (ptr->processId, processId)
        {
            ptr->pre->next = ptr ->next;
            ptr->next->pre = ptr->pre;
            return;
        }
        else
            ptr = ptr->next;
    }
}
ProcessListType* thread_list_find(int processId)
{
    ProcessListType *ptr = head.next;
    /* Linked list is empty*/
    if (ptr == &tail)
        return NULL;
    while (ptr != &tail)
    {
        if (ptr->processId, processId)
        {
            return ptr;
        }
        else
            ptr = ptr->next;
    }
    return NULL;
}
/*******************************************************************/