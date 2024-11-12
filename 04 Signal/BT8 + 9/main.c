#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
/************************* DEFINE **************************************/
#define IS_CHILD_PID            0
/************************ GLOBAL VARIABLE ******************************/
void signalHandler_INT()
{
    printf("This is Ctrl + C\n");
}
void signalHandler_QUIT()
{
    printf("This is Ctrl + \\\n");
}
void signalHandler_TSTP(int i)
{
    printf("This is Ctrl + Z\n");
}
void signalHandler_CHLD()
{
    printf("Child is finish\n");
}
void signalHandler_USRx(int sigNumx)
{
    if (sigNumx == SIGUSR1)         /* 10 */
    {
        printf("Child is USR1\n");
    }
    else if (sigNumx == SIGUSR2)    /* 12 */
    {
        printf("Child is USR2\n");
    }
}
/*******************************************************************/
int main(int argc, char *argv[])
{
    int childPid = fork();
    if (childPid == IS_CHILD_PID)
    {
        /* Is child */
        printf("PID child: %i\n",getpid());
    }
    else
    {
        struct sigaction sa;
        sa.sa_handler = signalHandler_TSTP;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIGTSTP, &sa, NULL) == -1 ) {
            perror("Couldn't set SIGTSTP handler");
            exit(EXIT_FAILURE);
        }
        /* Is parents */
        char ch[10];
        sigset_t signalSet, signalOldSet;
        sigemptyset(&signalSet);
        sigemptyset(&signalOldSet);
        //sigaddset(&signalSet, SIGINT);    /* this line block SIGINT*/
        //sigaddset(&signalSet, SIGUSR1);   /* this line block SIGUSR1*/
        //sigaddset(&signalSet, SIGUSR2);   /* this line block SIGUSR2*/
        printf("PID parents %i\n",getpid());
        signal(SIGINT, signalHandler_INT);      /* Ctrl + C *//* Enable INT signal handler */
        signal(SIGCHLD, signalHandler_CHLD);    /* Enable CHLD signal handler */
        signal(SIGUSR1, signalHandler_USRx);
        signal(SIGUSR2, signalHandler_USRx);
        signal(SIGQUIT, signalHandler_QUIT);    /* Ctrl + \ */
        signal(SIGTSTP, signalHandler_TSTP);    /* Ctrl + \ */
        if (sigprocmask(SIG_SETMASK, &signalSet,&signalOldSet) == 0)
        {
            if (sigismember(&signalSet, SIGUSR1) == 1)
            {
                printf("SIGUSR1 is existed\n");
            }
            else if (sigismember(&signalSet, SIGUSR1) == 0)
            {
                printf("SIGUSR1 is not existed\n");
            }
            
        }
        while (!(ch[0] == 'q' || ch[0] == 'Q'))
        {
            scanf("%s",ch);
        }
        printf("123\n");
    }
    return 0;
}