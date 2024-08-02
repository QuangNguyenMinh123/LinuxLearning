#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "sql.h"
#include "process_list.h"
#include "log.h"
/*******************************************************************/
#define DATABASE_FILE           "database.db"
/*******************************************************************/
int nodeIndex;
int tableIdx;
sqlite3* DB;
int logFileID;
char sql_ConnectionTable[300] = 
"CREATE TABLE Connection("
"Time   CHAR(50), "
"ID INT     NOT NULL, "
"Ip CHAR(50), "
"Port   INT     NOT NULL, "
"Status CHAR(50));";
char sql_NodeTable[230]; 
char sql_NodeTable1[20] = 
"CREATE TABLE Node";
char sql_NodeTable2[200] =
"("
"Time   CHAR(50), "
"Temp   CHAR(20));";
/*******************************************************************/
void sql_init(int LogFile)
{
    int exit = 0;
    /* Init, create new table*/
    nodeIndex = 1;
    tableIdx = 1;       /* tableIdx = 0 -> table managing connection status */
    /* Manage all node connection status */
    /* Table name */
    /* Format: Timestamp    ID  Ip  Port    Status */
    exit = sqlite3_open(DATABASE_FILE, &DB);
    char* messaggeError;
    if (exit != SQLITE_OK) {
        for (int tryCount = 1; tryCount <= 3; tryCount ++)
        {
            exit = sqlite3_open(DATABASE_FILE, &DB);
            if (exit != SQLITE_OK)
            {
                printf("Open SQL Database after %d try\n",tryCount);
                sleep(1);
            }
            else
                break;
        }
    }
    printf("Open SQL Database Successfully\n");
    logFileID = LogFile;
    log_write(logFileID, "Open SQL Database Successfully\n");
    exit = sqlite3_exec(DB, sql_ConnectionTable, NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        printf("Error Create Status Table\n");
        sqlite3_free(messaggeError);
    }
    else
        printf("Table created Successfully\n");
}

void sql_insert(ConnectionType* node)
{
    int exit = 0;
    char buffer[200];
    char timeBuff[20];
    time_t now;
    time(&now);
    int int1 = node->temp;
    float tempFrac = node->temp - int1;
    int int2 = trunc(tempFrac * 10000);
    strftime(timeBuff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    /*                                          time  temp*/
    sprintf(buffer, "INSERT INTO Node%d VALUES('%s', '%d.%d');",
                            node->Idx,timeBuff, int1, int2);
    if (sqlite3_exec(DB, buffer, NULL, 0, 0) != 0)
        printf("Failed insert data to SQL in node%d\n", node->Idx);
}

void sql_newnode(ConnectionType* node)
{
    int exit = 0;
    char buffer[200];
    char* messaggeError;
    char timeBuff[20];
    /* Creating new table */
    /* Table name */
    /* Format: Timestamp    Ip  Port    Temp */
    sprintf(sql_NodeTable, "%s%d%s", sql_NodeTable1,tableIdx,sql_NodeTable2);
    exit = sqlite3_exec(DB, sql_NodeTable, NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        printf("Error Create Table for node %d\n",tableIdx);
        log_write(logFileID, "Unable to connect to SQL server\n");
        sqlite3_free(messaggeError);
    }
    else
        printf("Create Table for node %d Successfully\n", tableIdx);
    /* New Connection*/
    time_t now;
    time(&now);
    strftime(timeBuff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    /*                                              time id   ip   port status  */
    sprintf(buffer, "INSERT INTO Connection VALUES('%s', %d, '%s', %d, 'Connected');",
                        timeBuff, nodeIndex, node->Ip, node->port);
    sqlite3_exec(DB, buffer, NULL, 0, &messaggeError);
    
    if (exit != SQLITE_OK) {
        printf("Error insert for node %d\n",tableIdx);
        sqlite3_free(messaggeError);
    }
    else
        printf("Insert for node %d Successfully\n", tableIdx);
    sprintf(buffer, "Create new table for node%d\n",nodeIndex);
    log_write(logFileID, buffer);
    tableIdx++;
    nodeIndex++;

}

void sql_disconnect(ConnectionType* node)
{
    char buffer[200];
    char timeBuff[20];
    time_t now;
    time(&now);
    strftime(timeBuff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
    /*                                              time id   ip   port status  */
    sprintf(buffer, "INSERT INTO Connection VALUES('%s', %d, '%s', %d, 'Disconnected');",
                        timeBuff, node->Idx, node->Ip, node->port);
    sqlite3_exec(DB, buffer, NULL, 0, 0);
}

void sql_deinit(void)
{
    sqlite3_close(DB);
}
/*******************************************************************/
