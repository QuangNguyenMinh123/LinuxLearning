#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*******************************************************************/
#define TRUE            1
#define FALSE           0

/*******************************************************************/

/*******************************************************************/
void menu_printBar()
{
    printf("/*******************************/\n");
}

void menu_MenuA(void)
{
    menu_printBar();
    printf("Nhap thong tin sinh vien: \n");
}

void menu_MenuB(void)
{
    menu_printBar();
    printf("1: Find student by ID\n");
    printf("2: Display all student info\n");
    printf("3: Remove student using ID\n");
}

void menu_printAll(messageType * ptr)
{
    menu_printBar();
    printf("ID\tName\t\tAge\tHometown\t\t\tPhone Number\t\t");
    while ( ptr != NULL)
    {
        printf("%ld\t",ptr->ID);
        printf("%s\t\t",ptr->student.name);
        printf("%d\t",ptr->student.age);
        printf("%s\t\t\t",ptr->student.hometown);
        printf("%s\n",ptr->student.phoneNo);
        ptr++;
    }
}

void menu_print1(long Id, messageType * searchPtr)
{
    menu_printBar();
    printf("ID\tName\t\tAge\tHometown\t\t\tPhone Number\t\t");
    while ( searchPtr != NULL)
    {
        if (searchPtr->ID == Id)
            break;
        else
            searchPtr ++;
    }
    printf("%ld\t",searchPtr->ID);
    printf("%s\t\t",searchPtr->student.name);
    printf("%d\t",searchPtr->student.age);
    printf("%s\t\t\t",searchPtr->student.hometown);
    printf("%s\n",searchPtr->student.phoneNo);
}
/*******************************************************************/