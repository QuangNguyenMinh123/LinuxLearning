#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include "menu.h"
#include "keyboard_input.h"
#include "types.h"
/***************************************************************** */
#define TRUE                        1
#define FALSE                       0
#define BUFF_SIZE                   1024
/* Process A: input info from keyboard*/
/*******************************************************************/
char buffReceive[BUFF_SIZE];
char buffSend[BUFF_SIZE];
messageType mess;
/*******************************************************************/
int checkID(char *str)
{
    while (*str != 0)
    {
        if (*str < '0' || *str > '9')
            return FALSE;
        else
            str++;
    }
    return TRUE;
}
void strCopy(char *des, char *src)
{
    while (*src != 0)
    {
        *des = *src;
        des++;
        src++;
    }
}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    memset(buffReceive, 0, sizeof(buffReceive));
    memset(buffSend, 0, sizeof(buffSend));
    keyboard_input_init(buffSend);
    while (1)
    {
        menu_printBar();
        printf("Nhap thong tin sinh vien:\n");
        /* ID */
        printf("ID: ");
        while (!keyboard_input_dataAvail());
        mess.ID = strtol(buffSend,NULL,10);
        /* Name */
        printf("Name: ");
        fflush(stdout);
        while (!keyboard_input_dataAvail());
        strCopy(mess.student.name, buffSend);
        /* Age */
        printf("Age: ");
        fflush(stdout);
        while (!keyboard_input_dataAvail());
        mess.student.age = strtol(buffSend,NULL,10);
        /* Hometown */
        printf("Hometown: ");
        fflush(stdout);
        while (!keyboard_input_dataAvail());
        strCopy(mess.student.name, buffSend);
        /* PhoneNo */
        printf("Phone number: ");
        fflush(stdout);
        while (!keyboard_input_dataAvail());
        strCopy(mess.student.name, buffSend);

    }
    keyboard_input_deinit();
	return 0;
}