#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include<unistd.h>
/*******************************************************************************/
using namespace std;
/*******************************************************************************/
typedef struct PositionType {
	int startx;
	int starty;
	int endx;
	int endy;
} PositionType;

typedef struct VerticalCrollType {
	int TopFix;
	int starty;
	int endx;
} VerticalCrollType;

class LCD
{
private:
	int maxRow;
	int maxCol;
public:
	LCD();
	
	void LCDClear(void);
	void LCDReset(void);
	void LCDInverse(bool isReverse);
	void LCDDisplayOn(bool isOn);
	void LCDSetWindows(int startx, int starty, int endx, int endy);
	int LCDColSize(void);
	int LCDRowSize(void);
	void LCDPrintString(char* str, int size);
	void LCDFill(uint16_t color);
	void LCDRotate(uint8_t mode);
};
/*******************************************************************************/
#define WHITE_16                    	0xffff
#define BLACK_16                    	0x00
#define RED_16                      	0xf800
#define GREEN_16                    	0x07e0
#define BLUE_16                     	0x001f
#define PURPLE_16                   	0xf81f
#define YELLOW_16                   	0xffe0
#define AQUA_16                     	0x07ff
/*******************************************************************************/
#define MAJIC_NO						100
#define IOCTL_SET_WINDOW				_IOW(MAJIC_NO, 3, PositionType)
#define IOCTL_CLEAR						_IO(MAJIC_NO, 4)
#define IOCTL_RESET						_IO(MAJIC_NO, 5)
#define IOCTL_INVERSE_ON				_IO(MAJIC_NO, 6)
#define IOCTL_INVERSE_OFF				_IO(MAJIC_NO, 7)
#define IOCTL_DISPLAY_ON				_IO(MAJIC_NO, 8)
#define IOCTL_DISPLAY_OFF				_IO(MAJIC_NO, 9)
/*******************************************************************************/
#define PROC_FILE_PATH          	"/proc/ili9341"
#define LCD_INVERSE_FILE			"/sys/ili9341/inverse_on_off"
#define LCD_DISPLAY_ON_FILE			"/sys/ili9341/display_on_off"
#define LCD_FILL_COLOR_FILE			"/sys/ili9341/fill_color"
#define LCD_ROTATE_FILE				"/sys/ili9341/rotate"
#define LCD_INIT_FILE				"/sys/ili9341/init"
#define YOUR_STRING					"THIS IS MY STRING"
/*******************************************************************************/
#define ms				1000000
/*******************************************************************************/
/* Ioctl command */
void LCD::LCDClear(void)
{
	int proc_file = open(PROC_FILE_PATH, O_WRONLY);
	if(proc_file < 0) {
		perror("Error open file PROC_FILE_PATH");
		return;
	}
    if(ioctl(proc_file, IOCTL_CLEAR, NULL)) {
		cout<<"Error setting window\n";
	}
	close(proc_file);
}

void LCD::LCDReset(void)
{
	int proc_file = open(PROC_FILE_PATH, O_RDWR);
	if(proc_file < 0) {
		perror("Error open file PROC_FILE_PATH");
		return;
	}
    if(ioctl(proc_file, IOCTL_RESET, NULL)) {
		cout<<"Error setting window\n";
	}
	close(proc_file);
}
/*******************************************************************************/
/* Sysfs command */
LCD::LCD()
{
	int readData[2] = {1,1};
	int proc_file = open(LCD_INIT_FILE, O_RDWR);
	if(proc_file < 0) {
		perror("Error open file LCD_INIT_FILE");
		return;
	}
    read(proc_file, readData, sizeof(readData));
	this->maxRow = readData[0];
	this->maxCol = readData[0];
	close(proc_file);
}

void LCD::LCDInverse(bool isReverse)
{
	int proc_file = open(LCD_INVERSE_FILE, O_RDWR);
	if(proc_file < 0) {
		perror("Error open file LCD_INVERSE_FILE");
		return;
	}
    write(proc_file, &isReverse, 1);
	close(proc_file);
}

void LCD::LCDDisplayOn(bool isOn)
{
	int proc_file = open(LCD_DISPLAY_ON_FILE, O_RDWR);
	if(proc_file < 0) {
		perror("Error open file LCD_DISPLAY_ON_FILE");
		return;
	}
    write(proc_file, &isOn, 1);
	close(proc_file);
}

void LCD::LCDFill(uint16_t color)
{
	uint8_t buff[2] = {
		color >> 8,
		color & 0xff
	};
	int proc_file = open(LCD_FILL_COLOR_FILE, O_WRONLY);
	if(proc_file < 0) {
		perror("Error open file LCD_FILL_COLOR_FILE");
		return;
	}
    write(proc_file, buff, 2);
	close(proc_file);
}

void LCD::LCDRotate(uint8_t mode)
{
	int proc_file = open(LCD_ROTATE_FILE, O_RDWR);
	if(proc_file < 0) {
		perror("Error open file LCD_ROTATE_FILE");
		return;
	}
    write(proc_file, &mode, 1);
	close(proc_file);
}
/*******************************************************************************/
/* Write LCD */
int LCD::LCDColSize(void)
{
	return this->maxCol;
}

int LCD::LCDRowSize(void)
{
	return this->maxRow;
}

void LCD::LCDSetWindows(int startx, int starty, int endx, int endy)
{
	int proc_file = open(PROC_FILE_PATH, O_RDWR);
	PositionType pos = {startx, starty, endx, endy};
	if(proc_file < 0) {
		perror("Error open file PROC_FILE_PATH");
		return;
	}
	/* Send pos to ILI9341 */
    if(ioctl(proc_file, IOCTL_SET_WINDOW, &pos)) {
		cout<<"Error setting window\n";
	}
	close(proc_file);
}

void LCD::LCDPrintString(char* str, int size)
{
	int proc_file = open(PROC_FILE_PATH, O_RDWR);
	if(proc_file < 0) {
		perror("Error open file PROC_FILE_PATH");
		return;
	}
	/* Write to ILI9341 */
    write(proc_file, str, size);
	close(proc_file);
}
/*******************************************************************************/

/*******************************************************************************/
int main(void)
{
	LCD MyLCD;
	char str[99][30];
	int i,j; 
	for (i=0;i<10;i++)
	{
		str[i][0]	='T';
		str[i][1]	='H';
		str[i][2]	='I';
		str[i][3]	='S';
		str[i][4]	=' ';
		str[i][5]	='I';
		str[i][6]	='S';
		str[i][7]	=' ';
		str[i][8]	='L';
		str[i][9]	='I';
		str[i][10]	='N';
		str[i][11]	='E';
		str[i][12]	=' ';
		str[i][13]	= i + '0';
		str[i][14]	= '\n';
	}

	for (i=10;i<99;i++)
	{
		str[i][0]	='T';
		str[i][1]	='H';
		str[i][2]	='I';
		str[i][3]	='S';
		str[i][4]	=' ';
		str[i][5]	='I';
		str[i][6]	='S';
		str[i][7]	=' ';
		str[i][8]	='L';
		str[i][9]	='I';
		str[i][10]	='N';
		str[i][11]	='E';
		str[i][12]	=' ';
		str[i][13]	= (i/10) + '0';
		str[i][14]	= (i%10) + '0';
		str[i][15]	= '\n';
	}
	str[98][15] = '|';
	MyLCD.LCDSetWindows(0,0, 320, 240);
	for (i=0;i<10;i++)
	{
		MyLCD.LCDPrintString(str[i], 16);
	}

	for (i=10;i<99;i++)
	{
		MyLCD.LCDPrintString(str[i], 17);
	}

    return 0;
}

/*******************************************************************************/