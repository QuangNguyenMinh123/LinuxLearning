#include <netinet/in.h>     /* structure for storing address information */ 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <sys/types.h>
#include <pthread.h>
/*******************************************************************/
char bufSend[100] = {0};
char bufReceive[255] = {0};
pthread_t threadSend, threadReceive;
int sockD;
/*******************************************************************/
void *clientSend()
{

}

void *clientReceive()
{
    while (1)
    {
        recv(sockD, bufReceive, sizeof(bufReceive), 0);
        printf("Message: %s", bufReceive);
    }
}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{ 
	sockD = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servAddr;
    if (pthread_create(&threadSend,NULL,&clientSend,NULL))
    {
        printf("client sending thread is created\n");
    }
    if (pthread_create(&threadReceive,NULL,&clientReceive,NULL))
    {
        printf("client receiving thread is created\n");
    }
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(9001);        /* use some unused port number */ 
	servAddr.sin_addr.s_addr = INADDR_ANY;
	int connectStatus = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (connectStatus == -1) { 
		printf("Error...\n");
	}
    int err = 0;
    socklen_t size = sizeof (err);

    while(1);
	return 0;
}
