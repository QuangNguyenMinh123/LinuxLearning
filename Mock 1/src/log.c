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
/*******************************************************************/
int log_open(void)
{
    return open(LOG_FILE_PATH, O_CREAT | O_RDWR | O_APPEND, 0777);
}

void log_write(int fileIndex, char* str)
{
    time_t now;
    time(&now);
    char buff[20];
    //printf("%s",str);
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    write(fileIndex, buff, strlen(buff));
    write(fileIndex, ": ", strlen(": "));
    write(fileIndex, str, strlen(str));
}

void log_data(int fileIndex, int nodeIdx)
{
    char buffer[100] = {0};
    float temp = process_list_node(nodeIdx)->temp; 
    int int1 = temp;
    float tempFrac = temp - int1;
    int int2 = trunc(tempFrac * 10000);
    sprintf(buffer,"IP: %s, port: %d, temp: %d.%04d\n", process_list_node(nodeIdx)->Ip,
                             process_list_node(nodeIdx)->port, int1, int2);
    log_write(fileIndex, buffer);
}

void log_close(int fileIndex)
{
    close(fileIndex);
}
/*******************************************************************/