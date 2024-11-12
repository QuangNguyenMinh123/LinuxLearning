/* This app test bbb pwm for pin P9_14 in user space */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
/*******************************************************************************/
#define DUTY_FILE           "/sys/bbb_pwm/duty"
#define PERIOD_FILE         "/sys/bbb_pwm/period"
#define PERIOD              1000000
/*******************************************************************************/
char buff[20];
/*******************************************************************************/
void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
/*******************************************************************************/
int main(void)
{
    int duty_file = open(DUTY_FILE, O_RDWR);
    int period_file = open(PERIOD_FILE, O_RDWR);
    int cnt = 0;
    int value;
    if (duty_file <= 0)
    {
		printf("%s is not exist\n", DUTY_FILE);
		return -1;
    }
    if (period_file <= 0)
    {
		printf("%s is not exist\n", PERIOD_FILE);
		return -1;
    }
    cnt = PERIOD;
    sprintf(buff,"%d", cnt);
    write(period_file, buff, sizeof(buff));
    while (1)
    {
        for (cnt = 0; cnt <= 100; cnt ++)
        {
            value = PERIOD * cnt / 100;
            sprintf(buff,"%d", value);
            write(duty_file, buff, sizeof(buff));
            sleep_ms(25);
        }
    }
    return 0;
}
/*******************************************************************************/