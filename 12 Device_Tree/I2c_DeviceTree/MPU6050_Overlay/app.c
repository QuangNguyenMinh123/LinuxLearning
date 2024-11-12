#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <time.h>

#define CDEV_PATH "/proc/mpu6050"

char read_buf[1024];

int main(void) {
    int fd = open(CDEV_PATH, O_RDWR);
    if (fd <= 0)
    {
		printf("%s is not exist\n", CDEV_PATH);
		return -1;
    }

    while (1)
    {
        read(fd, read_buf, 3);
        printf("Data: %d,%d,%d\n", read_buf[0],read_buf[1],read_buf[2]);
	      sleep(1);
    }
    return 0;
}