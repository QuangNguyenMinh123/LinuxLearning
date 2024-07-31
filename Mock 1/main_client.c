#include <netinet/in.h>     /* structure for storing address information */ 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <sys/types.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>
/*******************************************************************/
#define TRUE            1
#define FALSE           0
#define SERVER_IP_ADDR  "192.168.0.106"     /* open cmd, type "hostname -I" to check ip address */
#define PORT             10000
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
    float increase = 1;
    float Temp = 0;
    float sum;
    while (disconnect == FALSE)
    {
        sum = Temp * 100 + increase;
        send(sockD, &sum, sizeof(sum), 0);
        printf("Client sends: %f\n",sum);
        increase ++;
        sleep(1);
    }
    printf("OUT1\n");
}
/*******************************************************************/
int main(int argc, char const* argv[]) 
{ 
	sockD = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servAddr;
    /********************/
    if (argc <= 1)
    {
        printf("argument: ./client <IP> <temperature>\n");
        //return -1;
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
    pthread_join(threadSend,NULL);
    printf("Disconnected\n");
	return 0;
}
