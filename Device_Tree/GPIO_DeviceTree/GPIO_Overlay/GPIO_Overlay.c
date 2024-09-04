#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/gpio/consumer.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/pinctrl/consumer.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		"GPIO using device tree overlay"
#define COMPATIBLE			"QuangNM13,MultiGPIO"
#define LOW					0
#define HIGH				1
#define NO_LED				5
/*******************************************************************************/

/*******************************************************************************/
static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);
static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs);
/*******************************************************************************/
static struct of_device_id my_driver_id[] = {
	{
		.compatible = COMPATIBLE,
	},
	{}
};
/* of = open firmware, thường được sử dụng cho các thiết bị được mô tả bằng Device Tree */
MODULE_DEVICE_TABLE(of, my_driver_id);

static struct file_operations file_ops = {
    .owner = THIS_MODULE,
    .write = my_write,
};


static struct platform_driver my_driver = {
	.probe = dt_probe,
	.remove = dt_remove,
	.driver = {
		.name = "my_device_driver",
		.of_match_table = my_driver_id
	}
};

static struct gpio_desc *my_gpio[NO_LED] = {NULL};
static struct proc_dir_entry *proc_file;

/*******************************************************************************/
/**
 * @brief Set pin as gpio
 */
struct pinctrl* SetGPIO(struct device* dev, char* mode)
{
	struct pinctrl *p;
	struct pinctrl_state *state;
	int ret;

	p = pinctrl_get(dev);
	if (IS_ERR(p))
	{
		printk("SetGPIO - Error! 1");
		return p;
	}
		
	state = pinctrl_lookup_state(p, mode);
	if (IS_ERR(p))
	{
		printk("SetGPIO - Error! 2");
		devm_pinctrl_put(p);
		return ERR_PTR(PTR_ERR(state));
	}
	ret = pinctrl_select_state(p, state);
	if (ret < 0)
	{
		printk("SetGPIO - Error! 3");
		devm_pinctrl_put(p);
		return ERR_PTR(ret);
	}
	return p;
}

/**
 * @brief Write data to buffer
 */
static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int i = 0;
	int char2Int;
	int minVal;
	char buffer[10] = {0};
	printk("my_write function is writing\n");
	i = copy_from_user(buffer, user_buffer, NO_LED);
	char2Int = (int) buffer[0] - '0';
	minVal = min(char2Int, NO_LED);
	if (minVal >= 0 && minVal <= NO_LED)
	{
		for (i = 0; i < minVal; i++)
			gpiod_set_value(my_gpio[i], 1);
		for (; i < NO_LED; i++)
			gpiod_set_value(my_gpio[i], 0);
	}
	return count;
}

static int dt_probe(struct platform_device *pdev)
{
	struct pinctrl* checkPinCtrl;
	struct device* dev = &pdev->dev;
	int i = 0;
	/* check for device properties */
	printk("I'm in dt_probe function\n");
	if (!device_property_present(dev, "author"))
	{
		printk("dt_probe - Error! Device property 'author' not found\n");
	}
	if (!device_property_present(dev, "gpio_pin-gpio"))
	{
		printk("dt_probe - Error! Device property 'gpio_pin-gpio' not found\n");
	}
	/* Init GPIO */
	for (i = 0;i < NO_LED; i++)
	{
		my_gpio[i] = gpiod_get_index(dev, "gpio_pin", i, GPIOD_OUT_HIGH);
		//my_gpio[i] = gpiod_get(dev, "gpio_pin", GPIOD_OUT_HIGH);
		if (IS_ERR(my_gpio[i]))
		{
			printk("dt_probe - Error! cannot setup the GPIO for gpio pin %d\n",i);
			gpiod_put(my_gpio[i]);
		}
		checkPinCtrl = SetGPIO(dev, "gpio");
		if (IS_ERR(checkPinCtrl))
		{
			printk("dt_probe - Error! cannot setup the pin mux for gpio pin %d\n",i);
		}
	}
	/* Create procfs file */
	proc_file = proc_create_data("my_gpio", 0666, NULL, &file_ops, NULL);
	
	if(proc_file == NULL) {
		printk("procfs_test - Error creating /proc/my-gpio\n");
		for (i = 0;i < NO_LED; i++)
		{
			gpiod_put(my_gpio[i]);
		}
		return -ENOMEM;
	}
	return 0;
}

static int dt_remove(struct platform_device *pdev)
{
	int i = 0;
	printk("dt_remove\n");
	for (i = 0;i < NO_LED; i++)
	{
		gpiod_put(my_gpio[i]);
	}
	proc_remove(proc_file);
	return 0;
}

static int __init DT_init(void)
{
	printk("DT_init - Loading the driver\n");
	if (platform_driver_register(&my_driver))
	{
		printk("DT_init - Error! Cannot load driver\n");
		return -1;
	}
	return 0;
}

static void __exit DT_exit(void)
{
	printk("DT_exit - Unloading the driver\n");
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