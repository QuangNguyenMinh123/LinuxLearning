#include <netinet/in.h>     /* structure for storing address information */ 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <sys/types.h>
#include <pthread.h>
#include "keyboard_input.h"
/*******************************************************************/
#define TRUE            1
#define FALSE           0
/*******************************************************************/
char bufReceive[255] = {0};
pthread_t threadSend, threadReceive;
int clientSocket;
int servSockD;
/* string store data to send to client */
char serMsg[255] = "Hello Client, this is host\n";
volatile int disconnect = TRUE;
/*******************************************************************/
void *serverSend(void *args)
{
    while (disconnect == FALSE)
    {
        if (keyboard_input_dataAvail())
        {
            write(clientSocket, serMsg, sizeof(serMsg));
            printf("Host sends: %s\n",serMsg);
        }
    }
}

void *serverReceive(void *args)
{
    int check = read(clientSocket, bufReceive, sizeof(bufReceive));
    while (check > 0)
    {
        check = read(clientSocket, bufReceive, sizeof(bufReceive));
        printf("Host receives: %s\n",bufReceive);
    }
    disconnect = TRUE;
}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    memset(serMsg, 0, sizeof(serMsg));
    /* create server socket similar to what was done in client program */
	servSockD = socket(AF_INET, SOCK_STREAM, 0);
	/* define server address */
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	/* bind socket to the specified IP and port */
	bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
    printf("Port is bound\n");
	/* listen for connections */
    printf("Waiting for connection...\n");
	listen(servSockD, 100);
	clientSocket = accept(servSockD, NULL, NULL);
    write(clientSocket, serMsg, sizeof(serMsg));
    disconnect = FALSE;
    printf("Connected\n");
    keyboard_input_init(serMsg);
    if (pthread_create(&threadSend,NULL,serverSend,NULL) == 0)
    {
        printf("Server sending thread is created\n");
    }
    if (pthread_create(&threadReceive,NULL,serverReceive,NULL) == 0)
    {
        printf("Server receiving thread is created\n");
    }
    pthread_join(threadReceive,NULL);
    pthread_join(threadSend,NULL);
    printf("Disconnected\n");
    keyboard_input_deinit();
	return 0;
}