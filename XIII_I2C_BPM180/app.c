#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <time.h>


#define CDEV_PATH "/dev/bmp180"

char read_buf[1024];

int main(void) {
    int fd = open(CDEV_PATH, O_RDWR);
    while (1)
    {
        read(fd, read_buf, 20);
        printf("Data: %s\n", read_buf);
	sleep(3);
    }
    return 0;
}
