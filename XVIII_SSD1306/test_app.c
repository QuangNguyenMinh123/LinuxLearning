#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
/*******************************************************************************/
#define REGISTER_UAPP 					_IO('R', 'g')
#define SCROLL_VERTICAL_UP 				_IO('V', 'u')
#define SCROLL_VERTICAL_DOWN 			_IO('V', 'd')
#define SCROLL_HORIZONTAL_RIGHT 		_IO('H', 'r')
#define SCROLL_HORIZONTAL_LEFT 			_IO('H', 'l')
#define CLEAR_SCREEN 					_IO('S', 'c')
#define ON_DISPLAY			 			_IO('S', 'n')
#define OFF_DISPLAY			 			_IO('S', 'f')
#define FILE_PATH                       "/proc/ssd1306"
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/
int main (void)
{
    int fd;
    printf("This is user-space application. PID: %d\n", getpid());
    fd = open(FILE_PATH, O_RDONLY);
    if(fd < 0) {
		perror("Could not open /dev/InterruptFromKernel");
		return -1;
	}
    /* Send PID to kernel via ioctl */
    ioctl(fd, REGISTER_UAPP, NULL);
    ioctl(fd, SCROLL_VERTICAL_UP, NULL);
    ioctl(fd, SCROLL_VERTICAL_DOWN, NULL);
    ioctl(fd, SCROLL_HORIZONTAL_RIGHT, NULL);
    ioctl(fd, SCROLL_HORIZONTAL_LEFT, NULL);
    ioctl(fd, CLEAR_SCREEN, NULL);
    ioctl(fd, ON_DISPLAY, NULL);
    ioctl(fd, OFF_DISPLAY, NULL);
    close(fd);
    return 0;
}