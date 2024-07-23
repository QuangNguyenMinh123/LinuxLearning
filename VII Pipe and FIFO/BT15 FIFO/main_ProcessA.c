#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
// #include "keyboard_input.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
/*******************************************************************/
#define TRUE            1
#define FALSE           0
#define FIFO_WRITE_FILE_PATH        "./AtoB"
#define FIFO_READ_FILE_PATH         "./BtoA"
#define BUFF_SIZE       1024
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
        printf("producer message: %s\n", buffReceive);
        close(fileReadDesc);
    }
}

void *writeThreadFunc()
{
    while (1)
    {
        if (keyboard_input_dataAvail())
        {
            printf("Message to A: %s", buffSend);
            fileWriteDesc = open(FIFO_WRITE_FILE_PATH, O_RDWR);
            write(fileWriteDesc, buffSend, strlen(buffSend) + 1);
            close(fileWriteDesc);
        }
    }
}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    char buffReceive[BUFF_SIZE];
    char buffSend[BUFF_SIZE];
    int fileDesc;
    mkfifo(FIFO_READ_FILE_PATH, 0666);
    //keyboard_input_init(buffSend);
    while (1)
    {
        fileDesc = open(FIFO_READ_FILE_PATH, O_RDWR);
        read(fileDesc, buffReceive, BUFF_SIZE);

        printf("producer message: %s\n", buffReceive);
        close(fileDesc);
    }
    //keyboard_input_deinit();
	return 0;
}