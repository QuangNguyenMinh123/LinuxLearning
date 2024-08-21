#include <linux/module.h>  /* Thu vien nay dinh nghia cac macro nhu module_init/module_exit */
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major/minor number */
#include <linux/device.h>  /* Thu vien nay dinh nghia cac ham class_create/device_create */
#include <linux/cdev.h>    /* Thu vien nay dinh nghia cac ham kmalloc */
#include <linux/slab.h>     /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include <linux/uaccess.h>  /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user */
#include <linux/i2c.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		"I2C BMP180"
#define DRIVER_NAME         "I2C BMP180"
#define DRIVER_CLASS        "I2C Class"

#define LOW					0
#define HIGH				1

#define I2C_BUS_AVAILABLE   1           /* i2c number to be used */
#define BMP180_ADDR         0x76
#define SLAVE_NAME          "BMP180"
/*******************************************************************************/

/*******************************************************************************/
static int __init i2c_init(void);
static void __exit i2c_exit(void);
static ssize_t ModuleWrite(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset);
static ssize_t ModuleRead(struct file *filp, char *user_buffer, size_t count, loff_t *offset);
/*******************************************************************************/
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = ModuleRead,
    .write      = NULL,
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

static struct i2c_adapter * bmp180_adapter = NULL;
static struct i2c_client * bmp180_client = NULL;

static const struct i2c_device_id bmp_id[] = {
    {SLAVE_NAME, 0}, 
    {}
};

static struct i2c_driver bmpDriver = {
    .driver = {
        .name = SLAVE_NAME;
        .owner = THIS_MODULE;
    }
};

static struct i2c_board_info bmp_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_NAME, BMP180_ADDR)
};
/* Variables for temperature calculation */
s32 dig_T1, dig_T2, dig_T3;
/*******************************************************************************/
s32 read_temperature(void) {
	int var1, var2;
	s32 raw_temp;
	s32 d1, d2, d3;

	/* Read Temperature */
	d1 = i2c_smbus_read_byte_data(bmp180_client, 0xFA);
	d2 = i2c_smbus_read_byte_data(bmp180_client, 0xFB);
	d3 = i2c_smbus_read_byte_data(bmp180_client, 0xFC);
	raw_temp = ((d1<<16) | (d2<<8) | d3) >> 4;

	/* Calculate temperature in degree */
	var1 = ((((raw_temp >> 3) - (dig_T1 << 1))) * (dig_T2)) >> 11;

	var2 = (((((raw_temp >> 4) - (dig_T1)) * ((raw_temp >> 4) - (dig_T1))) >> 12) * (dig_T3)) >> 14;
	return ((var1 + var2) *5 +128) >> 8;
}
/*******************************************************************************/
static int __init i2c_init(void)
{
	int i = 0;
    pr_info("I2C driver - Hello Kernel\n");
	/* 1. Allocating device number (cat /pro/devices)*/
	if (alloc_chrdev_region(&mdev.dev_num, 0, 1, DRIVER_NAME) < 0) {
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
    if ((mdev.m_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }
	/* 04. Creating device*/
    if ((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, DRIVER_NAME)) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }
	/* I2C init */

	return 0;
rm_class:
    class_destroy(mdev.m_class);
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

static ssize_t ModuleRead(struct file *filp, char *user_buffer, size_t count, loff_t *offset)
{
	int to_copy, not_copied, delta;
	char out_string[20];
	int temperature;

	/* Get amount of bytes to copy */
	to_copy = min(sizeof(out_string), count);

	/* Get temperature */
	temperature = read_temperature();
	snprintf(out_string, sizeof(out_string), "%d.%d\n", temperature/100, temperature%100);

	/* Copy Data to user */
	not_copied = copy_to_user(user_buffer, out_string, to_copy);

	/* Calculate delta */
	delta = to_copy - not_copied;

	return delta;
}

static void __exit i2c_exit(void)
{
	int i = 0;
	kfree(mdev.kmalloc_ptr);                        /* 05 */
    device_destroy(mdev.m_class, mdev.dev_num);     /* 04 */
    class_destroy(mdev.m_class);                    /* 03 */
    cdev_del(&mdev.m_cdev);                         /* 02 */
    unregister_chrdev_region(mdev.dev_num, 3);      /* 01 */
	/* I2C Deinit */

}
/*******************************************************************************/
module_init(i2c_init);
module_exit(i2c_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
/*******************************************************************************/