#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
/*******************************************************************************/
#define PROC_PATH           "/proc/my_pwm"
#define IOCTL_GPIO          _IOW('a','1',int32_t *)
#define IOCTL_PWM           _IOW('a','0',int32_t *)
/*******************************************************************************/
int option;
int pwm_value;
/*******************************************************************************/
void printMenu()
{

    printf("****Please Enter the Option******\n");
    printf("        1. GPIO mode             \n");
    printf("        2. PWM mode              \n");
    printf("        3. Change pwm            \n");
    printf("        4. Exit                  \n");
    printf("*********************************\n");
    printf(">>> ");
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
        printMenu();
        scanf("%d",&option); 
        switch (option) {
            case 1:
                printf("Converting to GPIO mode \n");
                ioctl(fd, IOCTL_GPIO, NULL);
                break;

            case 2:
                printf("Converting to PWM mode \n");
                ioctl(fd, IOCTL_PWM, NULL);
                break;

            case 3:
                printf("Please enter pwm value: ");
                scanf("%d",&pwm_value); 
                printf("Writing pwm value: %d to pin P9_21 \n",pwm_value);
                write(fd, &pwm_value, sizeof(pwm_value));
                break;

            case 4:
                close(fd);
                return 0;
        }
    }
    
    return 0;
}