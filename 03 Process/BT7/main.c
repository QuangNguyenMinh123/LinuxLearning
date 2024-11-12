#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
/*******************************************************************/
#define FORK_FAIL                       -1
#define IS_CHILD_PID                    0
#define NORMAL_TERMINATION              0
#define ABNORMAL_TERMINATION            !0
#define WAIT_FOR_CHILD_TO_TERMINATE     WEXITED
/*******************************************************************/
int childStatus;
int child_pid;
int processChildIsDone = 0;
/*******************************************************************/
void signalHanlder()
{
    int checkPID;
    printf("Signal handler: ");
    checkPID = wait(&childStatus);
    if (WIFEXITED(childStatus))
    {
        /* normal termination */
        printf("Child process is terminated normally\n");
    }
    else
    {
        /* abnormal termination*/
        printf("Child process is terminated abnormally\n");
    }
    processChildIsDone = 1;
}
/*******************************************************************/
int main(int argc, char *argv[])
{
    
    child_pid = fork();
    char keyInput;
    if (child_pid < FORK_FAIL)
    {
        printf("fork() failed");
    }
    else if (child_pid == IS_CHILD_PID)
    {
        /* Process B: child */
        printf("This is child PID. Child PID is %i\n", getpid());
        /* child process will wait for input from keyboard.
            If 't' or 'T' is pressed, abnormal termination will orcur.
            If others keyboard are pressed, child process will terminate normally */
        scanf("%c",&keyInput);
        if (keyInput == 't' || keyInput == 'T')
        {
            char ch[100] = {0};
            printf("%c is pressed. Abnormal termination\n",keyInput);
            snprintf(ch, sizeof(ch), "kill -9 %i",getpid());
            system(ch);
        }
        else
        {
            printf("%c is pressed. Normal termination\n",keyInput);
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        /* Process A: parents */
        
        pid_t ret;
        printf("This is parents PID. PID is %i\n\n\n", getpid());
        signal(SIGCHLD, signalHanlder);
        while (!processChildIsDone);
        return 0;

    }
}