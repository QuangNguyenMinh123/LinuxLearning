#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple LKM for a gpio interrupt");
/*******************************************************************************/
#define LOW					0
#define HIGH				1
#define GPIO_PIN			60
/*******************************************************************************/
typedef enum DirectionType {
	NOT_SETUP,
	INPUT,
	OUTPUT,
} DirectionType;
/*******************************************************************************/
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
/*******************************************************************************/
// static int32_t _value = 0;
static char _direction[8] = {};

struct foo_type {
	struct kobject *kobj;
}mdev;

struct kobj_attribute value = __ATTR(value, 0660, value_show, value_store);
struct kobj_attribute direction = __ATTR(direction, 0660, direction_show, direction_store);
DirectionType GPIO_Direction = NOT_SETUP;
/*******************************************************************************/
int strCompr(const char *str1, const char *str2)
{
	int ctr=0;
	while(str1[ctr]==str2[ctr])
    {
        if(str1[ctr]=='\0'|| str2[ctr]=='\0')
            break;
        ctr++;
    }
    if(str1[ctr]=='\0' && str2[ctr]=='\0')
        return 1;
    else
        return 0;
}

void strPaste(char *des, char *src)
{
	while (*src != 0)
	{
		*des = *src;
		src++;
		des++;
	}
}

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	int readInput = gpio_get_value(GPIO_PIN);
	if (GPIO_Direction == INPUT)
	{
		return sprintf(buf, "%d\n", readInput);
	}
	else
		return sprintf(buf, "%d\n", 0);
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int32_t numb = 0;
	sscanf(buf, "%d", &numb);
	if (GPIO_Direction == OUTPUT)
	{
		switch (numb)
		{
		case 0: /* off */
			gpio_set_value(GPIO_PIN, 0);
			break;
		
		case 1: /* on */
			gpio_set_value(GPIO_PIN, 1);
			break;
		
		default:
			return count;
		}
	}
	return count;
}

static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	return sprintf(buf, "%s\n", _direction);
}

static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	char input[4] = {};
	sscanf(buf, "%s", input);
	if (strCompr(input, "out"))
	{
		gpio_direction_output(GPIO_PIN, 0);
		GPIO_Direction = OUTPUT;
		strPaste(_direction, "out");
		return count;
	}
	else if (strCompr(input, "in"))
	{
		gpio_direction_input(GPIO_PIN);
		GPIO_Direction = INPUT;
		strPaste(_direction, "in");
		return count;
	}
	return count;
}
/*******************************************************************************/
static struct attribute *attrs[] = {
	&direction.attr,
	&value.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};
/*******************************************************************************/
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	
	/* 1. Create directory under /sys */
	mdev.kobj = kobject_create_and_add("bbb_gpio",NULL);
	/* 2. Creating group sys entry under /sys/bbb_gpio */
	if (sysfs_create_group(mdev.kobj, &attr_group))
	{
		pr_err("Cannot create group attribute...\n");
		goto rm_kboj;
	}
	/* Request gpio */
	gpio_request(GPIO_PIN, "led");
	printk("sysfs: Module is loaded\n");
	return 0;
rm_kboj:
	kobject_put(mdev.kobj);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	gpio_free(GPIO_PIN);
	sysfs_remove_group(mdev.kobj,&attr_group);
	kobject_put(mdev.kobj);
	printk("sysfs: Module is removed\n");
}
/*******************************************************************************/
module_init(ModuleInit);
module_exit(ModuleExit);


