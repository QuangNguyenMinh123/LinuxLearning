#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    FILE *fileStream;///home/nguyen-minh-quang/Desktop/BaiTapC
    char buff = 0;
    char buffer[100] = {0};
    if (argc == 2)
    {
        sprintf(buffer, "%s",argv[1]);
    }
    fileStream = fopen(buffer,"r");
    buff = fgetc(fileStream);
    while (buff != EOF)
    {
        printf("%c",buff);
        buff = fgetc(fileStream);
    }
    printf("\n");
    fclose(fileStream);
    return 0;
}