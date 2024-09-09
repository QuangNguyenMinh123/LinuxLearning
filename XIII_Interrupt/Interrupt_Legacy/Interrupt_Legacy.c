#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/gpio.h>
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
/** variable contains pin number o interrupt controller to which GPIO 17 is mapped to */
unsigned int irq_number[noLed] = {0};
int gpioArr[noLed] = {GPIO0_15, GPIO0_14, GPIO0_60, GPIO0_112, GPIO0_07};
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
		.name = "MyInterrupt",
		.of_match_table = of_match_ptr(my_driver_id),
	}
};
/*******************************************************************************/
/**
 * @brief Interrupt service routine is called, when interrupt is triggered
 */
static irqreturn_t gpio_irq_handler15(int irq, void *dev_id)
{
	printk("gpio_irq: This is pin 15!\n");
	return IRQ_HANDLED;
}
static irqreturn_t gpio_irq_handler14(int irq, void *dev_id)
{
	printk("gpio_irq: This is pin 14!\n");
	return IRQ_HANDLED;
}
static irqreturn_t gpio_irq_handler60(int irq, void *dev_id)
{
	printk("gpio_irq: This is pin 60!\n");
	return IRQ_HANDLED;
}
static irqreturn_t gpio_irq_handler112(int irq, void *dev_id)
{
	printk("gpio_irq: This is pin 112!\n");
	return IRQ_HANDLED;
}
static irqreturn_t gpio_irq_handler07(int irq, void *dev_id)
{
	printk("gpio_irq: This is pin 07!\n");
	return IRQ_HANDLED;
}
/*******************************************************************************/
static int dt_probe(struct platform_device *pdev)
{
	int i = 0;
	char str[]="rpi-gpio-0";
	irqreturn_t (*funcPtr[5]) (int irq, void *dev_id) = {NULL};
	
	printk("dt_probe\n");
	funcPtr[0] = gpio_irq_handler15;
	funcPtr[1] = gpio_irq_handler14;
	funcPtr[2] = gpio_irq_handler60;
	funcPtr[3] = gpio_irq_handler112;
	funcPtr[4] = gpio_irq_handler07;

	/* Setup the gpio */
	for (i=0; i < noLed; i++)
	{
		str[9]=i+'0';
		if(gpio_request(gpioArr[i], str)) {
			printk("Error!\nCan not allocate GPIO %d\n",gpioArr[i]);
			return -1;
		}
	}
	/* Set GPIO_PIN direction */
	for (i=0; i < noLed; i++)
	{
		if(gpio_direction_input(gpioArr[i])) {
			printk("Error!\nCan not set %d to input!\n",gpioArr[i]);
			gpio_free(gpioArr[i]);
			return -1;
		}
	}
	
	/* Setup the interrupt */
	for (i=0; i < noLed; i++)
	{
		irq_number[i] = gpio_to_irq(gpioArr[i]);

		if(request_irq(irq_number[i], funcPtr[i], IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0){
			printk("Error!\nCan not request interrupt nr.: %d\n", irq_number[i]);
			gpio_free(gpioArr[i]);
			return -1;
		}
		printk("GPIO_PIN %d is mapped to IRQ Nr.: %d\n", gpioArr[i],irq_number[i]);
	}

	printk("Done!\n");
	return 0;
}
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("qpio_irq: Loading module... ");
	
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
static void __exit ModuleExit(void) {
	int i= 0;
	printk("gpio_irq: Unloading module... ");
	for (i=0; i < noLed; i++)
	{
		free_irq(irq_number[i], NULL);
		gpio_free(gpioArr[i]);
	}
	platform_driver_unregister(&my_driver);
}
/*******************************************************************************/
module_init(ModuleInit);
module_exit(ModuleExit);


