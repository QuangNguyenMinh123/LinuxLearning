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
typedef DirectionType{
	INPUT,
	OUTPUT
} DirectionType;
/*******************************************************************************/
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
/*******************************************************************************/
static int32_t _value = 0;
static char _direction[8] = {};

struct foo_type {
	struct kobject *kobj;
}mdev;

struct kobj_attribute value = __ATTR(value, 0660, value_show, value_store);
struct kobj_attribute direction = __ATTR(direction, 0660, direction_show, direction_store);

DirectionType GPIO_Direction = INPUT;
/*******************************************************************************/
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	return sprintf(buf, "%d\n", _value);
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int32_t numb = 0;
	sscanf(buf, "%d", &numb);
	pr_info("num = %d\n",numb);
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
	sscanf(buf, "%d", &_value);
	return count;
}

static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	return sprintf(buf, "%s\n", _direction);
}

static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	switch (count - 1)
	{
	case 2:
		/* code */
		break;
	
	case 3:
		/* code */
		break;
	
	default:
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
	printk("sysfs: Loading module... ");
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
	gpio_direction_output(GPIO_PIN, 1);
	pr_info("sysfs: Initialize successfully!\n");
	return 0;
rm_kboj:
	kobject_put(mdev.kobj);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("sysfs: Unloading module... ");
	gpio_free(GPIO_PIN);
	sysfs_remove_group(mdev.kobj,&attr_group);
	kobject_put(mdev.kobj);

}
/*******************************************************************************/
module_init(ModuleInit);
module_exit(ModuleExit);


