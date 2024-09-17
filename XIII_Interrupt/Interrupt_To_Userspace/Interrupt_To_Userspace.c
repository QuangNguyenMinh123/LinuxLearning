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
MODULE_DESCRIPTION("A simple LKM for a gpio interrupt");
/*******************************************************************************/
#define LOW					0
#define HIGH				1
#define GPIO0_15			15
#define GPIO0_14			14
#define GPIO0_60			60
#define GPIO0_112			112
#define GPIO0_07			07
#define noLed				5
#define COMPATIBLE			"Interrupt_Pinmux"
/*******************************************************************************/
unsigned int irq_number = 0;
int gpioArr;
/*******************************************************************************/
/* Data struct for device tree*/
struct irq_data	*irq = NULL;
static struct gpio_desc *descGPIO = NULL;
/*******************************************************************************/
static int dt_probe(struct platform_device *pdev);
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
	.remove = NULL,
	.driver = {
		.name = "Interrupt_Userspace",
		.of_match_table = of_match_ptr(my_driver_id),
	}
};
/*******************************************************************************/
/**
 * @brief Interrupt service routine is called, when interrupt is triggered
 */
static irqreturn_t gpio_irq_handler15(int irq, void *dev_id)
{
	printk("gpio_irq: This is interrupt!\n");
	return IRQ_HANDLED;
}
/*******************************************************************************/
static int dt_probe(struct platform_device *pdev)
{
	irqreturn_t (*funcPtr) (int irq, void *dev_id) = {NULL};
	printk("dt_probe\n");
	funcPtr = gpio_irq_handler15;
	/* Set GPIO_PIN direction */
	
	/* Setup the interrupt */
	descGPIO = gpiod_get(&pdev->dev, "input", GPIOD_IN);
    if (IS_ERR(descGPIO))
	{
		printk("dt_probe - Error! retrieve GPIO desc \n");
	}
	if (IS_ERR(devm_pinctrl_get_select(&pdev->dev, "default")))
	{
		printk("dt_probe - Error! cannot setup the pin mux to default\n");
	}
	irq_number = gpiod_to_irq(descGPIO);
    if (irq_number < 0)
	{
		printk("dt_probe: request irq_number error\n");
		return -1;
	}
	printk("dt_probe: irq_number = %d\n",irq_number);
	if(request_irq(irq_number, funcPtr, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0){
		printk("Error! Can not request interrupt nr.: %d\n", irq_number);
		gpiod_put(descGPIO);
		return -1;
	}
	printk("Done!\n");
	return 0;
}
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void)
{
	printk("GPIO_IRQ: Loading module... ");
	
	if (platform_driver_register(&my_driver))
	{
		printk("DT_init - Error! Cannot load driver\n");
		return -1;
	}
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void)
{
	printk("GPIO_IRQ: Unloading module... ");
	free_irq(irq_number, NULL);
	gpiod_put(descGPIO);
	platform_driver_unregister(&my_driver);
}
/*******************************************************************************/
module_init(ModuleInit);
module_exit(ModuleExit);


