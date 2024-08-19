#include <linux/module.h>  /* Thu vien nay dinh nghia cac macro nhu module_init/module_exit */
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major/minor number */
#include <linux/device.h>  /* Thu vien nay dinh nghia cac ham class_create/device_create */
#include <linux/cdev.h>    /* Thu vien nay dinh nghia cac ham kmalloc */
#include <linux/slab.h>     /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include <linux/uaccess.h>  /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user */
#include <linux/gpio.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		"Turn on and off led by writing to file"
#define GPIO0_30			30
#define GPIO0_31			31
#define GPIO0_48			48
#define GPIO0_05			5
#define GPIO0_03			3
#define NO_LED				5
#define LOW					0
#define HIGH				1
/*******************************************************************************/
int gpioArr[NO_LED] = {GPIO0_30, GPIO0_31, GPIO0_48, GPIO0_05, GPIO0_03};
/*******************************************************************************/
static int __init gpio_init(void);
static void __exit gpio_exit(void);
static ssize_t ModuleWrite(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset);
/*******************************************************************************/
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = NULL,
    .write      = ModuleWrite,
    .open       = NULL,
    .release    = NULL,
};
/*******************************************************************************/
static int __init gpio_init(void)
{
	int i = 0;
	for (i = 0; i < NO_LED; i++)
	{
		gpio_request(gpioArr[i], "UserGPIO");
		gpio_direction_output(gpioArr[i], LOW);
		gpio_set_value(gpioArr[i], HIGH);
	}
	return 0;
}

static ssize_t ModuleWrite(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset)
{
	return 1;
}

static void __exit gpio_exit(void)
{
	int i = 0;
	for (i = 0; i < NO_LED; i++)
	{
		gpio_set_value(gpioArr[i], LOW);
		gpio_free(gpioArr[i]);
	}
}
/*******************************************************************************/
module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
/*******************************************************************************/