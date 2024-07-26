#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "log.h"
/*******************************************************************/
#define TRUE                    1
#define FALSE                   0
#define LOG_FILE_PATH           "./gateway.log"
/*******************************************************************/

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
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    write(fileIndex, buff, strlen(buff));
    write(fileIndex, ": ", strlen(": "));
    write(fileIndex, str, strlen(str));
}
void log_close(int fileIndex)
{
    close(fileIndex);
}
/*******************************************************************/