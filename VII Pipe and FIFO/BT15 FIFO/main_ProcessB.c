#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include "keyboard_input.h"
/*******************************************************************/
#define TRUE 1
#define FALSE 0
#define FIFO_FILE_PATH "./myfifo"
#define BUFF_SIZE 1024
static pthread_t threadKeyboardInput;
static char* bufferStart = 0;
static char* bufferPtr = 0;
static volatile int dataAvail = FALSE;
/*******************************************************************/

/*******************************************************************/
int main(int argc, char const *argv[])
{
    char buffReceive[BUFF_SIZE];
    char buffSend[BUFF_SIZE];
    int fileDesc;
    memset(buffReceive, 0, sizeof(buffReceive));
    keyboard_input_init(buffSend);
    mkfifo(FIFO_FILE_PATH, 0666);
    while (1)
    {
        printf("Message to comsumer : ");
        while (!keyboard_input_dataAvail())
        {
        }
        fileDesc = open(FIFO_FILE_PATH, O_RDWR); 
        write(fileDesc, buffSend, strlen(buffSend) + 1);
        close(fileDesc);
    }
    keyboard_input_deinit();
    return 0;
}