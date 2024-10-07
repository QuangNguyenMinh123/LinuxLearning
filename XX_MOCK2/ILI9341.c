#include "ILI9341.h"
#include "ILI9341_GUI.h"
#include <linux/delay.h>
#include <linux/fs.h>
/*******************************************************************************/

/*******************************************************************************/
u8 ILI9341_RamBuffer[ILI9341_DEF_ROW * ILI9341_DEF_COL * 2] = { 0 };
int ramBufStart = 0;
u8 ILI9341_Font1208_Buffer[8*12*2] = {0};
int scroll_val_down = 0;
int scroll_val_up = 0;
u8 memAccessControl = 0;
struct file *fileBuffer;
int bufferRow = 0;
ILI9341Type ili9341;
/*******************************************************************************/
/* Function to write data and cmd to ILI9341 */
void ILI9341_WriteReg(ILI9341Type *device, char buff)
{
	gpiod_set_value(device->dcPin, LOW);
	spi_write(device->ili9341, &buff, 1);
}

void ILI9341_WriteData(ILI9341Type *device, char buff)
{
	gpiod_set_value(device->dcPin, HIGH);
	spi_write(device->ili9341, &buff, 1);
}

void ILI9341_WriteRawData(ILI9341Type *device, char *buff, int size)
{
	gpiod_set_value(device->dcPin, HIGH);
	spi_write(device->ili9341, buff, size);
}

void ILI9341_CmdMulBytes(ILI9341Type *device, char *buff, int size)
{
	gpiod_set_value(device->dcPin, LOW);
	spi_write(device->ili9341, buff, 1);			/* Send the first byte */
	gpiod_set_value(device->dcPin, HIGH);
	spi_write(device->ili9341, &buff[1], size - 1);	/* Send the rest */
}
/* Function to Read data from ILI9341 */
void ILI9341_Read(ILI9341Type *device, bool isCommand, char* senBuff, int sendSize, char* saveBuff, int saveSize)
{
	if (isCommand)						/* If command is sent */
		gpiod_set_value(device->dcPin, LOW);
	else								/* If data is sent */
	{
		gpiod_set_value(device->dcPin, HIGH);
	}
	spi_write(device->ili9341, senBuff, sendSize);
	spi_read(device->ili9341, saveBuff, saveSize);
	gpiod_set_value(device->dcPin, LOW);

}
/*******************************************************************************/
/* Functions to save buffer */
ssize_t ILI9341_saveBuffer(ILI9341Type *device, u8 *buff, int size, int offset, int where)
{
	switch(where) {
   		case 0: /* SEEK_SET */
    		device->fileBuffer->f_pos = offset;
    	break;

   		case 1: /* SEEK_CUR */
    		device->fileBuffer->f_pos += offset;
    	break;

   		case 2: /* SEEK_END */
    		device->fileBuffer->f_pos -= offset;
    	break;
	}
	return kernel_write(device->fileBuffer, buff, size, &device->fileBuffer->f_pos);
}

ssize_t ILI9341_readRowBuffer(ILI9341Type *device, u8 *toSaveBuff, int offset, int where)
{
	switch(where) {
   		case 0: /* SEEK_SET */
    		device->fileBuffer->f_pos = offset;
    	break;

   		case 1: /* SEEK_CUR */
    		device->fileBuffer->f_pos += offset;
    	break;

   		case 2: /* SEEK_END */
    		device->fileBuffer->f_pos -= offset;
    	break;
	}
	return kernel_read(device->fileBuffer, toSaveBuff, device->maxCol * 2, &device->fileBuffer->f_pos);
}
/*******************************************************************************/
/* Function to print char and image */
void ILI9341_DisplayPixel(ILI9341Type *device, u16 color)
{
	u8 buff[2] = {
		color >> 8,
		color & 0xff
	};
	spi_write(device->ili9341, buff, 2);
}

void ILI9341_DisplayMultiPixel(ILI9341Type *device, u8 *color, unsigned int size)
{
	spi_write(device->ili9341, color, size);
}

void ILI9341_printImage(ILI9341Type *device, u16* data, unsigned int size)
{
	int i = 0;
	int printed = 0;
	int toprint;
	/* Print something */
	while (i < size)
	{
		ILI9341_RamBuffer[i*2 ] = *data >> 8;
		ILI9341_RamBuffer[i*2 + 1] = *data & 0xff;
		i ++;
		data++;
	}
	ILI9341_saveBuffer(device, ILI9341_RamBuffer, ILI9341_DEF_ROW * ILI9341_DEF_COL * 2, 0, SEEK_CUR);
	ILI9341_WriteReg(device, 0x2C);
	gpiod_set_value(device->dcPin, HIGH);
	toprint = size * 2;
	while (printed < toprint)
	{
		if ( printed + SPI_MAX_TRANSFER_BYTE < toprint)
		{
			ILI9341_DisplayMultiPixel(device, &ILI9341_RamBuffer[printed], SPI_MAX_TRANSFER_BYTE);
			printed += SPI_MAX_TRANSFER_BYTE;
		}
		else
		{
			ILI9341_DisplayMultiPixel(device, &ILI9341_RamBuffer[printed], toprint - printed);
			break;
		}
	}
	ILI9341_SetCursor(device,0,0);
}

void ILI9341_printCharOverlay(ILI9341Type *device, char ch, u16 color, u16 bgColor)
{
	int i = 0, j = 0;
	unsigned char *ptr = NULL;
	unsigned char shift = 7;
	int cnt = 0;
	ptr = ascii_0808[(int)ch];
	if (ch == '\n')
	{
		ILI9341_Nextline(device);
	}
	else
	{
		if (device->col + device->fontColSize > device->maxCol)		/* Move to next line and print*/
		{
			if (device->row + device->fontRowSize >= device->maxRow)	/* Move to beginning of the screen */
			{
				ILI9341_SetWindow(device, 0, 0, device->fontRowSize -1, device->fontColSize -1);
				device->col = device->fontColSize;
			}
			else													/* Move to next line and print */
			{
				ILI9341_SetWindow(device, device->row + device->fontRowSize, 0, 
										device->row + 2 * device->fontRowSize, device->fontColSize -1);
				device->col = device->fontColSize;
			}
			if (device->col + device->fontColSize >= device->maxCol )
				device->row += device->fontRowSize;
		}
		else														/* Keep printing*/
		{
			ILI9341_SetWindow(device, device->row, device->col, 
									device->row + device->fontRowSize -1, device->col + device->fontColSize -1);
			device->col += device->fontColSize;
		}
		ILI9341_WriteReg(device, 0x2C);
		gpiod_set_value(device->dcPin, HIGH);
		for (i = 0; i < device->fontRowSize; i ++)
		{
			shift = device->fontColSize - 1;
			for (j = 0; j < device->fontColSize; j++)
			{
				if (*ptr & (1 << shift))
				{
					ILI9341_Font1208_Buffer[2 * cnt] = color >> 8;
					ILI9341_Font1208_Buffer[2 * cnt + 1] = color & 0xff;
				}
				else
				{
					ILI9341_Font1208_Buffer[2 * cnt] = bgColor >> 8;
					ILI9341_Font1208_Buffer[2 * cnt + 1] = bgColor & 0xff;
				}
				shift--;
				cnt++;
			}
			ptr++;
		}
		ILI9341_DisplayMultiPixel(device, ILI9341_Font1208_Buffer, cnt);
		ILI9341_DisplayMultiPixel(device, &ILI9341_Font1208_Buffer[cnt], cnt);
	}
}

void ILI9341_printStringOverlay(ILI9341Type *device, char* ch, u16 charColor, u16 bgColor)
{
	while (*ch != 0)
	{
		ILI9341_printCharOverlay(device, *ch, charColor, bgColor);
		ch++;
	}
}

void ILI9341_printCharScroll(ILI9341Type *device, char ch, u16 color, u16 bgColor)
{
	int i = 0, j = 0;
	u8 buffer[12 * 10 * 2];
	unsigned char *ptr = NULL;
	unsigned char shift = 7;
	int cnt = 0;
	ptr = ascii_0808[(int)ch];
	if (ch == '\n')
	{
		if (device->totalRow + device->fontRowSize >= device->maxRow)
		{
			if (device->row + device->fontRowSize >= device->maxRow)
				device->row = 0;
			else
				device->row += device->fontRowSize;
			ILI9341_SetCursor(device, device->row,0);
			ILI9341_FillBlankLine(device);
			ILI9341_ScrollDownToPrint(device, device->fontSize);
		}
		else
			device->row += device->fontRowSize;
		device->totalRow += device->fontRowSize;
		
		ILI9341_SetWindow(device, device->row, 0, 
									device->row + device->fontRowSize, device->fontColSize -1);
	}
	else
	{
		if (device->col + device->fontColSize > device->maxCol)		/* Move to next line and print*/
		{
			if (device->totalRow + device->fontRowSize >= device->maxRow)	/* Move to beginning of the screen */
			{
				if (device->row + device->fontRowSize >= device->maxRow)
					device->row = 0;
				else
					device->row += device->fontRowSize;
				// printk("device->row = %d\n",device->row);
				ILI9341_SetCursor(device, device->row,0);
				ILI9341_FillBlankLine(device);
				ILI9341_ScrollDownToPrint(device, device->fontSize);
				ILI9341_SetWindow(device, device->row, 0, 
									device->row + device->fontRowSize, device->fontColSize -1);
				device->col += device->fontColSize;
			}
			else													/* Move to next line and print */
			{
				ILI9341_SetWindow(device, device->row + device->fontRowSize, 0, 
										device->row + 2 * device->fontRowSize, device->fontColSize -1);
				device->col = device->fontColSize;
			}
			device->totalRow += device->fontRowSize;
		}
		else														/* Keep printing*/
		{
			ILI9341_SetWindow(device, device->row, device->col, 
									device->row + device->fontRowSize, device->col + device->fontColSize -1);
			device->col += device->fontColSize;
		}
		ILI9341_WriteReg(device, 0x2C);
		gpiod_set_value(device->dcPin, HIGH);
		for (i = 0; i < device->fontRowSize; i ++)
		{
			shift = device->fontColSize - 1;
			for (j = 0; j < device->fontColSize; j++)
			{
				if (*ptr & (1 << shift))
				{
					buffer[2 * cnt] = color >> 8;
					buffer[2 * cnt + 1] = color & 0xff;
				}
				else
				{
					buffer[2 * cnt] = bgColor >> 8;
					buffer[2 * cnt + 1] = bgColor & 0xff;
				}
				shift--;
				cnt++;
			}
			ILI9341_saveBuffer(device, &buffer[i*device->fontColSize*2], device->fontColSize * 2, 
						(device->totalRow + i) * device->maxCol * 2 + (device->col - device->fontColSize) * 2, SEEK_SET);
			ptr++;
		}
		ILI9341_DisplayMultiPixel(device, buffer, cnt);
		ILI9341_DisplayMultiPixel(device, &buffer[cnt], cnt);
	}
	// printk("char = %c, device->row = %d, device->col = %d, totalRow = %d\n",ch,device->row,device->col,device->totalRow);
}

void ILI9341_printStringScroll(ILI9341Type *device, char* ch, u16 charColor, u16 bgColor)
{
	char buff[100];
	int i =0;
	memset(buff, 0, 100);
	while (*ch != 0)
	{
		ILI9341_printCharScroll(device, *ch, charColor, bgColor);
		// if (ILI9341_saveBuffer(device, ch, 1) < 0)
		// 	printk("Error for kernel_write\n");
		ch++;
		i++;
	}
}

/*******************************************************************************/
/* Function to move cursor and set window size ILI9341 */
void ILI9341_SetWindow(ILI9341Type *device, int StartRow, int StartCol, int EndRow, int EndCol)
{
	char buffx[5] = {
		0x2A, 					/* Cmd to set Column Address */
		(StartCol) >> 8,
		StartCol & 0x00ff,
		(EndCol) >> 8,
		(EndCol) & 0x00ff
	};

	char buffy[5] = {
		0x2B, 					/* Cmd to set Row Address */
		(StartRow) >> 8,
		StartRow & 0x00ff,
		(EndRow) >> 8,
		(EndRow) & 0x00ff
	};
	ILI9341_CmdMulBytes(device, buffx, 5);
	ILI9341_CmdMulBytes(device, buffy, 5);
	device->col = StartCol;
	device->row = StartRow;
}

void ILI9341_SetCursor(ILI9341Type *device, int Row, int Col) 
{
	if (Col > device->maxCol) Col = 0;
	if (Row > device->maxRow) Row = 0;
	ILI9341_SetWindow(device, Row, Col, Row, Col);
	device->col = Col;
	device->row = Row;
}

void ILI9341_Nextline(ILI9341Type *device)
{
	if (device->row + device->fontRowSize < device->maxRow)
		ILI9341_SetCursor(device, device->row + device->fontRowSize, 0);
	else
		ILI9341_SetCursor(device, 0, 0);
}
/*******************************************************************************/
/* Function to rotate screen */
#define MADCTL_MY  			0x80
#define MADCTL_MX  			0x40
#define MADCTL_MV  			0x20
#define MADCTL_ML 			0x10
#define MADCTL_RGB 			0x00
#define MADCTL_BGR 			0x08
#define MADCTL_MH  			0x04
void ILI9341_RotateMode(ILI9341Type *device, int mode)
{
	unsigned char buffer[2] = {
		0x36

	};
	device->col = ILI9341_DEF_COL;
	device->row = ILI9341_DEF_ROW;
	switch (mode)
	{
	case 0:				/* No rotattion */
		device->col = ILI9341_DEF_COL;
		device->row = ILI9341_DEF_ROW;
		device->maxCol = ILI9341_DEF_COL;
		device->maxRow = ILI9341_DEF_ROW;
		break;
	case 1:				/* Rotate 90 */

		break;
	case 2:				/* Rotate 180 */

		break;
	case 3:				/* Rotate 270 */

		break;
	default:
		break;
	}
}
/*******************************************************************************/
/* Functions to scroll screen */
void ILI9341_ScrollUp(ILI9341Type *device, u16 val)
{
	unsigned char bufferSetCrollUp[2] =
	{
		0x36,
		(1<<3) | (1<<4)
	};
	unsigned char bufferReady[7] = 
	{
		0x33,
		0,
		0,
		(device->maxRow) >> 8,
		(device->maxRow) & 0x00ff, 
		0,
		0
	};
	unsigned char bufferStartScroll[3] = 
	{
		0x37,
	};
	scroll_val_up += val;
	if (scroll_val_up >= device->maxRow)
		scroll_val_up = 0;
	bufferStartScroll[1] = scroll_val_up >> 8;
	bufferStartScroll[2] = scroll_val_up & 0x00ff;
	if ((memAccessControl & (1<<4)) == 0)
		memAccessControl |= (1<<4);
	device->totalRow -= device->fontRowSize;
	ILI9341_CmdMulBytes(device, bufferSetCrollUp, 2);
	ILI9341_CmdMulBytes(device, bufferReady, 7);
	ILI9341_CmdMulBytes(device, bufferStartScroll, 3);
}

void ILI9341_ScrollDown(ILI9341Type *device, u16 val)
{
	
}

void ILI9341_ScrollDownToPrint(ILI9341Type *device, u16 val)
{
	unsigned char bufferSetCrollDown [2] =
	{
		0x36,
		(1<<3) | (0<<4)
	};
	unsigned char bufferReady [7] = 
	{
		0x33,
		0,
		0,
		(device->maxRow) >> 8,
		(device->maxRow) & 0x00ff, 
		0,
		0
	};
	unsigned char bufferStartScroll [3] = 
	{
		0x37,
	};
	scroll_val_down += val;
	if (scroll_val_down >= device->maxRow)
		scroll_val_down = 0;
	bufferStartScroll[1] = scroll_val_down >> 8,
	bufferStartScroll[2] = scroll_val_down & 0x00ff;
	if ((memAccessControl & (1<<4)) == (1<<4))
		memAccessControl ^= (1<<4);
	ILI9341_CmdMulBytes(device, bufferSetCrollDown, 2);
	ILI9341_CmdMulBytes(device, bufferReady, 7);
	ILI9341_CmdMulBytes(device, bufferStartScroll, 3);
}
/*******************************************************************************/
/* Function to reset ILI9341 */
void ILI9341_Reset(ILI9341Type *device)
{
	gpiod_set_value(device->resetPin, LOW);
	mdelay(100);
	gpiod_set_value(device->resetPin, HIGH);
	mdelay(50);
	ILI9341_WriteReg(device, 0x01); 		/* send command 0x01 to reset */
	mdelay(120);
}

void ILI9341_SleepMode(ILI9341Type *device, bool isSleep)
{
	if (isSleep)
	{
		ILI9341_WriteReg(device, 0x10);		/* Command to enter sleep mode */
	}
	else
	{
		ILI9341_WriteReg(device, 0x11);		/* Command to exit sleep mode */
	}
	mdelay(120);
}

void ILI9341_NormalDisplayMode(ILI9341Type *device, bool isNormalMode)
{
	if (isNormalMode)
	{
		ILI9341_WriteReg(device, 0x13);		/* Command to enter sleep mode */
	}
	else
	{
		ILI9341_WriteReg(device, 0x12);		/* Command to exit sleep mode */
	}
	mdelay(120);
}

void ILI9341_InverseMode(ILI9341Type *device, bool isInverse)
{
	if (isInverse)
	{
		ILI9341_WriteReg(device, 0x21);		/* Command to inverse display */
	}
	else
	{
		ILI9341_WriteReg(device, 0x20);		/* Command to not inverse display */
	}
	mdelay(120);
}

void ILI9341_DispalyOn(ILI9341Type *device, bool isON)
{
	if (isON)
	{
		ILI9341_WriteReg(device, 0x29);		/* Command to turn on display */
		
	}
	else
	{
		ILI9341_WriteReg(device, 0x28);		/* Command to turn off display */
		
	}
	mdelay(120);
}

void ILI9341_Tearing(ILI9341Type *device, bool isTearing)
{
	char buff[2] =
	{
		0x35,			/* Command to turn on tearing */
		0x01
	};
	if (isTearing)		/* Command to turn on tearing */
	{
		ILI9341_CmdMulBytes(device, buff, 2);
	}
	else
	{
		ILI9341_WriteReg(device, 0x34);		/* Command to turn off tearing */
	}
}

void ILI9341_GammaSet(ILI9341Type *device, int value)
{
	char buff[2] =
	{
		0x26,			/* Command to set gamma */
		value & 0b00001111
	};
	ILI9341_CmdMulBytes(device, buff, 2);
}

void ILI9341_Idle(ILI9341Type *device, bool isIdle)
{
	if (isIdle)
	{
		ILI9341_WriteReg(device, 0x39);		/* Command to turn off isIdle */
	}
	else
	{
		ILI9341_WriteReg(device, 0x38);		/* Command to turn off isIdle */
	}
}

void ILI9341_Set16bitColor(ILI9341Type *device)
{
	char buff[2] =
	{
		0x3A,
		0x55
	};
	ILI9341_CmdMulBytes(device, buff, 2);
}

void ILI9341_SetBrightness(ILI9341Type *device, int value)
{
	char buff[2] =
	{
		0x51,
		value & 0b11111111
	};
	ILI9341_CmdMulBytes(device, buff, 2);
}

void ILI9341_SetCTRL(ILI9341Type *device){
	char buff[2] =
	{
		0x53,
		0b00101100
	};
	ILI9341_CmdMulBytes(device, buff, 2);
}

void ILI9341_SetAdaptiveBrightnessControl(ILI9341Type *device){
	char buff[2] =
	{
		0x55,
		0b11					/* Setting for moving image */
	};
	ILI9341_CmdMulBytes(device, buff, 2);
}

void ILI9341_SetFrameRate(ILI9341Type *device)
{
	char buff[3] =
	{
		0xB1,
		0x00,					/* DIVA = fosc */
		0b00011011				/* fps = 70hz */
	};
	ILI9341_CmdMulBytes(device, buff, 3);
}

void ILI9341_FillColor(ILI9341Type *device, u16 color)
{
	int i = 0;
	ILI9341_SetWindow(device, 0, 0, device->maxRow, device->maxCol);
	ILI9341_WriteReg(device, 0x2C);
	gpiod_set_value(device->dcPin, HIGH);
	while (i < ILI9341_DEF_COL * ILI9341_DEF_ROW)
	{
		ILI9341_DisplayPixel(device, color);
		i++;
	}
}

void ILI9341_CreateWindowAndFill(ILI9341Type *device, u16 color, int StartRow, int StartCol, int EndRow, int EndCol)
{
	int i = 0;
	ILI9341_SetWindow(device, StartRow, StartCol, EndRow, EndCol);
	ILI9341_WriteReg(device, 0x2C);
	gpiod_set_value(device->dcPin, HIGH);
	while (i < ILI9341_DEF_COL * ILI9341_DEF_ROW)
	{
		ILI9341_DisplayPixel(device, color);
		i++;
	}
}

void ILI9341_PrintCursor(ILI9341Type *device, u16 color)
{
	u8 buff[64*2];
	int i = 0;
	for (i=0;i<64;i++)
	{
		buff[i*2] = color >> 8;
		buff[i*2 + 1] = color & 0x00ff;
	}
	ILI9341_SetWindow(device, device->row, device->col, 
				device->row + device->fontRowSize, device->col + device->fontColSize);
	ILI9341_WriteReg(device, 0x2C);
	gpiod_set_value(device->dcPin, HIGH);
	ILI9341_DisplayMultiPixel(device, buff,64*2);
	ILI9341_SetCursor(device, device->row,device->col);
}

int ILI9341_FillBlankLine(ILI9341Type *device)
{
	u8 buff[64*2];
	int i = 0;
	for (i=0;i<64;i++)
	{
		buff[i*2] = BLACK_16;
		buff[i*2 + 1] = BLACK_16;
	}
	i = 0;
	ILI9341_SetWindow(device, device->row, device->col, 
				device->row + device->fontRowSize, device->maxCol);
	ILI9341_WriteReg(device, 0x2C);
	gpiod_set_value(device->dcPin, HIGH);
	while (device->col < device->maxCol)
	{
		ILI9341_DisplayMultiPixel(device, buff,64*2);
		device->col += device->fontColSize;
		i++;
	}
	return i;
}

/* Initialize fintion */
void ILI9341_PowerInit(ILI9341Type *device)
{
	ILI9341_WriteReg(device, 0xCF); 	/* Cmd for power control B */
	ILI9341_WriteData(device, 0x00); 	/* Dummy */
	ILI9341_WriteData(device, 0xD9);	/* */
	ILI9341_WriteData(device, 0X30); 

	ILI9341_WriteReg(device, 0xED);  	/* Cmd for Power on sequence control */
	ILI9341_WriteData(device, 0x64); 	/* */
	ILI9341_WriteData(device, 0x03); 
	ILI9341_WriteData(device, 0X12); 
	ILI9341_WriteData(device, 0X81);

	ILI9341_WriteReg(device, 0xE8);  		//  Driver timing control A
	ILI9341_WriteData(device, 0x85); 
	ILI9341_WriteData(device, 0x10); 
	ILI9341_WriteData(device, 0x7A); 

	ILI9341_WriteReg(device, 0xCB);  		// Power control A
	ILI9341_WriteData(device, 0x39); 	
	ILI9341_WriteData(device, 0x2C); 
	ILI9341_WriteData(device, 0x00); 
	ILI9341_WriteData(device, 0x34); 
	ILI9341_WriteData(device, 0x02); 

	ILI9341_WriteReg(device, 0xF7);  		// Pump ratio control
	ILI9341_WriteData(device, 0x20);

	ILI9341_WriteReg(device, 0xEA);  		// Driver timing control B
	ILI9341_WriteData(device, 0x00); 
	ILI9341_WriteData(device, 0x00);

	ILI9341_WriteReg(device, 0xC0);    //Power control 
	ILI9341_WriteData(device, 0x1B);   //VRH[5:0] 

	ILI9341_WriteReg(device, 0xC1);    //Power control 
	ILI9341_WriteData(device, 0x12);   //SAP[2:0];BT[3:0] //0x01

	ILI9341_WriteReg(device, 0xC5);    //VCM control 
	ILI9341_WriteData(device, 0x26); 	 //3F
	ILI9341_WriteData(device, 0x26); 	 //3C

	ILI9341_WriteReg(device, 0xC7);    //VCM control2 
	ILI9341_WriteData(device, 0XB0); 

	ILI9341_WriteReg(device, 0x36);    // Memory Access Control 
	memAccessControl = 0x08;
	ILI9341_WriteData(device, 0x08); 

	ILI9341_WriteReg(device, 0x3A);   // Pixel Format Set : 18 bit or 16 bit
	ILI9341_WriteData(device, 0x55); 

	ILI9341_WriteReg(device, 0xB1);   // Frame Rate Control
	ILI9341_WriteData(device, 0x00);   
	ILI9341_WriteData(device, 0x1A); 

	ILI9341_WriteReg(device, 0xB6);    // Display Function Control 
	ILI9341_WriteData(device, 0x0A); 
	ILI9341_WriteData(device, 0xA2); 
	ILI9341_WriteReg(device, 0xF2);    // 3Gamma Function Disable 
	ILI9341_WriteData(device, 0x00); 

	ILI9341_WriteReg(device, 0x26);    //Gamma curve selected 
	ILI9341_WriteData(device, 0x01); 

	ILI9341_WriteReg(device, 0xE0); //Set Gamma
	ILI9341_WriteData(device, 0x1F);
	ILI9341_WriteData(device, 0x24);
	ILI9341_WriteData(device, 0x24);
	ILI9341_WriteData(device, 0x0D);
	ILI9341_WriteData(device, 0x12);
	ILI9341_WriteData(device, 0x09);
	ILI9341_WriteData(device, 0x52);
	ILI9341_WriteData(device, 0xB7);
	ILI9341_WriteData(device, 0x3F);
	ILI9341_WriteData(device, 0x0C);
	ILI9341_WriteData(device, 0x15);
	ILI9341_WriteData(device, 0x06);
	ILI9341_WriteData(device, 0x0E);
	ILI9341_WriteData(device, 0x08);
	ILI9341_WriteData(device, 0x00);

	ILI9341_WriteReg(device, 0XE1); //Set Gamma
	ILI9341_WriteData(device, 0x00);
	ILI9341_WriteData(device, 0x1B);
	ILI9341_WriteData(device, 0x1B);
	ILI9341_WriteData(device, 0x02);
	ILI9341_WriteData(device, 0x0E);
	ILI9341_WriteData(device, 0x06);
	ILI9341_WriteData(device, 0x2E);
	ILI9341_WriteData(device, 0x48);
	ILI9341_WriteData(device, 0x3F);
	ILI9341_WriteData(device, 0x03);
	ILI9341_WriteData(device, 0x0A);
	ILI9341_WriteData(device, 0x09);
	ILI9341_WriteData(device, 0x31);
	ILI9341_WriteData(device, 0x37);
	ILI9341_WriteData(device, 0x1F);

	ILI9341_WriteReg(device, 0x2B); 			// page address
	ILI9341_WriteData(device, 0x00);
	ILI9341_WriteData(device, 0x00);
	ILI9341_WriteData(device, 0x01);
	ILI9341_WriteData(device, 0x3f);

	ILI9341_WriteReg(device, 0x2A); 
	ILI9341_WriteData(device, 0x00);
	ILI9341_WriteData(device, 0x00);
	ILI9341_WriteData(device, 0x00);
	ILI9341_WriteData(device, 0xef);

	ILI9341_WriteReg(device, 0x11); //Exit Sleep
	mdelay(120);
	ILI9341_WriteReg(device, 0x29); //display on	

	ILI9341_WriteReg(device, 0x36);
	ILI9341_WriteData(device, (1<<3)|(0<<6)|(0<<7));
}

void ILI9341_Init(ILI9341Type *device)
{
	ILI9341_Reset(device);
	/* Power setting */
	ILI9341_PowerInit(device);
	/* Display setting */
	ILI9341_RotateMode(device, 0);
	ILI9341_SleepMode(device, false);
	ILI9341_NormalDisplayMode(device, true);
	ILI9341_DispalyOn(device, true);
	ILI9341_InverseMode(device, false);
	ILI9341_GammaSet(device, 4);
	ILI9341_Tearing(device, true);
	ILI9341_Idle(device, false);
	ILI9341_Set16bitColor(device);
	ILI9341_SetBrightness(device, 1);
	ILI9341_SetCTRL(device);
	ILI9341_SetAdaptiveBrightnessControl(device);
	ILI9341_SetFrameRate(device);
	device->fontSize = 8;
	device->fontRowSize = fontInfo[0].RowSize;
	device->fontColSize = fontInfo[0].ColSize;
	device->totalRow = 0;
	/* Print something */
	/* Set cursor to beginning of the screen */
	ILI9341_SetWindow(device, 0, 0, device->maxRow, device->maxCol);
	ILI9341_printImage(device, LinuxLogo, ILI9341_DEF_COL * ILI9341_DEF_ROW);
}

void ILI9341_Deinit(ILI9341Type *device)
{
	ILI9341_Reset(device);
	gpiod_put(device->resetPin);
	gpiod_put(device->dcPin);
}
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/