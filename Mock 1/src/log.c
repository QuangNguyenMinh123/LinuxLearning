#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "log.h"
/*******************************************************************/
#define TRUE                    1
#define FALSE                   0
#define LOG_FILE_PATH           "./gateway.log"
/*******************************************************************/
pthread_mutex_t mutexFileLog = PTHREAD_MUTEX_INITIALIZER;
/*******************************************************************/
int log_open(void)
{
    pthread_mutex_init(&mutexFileLog, NULL);
    return open(LOG_FILE_PATH, O_CREAT | O_RDWR | O_APPEND, 0777);
}

void log_write(int fileIndex, char* str)
{
    pthread_mutex_lock(&mutexFileLog);
    time_t now;
    time(&now);
    char buff[20];
    printf("%s",str);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    write(fileIndex, buff, strlen(buff));
    write(fileIndex, ": ", strlen(": "));
    write(fileIndex, str, strlen(str));
    pthread_mutex_unlock(&mutexFileLog);
}
void log_close(int fileIndex)
{
    pthread_mutex_destroy(&mutexFileLog);
    close(fileIndex);
}
/*******************************************************************/