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

struct m_foo_dev {
    int size;
    char *kmalloc_ptr;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev;
/*******************************************************************************/
static int __init gpio_init(void)
{
	int i = 0;
	/* 1. Allocating device number (cat /pro/devices)*/
	if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "UserGPIO") < 0) {
	    pr_err("Failed to alloc chrdev region\n");
	    return -1;
    }
	/* 02.1 Creating cdev structure */
    cdev_init(&mdev.m_cdev, &fops);
	/* 02.2 Adding character device to the system*/
    if ((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto rm_device_numb;
    }
	/* 03. Creating struct class */
    if ((mdev.m_class = class_create(THIS_MODULE, "GPIO_Class")) == NULL) {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }
	/* 04. Creating device*/
    if ((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "UserGPIO")) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }
	/* GPIO init */
	for (i = 0; i < NO_LED; i++)
	{
		gpio_request(gpioArr[i], "UserGPIO");
		gpio_direction_output(gpioArr[i], LOW);
		gpio_set_value(gpioArr[i], HIGH);
	}
	return 0;
rm_class:
    class_destroy(mdev.m_class);
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

static ssize_t ModuleWrite(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset)
{
	int i = 0;
	char buffer[10];
	copy_from_user(buffer, user_buffer, NO_LED);
	int char2Int = buffer[0] - '0';
	int minVal = min(char2Int, NO_LED);
	if (minVal >= 0 && minVal <= NO_LED)
	{
		for (i = 0; i < minVal; i++)
			gpio_set_value(gpioArr[i], HIGH);
		for (; i < NO_LED; i++)
			gpio_set_value(gpioArr[i], LOW);
	}
	return size;
}

static void __exit gpio_exit(void)
{
	int i = 0;
	kfree(mdev.kmalloc_ptr);                        /* 05 */
    device_destroy(mdev.m_class, mdev.dev_num);     /* 04 */
    class_destroy(mdev.m_class);                    /* 03 */
    cdev_del(&mdev.m_cdev);                         /* 02 */
    unregister_chrdev_region(mdev.dev_num, 3);      /* 01 */
	/* GPIO Deinit */
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