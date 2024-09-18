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
#include <linux/sched/signal.h>
#include <linux/ioctl.h>
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple LKM for a gpio interrupt");
/*******************************************************************************/
#define noLed				5
#define COMPATIBLE			"Interrupt_Pinmux"
#define REGISTER_UAPP 		_IO('R', 'g')
#define SIGNR0 				44
#define SIGNR1 				45
#define SIGNR2 				46
#define SIGNR3 				47
#define SIGNR4 				48
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
static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg);
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
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.release = NULL,
	.unlocked_ioctl = my_ioctl,
};
unsigned int irq_number[noLed] = {0};
static struct task_struct *task = NULL;
static struct proc_dir_entry *proc_file;
/*******************************************************************************/
/**
 * @brief Interrupt service routine is called, when interrupt is triggered
 */
static irqreturn_t gpio_irq_handler0(int irq, void *dev_id)
{
	struct siginfo info;
	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR0;
		info.si_code = SI_QUEUE;

		/* Send the signal */
		if(send_sig_info(SIGNR0, (struct kernel_siginfo *) &info, task) < 0)
			printk("gpio_irq_signal: Error sending signal\n");
	}
	return IRQ_HANDLED;
}

static irqreturn_t gpio_irq_handler1(int irq, void *dev_id)
{
	struct siginfo info;
	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR1;
		info.si_code = SI_QUEUE;

		/* Send the signal */
		if(send_sig_info(SIGNR1, (struct kernel_siginfo *) &info, task) < 0)
			printk("gpio_irq_signal: Error sending signal\n");
	}
	return IRQ_HANDLED;
}

static irqreturn_t gpio_irq_handler2(int irq, void *dev_id)
{
	struct siginfo info;
	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR2;
		info.si_code = SI_QUEUE;

		/* Send the signal */
		if(send_sig_info(SIGNR2, (struct kernel_siginfo *) &info, task) < 0)
			printk("gpio_irq_signal: Error sending signal\n");
	}
	return IRQ_HANDLED;
}

static irqreturn_t gpio_irq_handler3(int irq, void *dev_id)
{
	struct siginfo info;
	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR3;
		info.si_code = SI_QUEUE;

		/* Send the signal */
		if(send_sig_info(SIGNR3, (struct kernel_siginfo *) &info, task) < 0)
			printk("gpio_irq_signal: Error sending signal\n");
	}
	return IRQ_HANDLED;
}

static irqreturn_t gpio_irq_handler4(int irq, void *dev_id)
{
	struct siginfo info;
	if(task != NULL) {
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGNR4;
		info.si_code = SI_QUEUE;

		/* Send the signal */
		if(send_sig_info(SIGNR4, (struct kernel_siginfo *) &info, task) < 0)
			printk("gpio_irq_signal: Error sending signal\n");
	}
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
	/* Register file operations to this driver */
	proc_file = proc_create_data("InterruptFromKernel", 0666, NULL, &fops, NULL);
	
	if(proc_file == NULL) {
		printk("dt_probe - Error creating /proc/InterruptFromKernel\n");
		for (i = 0;i < noLed; i++)
		{
			gpiod_put(descGPIO[i]);
		}
		return -ENOMEM;
	}
	printk("Done!\n");
	return 0;
}
/**
 * @brief This function is called to save process ID to task
 */
static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	if(cmd == REGISTER_UAPP) {
		task = get_current();
		printk("gpio_irq_signal: Userspace app with PID %d is registered\n", task->pid);
	}
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
	proc_remove(proc_file);
}
/*******************************************************************************/
module_init(ModuleInit);
module_exit(ModuleExit);


