#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>     /* for socket APIs */ 
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>
/*******************************************************************/
#define TRUE                1
#define FALSE               0
#define SERVER_IP_ADDR      "192.168.0.106"     /* open cmd, type "hostname -I" to check ip address */
#define PORT                10000
/*******************************************************************/
int sockD;
volatile int disconnect = TRUE;
int temperature;
/*******************************************************************/
void signalHandler_INT()
{
    printf("This is Ctrl + C\n");
    disconnect = TRUE;
}

void signalHandler_PIPE()
{
    printf("Broken pipe\n");
    disconnect = TRUE;
}

/*******************************************************************/
int main(int argc, char const* argv[]) 
{
    struct sockaddr_in servAddr;
	sockD = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if (sockD <= 0)
    {
        printf("Error in creating server socket\n");
        return -1;
    }

    // if (setsockopt (sockD, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    //     printf("setsockopt failed\n");
    // if (setsockopt (sockD, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0)
    //     printf("setsockopt failed\n");
	
    signal(SIGINT, signalHandler_INT);
    sigaction(SIGPIPE, &(struct sigaction){signalHandler_PIPE}, NULL);
    printf("Mock 1\n");
    /********************/
    if (argc <= 1)
    {
        printf("argument: ./client <IP> <temperature>\n");
        temperature = 2;
        servAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    }
    else
    {
        printf("IP: \"%s\"\n", argv[1]);
        printf("Temperature: \"%s\"\n", argv[2]);
        temperature = atoi(argv[2]);
        servAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    }
    
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);        /* port number */
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
    float increase = 0;
    float sum;
    while (disconnect == FALSE)
    {
        sum = temperature * 100 + increase;
        write(sockD, &sum, sizeof(sum));
        printf("Client sends: %f\n",sum);
        increase ++;
        if (increase == 100)
            break;
        sleep(1);
    }
    close(sockD);
    printf("Disconnected\n");
	return 0;
}
