#ifndef _PROCESS_LIST_H
#define _PROCESS_LIST_H
#include <pthread.h>
/*******************************************************************/
#define bool                            char
#define TRUE                            1
#define FALSE                           0
#define INET_ADDRSTRLEN                 16
/*******************************************************************/
typedef struct ConnectionType{
    int pre;
    char Ip[INET_ADDRSTRLEN];        /* 16 = INET_ADDRSTRLEN */
    int port;
    int socketId;
    struct ConnectionType* self;
    int Idx;
    bool Connected;
    bool newData;
    float temp;
    int next;
}ConnectionType;
/*******************************************************************/
void process_list_init(int serverSock);
ConnectionType process_list_new(char* Ip, int port, int socketId);
int process_list_connectionCount(void);
void process_list_closeAll(void);
void process_list_Disconnect(int x);
int process_list_readDataFromNode(int nodex, float *buff);
int process_list_connectionIdx(void);
bool process_list_checkConnect(int nodex);
void process_list_WriteData(int nodex, float data);
int process_list_ReadData(int nodex, float *data);
ConnectionType* process_list_node(int index);
/*******************************************************************/
#endif