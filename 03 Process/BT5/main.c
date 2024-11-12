#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
/*******************************************************************/
#define IS_CHILD_PID        0
/*******************************************************************/
int main(int argc, char *argv[])
{
    int child_pid;
    child_pid = fork();
    if (child_pid == IS_CHILD_PID)
    {
        printf("This is child PID. Child PID is %i\n", getpid());
        printf("My parents PID is %i\n\n\n", getppid());
    }
    else
    {
        printf("This is parents PID. PID is %i\n\n\n", getpid());
    }
    return 0;
}