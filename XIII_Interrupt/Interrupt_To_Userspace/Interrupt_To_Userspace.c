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
#define noLed				5
#define COMPATIBLE			"Interrupt_Pinmux"
/*******************************************************************************/
/* Function prototype */
static int dt_probe(struct platform_device *pdev);
static irqreturn_t gpio_irq_handler0(int irq, void *dev_id);
static irqreturn_t gpio_irq_handler1(int irq, void *dev_id);
static irqreturn_t gpio_irq_handler2(int irq, void *dev_id);
static irqreturn_t gpio_irq_handler3(int irq, void *dev_id);
static irqreturn_t gpio_irq_handler4(int irq, void *dev_id);
static irqreturn_t (*funcPtr[noLed]) (int irq, void *dev_id) = {
	gpio_irq_handler0,
	gpio_irq_handler1,
	gpio_irq_handler2,
	gpio_irq_handler3,
	gpio_irq_handler4
};
/*******************************************************************************/
/* Data struct for device tree*/
static struct gpio_desc *descGPIO[noLed] = {NULL};
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
unsigned int irq_number[noLed] = {0};
/*******************************************************************************/
/**
 * @brief Interrupt service routine is called, when interrupt is triggered
 */
static irqreturn_t gpio_irq_handler0(int irq, void *dev_id)
{
	printk("gpio_irq: Interrupt 0!\n");
	return IRQ_HANDLED;
}
static irqreturn_t gpio_irq_handler1(int irq, void *dev_id)
{
	printk("gpio_irq: Interrupt 1!\n");
	return IRQ_HANDLED;
}
static irqreturn_t gpio_irq_handler2(int irq, void *dev_id)
{
	printk("gpio_irq: Interrupt 2!\n");
	return IRQ_HANDLED;
}
static irqreturn_t gpio_irq_handler3(int irq, void *dev_id)
{
	printk("gpio_irq: Interrupt 3!\n");
	return IRQ_HANDLED;
}
static irqreturn_t gpio_irq_handler4(int irq, void *dev_id)
{
	printk("gpio_irq: Interrupt 4!\n");
	return IRQ_HANDLED;
}
/*******************************************************************************/
static int dt_probe(struct platform_device *pdev)
{
	int i = 0;
	printk("dt_probe\n");
	/* Setup the interrupt */
	for (i = 0; i < noLed; i++)
	{
		descGPIO[i] = gpiod_get_index(&pdev->dev, "input", i, GPIOD_IN);
		if (IS_ERR(descGPIO[i]))
		{
			printk("dt_probe - Error! retrieve GPIO desc \n");
			gpiod_put(descGPIO[i]);
		}
	}
	/* Change pinmux to input */
	if (IS_ERR(devm_pinctrl_get_select(&pdev->dev, "default")))
	{
		printk("dt_probe - Error! cannot setup the pin mux to default\n");
	}
	/* Change GPIO to Interrupt */
	for (i = 0; i < noLed; i++)
	{
		irq_number[i] = gpiod_to_irq(descGPIO[i]);
		if (irq_number[i] < 0)
		{
			printk("dt_probe: request irq_number %d error\n", i);
			return -1;
		}
		printk("dt_probe: irq_number %d = %d\n",i,irq_number[i]);
		if(request_irq(irq_number[i], funcPtr[i], IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0){
			printk("Error! Can not request interrupt nr.: %d\n", irq_number[i]);
			gpiod_put(descGPIO[i]);
			return -1;
		}
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
	int i = 0;
	printk("GPIO_IRQ: Unloading module... ");
	for (i = 0; i < noLed; i++)
	{
		free_irq(irq_number[i], NULL);
		gpiod_put(descGPIO[i]);
	}
	platform_driver_unregister(&my_driver);
}
/*******************************************************************************/
module_init(ModuleInit);
module_exit(ModuleExit);


