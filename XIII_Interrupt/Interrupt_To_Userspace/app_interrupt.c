#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
/*******************************************************************************/
#define SIGTX 44
#define REGISTER_UAPP _IO('R', 'g')
/*******************************************************************************/
void signalhandler(int sig) {
	printf("Button was pressed!\n");
}
/*******************************************************************************/

/*******************************************************************************/
int main (void)
{
    int fd;
	signal(SIGTX, signalhandler);
    printf("This is user-space application. PID: %d\n", getpid());
    fd = open("/dev/irq_signal", O_RDONLY);
    return 0;
}