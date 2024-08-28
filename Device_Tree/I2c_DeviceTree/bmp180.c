#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
/*******************************************************************************/
#define BMP180_ADDRESS					0x77
#define BMP085_OVERSAMPLING_SETTING		3
/*******************************************************************************/
/* Declate the probe and remove functions */
static int bmp180_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int bmp180_remove(struct i2c_client *client);
/*******************************************************************************/
static struct i2c_client *bmp180_slave;

static struct of_device_id device_id[] = {
	{
		.compatible = "Mybmp180",
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, device_id);

static struct i2c_device_id i2c_id[] = {
	{"my_bmp180", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, i2c_id);

static struct i2c_driver bmp180_driver = {
	.probe = bmp180_probe,
	.remove = bmp180_remove,
	.id_table = i2c_id,
	.driver = {
		.name = "my_bmp180",
		.of_match_table = device_id,
	},
};

static struct proc_dir_entry *proc_file;
/*******************************************************************************/
/**
 * @brief Read bmp180 data
 */
static ssize_t bmp180_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[3];
	buffer[0] = i2c_smbus_read_byte_data(bmp180_slave, 0xF6);
	buffer[1] = i2c_smbus_read_byte_data(bmp180_slave, 0xF7);
	buffer[2] = i2c_smbus_read_byte_data(bmp180_slave, 0xF8);
	i2c_smbus_write_byte_data(bmp180_slave, 0xF4, 0x34 + (BMP085_OVERSAMPLING_SETTING<<6));
	copy_to_user(user_buffer, buffer, 3);
	return count;
}

static struct file_operations fops = {
	.write = NULL,
	.read = bmp180_read,
};

/**
 * @brief This function is called on loading the driver 
 */
static int bmp180_probe(struct i2c_client *client, const struct i2c_device_id *id) {
	printk("bmp180_driver - Now I am in the Probe function!\n");

	if(client->addr != BMP180_ADDRESS) {
		printk("bmp180_driver - Wrong I2C address!\n");
		return -1;
	}

	bmp180_slave = client;
		
	/* Creating procfs file */
	proc_file = proc_create("my_bmp180", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("bmp180_driver - Error creating /proc/my_bmp180\n");
		return -ENOMEM;
	}

	u8 dummy;
	/* Write 0XB6 to 0xE0 to reset bmp180 */
	i2c_smbus_write_byte_data(bmp180_slave, 0xE0, 0XB6);
	/* Read chip ID from 0xD0, returned value should be 0x55 */
	dummy = i2c_smbus_read_byte_data(bmp180_slave, 0xD0);
	i2c_smbus_write_byte_data(bmp180_slave, 0xF4, 0x34 + (BMP085_OVERSAMPLING_SETTING<<6));
	printk("bmp180 Chip ID: %d\n", dummy);
	return 0;
}

/**
 * @brief This function is called on unloading the driver 
 */
static int bmp180_remove(struct i2c_client *client) {
	printk("bmp180_driver - Now I am in the Remove function!\n");
	proc_remove(proc_file);
	i2c_smbus_write_byte_data(bmp180_slave, 0xE0, 0XB6);
	return 0;
}
/*******************************************************************************/
/* This will create the init and exit function automatically */
module_i2c_driver(bmp180_driver);
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A driver for reading bmp180, using device tree");
/*******************************************************************************/