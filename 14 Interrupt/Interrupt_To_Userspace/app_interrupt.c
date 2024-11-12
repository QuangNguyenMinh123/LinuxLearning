#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
/*******************************************************************************/
#define SIGTX0               44
#define SIGTX1               45
#define SIGTX2               46
#define SIGTX3               47
#define SIGTX4               48
#define REGISTER_UAPP 		_IO('R', 'g')
#define FILE_PATH           "/proc/InterruptFromKernel"
/*******************************************************************************/
void signalhandler0(int sig) {
	printf("Interrupt 0 from kernel!\n");
}

void signalhandler1(int sig) {
	printf("Interrupt 1 from kernel!\n");
}

void signalhandler2(int sig) {
	printf("Interrupt 2 from kernel!\n");
}

void signalhandler3(int sig) {
	printf("Interrupt 3 from kernel!\n");
}

void signalhandler4(int sig) {
	printf("Interrupt 4 from kernel!\n");
}
/*******************************************************************************/

/*******************************************************************************/
int main (void)
{
    int fd;
	signal(SIGTX0, signalhandler0);
    signal(SIGTX1, signalhandler1);
    signal(SIGTX2, signalhandler2);
    signal(SIGTX3, signalhandler3);
    signal(SIGTX4, signalhandler4);
    printf("This is user-space application. PID: %d\n", getpid());
    fd = open(FILE_PATH, O_RDONLY);
    if(fd < 0) {
		perror("Could not open /dev/InterruptFromKernel");
		return -1;
	}
    /* Send PID to kernel via ioctl */
    if(ioctl(fd, REGISTER_UAPP, NULL)) {
		perror("Error registering app");
		close(fd);
		return -1;
	}
    printf("Process ID is sent to kernel\n");
    /* Wait for Signal */
	printf("Waiting for signal...\n");
    while (1)
    {

    }
    close(fd);
    return 0;
}