#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
// #include "keyboard_input.h"
#include <sys/stat.h>
#include <sys/types.h>
/*******************************************************************/
#define TRUE            1
#define FALSE           0
#define FIFO_FILE_PATH   "./myfifo"
#define BUFF_SIZE       1024
/*******************************************************************/

/*******************************************************************/

/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    char buffReceive[BUFF_SIZE];
    char buffSend[BUFF_SIZE];
    int fileDesc;
    mkfifo(FIFO_FILE_PATH, 0666);
    //keyboard_input_init(buffSend);
    while (1)
    {
        fileDesc = open(FIFO_FILE_PATH, O_RDWR);
        read(fileDesc, buffReceive, BUFF_SIZE);

        printf("producer message: %s\n", buffReceive);
        close(fileDesc);
    }
    //keyboard_input_deinit();
	return 0;
}