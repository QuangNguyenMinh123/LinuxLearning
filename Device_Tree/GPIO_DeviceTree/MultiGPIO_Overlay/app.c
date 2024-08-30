#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
/*******************************************************************************/
#define PROC_PATH       "/proc/my_gpio"
/*******************************************************************************/

void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

/*******************************************************************************/
int main (void)
{
    struct timespec ts;
    int fd = open(PROC_PATH, O_RDWR);
    if (fd <= 0)
    {
		printf("%s is not exist\n", PROC_PATH);
		return -1;
    }
    while (1)
    {
        char i = '0';
        for (; i < '6'; i++)
        {
            write(fd, &i, 1);
	        sleep_ms(100);
        }
    }
    return 0;
}