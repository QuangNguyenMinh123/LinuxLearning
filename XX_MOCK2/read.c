#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>
char buffer[50];
int main()
{
    // file pointer variable to store the value returned by
    // fopen
    int fptr, procfile;
    // opening the file in read mode
    fptr = open("test.cpp", O_RDONLY);
    
    // procfile = open("buffFile.txt", O_RDWR);
    procfile = open("/proc/ili9341", O_RDWR);
    // checking if the file is opened successfully
    if (fptr == 0) {
        printf("test.cpp is not opened. The program will now exit.\n");
        exit(0);
    }
    else {
        printf("test.cpp is opened Successfully.\n");
    }
    if (procfile == 0) {
        printf("/proc/ili9341 is not opened. The program will now exit.\n");
        exit(0);
    }
    else {
        printf("/proc/ili9341 is opened Successfully.\n");
    }

    while (read(fptr, &buffer[0], 50) != 0) {
        
        write(procfile, buffer, 50);
    }
    printf("/proc/ili9341 is not opened. The program will now exit.\n");
    write(procfile, "123456\n", 6);
    close(fptr);
    close(procfile);
    return 0;
}