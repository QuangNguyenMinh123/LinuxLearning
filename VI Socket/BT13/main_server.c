#include <netinet/in.h>     /* structure for storing address information */ 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <sys/types.h>
#include <pthread.h>
/*******************************************************************/
char bufSend[100] = {0};
char bufReceive[100] = {0};
pthread_t threadSend, threadReceive;
int clientSocket;
/* string store data to send to client */
char serMsg[255] = "Hello Client, this is host\n";
/*******************************************************************/
void *serverSend(void *args)
{
    int i = 0;
    while (1)
    {
	    /* send's messages to client socket */
        sprintf(serMsg, "%d\n",i);
	    write(clientSocket, serMsg, sizeof(serMsg));
        i++;
        sleep(1);
    }
}

void *serverReceive(void *args)
{

}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    memset(serMsg, 0, sizeof(serMsg));
    /* create server socket similar to what was done in client program */
	int servSockD = socket(AF_INET, SOCK_STREAM, 0);
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
	listen(servSockD, 1);
	clientSocket = accept(servSockD, NULL, NULL);
    write(clientSocket, serMsg, sizeof(serMsg));
    if (pthread_create(&threadSend,NULL,serverSend,NULL))
    {
        printf("Server sending thread is created\n");
    }
    if (pthread_create(&threadReceive,NULL,serverReceive,NULL))
    {
        printf("Server receiving thread is created\n");
    }
    while (1);
	return 0;
}
