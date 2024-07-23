#include "keyboard_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
/*******************************************************************/
#define TRUE            1
#define FALSE           0
static pthread_t threadKeyboardInput;
static char* bufferStart = 0;
static char* bufferPtr = 0;
static volatile int dataAvail = FALSE;
/*******************************************************************/
void* keyboardHandler()
{
    char ch = 0;
    while (1)
    {
        ch = getchar();     /* scanf() ignores space so it shall not be used */
        if (ch == '\n')
        {
            *bufferPtr = 0;
            dataAvail = TRUE;
            bufferPtr = bufferStart;
        }
        else
        {
            *bufferPtr = ch;
            bufferPtr++;
            dataAvail = FALSE;
        }
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
        printf("Keyboard handler thread is created\n");
        bufferStart = str;
        bufferPtr = str;
        memset(str, 0, sizeof(str));
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