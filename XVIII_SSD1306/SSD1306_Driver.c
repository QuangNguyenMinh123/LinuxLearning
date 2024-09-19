#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/of_device.h>
// #include "SSD1306.h"
/*******************************************************************************/
#define SSD1306_ADDRESS				0x3c
#define SSD1306_MAX_LINE			7
#define SSD1306_MAX_SEG				128
#define SSD1306_DEF_FONT_SIZE		5
typedef struct SSD1306_Type{
	struct i2c_client *device;
	uint8_t line_num;
	uint8_t cursor_pos;
	uint8_t font_size;
}SSD1306_Type;
SSD1306_Type ssd1306 =
{
	NULL,
	0,
	0,
	SSD1306_DEF_FONT_SIZE
};
/*******************************************************************************/
static const unsigned char ssd1306_font[][SSD1306_DEF_FONT_SIZE] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x27, 0x00, 0x00},
};

static const unsigned char image[] = 
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x71, 0x00, 0x00, 0x01, 0x07, 0x1f, 0x0f, 0x0f, 0x0f, 
0x0f, 0x0f, 0x0f, 0x1f, 0x07, 0x01, 0x00, 0x00, 0x70, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x8f, 0x87, 0xc7, 0xc7, 0xe3, 0xe3, 0xe3, 
0xe3, 0xe3, 0xe3, 0xc7, 0xc7, 0xc1, 0x80, 0x80, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc1, 0xc7, 0xc7, 0xe3, 0xe3, 0xe3, 
0xe3, 0x63, 0x63, 0xc7, 0xc7, 0x87, 0x8f, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0xf8, 0xfe, 0xff, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 
0x87, 0x87, 0x87, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xfc, 0xf8, 0x20, 0x00, 0x00, 
0x00, 0x00, 0x20, 0x70, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 
0x00, 0x00, 0x00, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0xfe, 0xf8, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xf8, 0xf0, 0xe0, 0xc3, 0x87, 0x8f, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 
0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x07, 0x03, 0x03, 0x21, 0x30, 0x38, 0x3c, 0x3c, 
0x3c, 0x3c, 0x38, 0x30, 0x20, 0x21, 0x03, 0x07, 0x0f, 0x0f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 
0x30, 0x30, 0x30, 0x1f, 0x1f, 0x0f, 0x8f, 0x87, 0xc3, 0xe0, 0xf0, 0xf8, 0xfe, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 
0xfe, 0xfe, 0xc0, 0x00, 0x00, 0x08, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 
0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x08, 0x00, 0x00, 0xc0, 0xf8, 0xfe, 
0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x08, 0x18, 0x18, 0x38, 0x38, 
0x38, 0x38, 0x18, 0x18, 0x08, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x3f, 0xff, 0x7f, 0x3f, 0x3f, 0xff, 
0xff, 0x3f, 0x3f, 0x3f, 0xff, 0x7f, 0x3f, 0x3f, 0xff, 0xff, 0x3f, 0x3f, 0x3f, 0xff, 0x7f, 0x3f, 
0x3f, 0xff, 0xff, 0x3f, 0xbf, 0xff, 0xff, 0xff, 0x3f, 0x3f, 0xbf, 0x3f, 0x3f, 0x3f, 0xbf, 0x3f, 
0x3f, 0xff, 0xff, 0x37, 0xff, 0x7f, 0x3f, 0xbf, 0xbf, 0x3f, 0x3f, 0xff, 0xfe, 0x3e, 0x3c, 0xbc, 
0x3c, 0x3c, 0xfe, 0xfe, 0x07, 0x3f, 0xbf, 0xbf, 0x3f, 0x7f, 0xff, 0x37, 0xff, 0xff, 0x3f, 0x3f, 
0xbf, 0x3f, 0x3f, 0xff, 0xff, 0x3f, 0x3f, 0xbf, 0xbf, 0x3f, 0x7f, 0xbf, 0x0f, 0xbf, 0xbf, 0xff, 
0x37, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x3f, 0xbf, 0xbf, 0x3f, 0x7f, 0xff, 0x3f, 0x3f, 
0xbf, 0xff, 0x3f, 0xbf, 0xbf, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xe0, 0xe3, 0xe0, 0xfe, 0xf0, 0xe3, 
0xe0, 0xfc, 0xfe, 0xe0, 0xe3, 0xe0, 0xfe, 0xf0, 0xe3, 0xe0, 0xfc, 0xfe, 0xe0, 0xe3, 0xe0, 0xfe, 
0xf0, 0xe3, 0xe0, 0xfc, 0xef, 0xef, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 
0xe0, 0xff, 0xff, 0xe0, 0xff, 0xf7, 0xe4, 0xec, 0xed, 0xe1, 0xe1, 0xff, 0xf8, 0xe0, 0xe7, 0xef, 
0xe7, 0xe7, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xe0, 0xe0, 0xff, 0xe0, 0xff, 0xf3, 0xe1, 0xed, 
0xed, 0xe5, 0xe0, 0xe1, 0xef, 0xe0, 0xff, 0xff, 0xff, 0xe0, 0xe0, 0xff, 0xe0, 0xef, 0xef, 0xff, 
0xe0, 0xff, 0xff, 0xef, 0xef, 0xff, 0xf8, 0xe0, 0xe7, 0xef, 0xef, 0xe0, 0xf0, 0xff, 0xe0, 0xff, 
0xff, 0xf8, 0x20, 0x2f, 0x6f, 0x27, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
/*******************************************************************************/
/* SSD1306 library */
void SSD1306_i2c_Write(SSD1306_Type *ssd1306, unsigned char *buf, unsigned int len)
{
	i2c_master_send(ssd1306->device, buf, len);
}

void SSD1306_Write(SSD1306_Type *ssd1306, bool is_cmd, unsigned char data)
{
	unsigned char buf[2] = {0};
	if (is_cmd == true)
		buf[0] = 0;
	else
		buf[0] = 0x40;
	buf[1] = data;
	SSD1306_i2c_Write(ssd1306, buf, 2);
}

static int SSD1306_Read(SSD1306_Type *ssd1306, unsigned char *out_buf, unsigned int len)
{
	return i2c_master_recv(ssd1306->device, out_buf, len);
}

void SSD1306_Clear(SSD1306_Type *ssd1306)
{
	unsigned int total = 128 * 8;
	int i;
	for (i = 0; i < total; i++)
	{
		SSD1306_Write(ssd1306, false, 0);
	}
}

void SSD1306_Init(SSD1306_Type *ssd1306)
{
	SSD1306_Write(ssd1306, true, 0xAE);			/* Command to turn off display */
	SSD1306_Write(ssd1306, true, 0xD5);			/* Set display clock divide ratio and oscillator frequency */
	SSD1306_Write(ssd1306, true, 0x80);			/* Default setting for display clock divide ratio and oscillator frequency */
	SSD1306_Write(ssd1306, true, 0xA8);			/* Set multiplex ratio */
	SSD1306_Write(ssd1306, true, 0x3F);			/* 64 COM line */
	SSD1306_Write(ssd1306, true, 0xD3);			/* Set display offset */
	SSD1306_Write(ssd1306, true, 0x00);			/* 0 offset */
	SSD1306_Write(ssd1306, true, 0x40);			/* set first line as the start line of the display */
	SSD1306_Write(ssd1306, true, 0x8D);			/* charge pump */
	SSD1306_Write(ssd1306, true, 0x14);			/* enable charge pump during display on */
	SSD1306_Write(ssd1306, true, 0x20);			/* Set memory addressing mode */
	SSD1306_Write(ssd1306, true, 0x00);			/* Horizontal addressing mode */
	SSD1306_Write(ssd1306, true, 0xA1);			/* Set segment remap with column address 127 mapped to segment 0 */
	SSD1306_Write(ssd1306, true, 0xC8);			/* Set com output scan direction, scan from com 63 to com 0 */
	SSD1306_Write(ssd1306, true, 0xDA);			/* Set com pins hardware configuration */
	SSD1306_Write(ssd1306, true, 0x12);			/* Alternative com pin configuration, disable com left/right remap */
	SSD1306_Write(ssd1306, true, 0x81);			/* Set contrast control */
	SSD1306_Write(ssd1306, true, 0x80);			/* Set contrast to 128 */
	SSD1306_Write(ssd1306, true, 0xD9);			/* Set precharge period */
	SSD1306_Write(ssd1306, true, 0xF1);			/* Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK */
	SSD1306_Write(ssd1306, true, 0xDB);			/* Set Vcomh deselect level */
	SSD1306_Write(ssd1306, true, 0x20);			/* Vcomh deselect level ~ 0.77 Vcc */
	SSD1306_Write(ssd1306, true, 0xA4);			/* Entire display ON, resume RAM content display */
	SSD1306_Write(ssd1306, true, 0xA6);			/* Set display in normal mode, 1 = ON, 0 = OFF */
	SSD1306_Write(ssd1306, true, 0x2E);			/* Deactive Scroll */
	SSD1306_Write(ssd1306, true, 0xAF);			/* Display ON in normal mode */
	SSD1306_Clear(ssd1306);
	ssd1306->line_num = 0;
	ssd1306->cursor_pos = 0;
	ssd1306->font_size = SSD1306_DEF_FONT_SIZE;
}

static void SSD1306_set_cursor(SSD1306_Type *ssd1306, uint8_t line_numb, uint8_t cursor_position)
{
	if ((line_numb <= SSD1306_MAX_LINE) && (cursor_position < SSD1306_MAX_SEG))
	{
		ssd1306->line_num = line_numb;
		ssd1306->cursor_pos = cursor_position;
		SSD1306_Write(ssd1306, true, 0x21);					/* cmd for the column start and end address */
		SSD1306_Write(ssd1306, true, cursor_position);		/* Column start address */
		SSD1306_Write(ssd1306, true, SSD1306_MAX_SEG-1);	/* Column end address */
		SSD1306_Write(ssd1306, true, 0x22);					/* cmd for the page start and end address */
		SSD1306_Write(ssd1306, true, line_numb);				/* Page start address */
		SSD1306_Write(ssd1306, true, SSD1306_MAX_LINE);		/* Page end address */
	}
}

static void SSD1306_GotoNextLine(SSD1306_Type *ssd1306)
{
	ssd1306->line_num ++;
	ssd1306->line_num = (ssd1306->line_num & SSD1306_MAX_LINE);
	SSD1306_set_cursor(ssd1306, ssd1306->line_num, 0);
}

static void SSD1306_PrintChar(SSD1306_Type *ssd1306, unsigned char c)
{
	uint8_t dataByte = 1;
	uint8_t temp = 0;
	if (((ssd1306->cursor_pos + ssd1306->font_size) >= SSD1306_MAX_SEG ) || (c=='\n'))
		SSD1306_GotoNextLine(ssd1306);
	if (c != '\n') 
	{
		c -= 0x20;
		do {
			dataByte = ssd1306_font[c][temp];				
			SSD1306_Write(ssd1306, false, dataByte);
			ssd1306->cursor_pos ++;
			temp ++;
		}
		while (temp < ssd1306->font_size);
		SSD1306_Write(ssd1306, false, 0);
		ssd1306->cursor_pos ++;
	}
}

static void SSD1306_PrintString(SSD1306_Type *ssd1306, unsigned char *str)
{
	while (*str)
	{
		SSD1306_PrintChar(ssd1306, *str);
		str++;
	}
}

static void SSD1306_PrintImage(SSD1306_Type *ssd1306, unsigned char *str, int len)
{
	int i = 0;
	SSD1306_set_cursor(ssd1306, 0, 0);
	while (i < len)
	{
		SSD1306_Write(ssd1306, false, *str);
		ssd1306->cursor_pos ++;
		i++;
		str++;
	}
}

static void SSD1306_SetBrightness(SSD1306_Type *ssd1306, uint8_t brightness)
{
	SSD1306_Write(ssd1306, true, 0x81);
	SSD1306_Write(ssd1306, true, brightness);
}
/*******************************************************************************/
/* Declate the probe and remove functions */
static int SSD1306_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int SSD1306_remove(struct i2c_client *client);
/*******************************************************************************/


static struct of_device_id device_id[] = {
	{
		.compatible = "QuangNM13,SSD1306",
	}, { /* sentinel */ }
};

static struct i2c_device_id i2c_id[] = {
	{"QuangNM13,SSD1306", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, i2c_id);

static struct i2c_driver ssd1306_driver = {
	.probe = SSD1306_probe,
	.remove = SSD1306_remove,
	.id_table = i2c_id,
	.driver = {
		.name = "ssd1306_driver",
		.of_match_table = device_id,
	},
};
/* This will create the init and exit function automatically */
module_i2c_driver(ssd1306_driver);
static struct proc_dir_entry *proc_file;
/*******************************************************************************/
/**
 * @brief Read ssd1306 data
 */
static ssize_t SSD1306_ProcRead(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[3];
	int cnt;
	printk("SSD1306_ProcRead: \n");
	cnt = copy_to_user(user_buffer, buffer, 3);
	return count;
}

static ssize_t SSD1306_ProcWrite(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[3];
	int cnt;
	printk("SSD1306_ProcWrite: \n");
	cnt = copy_to_user(user_buffer, buffer, 3);
	return count;
}

static struct file_operations fops = {
	.write = SSD1306_ProcWrite,
	.read = SSD1306_ProcRead,
};
/*******************************************************************************/
/**
 * @brief This function is called on loading the driver 
 */
static int SSD1306_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("SSD1306_probe\n");
	if(client->addr != SSD1306_ADDRESS) {
		printk("SSD1306_probe: Cannot find ssd1306 device!\n");
		return -1;
	}
	ssd1306.device = client;
	/* Creating procfs file */
	proc_file = proc_create("ssd1306", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("SSD1306_probe: Error creating /proc/ssd1306\n");
		return -ENOMEM;
	}
	SSD1306_Init(&ssd1306);
	SSD1306_PrintImage(&ssd1306, image, sizeof(image));
	return 0;
}

/**
 * @brief This function is called on unloading the driver 
 */
static int SSD1306_remove(struct i2c_client *client) {
	printk("SSD1306_remove\n");
	proc_remove(proc_file);
	return 0;
}
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A driver for ssd1306 using device tree");
/*******************************************************************************/