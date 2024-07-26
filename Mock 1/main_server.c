#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include <pthread.h>
#include "log.h"
/*******************************************************************/

/*******************************************************************/
pthread_t thread;
int server;
float temp;
int run = 0;
int acc = 0;
int logfile;
char buffer[1024] = {0};
/*******************************************************************/
void *Receive(void *args)
{
    while (acc <= 0);
    while (1)
    {
        recv(acc, &temp, sizeof(temp), 0);
        if (temp > 0);
        {
            printf("Temp = %f\n",temp);
            sprintf(buffer, "Temp = %f\n",temp);
            log_write(logfile, buffer);
        }
        
    }
}
/*******************************************************************/
int main()
{
	// Two buffers for message communication
	char buffer1[256], buffer2[256];
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server < 0)
		printf("Error in server creating\n");
	else
		printf("Server Created\n");
	logfile = log_open();
	struct sockaddr_in my_addr, peer_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	
	// This ip address will change according to the machine
	my_addr.sin_addr.s_addr = inet_addr("192.168.0.106");
	
	my_addr.sin_port = htons(12000);

	if (bind(server, (struct sockaddr*) &my_addr, sizeof(my_addr)) == 0)
		printf("Binded Correctly\n");
	else
		printf("Unable to bind\n");
		
	if (listen(server, 3) == 0)
		printf("Listening ...\n");
	else
		printf("Unable to listen\n");
	
	socklen_t addr_size;
	addr_size = sizeof(struct sockaddr_in);
	
	// Ip character array will store the ip address of client
	char *ip;
	if (pthread_create(&thread,NULL,&Receive,NULL) == 0)
    {
        printf("Receive thread is created\n");
    }
	// while loop is iterated infinitely to 
	// accept infinite connection one by one
	while (1)
	{
		acc = accept(server, (struct sockaddr*) &peer_addr, &addr_size);
		printf("Connection Established\n");
		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN);
	
		// "ntohs(peer_addr.sin_port)" function is 
		// for finding port number of client
		printf("connection established with IP : %s and PORT : %d\n", 
											ip, ntohs(peer_addr.sin_port));

		//recv(acc, &temp, sizeof(temp), 0);
		//printf("Client : %f\n", temp);
		strcpy(buffer1, "Hello");
		send(acc, buffer1, 256, 0);
        run = 1;
	} 
	return 0;
}
