#include <netinet/in.h>     /* structure for storing address information */ 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <sys/types.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>
#include "random.h"
/*******************************************************************/
#define TRUE            1
#define FALSE           0
#define SERVER_IP_ADDR  "192.168.0.106"     /* open cmd, type "hostname -I" to check ip address */
#define PORT             2000
/*******************************************************************/
char bufSend[255] = {0};
char bufReceive[255] = {0};
pthread_t threadSend, threadReceive;
int sockD;
volatile int disconnect = TRUE;
int temperature;
/*******************************************************************/
void signalHandler_INT()
{
    printf("This is Ctrl + C\n");
    disconnect = TRUE;
    close(sockD);
}

void *clientSend(void *args)
{
    float Temp = 0;
    while (disconnect == FALSE)
    {
        Temp = randomFloat(temperature, temperature +1);
        send(sockD, &Temp, sizeof(Temp), 0);
        printf("Client sends: %f\n",Temp);
        sleep(1);
    }
    printf("OUT1\n");
}
// void *clientReceive(void *args)
// {
//     int check = read(sockD, bufReceive, sizeof(bufReceive));
//     printf("OUT2\n");
//     while (check >= 0 && disconnect == FALSE)
//     {
//         check = read(sockD, bufReceive, sizeof(bufReceive));
//     }
//     printf("OUT3\n");
//     disconnect = TRUE;
// }
/*******************************************************************/
int main(int argc, char const* argv[]) 
{ 
	sockD = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servAddr;
    /********************/
    if (argc <= 1)
    {
        printf("argument: ./client <IP> <temperature>\n");
        return -1;
    }
    else
    {
        printf("IP: %s\n", argv[1]);
        printf("Temperature: %s\n", argv[2]);
    }
    signal(SIGINT, signalHandler_INT);
    temperature = atoi(argv[2]);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(10000);        /* port number */
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	int connectStatus = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (connectStatus == -1) { 
		printf("Error...\n");
        return -1;
	}
    else
    {
        printf("Connected to server\n");
    }
    printf("Port: %d\n",ntohs(servAddr.sin_port));
    disconnect = FALSE;
    if (pthread_create(&threadSend,NULL,&clientSend,NULL) == 0)
    {
        printf("Client sending thread is created\n");
    }
	// if (pthread_create(&threadReceive,NULL,&clientReceive,NULL) == 0)
    // {
    //     printf("Client receiving thread is created\n");
    // }
    pthread_join(threadSend,NULL);
    // pthread_join(threadReceive,NULL);
    printf("Disconnected\n");
	return 0;
}
