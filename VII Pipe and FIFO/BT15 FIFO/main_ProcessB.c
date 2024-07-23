#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "keyboard_input.h"
/*******************************************************************/
#define TRUE            1
#define FALSE           0
/*******************************************************************/
char bufReceive[255] = {0};
int clientSocket;
int servSockD;
/* string store data to send to client */
char serMsg[255] = "Hello Client, this is host\n";
volatile int disconnect = TRUE;
/*******************************************************************/

/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    memset(serMsg, 0, sizeof(serMsg));
    keyboard_input_init(serMsg);
    keyboard_input_deinit();
	return 0;
}