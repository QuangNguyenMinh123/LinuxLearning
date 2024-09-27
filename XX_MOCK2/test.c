#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
/*******************************************************************************/
typedef struct position {
	unsigned char x;
	unsigned char y;
} Position_t;
#define MAJIC_NO				100
#define IOCTL_GOTOXY			_IOW(MAJIC_NO, 3, Position_t)
#define IOCTL_CLEAR				_IO(MAJIC_NO, 4)
#define PROC_FILE_PATH          "/proc/nokia5110"
#define YOUR_STRING				"THIS IS MY STRING"
/*******************************************************************************/

/*******************************************************************************/
int main(void)
{
    int proc_file;
    Position_t pos = {30 , 3};
    proc_file = open(PROC_FILE_PATH, O_RDWR);
	/* open /proc/nokia5110 */
    if(proc_file < 0) {
		perror("Could not open /proc/nokia5110");
		return -1;
	}
	/* Clear nokia5110 screen */
	if(ioctl(proc_file, IOCTL_CLEAR, NULL)) {
		perror("Error registering app");
		close(proc_file);
		return -1;
	}
	/* Send pos to nokia5110 */
    if(ioctl(proc_file, IOCTL_GOTOXY, &pos)) {
		perror("Error registering app");
		close(proc_file);
		return -1;
	}
	/* Write to nokia5110 */
    write(proc_file, YOUR_STRING, sizeof(YOUR_STRING));
	/* Close */
    close(proc_file);
    return 0;
}

/*******************************************************************************/