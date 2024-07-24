#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "keyboard_input.h"
#include <sys/types.h>
#include <pthread.h>
/*******************************************************************/
#define TRUE                        1
#define FALSE                       0
#define FIFO_WRITE_FILE_PATH        "./BtoA"
#define FIFO_READ_FILE_PATH         "./AtoB"
#define BUFF_SIZE                   1024
/*******************************************************************/
pthread_t readThread, writeThread;
char buffReceive[BUFF_SIZE];
char buffSend[BUFF_SIZE];
int fileWriteDesc, fileReadDesc;
/*******************************************************************/
void *readThreadFunc()
{
    while (1)
    {
        fileReadDesc = open(FIFO_READ_FILE_PATH, O_RDWR);
        read(fileReadDesc, buffReceive, BUFF_SIZE);
        printf("A message: %s\n", buffReceive);
        close(fileReadDesc);
    }
}

void *writeThreadFunc()
{
    while (1)
    {
        if (keyboard_input_dataAvail())
        {
            printf("Message to A: %s\n", buffSend);
            fileWriteDesc = open(FIFO_WRITE_FILE_PATH, O_RDWR);
            write(fileWriteDesc, buffSend, strlen(buffSend) + 1);
            close(fileWriteDesc);
        }
    }
}
/*******************************************************************/
int main(int argc, char const *argv[])
{
    memset(buffReceive, 0, sizeof(buffReceive));
    memset(buffSend, 0, sizeof(buffSend));
    keyboard_input_init(buffSend);
    mkfifo(FIFO_READ_FILE_PATH, 0666);
    mkfifo(FIFO_WRITE_FILE_PATH, 0666);
    if (pthread_create(&writeThread, NULL, writeThreadFunc, NULL) == 0)
        printf("writeThread is created\n");
    if (pthread_create(&readThread, NULL, readThreadFunc, NULL) == 0)
        printf("readThread is created\n");
    pthread_join(writeThread,NULL);
    pthread_join(readThread,NULL);
    keyboard_input_deinit();
    return 0;
}