#ifndef _TYPES_H
#define _TYPES_H
/*******************************************************************/
typedef struct studentType{
    char name[50];
    int age;
    char hometown[100];
    char phoneNo[100];
}studentType;
typedef struct messageType{
    long ID;
    studentType student;
}messageType;
/*******************************************************************/

/*******************************************************************/
#endif