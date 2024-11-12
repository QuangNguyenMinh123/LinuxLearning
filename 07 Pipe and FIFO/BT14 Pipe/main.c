#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
/*******************************************************************/
#define TRUE            1
#define FALSE           0
#define MSG_SIZE        255
#define READ_PIPE_IDX   0
#define WRITE_PIPE_IDX  1
/*******************************************************************/
char buffChild[MSG_SIZE] = {0};
char buffParent[MSG_SIZE] = {0};
volatile int threadChildWriteFinish = FALSE;
volatile int threadParentWriteFinish = FALSE;
pthread_t childWriteThread, childReadThread, parentWriteThread, parentReadThread;
int chToPa[2], paToCh[2];
pthread_mutex_t mutexParent, mutexChild;
/*******************************************************************/
void *childWriteThreadFunc()
{
    /* Send 1 3 5 7 9, delay(1) */
    char ch = 'a';
    int stop = 0;
    while (ch < 'f' + 1)
    {
        pthread_mutex_lock(&mutexChild);
        write(chToPa[WRITE_PIPE_IDX], &ch, 1);
        pthread_mutex_unlock(&mutexChild);
        printf("Child sends: %c\n",ch);
        ch += 1;
        sleep(2);
    }
    close(chToPa[1]);
    threadChildWriteFinish = TRUE;
}

void *childReadThreadFunc()
{
    int numread;
    while (threadParentWriteFinish == FALSE)
    {
        pthread_mutex_lock(&mutexParent);
        numread = read(paToCh[READ_PIPE_IDX], buffChild, MSG_SIZE);
        pthread_mutex_unlock(&mutexParent);
        if (numread == -1)
        {
            printf("childread() failed, break\n");
            break;
        }
        else if (numread == 0)
        {
            printf("Child read end of file, break\n");
            break;
        }
        else
            printf("Child read: %c\n",buffChild[0]);
        
    }
}

void *parentWriteThreadFunc()
{
    /* Send 0 2 4 6 8 , delay (1)*/
    char i = '1';
    while (i < '9' + 1)
    {
        pthread_mutex_lock(&mutexParent);
        write(paToCh[WRITE_PIPE_IDX], &i, 1);
        pthread_mutex_unlock(&mutexParent);
        printf("Parent sends: %c\n",i);
        i += 2;
        sleep(1);
    }
    close(paToCh[1]);
    threadParentWriteFinish = TRUE;
}

void *parentReadThreadFunc()
{
    int numread;
    while (threadChildWriteFinish == FALSE)
    {
        pthread_mutex_lock(&mutexChild);
        numread = read(chToPa[READ_PIPE_IDX], buffParent, MSG_SIZE);
        pthread_mutex_unlock(&mutexChild);
        if (numread == -1)
        {
            printf("parentread() failed, break\n");
            break;
        }
        else if (numread == 0)
        {
            printf("Parent read end of file, break\n");
            break;
        }
        else
            printf("Parent read: %c\n",buffParent[0]);
    }
}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    memset(buffChild, 0, sizeof(buffChild));
    memset(buffParent, 0, sizeof(buffParent));
    pid_t childPid;
    pthread_mutex_init(&mutexChild, NULL);
    pthread_mutex_init(&mutexParent, NULL);
    if (pipe(chToPa) < 0)
    {
        printf("chToPa pipe is create unsuccessfully\n");
        return -1;
    }
    if (pipe(paToCh) < 0)
    {
        printf("paToCh pipe is create unsuccessfully\n");
        return -1;
    }

    childPid = fork();
    if (childPid == 0)
    {
        /* This is child function */
        if (close(chToPa[0]) == -1)
            printf("close(chToPa[0]) failed\n");
        if (close(paToCh[1]) == -1)
            printf("close(paToCh[1]) failed\n");
        if (pthread_create(&childReadThread, NULL, childReadThreadFunc, NULL) == 0)
            printf("childReadThread is created\n");
        if (pthread_create(&childWriteThread, NULL, childWriteThreadFunc, NULL) == 0)
            printf("childWriteThread is created\n");
        pthread_join(childWriteThread,NULL);
        pthread_join(childReadThread,NULL);
        close(paToCh[0]);
    }
    else
    {
        /* This is parent function */
        if (close(chToPa[1]) == -1)
            printf("close(chToPa[1]) failed\n");
        if (close(paToCh[0]) == -1)
            printf("close(paToCh[0]) failed\n");
        if (pthread_create(&parentReadThread, NULL, parentReadThreadFunc, NULL) == 0)
            printf("parentReadThread is created\n");
        if (pthread_create(&parentWriteThread, NULL, parentWriteThreadFunc, NULL) == 0)
            printf("parentWriteThread is created\n");
        pthread_join(parentWriteThread,NULL);
        pthread_join(parentReadThread,NULL);
        close(chToPa[0]);
    }
    pthread_mutex_destroy(&mutexChild);
    pthread_mutex_destroy(&mutexParent);
	return 0;
}