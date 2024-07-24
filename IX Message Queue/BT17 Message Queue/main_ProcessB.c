#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "keyboard_input.h"
#include <sys/types.h>
#include <pthread.h>
#include "types.h"
/* process B: read data from process A*/
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
void *checkThreadFunc()
{
    while (1)
    {
        fileReadDesc = open(FIFO_READ_FILE_PATH, O_RDWR);
        read(fileReadDesc, buffReceive, BUFF_SIZE);
        printf("A message: %s\n", buffReceive);
        close(fileReadDesc);
    }
}
/*******************************************************************/
int main(int argc, char const *argv[])
{
    memset(buffReceive, 0, sizeof(buffReceive));
    memset(buffSend, 0, sizeof(buffSend));
    keyboard_input_init(buffSend);
    if (pthread_create(&readThread, NULL, checkThreadFunc, NULL) == 0)
        printf("readThread is created\n");
    pthread_join(writeThread,NULL);
    pthread_join(readThread,NULL);
    keyboard_input_deinit();
    return 0;
}