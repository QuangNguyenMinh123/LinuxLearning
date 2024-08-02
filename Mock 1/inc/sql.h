#pragma once
#ifndef SQL_H
#define SQL_H
#include "process_list.h"
/*******************************************************************/
typedef enum{
    NEW_CONNECTION,
    UPDATE
} SQL_COMMAND;
/*******************************************************************/
void sql_init(void);
void sql_newnode(ConnectionType* node);
void sql_insert(ConnectionType* node);
void sql_disconnect(ConnectionType* node);
void sql_deinit(void);
/*******************************************************************/
#endif