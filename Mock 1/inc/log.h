#ifndef _LOG_H
#define _LOG_H
/*******************************************************************/

/*******************************************************************/
int log_open(void);
void log_write(int fileIndex, char* str);
void log_data(int fileIndex, int nodeIdx);
void log_close(int fileIndex);
/*******************************************************************/
#endif