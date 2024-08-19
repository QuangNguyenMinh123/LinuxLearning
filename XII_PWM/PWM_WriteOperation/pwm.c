#include <linux/module.h>  /* Thu vien nay dinh nghia cac macro nhu module_init/module_exit */
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major/minor number */
#include <linux/device.h>  /* Thu vien nay dinh nghia cac ham class_create/device_create */
#include <linux/cdev.h>    /* Thu vien nay dinh nghia cac ham kmalloc */
#include <linux/slab.h>     /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include <linux/uaccess.h>  /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user */
#include <linux/pwm.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		"PWM"
#define PWM_PIN				50
#define LOW					0
#define HIGH				1
/*******************************************************************************/

/*******************************************************************************/
static int __init pwm_init(void);
static void __exit pwm_exit(void);
static ssize_t ModuleWrite(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset);
/*******************************************************************************/
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = NULL,
    .write      = ModuleWrite,
    .open       = NULL,
    .release    = NULL,
};

struct m_foo_dev {
    int size;
    char *kmalloc_ptr;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev;

struct pwm_device *pwm0 = NULL;
u32 pwm_on_time = 500000000;
/*******************************************************************************/
static int __init pwm_init(void)
{
	int i = 0;
	/* 1. Allocating device number (cat /pro/devices)*/
	if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "UserPWM") < 0) {
	    pr_err("Failed to alloc chrdev region\n");
	    return -1;
    }
	/* 02.1 Creating cdev structure */
    cdev_init(&mdev.m_cdev, &fops);
	/* 02.2 Adding character device to the system*/
    if ((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto rm_device_numb;
    }
	/* 03. Creating struct class */
    if ((mdev.m_class = class_create(THIS_MODULE, "PWM_Class")) == NULL) {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }
	/* 04. Creating device*/
    if ((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "UserPWM")) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }
	/* PWM init */
	pwm0 = pwm_request(PWM_PIN, "my-pwm");
	if(pwm0 == NULL) {
		printk("Could not get PWM0!\n");
		goto rm_class;
	}

	pwm_config(pwm0, pwm_on_time, 1000000000);
	pwm_enable(pwm0);
	return 0;
rm_class:
    class_destroy(mdev.m_class);
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

static ssize_t ModuleWrite(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset)
{
	int i = 0;
	char buffer[10];
	copy_from_user(buffer, user_buffer, 1);
	pwm_config(pwm0, 100000000 * (buffer[0] - 'a'), 1000000000);
	return size;
}

static void __exit pwm_exit(void)
{
	int i = 0;
	kfree(mdev.kmalloc_ptr);                        /* 05 */
    device_destroy(mdev.m_class, mdev.dev_num);     /* 04 */
    class_destroy(mdev.m_class);                    /* 03 */
    cdev_del(&mdev.m_cdev);                         /* 02 */
    unregister_chrdev_region(mdev.dev_num, 3);      /* 01 */
	/* PWM Deinit */
	pwm_disable(pwm0);
	pwm_free(pwm0);
}
/*******************************************************************************/
module_init(pwm_init);
module_exit(pwm_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
/*******************************************************************************/