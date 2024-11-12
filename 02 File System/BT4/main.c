#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
 /*******************************************************************/
#define FD_SUCEESS      
#define FD_FAIL                 -1
#define STRING_O_APPEND         "TEST O_APPEND"
#define STRING_L_SEEK           "LSEEK"
 /*******************************************************************/

int main(int argc, char *argv[])
{
    /*System call
    int open(const char *pathname, int flags,mode_t mode );
    ssize_t read(int fd, void *buf, size_t count);
    ssize_t write(int fd, const void *buf, size_t count);
    off_t lseek(int fd, off_t offset, int whence);
    int close(int fd);
    */
    char file_path[200] = "./src/test.txt";
    int stringAppendLen = 0;
    int stringLseek = 0;
    char *ptr = STRING_O_APPEND;
    while (*ptr != 0)
    {
        stringAppendLen++;
        ptr++;
    }
    ptr = STRING_L_SEEK;
    while (*ptr != 0)
    {
        stringLseek++;
        ptr++;
    }
    int fd = open(file_path, O_WRONLY | O_RDONLY | O_SYNC, 0777);
    if (fd == FD_FAIL)
    {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }
    else
    {
        write(fd, STRING_O_APPEND, stringAppendLen);
        int offSet = lseek(fd, 0, SEEK_SET);
        write(fd, STRING_L_SEEK, stringLseek);
        write(fd, STRING_O_APPEND, stringAppendLen);
        close(fd);
    }
    exit(EXIT_SUCCESS);
}