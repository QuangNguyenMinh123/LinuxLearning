#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A SPI driver for Nokia5110 LCD");
/*******************************************************************************/
#define LOW					0
#define HIGH				1
#define COMPATIBLE			"QuangNM13,Nokia5110"
/*******************************************************************************/
static struct gpio_desc *resetPin = NULL;
static struct gpio_desc *dcPin = NULL;
/*******************************************************************************/
static int Nokia5110_probe(struct spi_device *pdev);
static int Nokia5110_remove(struct spi_device *pdev);
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/

static struct of_device_id nokia5110_id[] = {
	{
		.compatible = COMPATIBLE,
	},
	{}
};
MODULE_DEVICE_TABLE(spi0, nokia5110_id);
static struct spi_driver nokia5110_driver = {
	.probe = Nokia5110_probe,
	.remove = Nokia5110_remove,
	.driver = {
		.name = "Nokia5110_Driver",
		.of_match_table = nokia5110_id,
	}
};
module_spi_driver(nokia5110_driver);
/*******************************************************************************/
static int Nokia5110_probe(struct spi_device *pdev)
{
	struct device *dev = &pdev->dev;
	printk("Nokia5110 Probe\n");
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
	resetPin = gpiod_get(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(resetPin))
	{
		printk("Nokia5110 Probe - Error! cannot setup the reset-gpio\n");
		gpiod_put(resetPin);
	}
	dcPin = gpiod_get(dev, "commandData", GPIOD_OUT_LOW);
	if (IS_ERR(dcPin))
	{
		printk("Nokia5110 Probe - Error! cannot setup the dcPin\n");
		gpiod_put(dcPin);
	}
	/* Obtain SPI device */
	return 0;
}
/**
 * @brief This function is called, when the module is removed
 */
static int Nokia5110_remove(struct spi_device *pdev)
{
	printk("Nokia5110 Remove\n");
	gpiod_put(resetPin);
	gpiod_put(dcPin);
	return 0;
}
/*******************************************************************************/

/*******************************************************************************/


