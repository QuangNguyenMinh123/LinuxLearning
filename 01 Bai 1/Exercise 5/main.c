#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/**************************************************************************************************/
#define BIT_0       0b1
#define BIT_1       0b10
#define BIT_2       0b100
#define BIT_3       0b1000
#define BIT_4       0b10000
#define BIT_5       0b100000
#define BIT_6       0b1000000
#define BIT_7       0b10000000

#define USER        "nguyen-minh-quang"
#define FILE_NAME   "event4"
/**************************************************************************************************/
FILE *fileStream;
char content[3];
char quit = 0;
char str[100];
/**************************************************************************************************/
void readData(FILE* file, char *buffer, int size)
{
    fseek(file, 0, SEEK_SET);
    fread(buffer, sizeof(char), size, file);
}
/**************************************************************************************************/
int main(void)
{
    int i = 0;
    printf("Changing file owner\n");
    sprintf(str,"sudo chown %s /dev/input/%s", USER, FILE_NAME);
    if (system(str) == 0)
    {
        sprintf(str,"/dev/input/%s", FILE_NAME);
        fileStream = fopen(str,"r");
        if (fileStream == NULL)
        {
            printf("File is not exist\n");
        }
        else
        {
            printf("Change owner successfully\n");
            while (quit != 'q' || quit != 'Q')
            {
                printf("************************\n");
                readData(fileStream, content, 3);
                printf("%d %d %d \n", content[0], content[1], content[2]);
            }      
        }
    } 
    else
    {
        printf("Failed to change owner\n");
    }
    return 0;
    }