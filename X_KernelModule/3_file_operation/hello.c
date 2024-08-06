/******************************************************************************
*  @brief      Simple Linux device driver
*
*  @author     QuangNM13    nguyenminhquangcn1@gmail.com
*******************************************************************************/

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define AUTHOR              "QuangNM13 nguyenminhquangcn1@gmail.com"
#define DESCRIPTION         "File operation kernel module"

struct m_foo_dev{
    dev_t dev_num;
    struct class* m_class;
    struct cdev m_cdev;
} device;

/* Function prototype */
static int      __init moduleInit(void);
static void     __exit moduleExit(void);
static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_read(struct file *filp, char __user *user_buf, size_t size, loff_t* offser);
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t* offser);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = m_read,
    .write      = m_write,
    .open       = m_open,
    .release    = m_release,
};

/* This function will be called when we open the Device file */
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;
}

/* This function will be called when we close the Device file */
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;
}

/* This function will be called when we read the Device file */
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call read() called...!!!\n");
    return 0;
}

/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call write() called...!!!\n");
    return size;
}

static int
__init moduleInit(void)
{
    /* 1. Allocating device number (cat /pro/devices) */
    if (alloc_chrdev_region(&device.dev_num, 0, 1, "m_cdev") < 0)
    {
        pr_err("Failed to aclloc chrdev region\n");
        return -1;
    }
    pr_info("Major = %d Minor %d\n", MAJOR(device.dev_num), MINOR(device.dev_num));

    /* 02.1 Creating cdev structure */
    cdev_init(&device.m_cdev, &fops);

    /* 02.1 Adding character device to the system */
    if ((cdev_add(&device.m_cdev, device.dev_num, 1)) < 0)
    {
        pr_err("Cannot add the device to the system\n");
        goto rm_device_numb;
    }

    /* 3. Creating struct class */
    if ((device.m_class = class_create(THIS_MODULE, "m_class")) == NULL)
    {
        pr_err("cannot create the struct class for my device\n");
        goto rm_device_numb;
    }

    /* 4 Creating device */
    if (device_create(device.m_class, NULL, device.dev_num, NULL, "m_device") == NULL)
    {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }
    pr_info("Hello world kernel module\n");
    return 0;

rm_class:
    class_destroy(device.m_class);
rm_device_numb:
    unregister_chrdev_region(device.dev_num, 1);
    return -1;
}

static void
__exit moduleExit(void)
{
    device_destroy(device.m_class, device.dev_num);
    class_destroy(device.m_class);
    cdev_del(&device.m_cdev);
    unregister_chrdev_region(device.dev_num, 3);
    pr_info("Goodbye\n");
}

module_init(moduleInit);
module_exit(moduleExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);  