#ifndef _ILI9341_H_
#define _ILI9341_H_
/*******************************************************************************/
#include <linux/gpio.h>
#include <linux/spi/spi.h>
/*******************************************************************************/
#define FONT_SIZE			6
#define MAX_COL				320
#define MAX_ROW				240
#define LOW					0
#define HIGH				1
/*******************************************************************************/
typedef struct ILI9341Type{
	struct spi_device *ili9341;
	int col;
	int row;
    struct gpio_desc *resetPin;
    struct gpio_desc *dcPin;
} ILI9341Type;
/*******************************************************************************/
extern uint8_t LCD_Font5x7[][FONT_SIZE];
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/
void ILI9341_printImage(ILI9341Type *device, char* ch, int size);

void ILI9341_printChar(ILI9341Type *device, char ch);

void ILI9341_print(ILI9341Type *device, char* ch);

void ILI9341_Cmd1Byte(ILI9341Type *device, char buff);

void ILI9341_CmdMulBytes(ILI9341Type *device, char *buff, int size);

void ILI9341_Read(ILI9341Type *device, bool isCommand, char* senBuff, int sendSize, char* saveBuff, int saveSize);

void ILI9341_Reset(ILI9341Type *device);

void ILI9341_SleepMode(ILI9341Type *device, bool isSleep);

void ILI9341_InverseMode(ILI9341Type *device, bool isInverse);

void ILI9341_DispalyOn(ILI9341Type *device, bool isON);

void ILI9341_gotoCol(ILI9341Type *device, int Col);

void ILI9341_gotoRow(ILI9341Type *device, int Row);

void ILI9341_goto(ILI9341Type *device, int Col, int Row);

void ILI9341_WriteMem(ILI9341Type *device, char *Userbuff, int size);

void ILI9341_Tearing(ILI9341Type *device, bool isTearing);

void ILI9341_GammaSet(ILI9341Type *device, int value);

void ILI9341_ColorSet(ILI9341Type *device, int red, int green, int blue);

void ILI9341_Nextline(ILI9341Type *device);

void ILI9341_ClearScreen(ILI9341Type *device);

void ILI9341_Init(ILI9341Type *device);

void ILI9341_Deinit(ILI9341Type *device);
/*******************************************************************************/

/*******************************************************************************/
#endif