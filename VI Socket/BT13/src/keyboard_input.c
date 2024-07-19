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
static char *bufferPtr = 0;
static int dataAvail = FALSE;
/*******************************************************************/
static void* keyboardHandler()
{
    char ch = 0;
    while (1)
    {
        ch = getchar();
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
    if (pthread_create(&threadKeyboardInput,NULL, &keyboardHandler,NULL) == 0)
    {
        printf("Server sending thread is created\n");
        bufferStart = str;
        bufferPtr = str;
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
    else
    {
        dataAvail = TRUE;
        return FALSE;
    }

}

void keyboard_input_deinit(void)
{
    pthread_cancel(threadKeyboardInput);
}
/*******************************************************************/