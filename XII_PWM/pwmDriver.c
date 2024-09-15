#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include <linux/pwm.h>
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple pwm driver");
/*******************************************************************************/
#define COMPATIBLE           "p9_14_pwm_device"
/*******************************************************************************/

/*******************************************************************************/
static int dt_probe(struct platform_device *pdev);
static int dt_remove(struct platform_device *pdev);
static ssize_t duty_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t duty_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static ssize_t period_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf);
static ssize_t period_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
/*******************************************************************************/
struct kobj_attribute duty = __ATTR(duty, 0660, duty_show, duty_store);
struct kobj_attribute period = __ATTR(period, 0660, period_show, period_store);
static int32_t _duty = 0;
static int32_t _period = 0;

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
	.remove = dt_remove,
	.driver = {
		.name = "MyPWMDriver",
		.of_match_table = of_match_ptr(my_driver_id),
	}
};
struct pwm_device *pwm = NULL;
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

int pwm_trigger(void)
{
	pwm_disable(pwm);
	if (pwm_config(pwm, _duty, _period) < 0)
	{
		pr_info("pwm_trigger: config failed\n");
		return -1;
	}
	if (pwm_enable(pwm) < 0)
	{
		pr_info("pwm_trigger: enable failed\n");
		return -1;
	}
	return 0;
}

static ssize_t duty_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", _duty);
}

static ssize_t duty_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d", &_duty);
	pwm_trigger();
	return count;
}

static ssize_t period_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
	return sprintf(buf, "%d\n", _period);
}

static ssize_t period_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	sscanf(buf, "%d", &_period);
	pwm_trigger();
	return count;
}

static int dt_probe(struct platform_device *pdev)
{
    printk("MyPWM: dt_probe successfully\n");
	/* 1. Read pwm device */
	pwm = pwm_get(&pdev->dev,"led-pwm");
	if (IS_ERR(pwm))
	{
		pr_info("PWM failed\n");
		return -1;
	}
	pr_info("Period: %d\n",pwm->args.period);
	_period = 1000000;
	_duty = _period/2;
	pwm_trigger();
    return 0;
}

static int dt_remove(struct platform_device *pdev)
{
	pr_info("dt_remove: pwm is removed\n");
	pwm_disable(pwm);
	pwm_put(pwm);
	return 0;
}
/*******************************************************************************/
static struct attribute *attrs[] = {
	&period.attr,
	&duty.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};
/*******************************************************************************/
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void)
{
	printk("MyPWM: Module is loaded\n");
    if (platform_driver_register(&my_driver))
	{
		printk("ModuleInit - Error! Cannot load driver\n");
		return -1;
	}
	/* 1. Create directory under /sys */
	mdev.kobj = kobject_create_and_add("bbb_pwm",NULL);
	/* 2. Creating group sys entry under /sys/bbb_gpio */
	if (sysfs_create_group(mdev.kobj, &attr_group))
	{
		pr_err("MyPWM: Cannot create group attribute...\n");
		goto rm_kboj;
	}
	/* Request PWM */
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


