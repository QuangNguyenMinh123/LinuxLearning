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
void *clientSend()
{

}

void *clientReceive()
{

}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{ 
	int sockD = socket(AF_INET, SOCK_STREAM, 0);
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
	servAddr.sin_port = htons(9001);// use some unused port number 
	servAddr.sin_addr.s_addr = INADDR_ANY;s
	int connectStatus = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (connectStatus == -1) { 
		printf("Error...\n");
	}
	else {
        while (1)
        {
            char strData[255];
            recv(sockD, strData, sizeof(strData), 0);
            printf("Message: %s\n", strData);
        }
	}
	return 0;
}
