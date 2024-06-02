#include <stdio.h>
FILE *fileStream;
char fileName[100];
char *ptr = 0;
char check = 0;
int main(void)
{
    ptr = fileName;
    printf("Please type in the file: ");
    fgets(fileName, sizeof(fileName), stdin);
    while (*ptr != '\n')
    {
        ptr++;
    }
    *ptr = 0;
    fileStream = fopen(fileName,"r");
    if (fileStream == NULL)
    {
        printf("File %s is not exist in project directory\n",fileName);
    }
    else
    {
        printf("File content:\n");
        printf("------------------------------------------\n");
        check = fgetc(fileStream);
        while (check != EOF)
        {
            printf("%c",check);
            check = fgetc(fileStream);
        }
        printf("\n");
        fclose(fileStream);
    }
    return 0;
}