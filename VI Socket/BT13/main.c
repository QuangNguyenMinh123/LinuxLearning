#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "chat.h"
/*******************************************************************/
#define TRUE                1
#define FALSE               0
#define IS_CLIENT           TRUE
#define LISTEN_BACKLOG      50
#define BUFF_SIZE           256
#define handle_error(msg)   \
    do {perror(msg); exit (EXIT_FAILURE);} while (0)
/*******************************************************************/

/*******************************************************************/

/*******************************************************************/

/*******************************************************************/
int main(int argc, char *argv[])
{
    int portNo, len, opt;
    int socketDesc;
    struct sockaddr_in serverAdr, clientAdr;
    memset(&serverAdr, 0, sizeof(struct sockaddr_in));
    memset(&clientAdr, 0, sizeof(struct sockaddr_in));
    /* error: address is already in use */
    // if (setsockopt(socketDesc, SOL_SOCKET | SO_REUSEADDR, &opt, sizeof(opt) ))
    //     handle_error("setsockopt()");
#if (IS_CLIENT == TRUE)
    /* If device is client */
    if (argc < 3)
    {
        printf("Command : ./client <server address> <port number>\n");
        exit(EXIT_FAILURE);
    }
    else
        portNo = atoi(argv[2]);
    serverAdr.sin_family = AF_INET;
    serverAdr.sin_port = htons(portNo);
    serverAdr.sin_addr.s_addr = INADDR_ANY;
    if (inet_pton(AF_INET, argv[1],&serverAdr) == -1)
        handle_error("inet_pton()");
    /* Creating socket */
    socketDesc = socket(AF_INET, SOCK_STREAM, 0); /* Domain, type, protocol*/
    if (socketDesc == -1)
        printf("Creating socket failed\n");
    /* Connecting to server */
    if (connect(socketDesc, (struct sockaddr*)&serverAdr, sizeof(serverAdr)) != 0)
        handle_error("connect()");
    chat(socketDesc);
#else
    int len;
    int newSocketDesc;
    if (argc < 2)
    {
        printf("No port provided\nCommand: ./server <port_number>\n");
        exit(EXIT_FAILURE);
    }
    else
        portNo = atoi(argv[1]);
    /* Creating socket */
    socketDesc = socket(AF_INET, SOCK_STREAM, 0); /* Domain, type, protocol*/
    if (socketDesc == -1)
        printf("Creating socket failed\n");
    /* If device is host */
    /* Initialize server address */
    serverAdr.sin_family = AF_INET;
    serverAdr.sin_port = hton(portNo);
    serverAdr.sin_addr.s_addr = INADDR_ANY;
    /* Attach socket to server address */
    if (bind(socketDesc, (struct sockaddr*)&serverAdr, sizeof(serverAdr)) == -1)
        handle_error("bind()");
    /* Listen to upto 50 connection in queue */
    if (listen(socketDesc, LISTEN_BACKLOG) == -1)
        handle_error("listen()");
    /* obtain client data*/
    len = sizeof(clientAdr);
    while (1)
    {
        printf("server is listening at port: %d\n",portNo);
        newSocketDesc = accept(socketDesc, (struct sockaddr*)&clientAdr, (socklen_t *)&len);
        if (newSocketDesc)
            handle_error("accep()");
        system("clear");
        char temp[BUFF_SIZE];
        inet_ntop(clientAdr.sin_family, (struct sockaddr*)&clientAdr, temp, sizeof(temp));
        printf("Server: got connection, port number: %d\n",clientAdr.sin_port);
    }
    close(socketDesc);
#endif
    return 0;
}