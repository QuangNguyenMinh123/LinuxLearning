#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
/*******************************************************************/
int counter = 0;
pthread_t thread1, thread2;
typedef struct Human{
    char name[100];
    char birthday[10];
    char hometown[100];
}Human;
Human A = 
{
    .name       = "HumanName",
    .birthday   = "25/11/2000",
    .hometown   = "CNTT"
};
void *threadHandler(void *args)
{
    pthread_t threadId = pthread_self();
    if (pthread_equal(threadId, thread1))
    {
        /* If thread 1 is running */
        printf("Thread 1 is running\n");
        while (1)
        {
            counter++;
            if (counter%10000 == 0)
                printf("counter = %i\n",counter);
        }
    }
    else if (pthread_equal(threadId, thread2))
    {
        
        while (1)
        {
            // If thread 2 is running
            printf("Thread 2 is running\n");
            printf("Name: %s\n",A.name);
            printf("Birthday: %s\n",A.birthday);
            printf("Hometown: %s\n",A.hometown);
        }
    }
}
/*******************************************************************/
int main(int argc, char *argv[])
{
    int ret = 0;
    if (ret = pthread_create(&thread1,NULL,&threadHandler,NULL))
    {
        printf("Thread1 is created\n");
    }
    if (ret = pthread_create(&thread2,NULL,&threadHandler,NULL))
    {
        printf("Thread2 is created\n");
    }
    sleep(3);
    pthread_cancel(thread1);
    pthread_cancel(thread2);
    return 0;
}