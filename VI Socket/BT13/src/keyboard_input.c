#include "keyboard_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
/*******************************************************************/
#define TRUE            1
#define FALSE           0
static pthread_t threadKeyboardInput;
static char* bufferStart = 0;
static volatile int dataAvail = FALSE;
/*******************************************************************/
void* keyboardHandler()
{
    char ch = 0;
    while (1)
    {
        scanf("%s", bufferStart);
        dataAvail = TRUE;
    }
}
/*******************************************************************/
void keyboard_input_init(char *str)
{
    /* str: string that saves input from keyboard, or buffer that used
    to send data to host/client 
    syntax: keyboard_input_init(sendbuff) */
    if (pthread_create(&threadKeyboardInput,NULL, &keyboardHandler,NULL) == 0)
    {
        printf("Server sending thread is created\n");
        bufferStart = str;
    }
    else
    {
        printf("Failed to creating keyboard handler thread\n");
    }
}

int keyboard_input_dataAvail(void)
{
    if (dataAvail == TRUE)
    {
        dataAvail = FALSE;
        return TRUE;
    }

}

void keyboard_input_deinit(void)
{
    pthread_cancel(threadKeyboardInput);
}
/*******************************************************************/