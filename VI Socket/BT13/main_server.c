#include <netinet/in.h>     /* structure for storing address information */ 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <sys/types.h>
#include <pthread.h>
/*******************************************************************/
char bufSend[100] = {0};
char bufReceive[100] = {0};
pthread_t threadSend, threadReceive;
/*******************************************************************/
void *serverSend()
{

}

void *serverReceive()
{

}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    if (pthread_create(&threadSend,NULL,&serverSend,NULL))
    {
        printf("Server sending thread is created\n");
    }
    if (pthread_create(&threadReceive,NULL,&serverReceive,NULL))
    {
        printf("Server receiving thread is created\n");
    }
    /* create server socket similar to what was done in client program */
	int servSockD = socket(AF_INET, SOCK_STREAM, 0);

	/* string store data to send to client */
	char serMsg[255] = "Hello Client, this is host\n";

	/* define server address */
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	/* bind socket to the specified IP and port */
	bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
    printf("Port is bound\n");
	/* listen for connections */
	listen(servSockD, 1);
	int clientSocket = accept(servSockD, NULL, NULL);
    int i = 0;
    write(clientSocket, serMsg, sizeof(serMsg));
    while ( 1)
    {
	    /* send's messages to client socket */
        sprintf(serMsg, "%d\n",i);
	    write(clientSocket, serMsg, sizeof(serMsg));
        i++;
        sleep(1);
    }

	return 0;
}
