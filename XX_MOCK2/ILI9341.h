#ifndef _ILI9341_H_
#define _ILI9341_H_
/*******************************************************************************/
#include <linux/gpio.h>
#include <linux/spi/spi.h>
/*******************************************************************************/
#define MAX_COL						320
#define MAX_ROW						240
#define LOW							0
#define HIGH						1
/*******************************************************************************/
typedef struct ILI9341Type{
	struct spi_device *ili9341;
	int col;
	int row;
    struct gpio_desc *resetPin;
    struct gpio_desc *dcPin;
	int maxRow;
	int maxCol;
	/* Font */
	int fontSize;
	int fontColSize;
	int fontRowSize;
} ILI9341Type;

typedef struct ILI9341VerticalScrollType{
	int Topfix;
	int Botfix;
} ILI9341VerticalScrollType;
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/
void ILI9341_printImage(ILI9341Type *device, u16* data, unsigned int size);

void ILI9341_printChar(ILI9341Type *device, char ch, u16 charColor, u16 bgColor);

void ILI9341_printString(ILI9341Type *device, char* ch, u16 charColor, u16 bgColor);

void ILI9341_Cmd1Byte(ILI9341Type *device, char buff);

void ILI9341_SetWindow(ILI9341Type *device, int StartRow, int StartCol, int EndRow, int EndCol);

void ILI9341_CmdMulBytes(ILI9341Type *device, char *buff, int size);

void ILI9341_Read(ILI9341Type *device, bool isCommand, char* senBuff, int sendSize, char* saveBuff, int saveSize);

void ILI9341_Reset(ILI9341Type *device);

void ILI9341_SleepMode(ILI9341Type *device, bool isSleep);

void ILI9341_InverseMode(ILI9341Type *device, bool isInverse);

void ILI9341_DispalyOn(ILI9341Type *device, bool isON);

void ILI9341_SetCursor(ILI9341Type *device, int Row, int Col);

void ILI9341_Tearing(ILI9341Type *device, bool isTearing);

void ILI9341_GammaSet(ILI9341Type *device, int value);

void ILI9341_Nextline(ILI9341Type *device);

void ILI9341_FillColor(ILI9341Type *device, u16 color);

void ILI9341_SetBrightness(ILI9341Type *device, int value);

void ILI9341_Init(ILI9341Type *device);

void ILI9341_Deinit(ILI9341Type *device);

void ILI9341_RotateMode(ILI9341Type *device, int mode);

void ILI9341_ScrollUp(ILI9341Type *device, u16 val);

void ILI9341_PartialScrollUp(ILI9341Type *device, u16 TopRowFix, u16 BotRowFix, u16 val);

void ILI9341_ScrollDown(ILI9341Type *device, u16 val);



/*******************************************************************************/

/*******************************************************************************/
#endif