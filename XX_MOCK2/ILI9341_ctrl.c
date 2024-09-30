#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/pinctrl/consumer.h>
#include "ILI9341.h"
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A SPI driver for ILI9341 LCD");
/*******************************************************************************/
#define COMPATIBLE			"QuangNM13,ILI9341"
/*******************************************************************************/

/*******************************************************************************/
typedef struct position {
	unsigned char x;
	unsigned char y;
} Position_t;
#define MAJIC_NO				100
#define IOCTL_GOTOXY			_IOW(MAJIC_NO, 3, Position_t)
#define IOCTL_CLEAR				_IO(MAJIC_NO, 4)
/*******************************************************************************/
ILI9341Type ili9341 =
{
	NULL,
	0,
	0
};
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/
static int ILI9341_Driver_probe(struct spi_device *pdev);
static int ILI9341_Driver_remove(struct spi_device *pdev);
static long int ILI9341_Driver_Ioctl(struct file *file, unsigned cmd, unsigned long arg);
static ssize_t ILI9341_Driver_ProcWrite(struct file *File, const char *user_buffer, size_t count, loff_t *offs);
/*******************************************************************************/

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
static long int ILI9341_Driver_Ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	Position_t *pos = NULL;
	void __user *argp = (void __user *)arg;
	printk("ILI9341_Driver_Ioctl: \n");
	if (cmd == IOCTL_GOTOXY)
	{
		pos = kmalloc(sizeof(Position_t), GFP_KERNEL);
		copy_from_user(pos, argp, sizeof(Position_t));
		printk("x = %d, y = %d\n",(int)pos->x, (int)pos->y);
		ILI9341_goto(&ili9341, pos->x, pos->y);
	}
	else if (cmd == IOCTL_CLEAR)
	{
		ILI9341_ClearScreen(&ili9341);
	}
	
	return 0;
}

static ssize_t ILI9341_Driver_ProcWrite(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[100];
	int cnt;
	memset(buffer, 0 , sizeof(buffer));
	printk("ILI9341_Driver_ProcWrite: \n");
	cnt = copy_from_user(buffer, user_buffer, count - 1);
	return count;
}

static int ILI9341_Driver_probe(struct spi_device *pdev)
{
	struct device *dev = &pdev->dev;
	printk("ILI9341_Driver_probe\n");
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
	/* Initialize device */
	ILI9341_Init(&ili9341);
	/* Create proc */
	proc_file = proc_create("ili9341", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("ILI9341_Driver_probe: Error creating /proc/ili9341\n");
		return -ENOMEM;
	}
	return 0;
}
/**
 * @brief This function is called, when the module is removed
 */
static int ILI9341_Driver_remove(struct spi_device *pdev)
{
	struct pinctrl* checkPinCtrl;
	printk("ILI9341_Driver_remove\n");
	ILI9341_Deinit(&ili9341);
	proc_remove(proc_file);
	checkPinCtrl = devm_pinctrl_get_select(&pdev->dev, "spi0_pinmux_default");
	if (IS_ERR(checkPinCtrl))
	{
		printk("ILI9341_Driver_remove: - Error! cannot reset spi0 pinmux to default\n");
	}
	return 0;
}
/*******************************************************************************/