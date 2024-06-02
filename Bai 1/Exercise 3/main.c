#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
int processID = 0;
char check = 0;
FILE *fileStream;
char buffer[100];
char fileName[100];
int fileSize = 0;
int main(int argc, char argv[])
{
    printf("Please type in the process ID: ");
    scanf("%i",&processID);
    sprintf(fileName, """/proc/%d/cmdline",processID);
    fileStream = fopen(fileName,"r");
    if (fileStream == NULL)
        printf("Process has ID %d is not existed\n",processID);
    else
    {
        fileSize = fread(buffer, sizeof(char), 1024, fileStream);
        if (fileSize)
        {
            printf("Process has ID %d content: \n",processID);
            printf("%s\n", buffer);
        }
        else
            printf("Process ID %d is exist but process's identity is not found\n", processID);
        fclose(fileStream);
    }
    return 0;
}