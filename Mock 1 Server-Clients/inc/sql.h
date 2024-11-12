#pragma once
#ifndef SQL_H
#define SQL_H
#include "process_list.h"
/*******************************************************************/

/*******************************************************************/
void sql_init(int LogFile);
void sql_newnode(ConnectionType* node);
void sql_insert(ConnectionType* node);
void sql_disconnect(ConnectionType* node);
void sql_deinit(void);
/*******************************************************************/
#endif