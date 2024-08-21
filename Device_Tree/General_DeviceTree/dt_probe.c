#include <linux/module.h> 					/* Thu vien nay dinh nghia cac macro nhu module_init/module_exit */
#include <linux/init.h> 
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/of_device.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		""
#define LOW					0
#define HIGH				1
/*******************************************************************************/

/*******************************************************************************/
static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);
/*******************************************************************************/
static struct of_device_id my_driver_id[] = {
	{
		.compatible = "brightlight, mydev",
	},
	{}
};
MODULE_DEVICE_TABLE(of, my_driver_id);

static struct platform_driver my_driver = {
	.probe = dt_probe,
	.remove = dt_remove,
	.driver = {
		.name = "my_device_driver",
		.of_match_table = my_driver_id
	}
};

/*******************************************************************************/
static int dt_probe(struct platform_device *pdev)
{
	struct device* dev = &pdev->dev;
	const char* label;
	int my_value, ret;
	/* check for device properties */
	printk("I'm in dt_probe function\n");
	if (!device_property_present(dev, "label"))
	{
		printk("dt_probe - Error! Device property 'label' not found\n");
		return -1;
	}
	if (!device_property_present(dev, "my_value"))
	{
		printk("dt_probe - Error! Device property 'my_value' not found\n");
		return -1;
	}
	/* Read device property */
	ret = device_property_read_string(dev, "label", &label);
	if (ret)
	{
		printk("dt_probe - Error! Cannot read 'label'\n");
		return -1;
	}
	printk("dt_probe 'label': %s\n",label);
	ret = device_property_read_u32(dev, "my_value", &my_value);
	if (ret)
	{
		printk("dt_probe - Error! Cannot read 'my_value'\n");
		return -1;
	}
	printk("dt_probe 'my_value': %d\n",my_value);
	return 0;
}

static int dt_remove(struct platform_device *pdev)
{
	printk("I'm in dt_remove function\n");
	return 0;
}

static int __init DT_init(void)
{
	printk("dt_probe - Loading the driver\n");
	if (platform_driver_register(&my_driver))
	{
		printk("dt_probe - Error! Cannot load driver\n");
		return -1;
	}
	return 0;
}

static void __exit DT_exit(void)
{
	printk("dt_probe - Unloading the driver\n");
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