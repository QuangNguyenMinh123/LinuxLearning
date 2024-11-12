#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>
/*******************************************************************************/
#define SIGTX0                          44
#define SIGTX1                          45
#define SIGTX2                          46
#define SIGTX3                          47
#define SIGTX4                          48
#define STOP_SCROLL						_IO('S', 's')
#define REGISTER_UAPP 		            _IO('R', 'g')

#define SSD1306_PROC_FILE               "/sys/ssd1306"
#define BRIGHTNESS_FILE                 "/sys/SSD1306_Device/brightness"
#define SCROLL_RIGHT_FILE               "/sys/SSD1306_Device/scroll_right"
#define SCROLL_LEFT_FILE                "/sys/SSD1306_Device/scroll_left"
#define INTERRUPT_FILE                  "/proc/InterruptFromKernel"
/*******************************************************************************/
int brightness = 1;
int scroll_speed = 7;
int brightness_file, scroll_left_file, scroll_right_file, proc_file;
/*******************************************************************************/
void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}
/*******************************************************************************/
void ScrollRight_Handler(int sig) {
	write(scroll_right_file, &scroll_speed, sizeof(scroll_speed));
}

void ScrollLeft_Handler(int sig) {
	write(scroll_left_file, &scroll_speed, sizeof(scroll_speed));
}

void StopScroll_Handler(int sig) {
	/* Stop Scrolling */
    ioctl(proc_file, STOP_SCROLL, NULL);
}

void BrightnessIncrease_Handler(int sig) {
	brightness++;
    write(brightness_file, &brightness, sizeof(brightness));
}

void BrightnessDecrease_Handler(int sig) {
    brightness--;
	write(brightness_file, &brightness, sizeof(brightness));
}
/*******************************************************************************/
int main (void)
{
    
    int interrupt_file;
    signal(SIGTX0, ScrollRight_Handler);
    signal(SIGTX1, ScrollLeft_Handler);
    signal(SIGTX2, StopScroll_Handler);
    signal(SIGTX3, BrightnessIncrease_Handler);
    signal(SIGTX4, BrightnessDecrease_Handler);
    printf("This is user-space application. PID: %d\n", getpid());
    /* open files */
    /* Proc file */
    proc_file = open(SSD1306_PROC_FILE, O_RDONLY);
    if(proc_file < 0) {
		perror("Could not open /sys/ssd1306");
		return -1;
	}
    /* Interrupt */
    interrupt_file = open(INTERRUPT_FILE, O_RDONLY);
    if(interrupt_file < 0) {
		perror("Could not open /proc/InterruptFromKernel");
		return -1;
	}
    /* Brightness */
    brightness_file = open(BRIGHTNESS_FILE, O_WRONLY);
    if(brightness_file < 0) {
		perror("Could not open /sys/SSD1306_Device/brightness");
		return -1;
	}
    /* Scroll right */
    scroll_right_file = open(SCROLL_RIGHT_FILE, O_WRONLY);
    if(scroll_right_file < 0) {
		perror("Could not open /sys/SSD1306_Device/scroll_right_file");
		return -1;
	}
    /* Scroll left */
    scroll_left_file = open(SCROLL_LEFT_FILE, O_WRONLY);
    if(scroll_left_file < 0) {
		perror("Could not open /sys/SSD1306_Device/scroll_left_file");
		return -1;
	}
    /* Send PID to kernel via ioctl */
    if(ioctl(interrupt_file, REGISTER_UAPP, NULL)) {
		perror("Error registering app");
		close(interrupt_file);
		return -1;
	}
    /* wait for IRQ */
    while (1)
    {

    }
    close(brightness_file);
    close(scroll_left_file);
    close(scroll_right_file);
    close(interrupt_file);
    return 0;
}