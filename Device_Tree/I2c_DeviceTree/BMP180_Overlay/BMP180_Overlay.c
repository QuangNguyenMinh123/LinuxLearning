#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/of_device.h>
/*******************************************************************************/
#define BMP180_ADDRESS					0x77
#define BMP180_OVERSAMPLING_SETTING		3
/*******************************************************************************/
/* Declate the probe and remove functions */
static int BMP180_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int BMP180_remove(struct i2c_client *client);
/*******************************************************************************/
static struct i2c_client *bmp180_slave;

static struct of_device_id device_id[] = {
	{
		.compatible = "QuangNM13,BMP180",
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, device_id);			/* Tell OS to find device compatible with "Mybmp180" */

static struct i2c_device_id i2c_id[] = {
	{"bmp180_device_id", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, i2c_id);

static struct i2c_driver bmp180_driver = {
	.probe = BMP180_probe,
	.remove = BMP180_remove,
	.id_table = i2c_id,
	.driver = {
		.name = "bmp180_driver",
		.of_match_table = device_id,
	},
};

static struct proc_dir_entry *proc_file;
/*******************************************************************************/
/**
 * @brief Read bmp180 data
 */
static ssize_t BMP180_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[3];
	int cnt;
	buffer[0] = i2c_smbus_read_byte_data(bmp180_slave, 0xF6);
	buffer[1] = i2c_smbus_read_byte_data(bmp180_slave, 0xF7);
	buffer[2] = i2c_smbus_read_byte_data(bmp180_slave, 0xF8);
	i2c_smbus_write_byte_data(bmp180_slave, 0xF4, 0x34 + (BMP180_OVERSAMPLING_SETTING<<6));
	cnt = copy_to_user(user_buffer, buffer, 3);
	return count;
}

static struct file_operations fops = {
	.write = NULL,
	.read = BMP180_read,
};

/**
 * @brief This function is called on loading the driver 
 */
static int BMP180_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	u8 dummy;
	printk("bmp180_driver - Now I am in the Probe function!\n");

	if(client->addr != BMP180_ADDRESS) {
		printk("bmp180_driver - Cannot find bmp180 device!\n");
		return -1;
	}

	bmp180_slave = client;
	/* Creating procfs file */
	proc_file = proc_create("my_bmp180", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("bmp180_driver - Error creating /proc/my_bmp180\n");
		return -ENOMEM;
	}
	/* Write 0XB6 to 0xE0 to reset bmp180 */
	i2c_smbus_write_byte_data(bmp180_slave, 0xE0, 0XB6);
	/* Read chip ID from 0xD0, returned value should be 0x55 */
	dummy = i2c_smbus_read_byte_data(bmp180_slave, 0xD0);
	i2c_smbus_write_byte_data(bmp180_slave, 0xF4, 0x34 + (BMP180_OVERSAMPLING_SETTING<<6));
	printk("bmp180 Chip ID: %d\n", dummy);
	return 0;
}

/**
 * @brief This function is called on unloading the driver 
 */
static int BMP180_remove(struct i2c_client *client) {
	printk("bmp180_driver - Now I am in the Remove function!\n");
	proc_remove(proc_file);
	i2c_smbus_write_byte_data(bmp180_slave, 0xE0, 0XB6);
	return 0;
}

// static int __init BMP180_init(void)
// {
// 	printk("BMP180_init - Loading the driver\n");
// 	return 0;
// }

// static void __exit BMP180_exit(void)
// {
// 	printk("BMP180_exit - Unloading the driver\n");
// }
/*******************************************************************************/

/* This will create the init and exit function automatically */
module_i2c_driver(bmp180_driver);
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A driver for reading bmp180, using device tree");
/*******************************************************************************/