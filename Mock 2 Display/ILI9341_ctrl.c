#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/pinctrl/consumer.h>
#include "ILI9341.h"
#include "ILI9341_GUI.h"
#include <linux/sched/signal.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A SPI driver for ILI9341 LCD");
/*******************************************************************************/
#define COMPATIBLE			"QuangNM13,ILI9341"
/*******************************************************************************/
/* Define for IRQ number */
#define SIGNR0 				44
#define SIGNR1 				45
#define SIGNR2 				46
#define SIGNR3 				47
#define SIGNR4 				48
#define noLed				5
/*******************************************************************************/
typedef struct PositionType {
	int startRow;
	int startCol;
	int endRow;
	int endCol;
} PositionType;
typedef enum ButtonType{
	RELEASE, 
	PRESS
}ButtonType;
/*******************************************************************************/
#define UP_BUTTON						1
#define DOWN_BUTTON						0
#define LEFT_BUTTON						1
#define RIGHT_BUTTON					0
#define SELECT_BUTTON					4
#define MAX_TIMEOUT						25		/* Scan every 50ms -> timeout = 1.5 */
/*******************************************************************************/
#define MAJIC_NO						100
#define IOCTL_SET_WINDOW				_IOW(MAJIC_NO, 3, PositionType)
#define IOCTL_CLEAR						_IO(MAJIC_NO, 4)
#define IOCTL_RESET						_IO(MAJIC_NO, 5)
#define IOCTL_INVERSE_ON				_IO(MAJIC_NO, 6)
#define IOCTL_INVERSE_OFF				_IO(MAJIC_NO, 7)
#define IOCTL_DISPLAY_ON				_IO(MAJIC_NO, 8)
#define IOCTL_DISPLAY_OFF				_IO(MAJIC_NO, 9)
/*******************************************************************************/
ILI9341Type ili9341;
/*******************************************************************************/
static ssize_t reset_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t clear_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t inverse_on_off_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t display_on_off_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t fill_color_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t rotate_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t set_brightness_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t setCharColor_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t setBgColor_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t scroll_up_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t scroll_down_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t scroll__left_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t scroll__right_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t nextline_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t init_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
/*******************************************************************************/
static int ILI9341_Driver_probe(struct spi_device *pdev);
static int ILI9341_Driver_remove(struct spi_device *pdev);
static long int ILI9341_Driver_Ioctl(struct file *file, unsigned cmd, unsigned long arg);
static ssize_t ILI9341_Driver_ProcWrite(struct file *File, const char *user_buffer, size_t count, loff_t *offs);
/*******************************************************************************/
/* Define for IRQ handler */
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
/* Data struct for device tree*/
static struct gpio_desc *descGPIO[noLed] = {NULL};
unsigned int irq_number[noLed] = {0};
/*******************************************************************************/
struct kobject *kobj = NULL;	/* pointer point to /sys/ili9341 */
struct kobj_attribute reset_attr = __ATTR(reset, 0660, NULL, reset_store);
struct kobj_attribute clear_attr = __ATTR(clear, 0660, NULL, clear_store);
struct kobj_attribute inverse_on_off_attr = __ATTR(inverse_on_off, 0660, NULL, inverse_on_off_store);
struct kobj_attribute display_on_off_attr = __ATTR(display_on_off, 0660, NULL, display_on_off_store);
struct kobj_attribute fill_color_attr = __ATTR(fill_color, 0660, NULL, fill_color_store);
struct kobj_attribute rotate_attr = __ATTR(rotate, 0660, NULL, rotate_store);
struct kobj_attribute set_brightness_attr = __ATTR(set_brightness, 0660, NULL, set_brightness_store);
struct kobj_attribute setCharColor_attr = __ATTR(setCharColor, 0660, NULL, setCharColor_store);
struct kobj_attribute setBgColor_attr = __ATTR(setBgColor, 0660, NULL, setBgColor_store);
struct kobj_attribute scroll_up_attr = __ATTR(scroll_up, 0660, NULL, scroll_up_store);
struct kobj_attribute scroll_down_attr = __ATTR(scroll_down, 0660, NULL, scroll_down_store);
struct kobj_attribute scroll_left_attr = __ATTR(scroll_left, 0660, NULL, scroll__left_store);
struct kobj_attribute scroll_right_attr = __ATTR(scroll_right, 0660, NULL, scroll__right_store);
struct kobj_attribute nextline_attr = __ATTR(nextline, 0660, NULL, nextline_store); 
struct kobj_attribute init_attr = __ATTR(init, 0660, init_show, NULL);
/*******************************************************************************/
/* Global variable for thread */
static struct task_struct *kthread;
ButtonType buttonPreState[noLed] = {RELEASE};
ButtonType buttonCurState[noLed] = {RELEASE};
int threadFunc(void *args);
int timeout[noLed] = {0};
/*******************************************************************************/

static struct of_device_id ili9341_id[] = {
	{
		.compatible = COMPATIBLE,
	},
	{}
};
MODULE_DEVICE_TABLE(spi0, ili9341_id);
static struct spi_driver ili9341_driver = {
	.probe = ILI9341_Driver_probe,
	.remove = ILI9341_Driver_remove,
	.driver = {
		.name = "ILI9341_Driver",
		.of_match_table = ili9341_id,
	}
};
module_spi_driver(ili9341_driver);
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = NULL,
	.release = NULL,
	.unlocked_ioctl = ILI9341_Driver_Ioctl,
	.write = ILI9341_Driver_ProcWrite,
};
static struct proc_dir_entry *proc_file;
/*******************************************************************************/
static struct attribute *attrs[] = {
	&reset_attr.attr,
	&clear_attr.attr,
	&inverse_on_off_attr.attr,
	&display_on_off_attr.attr,
	&fill_color_attr.attr,
	&rotate_attr.attr,
	&set_brightness_attr.attr,
	&setCharColor_attr.attr,
	&setBgColor_attr.attr,
	&scroll_up_attr.attr,
	&scroll_down_attr.attr,
	&scroll_left_attr.attr,
	&scroll_right_attr.attr,
	&nextline_attr.attr,
	&init_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};
/*******************************************************************************/
static ssize_t reset_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	if (*buf != '0')
		ILI9341_Reset(&ili9341);
	return count;
}

static ssize_t clear_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	if (*buf == '1')
		ILI9341_FillColor(&ili9341, DARK_GREEN_16);
	return count;
}

static ssize_t inverse_on_off_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	if (*buf == '0')
		ILI9341_InverseMode(&ili9341, false);
	else
		ILI9341_InverseMode(&ili9341, true);
	return count;
}

static ssize_t display_on_off_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	if (*buf == '0')
		ILI9341_DispalyOn(&ili9341, false);
	else
		ILI9341_DispalyOn(&ili9341, true);
	return count;
}

static ssize_t fill_color_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	u16 color = 0;
	color = buf[0] << 8 | buf[1];
	if (count == 2)
	{
		ILI9341_FillColor(&ili9341, color);
	}
	else
		return -1;
	return count;
}

static ssize_t rotate_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	ILI9341_RotateMode(&ili9341, *buf);
	return count;
}

static ssize_t set_brightness_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int32_t numb = 0;
	sscanf(buf, "%d", &numb);
	ILI9341_SetBrightness(&ili9341, numb);
	return count;
}

static ssize_t setCharColor_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	return count;
}

static ssize_t setBgColor_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	return count;
}
/* Screen Scroll*/
static ssize_t scroll_up_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	ILI9341_ScrollUp(&ili9341);
	return count;
}

static ssize_t scroll_down_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	ILI9341_ScrollDown(&ili9341);
	return count;
}

static ssize_t scroll__left_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	return count;
}

static ssize_t scroll__right_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	ILI9341_print1Line(&ili9341, ili9341.displayRow/ili9341.fontRowSize - 1, 
							(ili9341.displayRow % ili9341.maxRow));
	return count;
}

static ssize_t nextline_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	ILI9341_print1Line(&ili9341, 0, 0);
	return count;
}

static ssize_t init_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	ILI9341_Init(&ili9341);
	return sprintf(buf, "%d%d", ili9341.maxRow, ili9341.maxCol);
}
/*******************************************************************************/
int threadFunc(void *args)
{
	int i = 0;
	while (!(kthread_should_stop()))
	{
		for (i=0; i < noLed; i ++)
		{
			buttonCurState[i] = gpiod_get_value(descGPIO[i]);
			if (buttonCurState[i] == LOW)
			{
				timeout[i] = 0;
			}
			else
			{
				timeout[i] ++;
				if (Screen == SCREEN_READ)		/* If in reading mode */
				{
					if (timeout[i] == 1)
					{
						if (i == SELECT_BUTTON)
						{
							Screen = SCREEN_MENU;
							ILI9341_Menu(&ili9341);
						}
					}
					if ((timeout[i] == 15) || (timeout[i] == 1))
					{
						if (i == UP_BUTTON)
						{
							ILI9341_ScrollUp(&ili9341);
						}
						else if (i == DOWN_BUTTON)
						{
							ILI9341_ScrollDown(&ili9341);
						}
					}
					else if (timeout[i] >= MAX_TIMEOUT)
					{
						if (timeout[i] % 1 == 0)
						{
							if (i == UP_BUTTON)
							{
								ILI9341_ScrollUp(&ili9341);
							}
							else if (i == DOWN_BUTTON)
							{
								ILI9341_ScrollDown(&ili9341);
							}
						}
					}
				}
				else				/* If in menu */
				{
					if (timeout[i] == 1)
					{
						if (i == SELECT_BUTTON)
						{
							Screen = SCREEN_READ;
							ILI9341_OpenFile(&ili9341);
						}
					}
					if ((timeout[i] == 15) || (timeout[i] == 1))
					{
						if (i == UP_BUTTON)
						{
							ILI9341_DecreateFileIndex(&ili9341);
						}
						else if (i == DOWN_BUTTON)
						{
							ILI9341_IncreateFileIndex(&ili9341);
						}
					}
					else if (timeout[i] >= MAX_TIMEOUT)
					{
						if (timeout[i] % 2 == 0)
						{
							if (i == UP_BUTTON)
							{
								ILI9341_DecreateFileIndex(&ili9341);
							}
							else if (i == DOWN_BUTTON)
							{
								ILI9341_IncreateFileIndex(&ili9341);
							}
						}
					}
				}
				
			}
			buttonPreState[i] = buttonCurState[i];
		}
		msleep(25); /* scan every 50ms */
	}
	return 0;
}
/*******************************************************************************/
/**
 * @brief Interrupt service routine is called, when interrupt is triggered
 */
static struct task_struct *task = NULL;
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
	printk("This is IRQ 0\n");
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
	printk("This is IRQ 1\n");
	return IRQ_HANDLED;
}

static irqreturn_t gpio_irq_handler2(int irq, void *dev_id)
{
	/* sw 1 = P9_12, down */
	ILI9341_ScrollDown(&ili9341);
	return IRQ_HANDLED;
}

static irqreturn_t gpio_irq_handler3(int irq, void *dev_id)
{
	/* sw 1 = P9_12, up */
	ILI9341_ScrollUp(&ili9341);
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
	printk("This is IRQ 4\n");
	return IRQ_HANDLED;
}
/*******************************************************************************/
static long int ILI9341_Driver_Ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	PositionType *pos = NULL;
	void __user *argp = (void __user *)arg;
	if (cmd == IOCTL_SET_WINDOW)
	{
		pos = kmalloc(sizeof(PositionType), GFP_KERNEL);
		if (copy_from_user(pos, argp, sizeof(PositionType)))
		{
			printk("ILI9341_Driver_Ioctl: Error setting window\n");
			return -1;
		}
		ILI9341_SetWindow(&ili9341, pos->startRow, pos->startCol, pos->endRow, pos->endCol);
	}
	else if (cmd == IOCTL_CLEAR)
	{
		ILI9341_FillColor(&ili9341, DARK_GREEN_16);
	}
	else if (cmd == IOCTL_RESET)
	{
		ILI9341_Reset(&ili9341);
	}
	return 0;
}

static ssize_t ILI9341_Driver_ProcWrite(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
	u8 buffer[1000];
	int cnt;
	memset(buffer, 0 , sizeof(buffer));
	cnt = copy_from_user(buffer, user_buffer, count - 1);
	if (Screen == SCREEN_MENU)
		ILI9341_Init(&ili9341);
	Screen = SCREEN_READ;
	ILI9341_printStringScroll(&ili9341, buffer, WHITE_16, DARK_GREEN_16);
	return count;
}

static int ILI9341_Driver_probe(struct spi_device *pdev)
{
	int i = 0;
	struct device *dev = &pdev->dev;
	/* Check device properties */
	if (!device_property_present(dev, "commandData-gpio"))
	{
		printk("ILI9341_Driver_probe - Error! Device property 'commandData-gpio' not found\n");
	}
	if (!device_property_present(dev, "reset-gpio"))
	{
		printk("ILI9341_Driver_probe - Error! Device property 'reset-gpio' not found\n");
	}
	/* Obtain GPIO */
	ili9341.resetPin = gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ili9341.resetPin))
	{
		printk("ILI9341_Driver_probe - Error! cannot setup the resetPin\n");
		gpiod_put(ili9341.resetPin);
	}
	gpiod_set_value(ili9341.resetPin, HIGH);
	ili9341.dcPin = gpiod_get(dev, "commandData", GPIOD_OUT_HIGH);
	if (IS_ERR(ili9341.dcPin))
	{
		printk("ILI9341_Driver_probe - Error! cannot setup the dcPin\n");
		gpiod_put(ili9341.dcPin);
	}
	gpiod_set_value(ili9341.dcPin, HIGH);
	/* Obtain IRQ */
	for (i = 0; i < noLed; i++)
	{
		descGPIO[i] = gpiod_get_index(&pdev->dev, "input", i, GPIOD_IN);
		if (IS_ERR(descGPIO[i]))
		{
			printk("ILI9341_Driver_probe - Error! retrieve GPIO desc \n");
			gpiod_put(descGPIO[i]);
		}
		gpiod_direction_input(descGPIO[i]);
	}
	/* Change pinmux to input */
	if (IS_ERR(devm_pinctrl_get_select(&pdev->dev, "default")))
	{
		printk("ILI9341_Driver_probe - Error! cannot setup the pin mux to default\n");
	}
	/* Create thread for reading button */
	kthread = kthread_create(threadFunc, NULL, "ili9341_Thread");
	if (kthread != NULL)
	{
		wake_up_process(kthread);
	}
	else
	{
		printk("kthread is not created, terminate program\n");
		return -1;
	}
	/* Obtain SPI device */
	ili9341.ili9341 = pdev;
	ili9341.col = 0;
	ili9341.row = 0;
	ili9341.maxCol = MAX_COL;
	ili9341.maxRow = MAX_ROW;
	ili9341.displayRow = 0;
	ili9341.totalRow = 0;
	/* Create proc */
	proc_file = proc_create("ili9341", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("ILI9341_Driver_probe: Error creating /proc/ili9341\n");
		return -ENOMEM;
	}
	/* Create sys/ili9341 */
	kobj = kobject_create_and_add("ili9341",NULL);
	if (sysfs_create_group(kobj, &attr_group))
	{
		pr_err("Cannot create group attribute...\n");
		goto rm_kboj;
	}
	/* Create and open /temp/ili9341 */
	ili9341.fileBuffer = filp_open("/tmp/ili9341_buffer", O_RDWR | O_CREAT, 0666);
	if (!ili9341.fileBuffer)
	{
		printk("Cannot create /tmp/ili9341_buffer file buffer for ili9341\n");
		goto rm_kboj;
	}
	/* Initialize device */
	ILI9341_Menu(&ili9341);
	return 0;
rm_kboj:
	kobject_put(kobj);
	return -1;
}
/**
 * @brief This function is called, when the module is removed
 */
static int ILI9341_Driver_remove(struct spi_device *pdev)
{
	int i = 0;
	struct pinctrl* checkPinCtrl;
	ILI9341_Deinit(&ili9341);
	proc_remove(proc_file);
	checkPinCtrl = devm_pinctrl_get_select(&pdev->dev, "spi0_pinmux_default");
	kthread_stop(kthread);
	if (IS_ERR(checkPinCtrl))
	{
		printk("ILI9341_Driver_remove: - Error! cannot reset spi0 pinmux to default\n");
	}
	sysfs_remove_group(kobj,&attr_group);
	kobject_put(kobj);
	if (ili9341.fileBuffer)
	{
		filp_close(ili9341.fileBuffer, NULL);
	}
	for (i = 0; i < noLed; i++)
	{
		gpiod_put(descGPIO[i]);
	}
	return 0;
}
/*******************************************************************************/