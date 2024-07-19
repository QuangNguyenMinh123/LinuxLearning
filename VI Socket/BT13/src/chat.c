#include "chat.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*******************************************************************/
#define BUFFER_SIZE         256
/*******************************************************************/
void chat(int socketDesc)
{
     int num_write, num_read;
    char sendbuff[BUFFER_SIZE];
    char recvbuff[BUFFER_SIZE];
    while (1)
    {
        memset(sendbuff, '0', BUFFER_SIZE);
        memset(recvbuff, '0', BUFFER_SIZE);
        printf("Please enter the message: ");
        fgets(sendbuff, BUFFER_SIZE, stdin);

        // Gui thong diep toi server bang ham write
        num_write = write(socketDesc, sendbuff, sizeof(sendbuff));
        if (num_write < 0)
        {
            handler_error("read()");
        }
        if (strncmp("exit", sendbuff, 4) == 0)
        {
            printf("Client exit ...\n");
            break;
        }
        
        // Nhan thong diep tu server bang ham read
        num_read = read(socketDesc, recvbuff, sizeof(recvbuff));
        if (num_read < 0)
        {
            handler_error("read()");
        }
        if (strncmp("exit", recvbuff, 4) == 0)
        {
            printf("Server exit ...\n");
            break;
        }
        printf("\nMessage form Server: %s\n", recvbuff);
    }
    close(socketDesc);

}
/*******************************************************************/