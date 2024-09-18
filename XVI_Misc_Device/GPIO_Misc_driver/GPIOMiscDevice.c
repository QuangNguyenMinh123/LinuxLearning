#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/pinctrl/consumer.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
/*******************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple demonstration for a misc device");
/*******************************************************************************/
#define MAX_SIZE 				256
#define COMPATIBLE				"GPIOMiscDevice"
/*******************************************************************************/
static char data[MAX_SIZE];
static size_t data_len;
/*******************************************************************************/
static struct gpio_desc *descGPIO = NULL;
/*******************************************************************************/
static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos) {
	int status;
	if(user_len > data_len)
		data_len = MAX_SIZE;
	else
		data_len = user_len;
	printk("misc_test - Write called\n");
	status = copy_from_user(data, user_buffer, data_len);
	if(status) {
		printk("misc_test - Error during copy_from_user\n");
		return -status;
	}
	if (data[0] == "1")
	{

	}
	else if (data[0] == "0")
	{

	}
	return data_len;
}
/*******************************************************************************/
static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = NULL,
	.write = my_write,
	.open = NULL,
	.release = NULL,
};

static struct miscdevice my_device = {
	.name = "testdev",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops,
};

static struct platform_driver my_driver = {
	.probe = dt_probe,
	.remove = NULL,
	.driver = {
		.name = "MiscDriver_for_GPIO",
		.of_match_table = of_match_ptr(my_driver_id),
	}
};
/*******************************************************************************/

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void) {
	int status;
	printk("misc_test - Register misc device\n");
	status = misc_register(&my_device);
	if(status) {
		printk("misc_test - Error during Register misc device\n");
		return -status;
	}
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void) {
	printk("misc_test - Deregister misc device\n");
	misc_deregister(&my_device);
}
/*******************************************************************************/
module_init(my_init);
module_exit(my_exit);
/*******************************************************************************/