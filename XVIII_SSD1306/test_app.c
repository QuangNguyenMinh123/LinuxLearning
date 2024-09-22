#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>
/*******************************************************************************/
#define CLEAR_SCREEN 					_IO('S', 'c')
#define ON_DISPLAY			 			_IO('S', 'n')
#define OFF_DISPLAY			 			_IO('S', 'f')
#define FILE_PATH                       "/proc/ssd1306"
#define BRIGHTNESS_FILE                 "/sys/SSD1306_Device/brightness"
/*******************************************************************************/
void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
/*******************************************************************************/

/*******************************************************************************/
int main (void)
{
    int fd;
    int brightness_file;
    char brightness = 0;
    printf("This is user-space application. PID: %d\n", getpid());
    fd = open(FILE_PATH, O_RDONLY);
    if(fd < 0) {
		perror("Could not open /proc/ssd1306");
		return -1;
	}
    brightness_file = open(BRIGHTNESS_FILE, O_WRONLY);
    if(brightness < 0) {
		perror("Could not open /sys/SSD1306_Device/brightness");
		return -1;
	}
    /* Send PID to kernel via ioctl */
    write(brightness_file, &brightness, sizeof(brightness));
    close(fd);
    close(brightness_file);
    return 0;
}