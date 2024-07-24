#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
/*******************************************************************/
typedef struct Human{
    char name[100];
    int nameLen;
    char birthday[20];
    int birthdayLen;
    char hometown[100];
    int hometownLen;
}Human;
typedef enum STEP{
    INPUTTING,
    SAVING,
    CHECKING
} STEP;
#define STRING_LEN          100
/*******************************************************************/
pthread_t thread1, thread2, thread3;
STEP ThreadStatus = INPUTTING;
Human Student, StudentBuff;
pthread_mutex_t mutexThreadStatus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexThreadStatus1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexThreadStatus2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condThreadStatus = PTHREAD_COND_INITIALIZER;
pthread_cond_t condThreadStatus1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t condThreadStatus2 = PTHREAD_COND_INITIALIZER;
int fileDes = 0;
/*******************************************************************/
static void allThreadCancel(void)
{
    pthread_exit(thread3);
    pthread_exit(thread2);
    pthread_exit(thread1);
}
int StrLen(char *str)
{
    int i = 0;
    while (*str != 0)
    {
        str++;
        i++;
    }
    return i;
}
static void getString(char *str)
{
    char ch = 0;
    while (1)
    {
        ch = getchar();
        if (ch == '\n')
        {
            *str = 0;
            return;
        }
        else
        {
            *str = ch;
            str++;
        }
    }
}
/*******************************************************************/
void *thread_Input(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&mutexThreadStatus);
        if (ThreadStatus == INPUTTING)
        {
            printf("/********New session********/\n");
            printf("Enter student infor:\n");
            /* Enter name */
            printf("Name: ");
            getString(Student.name);
            int i = strcmp(Student.name, "quit");
            if (strcmp(Student.name, "quit") == 0)
            {
                pthread_mutex_unlock(&mutexThreadStatus);
                return NULL;
            }
            Student.nameLen = StrLen(Student.name);
            /* Enter bd */
            printf("Birthday: ");
            getString(Student.birthday);
            if (strcmp(Student.birthday, "quit") == 0)
            {
                pthread_mutex_unlock(&mutexThreadStatus);
                return NULL;
            }
            Student.birthdayLen = StrLen(Student.birthday);
            /* Enter hometown */
            printf("Hometown: ");
            getString(Student.hometown);
            if (strcmp(Student.hometown, "quit") == 0)
            {
                pthread_mutex_unlock(&mutexThreadStatus);
                return NULL;
            }
            Student.hometownLen = StrLen(Student.hometown);
            ThreadStatus = SAVING;
            
            printf("Done inputting\n");
        }
        pthread_mutex_unlock(&mutexThreadStatus); 
        pthread_cond_signal(&condThreadStatus);
    }
}

void *thread_Saving(void *args)
{
    while(1)
    {
        pthread_mutex_lock(&mutexThreadStatus);
        pthread_cond_wait(&condThreadStatus, &mutexThreadStatus);
        if (ThreadStatus == SAVING)
        {
            write(fileDes, Student.name, Student.nameLen);
            write(fileDes, "\n", 1);
            write(fileDes, Student.birthday, Student.birthdayLen);
            write(fileDes, "\n", 1);
            write(fileDes, Student.hometown, Student.hometownLen);
            write(fileDes, "\n", 1);
            ThreadStatus = CHECKING;
        }
        else
        {
            pthread_mutex_unlock(&mutexThreadStatus);
            return NULL;
        }
        pthread_cond_signal(&condThreadStatus);
        pthread_mutex_unlock(&mutexThreadStatus);
        
        printf("Done saving\n");
    }
}
void *thread_Checking(void *args)
{
    while(1)
    {
        char buffer[200];
        char *ptr;
        pthread_mutex_lock(&mutexThreadStatus);
        pthread_cond_wait(&condThreadStatus, &mutexThreadStatus);
        if (ThreadStatus == CHECKING)
        {
            /* retrieve the last written data */
            lseek(fileDes, - (3 + Student.birthdayLen + 
                    Student.nameLen + Student.hometownLen), SEEK_END);
            read(fileDes, buffer, 3 + Student.birthdayLen + Student.nameLen
                             + Student.hometownLen);
            ptr = buffer;
            printf("/**/ Verifying ...\n");
            printf("Name: ");
            while (*(ptr) != '\n')
            {
                printf("%c", *ptr);
                ptr++;
            }
            printf("\n");ptr++;
            printf("Birth day: ");
            while (*(ptr) != '\n')
            {
                printf("%c", *ptr);
                ptr++;
            }
            printf("\n");ptr++;
            printf("HomeTown: ");
            while (*(ptr) != '\n')
            {
                printf("%c", *ptr);
                ptr++;
            }
            printf("\n");ptr++;
            ThreadStatus = INPUTTING;
            printf("Done in checking\n");
        }
        else
        {
            pthread_mutex_unlock(&mutexThreadStatus);
            return NULL;
        }
        pthread_cond_signal(&condThreadStatus);
        pthread_mutex_unlock(&mutexThreadStatus);
    }
}
/*******************************************************************/
int main(int argc, char *argv[])
{
    int ret = 0;
    char filePath[100] = "studentinfo.txt";
    pthread_mutex_init(&mutexThreadStatus, NULL);
    pthread_cond_init(&condThreadStatus, NULL);
    pthread_cond_init(&condThreadStatus1, NULL);
    pthread_cond_init(&condThreadStatus2, NULL);
    fileDes = open(filePath, O_APPEND | O_RDWR | O_SYNC, 0777);
    /* Creating thread */
    if (ret = pthread_create(&thread1,NULL,&thread_Input,NULL))
    {
        printf("Thread1 is created\n");
    }
    if (ret = pthread_create(&thread2,NULL,&thread_Saving,NULL))
    {
        printf("Thread2 is created\n");
    }
    if (ret = pthread_create(&thread3,NULL,&thread_Checking,NULL))
    {
        printf("Thread3 is created\n");
    }
    while (1);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
    close(fileDes);
    pthread_mutex_destroy(&mutexThreadStatus);
    pthread_cond_destroy(&condThreadStatus);
}