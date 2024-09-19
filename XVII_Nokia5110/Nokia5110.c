#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/pinctrl/consumer.h>

/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple driver for builtin led");
/*******************************************************************************/
#define LOW					0
#define HIGH				1
#define COMPATIBLE			"USER_BUILT_IN_LED"
/*******************************************************************************/
static struct gpio_desc *builtInLed = NULL;
/*******************************************************************************/
static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);
/*******************************************************************************/
static struct of_device_id my_driver_id[] = {
	{
		.compatible = COMPATIBLE,
	},
	{}
};
MODULE_DEVICE_TABLE(of, my_driver_id);
static struct platform_driver my_driver = {
	.probe = dt_probe,
	.remove = dt_remove,
	.driver = {
		.name = "MyInterrupt",
		.of_match_table = of_match_ptr(my_driver_id),
	}
};
/*******************************************************************************/
static int dt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	printk("Built_In_Module_Probe\n");
	builtInLed = gpiod_get_index(dev, "builtInLed", 0, GPIOD_OUT_HIGH);
	if (IS_ERR(builtInLed))
	{
		printk("Built_In_Module_Probe - Error! cannot setup the builtin LED\n");
		gpiod_put(builtInLed);
	}
	gpiod_set_value(builtInLed, HIGH);
	return 0;
}
/**
 * @brief This function is called, when the module is removed
 */
static int dt_remove(struct platform_device *pdev)
{
	printk("Built_In_Module_Remove\n");
	gpiod_put(builtInLed);
	return 0;
}
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("Built_In_Module_Init: Loading module... ");
	if (platform_driver_register(&my_driver))
	{
		printk("Built_In_Module_Init - Error! Cannot load driver\n");
		return -1;
	}
	return 0;
}
/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("Built_In_Module_Exit: Unloading module... ");
	platform_driver_unregister(&my_driver);
}
/*******************************************************************************/
module_init(ModuleInit);
module_exit(ModuleExit);
/*******************************************************************************/


