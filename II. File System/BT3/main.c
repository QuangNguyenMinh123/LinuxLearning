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
    struct stat attr;
    char file_path[100] = "test.txt";
    int fd = open(file_path, O_RDONLY | O_SYNC, 0777);
    if (fd == FD_FAIL)
    {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }
    else
    {
        stat(file_path, &attr);
        printf("File type: ");
        switch (attr.st_mode & __S_IFMT)
        {
           case __S_IFBLK:  printf("block device\n");            break;
           case __S_IFCHR:  printf("character device\n");        break;
           case __S_IFDIR:  printf("directory\n");               break;
           case __S_IFIFO:  printf("FIFO/pipe\n");               break;
           case __S_IFLNK:  printf("symlink\n");                 break;
           case __S_IFREG:  printf("regular file\n");            break;
           case __S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                  break;

        };
        printf("File name: %s\n",file_path);
        printf("Last file modification: %s\n",ctime(&attr.st_mtime));
        printf("Last status change: %s\n",ctime(&attr.st_ctime));
        printf("Last file access: %s\n",ctime(&attr.st_atime));
        printf("Size: %i bytes\n", attr.st_blksize);
        close(fd);
    }
    exit(EXIT_SUCCESS);
}