#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include "log.h"
#include "process_list.h"
/*******************************************************************/
#define TRUE                    1
#define FALSE                   0
#define LOG_FILE_PATH           "./gateway.log"
/*******************************************************************/
pthread_mutex_t mutexFileLog = PTHREAD_MUTEX_INITIALIZER;
int lineIndex;
/*******************************************************************/
int log_open(void)
{
    lineIndex = 1;
    return open(LOG_FILE_PATH, O_CREAT | O_RDWR | O_APPEND, 0777);
}

void log_write(int fileIndex, char* str)
{
    time_t now;
    time(&now);
    char buff[20];
    char lineBuff[15];
    sprintf(lineBuff, "%d\t\t",lineIndex);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    write(fileIndex, lineBuff, strlen(lineBuff));
    write(fileIndex, buff, strlen(buff));
    write(fileIndex, ": ", strlen(": "));
    write(fileIndex, str, strlen(str));
    lineIndex++;
}

void log_close(int fileIndex)
{
    close(fileIndex);
}
/*******************************************************************/