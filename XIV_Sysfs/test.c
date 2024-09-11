#include <stdio.h>
#include <time.h>
#include <errno.h> 
#include <stdlib.h>
/*******************************************************************************/
#define VALUE_PATH          "/sys/bbb_gpio/value"
#define DIRECTION_PATH      "/sys/bbb_gpio/direction"
/*******************************************************************************/
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

/*******************************************************************************/
int main(void)
{
    while (1)
    {
        system("cat /sys/bbb_gpio/value");
        msleep(100);
    }
    
    return 0;
}