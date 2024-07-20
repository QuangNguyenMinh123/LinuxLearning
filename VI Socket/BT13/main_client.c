#include <netinet/in.h>     /* structure for storing address information */ 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <sys/types.h>
#include <pthread.h>
#include "keyboard_input.h"
/*******************************************************************/

/*******************************************************************/
char bufSend[255] = {0};
char bufReceive[255] = {0};
pthread_t threadSend, threadReceive;
int sockD;
/*******************************************************************/
void *clientSend(void *args)
{
    while (1)
    {
        if (keyboard_input_dataAvail())
        {
            write(sockD, bufSend, sizeof(bufSend));
            printf("Client sends: %s\n",bufSend);
        }
    }
}

void *clientReceive(void *args)
{
    int check = read(sockD, bufReceive, sizeof(bufReceive));
    while (check > 0)
    {
        check = read(sockD, bufReceive, sizeof(bufReceive));
        printf("Client receives: %s\n",bufReceive);
    }
}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{ 
	sockD = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001);        /* port number */ 
	servAddr.sin_addr.s_addr = INADDR_ANY;
	int connectStatus = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (connectStatus == -1) { 
		printf("Error...\n");
        return -1;
	}
    else
    {
        printf("Connected\n");
    }
    keyboard_input_init(bufSend);
    if (pthread_create(&threadSend,NULL,&clientSend,NULL) == 0)
    {
        printf("Client sending thread is created\n");
    }
    if (pthread_create(&threadReceive,NULL,&clientReceive,NULL) == 0)
    {
        printf("Client receiving thread is created\n");
    }
    pthread_join(threadReceive,NULL);
    pthread_join(threadSend,NULL);
    printf("Disconnected\n");
    keyboard_input_deinit();
	return 0;
}
