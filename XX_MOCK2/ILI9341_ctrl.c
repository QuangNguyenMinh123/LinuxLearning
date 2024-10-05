#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/pinctrl/consumer.h>
#include "ILI9341.h"
#include "ILI9341_GUI.h"
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A SPI driver for ILI9341 LCD");
/*******************************************************************************/
#define COMPATIBLE			"QuangNM13,ILI9341"
/*******************************************************************************/

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
ILI9341Type ili9341;
/*******************************************************************************/
static ssize_t reset_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t clear_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t inverse_on_off_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t display_on_off_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t fill_color_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t rotate_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t set_brightness_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t init_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
/*******************************************************************************/
static int ILI9341_Driver_probe(struct spi_device *pdev);
static int ILI9341_Driver_remove(struct spi_device *pdev);
static long int ILI9341_Driver_Ioctl(struct file *file, unsigned cmd, unsigned long arg);
static ssize_t ILI9341_Driver_ProcWrite(struct file *File, const char *user_buffer, size_t count, loff_t *offs);
/*******************************************************************************/
struct kobject *kobj = NULL;	/* pointer point to /sys/ili9341 */
struct kobj_attribute reset_attr = __ATTR(reset, 0660, NULL, reset_store);
struct kobj_attribute clear_attr = __ATTR(clear, 0660, NULL, clear_store);
struct kobj_attribute inverse_on_off_attr = __ATTR(inverse_on_off, 0660, NULL, inverse_on_off_store);
struct kobj_attribute display_on_off_attr = __ATTR(display_on_off, 0660, NULL, display_on_off_store);
struct kobj_attribute fill_color_attr = __ATTR(fill_color, 0660, NULL, fill_color_store);
struct kobj_attribute rotate_attr = __ATTR(rotate, 0660, NULL, rotate_store);
struct kobj_attribute set_brightness_attr = __ATTR(set_brightness, 0660, NULL, set_brightness_store);
struct kobj_attribute init_attr = __ATTR(init, 0660, init_show, NULL);
/*******************************************************************************/
static struct of_device_id ili9341_id[] = {
	{
		.compatible = COMPATIBLE,
	},
	{}
};
MODULE_DEVICE_TABLE(spi0, ili9341_id);
static struct spi_driver ili9341_driver = {
	.probe = ILI9341_Driver_probe,
	.remove = ILI9341_Driver_remove,
	.driver = {
		.name = "ILI9341_Driver",
		.of_match_table = ili9341_id,
	}
};
module_spi_driver(ili9341_driver);
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = NULL,
	.release = NULL,
	.unlocked_ioctl = ILI9341_Driver_Ioctl,
	.write = ILI9341_Driver_ProcWrite,
};
static struct proc_dir_entry *proc_file;
/*******************************************************************************/
static struct attribute *attrs[] = {
	&reset_attr.attr,
	&clear_attr.attr,
	&inverse_on_off_attr.attr,
	&display_on_off_attr.attr,
	&fill_color_attr.attr,
	&rotate_attr.attr,
	&set_brightness_attr.attr,
	&init_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};
/*******************************************************************************/
static ssize_t reset_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	if (*buf != '0')
		ILI9341_Reset(&ili9341);
	return count;
}

static ssize_t clear_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	if (*buf == '1')
		ILI9341_FillColor(&ili9341, BLACK_16);
	return count;
}

static ssize_t inverse_on_off_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	if (*buf == '0')
		ILI9341_InverseMode(&ili9341, false);
	else
		ILI9341_InverseMode(&ili9341, true);
	return count;
}

static ssize_t display_on_off_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	if (*buf == '0')
		ILI9341_DispalyOn(&ili9341, false);
	else
		ILI9341_DispalyOn(&ili9341, true);
	return count;
}

static ssize_t fill_color_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	u16 color = 0;
	color = buf[0] << 8 | buf[1];
	if (count == 2)
	{
		ILI9341_FillColor(&ili9341, color);
	}
	else
		return -1;
	return count;
}

static ssize_t rotate_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	ILI9341_RotateMode(&ili9341, *buf);
	return count;
}

static ssize_t set_brightness_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int32_t numb = 0;
	sscanf(buf, "%d", &numb);
	ILI9341_SetBrightness(&ili9341, numb);
	return count;
}

static ssize_t init_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	ILI9341_Init(&ili9341);
	return sprintf(buf, "%d%d", ili9341.maxRow, ili9341.maxCol);
}
/*******************************************************************************/
static long int ILI9341_Driver_Ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	PositionType *pos = NULL;
	void __user *argp = (void __user *)arg;
	printk("ILI9341_Driver_Ioctl: \n");
	if (cmd == IOCTL_SET_WINDOW)
	{
		pos = kmalloc(sizeof(PositionType), GFP_KERNEL);
		copy_from_user(pos, argp, sizeof(PositionType));
		ILI9341_SetWindow(&ili9341, pos->startx, pos->starty, pos->endx, pos->endy);
	}
	else if (cmd == IOCTL_CLEAR)
	{
		ILI9341_FillColor(&ili9341, BLACK_16);
	}
	else if (cmd == IOCTL_RESET)
	{
		ILI9341_Reset(&ili9341);
	}
	
	return 0;
}

static ssize_t ILI9341_Driver_ProcWrite(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
	u8 buffer[1000];
	int cnt;
	memset(buffer, 0 , sizeof(buffer));
	cnt = copy_from_user(buffer, user_buffer, count - 1);
	ILI9341_printString(&ili9341, buffer, WHITE_16, BLACK_16);
	return count;
}

static int ILI9341_Driver_probe(struct spi_device *pdev)
{
	struct device *dev = &pdev->dev;
	/* Check device properties */
	if (!device_property_present(dev, "commandData-gpio"))
	{
		printk("dt_probe - Error! Device property 'commandData-gpio' not found\n");
	}
	if (!device_property_present(dev, "reset-gpio"))
	{
		printk("dt_probe - Error! Device property 'reset-gpio' not found\n");
	}
	/* Obtain GPIO */
	ili9341.resetPin = gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ili9341.resetPin))
	{
		printk("ILI9341_Driver_probe - Error! cannot setup the resetPin\n");
		gpiod_put(ili9341.resetPin);
	}
	gpiod_set_value(ili9341.resetPin, HIGH);
	ili9341.dcPin = gpiod_get(dev, "commandData", GPIOD_OUT_HIGH);
	if (IS_ERR(ili9341.dcPin))
	{
		printk("ILI9341_Driver_probe - Error! cannot setup the dcPin\n");
		gpiod_put(ili9341.dcPin);
	}
	gpiod_set_value(ili9341.dcPin, HIGH);
	/* Obtain SPI device */
	ili9341.ili9341 = pdev;
	ili9341.col = 0;
	ili9341.row = 0;
	ili9341.maxCol = MAX_COL;
	ili9341.maxRow = MAX_ROW;
	/* Initialize device */
	ILI9341_Init(&ili9341);
	/* Create proc */
	proc_file = proc_create("ili9341", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("ILI9341_Driver_probe: Error creating /proc/ili9341\n");
		return -ENOMEM;
	}
	/* Create sys/ili9341 */
	kobj = kobject_create_and_add("ili9341",NULL);
	if (sysfs_create_group(kobj, &attr_group))
	{
		pr_err("Cannot create group attribute...\n");
		goto rm_kboj;
	}

	return 0;
rm_kboj:
	kobject_put(kobj);
	return -1;
}
/**
 * @brief This function is called, when the module is removed
 */
static int ILI9341_Driver_remove(struct spi_device *pdev)
{
	struct pinctrl* checkPinCtrl;
	ILI9341_Deinit(&ili9341);
	proc_remove(proc_file);
	checkPinCtrl = devm_pinctrl_get_select(&pdev->dev, "spi0_pinmux_default");
	if (IS_ERR(checkPinCtrl))
	{
		printk("ILI9341_Driver_remove: - Error! cannot reset spi0 pinmux to default\n");
	}
	sysfs_remove_group(kobj,&attr_group);
	kobject_put(kobj);
	return 0;
}
/*******************************************************************************/