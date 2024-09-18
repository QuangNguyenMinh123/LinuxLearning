#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
/*******************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple demonstration for a misc device");
/*******************************************************************************/
#define MAX_SIZE 				256
#define COMPATIBLE				"GPIOMiscDevice"
/*******************************************************************************/
struct MiscGPIOType {
	struct platform_device *pdev;
	struct miscdevice mdev;
};
/*******************************************************************************/
static char data[MAX_SIZE];
static size_t data_len;
/*******************************************************************************/
static struct gpio_desc *descGPIO = NULL;
/*******************************************************************************/
static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);
static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos);
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

static struct of_device_id my_driver_id[] = {
	{
		.compatible = COMPATIBLE,
	},
	{}
};

static struct platform_driver my_platform = {
	.probe      = dt_probe,
	.remove     = dt_remove,
	.driver = {
		.name = "GPIOMisc_Driver",
		.of_match_table = of_match_ptr(my_driver_id),
	},
};
/*******************************************************************************/
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int dt_probe(struct platform_device *pdev)
{
	int ret;
	printk("dt_probe: Success\n");
	if (IS_ERR(devm_pinctrl_get_select(&pdev->dev, "default")))
	{
		printk("dt_probe - Error! cannot setup the pin mux to default\n");
	}
	descGPIO = gpiod_get_index(&pdev->dev, "pin", 0, GPIOD_OUT_HIGH);
	ret = misc_register(&my_device);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register miscdev\n");
		return ret;
	}
	return 0;
}

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
	gpiod_set_value(descGPIO, data[0] - '0');
	return data_len;
}

static int dt_remove(struct platform_device *pdev)
{
	printk("dt_remove: Success\n");
	gpiod_put(descGPIO);
	misc_deregister(&my_device);
	return 0;
}

/*******************************************************************************/
/**
 * @brief declare module Init and exit by using module_platform_driver_probe
 */
module_platform_driver_probe(my_platform, dt_probe)
/*******************************************************************************/