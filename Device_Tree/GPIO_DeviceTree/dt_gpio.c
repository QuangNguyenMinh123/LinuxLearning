#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/gpio/consumer.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		"GPIO using device tree"
#define LOW					0
#define HIGH				1
/*******************************************************************************/

/*******************************************************************************/
static int dt_gpio(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);
static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs);
/*******************************************************************************/
static struct of_device_id my_driver_id[] = {
	{
		.compatible = "QuangNM13,GPIO",
	},
	{}
};
/* of = open firmware, thường được sử dụng cho các thiết bị được mô tả bằng Device Tree */
MODULE_DEVICE_TABLE(of, my_driver_id);

static const struct proc_ops file_ops = {
	.proc_write = my_write,
};

static struct platform_driver my_driver = {
	.probe = dt_gpio,
	.remove = dt_remove,
	.driver = {
		.name = "my_device_driver",
		.of_match_table = my_driver_id
	}
};

static struct gpio_desc *my_gpio = NULL;
static struct proc_dir_entry *proc_file;

/*******************************************************************************/
/**
 * @brief Write data to buffer
 */
static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	switch (user_buffer[0]) {
		case '0':
		case '1':
			gpiod_set_value(my_gpio, user_buffer[0] - '0');
		default:
			break;
	}
	return count;
}

static int dt_gpio(struct platform_device *pdev)
{
	struct device* dev = &pdev->dev;
	const char* label;
	int ret;
	/* check for device properties */
	printk("I'm in dt_gpio function\n");
	if (!device_property_present(dev, "author"))
	{
		printk("dt_gpio - Error! Device property 'author' not found\n");
		return -1;
	}
	if (!device_property_present(dev, "gpio_pin-gpio"))
	{
		printk("dt_gpio - Error! Device property 'gpio_pin-gpio' not found\n");
		return -1;
	}
	/* Init GPIO */
	my_gpio = gpiod_get(dev, "gpio_pin", GPIOD_OUT_LOW);
	if (IS_ERR(my_gpio))
	{
		printk("dt_gpio - Error! cannot setup the GPIO \n");
		gpiod_put(my_gpio);
		return -1;
	}
	/* Create procfs file */
	proc_file = proc_create("my-led", 0666, NULL, &file_ops);
	if(proc_file == NULL) {
		printk("procfs_test - Error creating /proc/my-gpio\n");
		gpiod_put(my_gpio);
		return -ENOMEM;
	}
	return 0;
}

static int dt_remove(struct platform_device *pdev)
{
	printk("dt_remove\n");
	gpiod_put(my_gpio);
	proc_remove(proc_file);
	return 0;
}

static int __init DT_init(void)
{
	printk("dt_gpio - Loading the driver\n");
	if (platform_driver_register(&my_driver))
	{
		printk("dt_gpio - Error! Cannot load driver\n");
		return -1;
	}
	return 0;
}

static void __exit DT_exit(void)
{
	printk("dt_gpio - Unloading the driver\n");
	platform_driver_unregister(&my_driver);
}
/*******************************************************************************/
module_init(DT_init);
module_exit(DT_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
/*******************************************************************************/