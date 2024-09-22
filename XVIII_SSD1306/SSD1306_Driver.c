#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/of_device.h>
// #include "SSD1306.h"
/*******************************************************************************/
#define SSD1306_MAX_LINE				7
#define SSD1306_MAX_SEG					128
#define SSD1306_DEF_FONT_SIZE			5
#define SSD1306_ADDRESS					0x3c
#define CLEAR_SCREEN 					_IO('S', 'c')
#define ON_DISPLAY			 			_IO('S', 'n')
#define OFF_DISPLAY			 			_IO('S', 'f')
/*******************************************************************************/
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

struct foo_type {
	struct kobject *kobj;
}mdev;
/*******************************************************************************/
static void SSD1306_ScrollUp(SSD1306_Type *ssd1306, int val);
static void SSD1306_ScrollDown(SSD1306_Type *ssd1306, int val);
static void SSD1306_ScrollLeft(SSD1306_Type *ssd1306, int val);
static void SSD1306_ScrollRight(SSD1306_Type *ssd1306, int val);
static void SSD1306_SetBrightness(SSD1306_Type *ssd1306, uint8_t brightness);
/*******************************************************************************/
static ssize_t scroll_vertical_up_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t scroll_vertical_down_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t scroll_horizontal_left_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t scroll_horizontal_right_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t brightness_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
/*******************************************************************************/
struct kobj_attribute scroll_up_attr = __ATTR(scroll_up, 0660, NULL, scroll_vertical_up_store);
struct kobj_attribute scroll_down_attr = __ATTR(scroll_down, 0660, NULL, scroll_vertical_down_store);
struct kobj_attribute scroll_left_attr = __ATTR(scroll_left, 0660, NULL, scroll_horizontal_left_store);
struct kobj_attribute scroll_right_attr = __ATTR(scroll_right, 0660, NULL, scroll_horizontal_right_store);
struct kobj_attribute brightness_attr = __ATTR(brightness, 0660, NULL, brightness_store);
/*******************************************************************************/
static ssize_t scroll_vertical_up_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int val = 0;
	sscanf(buf, "%i", &val);
	SSD1306_ScrollUp(&ssd1306, val);
	return count;
}

static ssize_t scroll_vertical_down_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int val = 0;
	sscanf(buf, "%i", &val);
	SSD1306_ScrollDown(&ssd1306, val);
	return count;
}

static ssize_t scroll_horizontal_left_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int val = 0;
	sscanf(buf, "%i", &val);
	SSD1306_ScrollLeft(&ssd1306, val);
	return count;
}

static ssize_t scroll_horizontal_right_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int val = 0;
	sscanf(buf, "%i", &val);
	SSD1306_ScrollRight(&ssd1306, val);
	return count;
}

static ssize_t brightness_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int val = 0;
	sscanf(buf, "%i", &val);
	printk("brightness = %d\n",	val);
	SSD1306_SetBrightness(&ssd1306, val);
	return count;
}

static struct attribute *attrs[] = {
	&scroll_up_attr.attr,
	&scroll_down_attr.attr,
	&scroll_left_attr.attr,
	&scroll_right_attr.attr,
	&brightness_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};
/*******************************************************************************/
static const unsigned char ssd1306_font[][SSD1306_DEF_FONT_SIZE] =
{
    {0x00, 0x00, 0x00, 0x00, 0x00},   // space
    {0x00, 0x00, 0x2f, 0x00, 0x00},   // !
    {0x00, 0x07, 0x00, 0x07, 0x00},   // "
    {0x14, 0x7f, 0x14, 0x7f, 0x14},   // #
    {0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $
    {0x23, 0x13, 0x08, 0x64, 0x62},   // %
    {0x36, 0x49, 0x55, 0x22, 0x50},   // &
    {0x00, 0x05, 0x03, 0x00, 0x00},   // '
    {0x00, 0x1c, 0x22, 0x41, 0x00},   // (
    {0x00, 0x41, 0x22, 0x1c, 0x00},   // )
    {0x14, 0x08, 0x3E, 0x08, 0x14},   // *
    {0x08, 0x08, 0x3E, 0x08, 0x08},   // +
    {0x00, 0x00, 0xA0, 0x60, 0x00},   // ,
    {0x08, 0x08, 0x08, 0x08, 0x08},   // -
    {0x00, 0x60, 0x60, 0x00, 0x00},   // .
    {0x20, 0x10, 0x08, 0x04, 0x02},   // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
    {0x42, 0x61, 0x51, 0x49, 0x46},   // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
    {0x27, 0x45, 0x45, 0x45, 0x39},   // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
    {0x01, 0x71, 0x09, 0x05, 0x03},   // 7
    {0x36, 0x49, 0x49, 0x49, 0x36},   // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E},   // 9
    {0x00, 0x36, 0x36, 0x00, 0x00},   // :
    {0x00, 0x56, 0x36, 0x00, 0x00},   // ;
    {0x08, 0x14, 0x22, 0x41, 0x00},   // <
    {0x14, 0x14, 0x14, 0x14, 0x14},   // =
    {0x00, 0x41, 0x22, 0x14, 0x08},   // >
    {0x02, 0x01, 0x51, 0x09, 0x06},   // ?
    {0x32, 0x49, 0x59, 0x51, 0x3E},   // @
    {0x7C, 0x12, 0x11, 0x12, 0x7C},   // A
    {0x7F, 0x49, 0x49, 0x49, 0x36},   // B
    {0x3E, 0x41, 0x41, 0x41, 0x22},   // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C},   // D
    {0x7F, 0x49, 0x49, 0x49, 0x41},   // E
    {0x7F, 0x09, 0x09, 0x09, 0x01},   // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A},   // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F},   // H
    {0x00, 0x41, 0x7F, 0x41, 0x00},   // I
    {0x20, 0x40, 0x41, 0x3F, 0x01},   // J
    {0x7F, 0x08, 0x14, 0x22, 0x41},   // K
    {0x7F, 0x40, 0x40, 0x40, 0x40},   // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F},   // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E},   // O
    {0x7F, 0x09, 0x09, 0x09, 0x06},   // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46},   // R
    {0x46, 0x49, 0x49, 0x49, 0x31},   // S
    {0x01, 0x01, 0x7F, 0x01, 0x01},   // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F},   // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F},   // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F},   // W
    {0x63, 0x14, 0x08, 0x14, 0x63},   // X
    {0x07, 0x08, 0x70, 0x08, 0x07},   // Y
    {0x61, 0x51, 0x49, 0x45, 0x43},   // Z
    {0x00, 0x7F, 0x41, 0x41, 0x00},   // [
    {0x55, 0xAA, 0x55, 0xAA, 0x55},   // Backslash (Checker pattern)
    {0x00, 0x41, 0x41, 0x7F, 0x00},   // ]
    {0x04, 0x02, 0x01, 0x02, 0x04},   // ^
    {0x40, 0x40, 0x40, 0x40, 0x40},   // _
    {0x00, 0x03, 0x05, 0x00, 0x00},   // `
    {0x20, 0x54, 0x54, 0x54, 0x78},   // a
    {0x7F, 0x48, 0x44, 0x44, 0x38},   // b
    {0x38, 0x44, 0x44, 0x44, 0x20},   // c
    {0x38, 0x44, 0x44, 0x48, 0x7F},   // d
    {0x38, 0x54, 0x54, 0x54, 0x18},   // e
    {0x08, 0x7E, 0x09, 0x01, 0x02},   // f
    {0x18, 0xA4, 0xA4, 0xA4, 0x7C},   // g
    {0x7F, 0x08, 0x04, 0x04, 0x78},   // h
    {0x00, 0x44, 0x7D, 0x40, 0x00},   // i
    {0x40, 0x80, 0x84, 0x7D, 0x00},   // j
    {0x7F, 0x10, 0x28, 0x44, 0x00},   // k
    {0x00, 0x41, 0x7F, 0x40, 0x00},   // l
    {0x7C, 0x04, 0x18, 0x04, 0x78},   // m
    {0x7C, 0x08, 0x04, 0x04, 0x78},   // n
    {0x38, 0x44, 0x44, 0x44, 0x38},   // o
    {0xFC, 0x24, 0x24, 0x24, 0x18},   // p
    {0x18, 0x24, 0x24, 0x18, 0xFC},   // q
    {0x7C, 0x08, 0x04, 0x04, 0x08},   // r
    {0x48, 0x54, 0x54, 0x54, 0x20},   // s
    {0x04, 0x3F, 0x44, 0x40, 0x20},   // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C},   // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C},   // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C},   // w
    {0x44, 0x28, 0x10, 0x28, 0x44},   // x
    {0x1C, 0xA0, 0xA0, 0xA0, 0x7C},   // y
    {0x44, 0x64, 0x54, 0x4C, 0x44},   // z
    {0x00, 0x10, 0x7C, 0x82, 0x00},   // {
    {0x00, 0x00, 0xFF, 0x00, 0x00},   // |
    {0x00, 0x82, 0x7C, 0x10, 0x00},   // }
    {0x00, 0x06, 0x09, 0x09, 0x06}    // ~ (Degrees)
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

static void SSD1306_DeactiveScroll(SSD1306_Type *ssd1306)
{
	SSD1306_Write(ssd1306, true, 0x2E);
}

static void SSD1306_ActiveScroll(SSD1306_Type *ssd1306)
{
	SSD1306_Write(ssd1306, true, 0x2F);
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
	SSD1306_DeactiveScroll(ssd1306);
	while (*str)
	{
		SSD1306_PrintChar(ssd1306, *str);
		str++;
	}
}

static void SSD1306_PrintImage(SSD1306_Type *ssd1306,const unsigned char *str, int len)
{
	int i = 0;
	SSD1306_DeactiveScroll(ssd1306);
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

static void SSD1306_SaveRAMContent(SSD1306_Type *ssd1306)
{
	unsigned int total = 1024;
	char saveBuffer[1024] = {0};
	unsigned char buf[2] = {};
	int i;
	SSD1306_Read(ssd1306, saveBuffer, 10);
	printk("saveBuffer = %d\n",saveBuffer[0]);
	printk("saveBuffer = %d\n",saveBuffer[1]);
	printk("saveBuffer = %d\n",saveBuffer[2]);
	// for (i = 0; i < total; i++)
	// {
	// 	SSD1306_Read(ssd1306, )
	// }
}



static void SSD1306_ScrollUp(SSD1306_Type *ssd1306, int speed)
{
	/* speed 1->8 */
	SSD1306_Write(ssd1306, true, 0x29);

	SSD1306_Write(ssd1306, true, 0x00);			/* A[7:0], dummy */
	SSD1306_Write(ssd1306, true, 0x00);			/* B[2:0], start page = 0 */
	if ( (speed & 0b111) == 1)					/* C[2:0], time interval */
	{
		SSD1306_Write(ssd1306, true, 0b011);	/* 256 frames/scroll */
	}
	else if ((speed & 0b111) == 2)				/* 128 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b010);
	}
	else if ((speed & 0b111) == 3)				/* 64 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b001);
	}
	else if ((speed & 0b111) == 4)				/* 25 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b110);
	}
	else if ((speed & 0b111) == 5)				/* 5 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0x0);
	}
	else if ((speed & 0b111) == 6)				/* 4 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b101);
	}
	else if ((speed & 0b111) == 7)				/* 3 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b100);
	}
	else										/* 2 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b111);
	}
	SSD1306_Write(ssd1306, true, 0b111);		/* D[2:0], end page = 7 */
	SSD1306_Write(ssd1306, true, 0x01);			/* E[2:0], Vertical scrolling offset  */
}

static void SSD1306_ScrollDown(SSD1306_Type *ssd1306, int speed)
{

}

static void SSD1306_ScrollLeft(SSD1306_Type *ssd1306, int speed)
{
	SSD1306_DeactiveScroll(ssd1306);
	/* speed 1->8 */
	SSD1306_Write(ssd1306, true, 0x27);
	SSD1306_Write(ssd1306, true, 0x00);
	SSD1306_Write(ssd1306, true, 0x00);
	if ( (speed & 0b111) == 1)					/* 256 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b011);
	}
	else if ((speed & 0b111) == 2)				/* 128 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b010);
	}
	else if ((speed & 0b111) == 3)				/* 64 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b001);
	}
	else if ((speed & 0b111) == 4)				/* 25 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b110);
	}
	else if ((speed & 0b111) == 5)				/* 5 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0x0);
	}
	else if ((speed & 0b111) == 6)				/* 4 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b101);
	}
	else if ((speed & 0b111) == 7)				/* 3 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b100);
	}
	else										/* 2 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b111);
	}
	SSD1306_Write(ssd1306, true, 0b111);
	SSD1306_Write(ssd1306, true, 0x00);
	SSD1306_Write(ssd1306, true, 0xff);
	SSD1306_ActiveScroll(ssd1306);
}

static void SSD1306_ScrollRight(SSD1306_Type *ssd1306, int speed)
{
	SSD1306_DeactiveScroll(ssd1306);
	/* speed 1->8 */
	SSD1306_Write(ssd1306, true, 0x26);
	SSD1306_Write(ssd1306, true, 0x00);
	SSD1306_Write(ssd1306, true, 0x00);
	if ( (speed & 0b111) == 1)					/* 256 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b011);
	}
	else if ((speed & 0b111) == 2)				/* 128 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b010);
	}
	else if ((speed & 0b111) == 3)				/* 64 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b001);
	}
	else if ((speed & 0b111) == 4)				/* 25 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b110);
	}
	else if ((speed & 0b111) == 5)				/* 5 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0x0);
	}
	else if ((speed & 0b111) == 6)				/* 4 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b101);
	}
	else if ((speed & 0b111) == 7)				/* 3 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b100);
	}
	else										/* 2 frames/scroll */
	{
		SSD1306_Write(ssd1306, true, 0b111);
	}
	SSD1306_Write(ssd1306, true, 0b111);
	SSD1306_Write(ssd1306, true, 0x00);
	SSD1306_Write(ssd1306, true, 0xff);
	SSD1306_ActiveScroll(ssd1306);
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
 * @brief Write ssd1306 data
 */
static ssize_t SSD1306_ProcWrite(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[100];
	int cnt;
	memset(buffer, 0 , sizeof(buffer));
	printk("SSD1306_ProcWrite: \n");
	cnt = copy_from_user(buffer, user_buffer, count);
	printk("%s: \n",buffer);
	return count;
}

static long int SSD1306_Ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	if (cmd == CLEAR_SCREEN)
	{
		printk("SSD1306_Ioctl: clear\n");
		SSD1306_Clear(&ssd1306);
	}
	else if (cmd == ON_DISPLAY)
	{
		printk("SSD1306_Ioctl: on\n");
		SSD1306_Write(&ssd1306, true, 0xAF);
	}
	else if (cmd == OFF_DISPLAY)
	{
		printk("SSD1306_Ioctl: off\n");
		SSD1306_Write(&ssd1306, true, 0xAE);
	}
	return 0;
}

static struct file_operations fops = {
	.write = SSD1306_ProcWrite,
	.read = NULL,
	.unlocked_ioctl = SSD1306_Ioctl,
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
	/* test read ram */
	// SSD1306_SaveRAMContent(&ssd1306);
	/* scroll */
	SSD1306_DeactiveScroll(&ssd1306);
	SSD1306_ScrollUp(&ssd1306, 5);
	SSD1306_ActiveScroll(&ssd1306);

	/* 1. Create directory under /sys */
	mdev.kobj = kobject_create_and_add("SSD1306_Device",NULL);
	/* 2. Creating group sys entry under /sys/bbb_gpio */
	if (sysfs_create_group(mdev.kobj, &attr_group))
	{
		pr_err("Cannot create group attribute...\n");
		goto rm_kboj;
	}
	return 0;
rm_kboj:
	kobject_put(mdev.kobj);
	return -1;
}

/**
 * @brief This function is called on unloading the driver 
 */
static int SSD1306_remove(struct i2c_client *client) {
	printk("SSD1306_remove\n");
	proc_remove(proc_file);
	sysfs_remove_group(mdev.kobj,&attr_group);
	kobject_put(mdev.kobj);
	return 0;
}
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A driver for ssd1306 using device tree");
/*******************************************************************************/
/* https://embetronicx.com/tutorials/linux/device-drivers/ssd1306-i2c-linux-device-driver-using-raspberry-pi/ */