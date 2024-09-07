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
#include <linux/pwm.h>
#include <linux/pinctrl/consumer.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		"PWM using device tree overlay"
#define COMPATIBLE			"PWM_Overlay"
#define LOW					0
#define HIGH				1
/*******************************************************************************/
#define IOCTL_GPIO          _IOW('a','1',int32_t *)
#define IOCTL_PWM           _IOW('a','0',int32_t *)
/*******************************************************************************/
static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);
static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
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
    .unlocked_ioctl = my_ioctl,
    //.compat_ioctl = my_ioctl,
};


static struct platform_driver my_driver = {
	.probe = dt_probe,
	.remove = dt_remove,
	.driver = {
		.name = "PWM_Overlay_Driver",
		.of_match_table = of_match_ptr(my_driver_id),
	}
};

static struct gpio_desc *my_gpio= NULL;
static struct proc_dir_entry *proc_file;

/*******************************************************************************/
/**
 * @brief Check if device tree node is present
 */
void checkNode(struct device* dev, char *node)
{
    if (!device_property_present(dev, node))
	{
		printk("dt_probe - Error! Device property '%s' not found\n", node);
	}
}
/**
 * @brief Write data to buffer
 */
static ssize_t my_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
	int i;
    int char2Int;
	char buffer[10] = {0};
	printk("my_write function is writing\n");
	i = copy_from_user(buffer, user_buffer, 10);
	char2Int = (int) buffer[0] - '0';
	return count;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
    case IOCTL_GPIO:
        printk("my_ioctl: Changing to GPIO mode\n");
        break;
    
    case IOCTL_PWM:
        printk("my_ioctl: Changing to PWM mode\n");
        break;
    }
    return 0;
}

static int dt_probe(struct platform_device *pdev)
{
	struct pinctrl* checkPinCtrl;
	struct device* dev = &pdev->dev;
	int i = 0;
	/* check for device properties */
	printk("dt_probe function\n");
	checkNode(dev,"author");
	checkNode(dev,"gpio_pin-gpios");
	/* Init GPIO */
	checkPinCtrl = devm_pinctrl_get_select(dev, "gpio");
	if (IS_ERR(checkPinCtrl))
	{
		printk("SetGPIO - Error! cannot setup the pin mux for gpio\n");
	}
    my_gpio = gpiod_get(dev, "gpio_pin", GPIOD_OUT_HIGH);
    if (IS_ERR(my_gpio))
    {
        printk("dt_probe - Error! cannot setup the GPIO for gpio pin %d\n",i);
        gpiod_put(my_gpio);
    }
	/* Create procfs file */
	proc_file = proc_create_data("my_pwm", 0666, NULL, &file_ops, NULL);
	
	if(proc_file == NULL) {
		printk("procfs_test - Error creating /proc/my_pwm\n");
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
	if (platform_driver_register(&my_driver))
	{
		printk("DT_init: Error! Cannot load driver\n");
		return -1;
	}
	return 0;
}

static void __exit DT_exit(void)
{
	platform_driver_unregister(&my_driver);
}
/*******************************************************************************/

module_init(DT_init);
module_exit(DT_exit);

// module_platform_driver_probe(my_driver, dt_probe);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
/*******************************************************************************/