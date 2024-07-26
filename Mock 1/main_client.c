#include <netinet/in.h>     /* structure for storing address information */ 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <sys/types.h>
#include <pthread.h>
#include "keyboard_input.h"
#include <arpa/inet.h>
#include "random.h"
/*******************************************************************/
#define TRUE            1
#define FALSE           0
#define SERVER_IP_ADDR  "192.168.0.106"     /* open cmd, type "hostname -I" to check ip address */
#define PORT             2000
/*******************************************************************/
char bufSend[255] = {0};
char bufReceive[255] = {0};
pthread_t threadSend;
int sockD;
volatile int disconnect = TRUE;
int temperature;
/*******************************************************************/
void *clientSend(void *args)
{
    float Temp = 0;
    while (disconnect == FALSE)
    {
        Temp = randomFloat(temperature, temperature +1);
        send(sockD, &Temp, sizeof(Temp), 0);
        printf("Client sends: %f\n",Temp);
        sleep(2);
    }
}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{ 
	sockD = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servAddr;
    /********************/
    if (argc <= 1)
    {
        printf("argument: ./client <IP> <port> <temperature>\n");
        return -1;
    }
    else
    {
        printf("IP: %s\n", argv[1]);
        printf("Port: %s\n", argv[2]);
        printf("Temperature: %s\n", argv[3]);
    }
    temperature = atoi(argv[3]);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[2]));        /* port number */
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
    keyboard_input_init(bufSend);
    disconnect = FALSE;
    if (pthread_create(&threadSend,NULL,&clientSend,NULL) == 0)
    {
        printf("Client sending thread is created\n");
    }
    pthread_join(threadSend,NULL);
    printf("Disconnected\n");
    keyboard_input_deinit();
	return 0;
}
