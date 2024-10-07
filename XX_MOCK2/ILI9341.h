#ifndef _ILI9341_H_
#define _ILI9341_H_
/*******************************************************************************/
#include <linux/gpio.h>
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
/*******************************************************************************/
#define MAX_COL						320
#define MAX_ROW						240
#define LOW							0
#define HIGH						1

#define SEEK_SET					0
#define SEEK_CUR 					1
#define SEEK_END					2
#define SPI_MAX_TRANSFER_BYTE		159

#define ILI9341_FONT_SIZE			FONTSIZE_16
#define ILI9341_DEF_COL				240
#define ILI9341_DEF_ROW				320
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
	/* Display pointer which points to row is being display */
	int displayRow;
	int currentRow;
	int totalRow;
	/* Variable for saving data */
	struct file *fileBuffer;
} ILI9341Type;

typedef struct ILI9341VerticalScrollType{
	int Topfix;
	int Botfix;
} ILI9341VerticalScrollType;
/*******************************************************************************/
extern struct file *fileBuffer;
/*******************************************************************************/
extern ILI9341Type ili9341;
/*******************************************************************************/
void ILI9341_WriteReg(ILI9341Type *device, char buff);
void ILI9341_DisplayMultiPixel(ILI9341Type *device, u8 *color, unsigned int size);
/*******************************************************************************/
void ILI9341_printImage(ILI9341Type *device, u16* data, unsigned int size);

void ILI9341_printCharOverlay(ILI9341Type *device, char ch, u16 charColor, u16 bgColor);

void ILI9341_printStringOverlay(ILI9341Type *device, char* ch, u16 charColor, u16 bgColor);

void ILI9341_printStringScroll(ILI9341Type *device, char* ch, u16 charColor, u16 bgColor);

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

void ILI9341_ScrollDown(ILI9341Type *device, u16 val);

void ILI9341_ScrollDownToPrint(ILI9341Type *device, u16 val);

int ILI9341_FillBlankLine(ILI9341Type *device);

ssize_t ILI9341_saveBuffer(ILI9341Type *device, u8 *buff, int size, int offset, int where);

ssize_t ILI9341_readRowBuffer(ILI9341Type *device, u8 *toSaveBuff, int offset, int where);
/*******************************************************************************/

/*******************************************************************************/
#endif