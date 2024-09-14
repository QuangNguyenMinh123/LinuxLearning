#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple LKM for a gpio interrupt");
/*******************************************************************************/
#define COMPATIBLE           "P9_14_USERPWM"
/*******************************************************************************/

/*******************************************************************************/
static int dt_probe(struct platform_device *pdev);
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
/*******************************************************************************/
struct kobj_attribute value = __ATTR(value, 0660, value_show, value_store);
struct kobj_attribute direction = __ATTR(direction, 0660, direction_show, direction_store);
// static int32_t _value = 0;
static char _direction[8] = {};

struct foo_type {
	struct kobject *kobj;
}mdev;

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
		.name = "MyPWM",
		.of_match_table = of_match_ptr(my_driver_id),
	}
};
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
	return 0;
}

static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	return count;
}

static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	return sprintf(buf, "%s\n", _direction);
}

static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	return count;
}

static int dt_probe(struct platform_device *pdev)
{
    return 0;
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
	mdev.kobj = kobject_create_and_add("bbb_pwm",NULL);
	/* 2. Creating group sys entry under /sys/bbb_gpio */
	if (sysfs_create_group(mdev.kobj, &attr_group))
	{
		pr_err("Cannot create group attribute...\n");
		goto rm_kboj;
	}
    if (platform_driver_register(&my_driver))
	{
		printk("DT_init - Error! Cannot load driver\n");
		return -1;
	}
	/* Request PWM */
	printk("MyPWM: Module is loaded\n");
	return 0;
rm_kboj:
	kobject_put(mdev.kobj);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	sysfs_remove_group(mdev.kobj,&attr_group);
	kobject_put(mdev.kobj);
    platform_driver_unregister(&my_driver);
	printk("MyPWM: Module is removed\n");
}
/*******************************************************************************/
module_init(ModuleInit);
module_exit(ModuleExit);


