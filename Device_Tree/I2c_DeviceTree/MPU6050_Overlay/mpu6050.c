#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
/*******************************************************************************/
#define MPU6050_ADDRESS					0x68
#define BMP085_OVERSAMPLING_SETTING		1
/*******************************************************************************/
/* Declate the probe and remove functions */
static int mpu6050_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int mpu6050_remove(struct i2c_client *client);
/*******************************************************************************/
static struct i2c_client *mpu6050_slave;

static struct of_device_id device_id[] = {
	{
		.compatible = "Mympu6050",
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, device_id);			/* Tell OS to find device compatible with "Mympu6050" */

static struct i2c_device_id i2c_id[] = {
	{"my_mpu6050_device_id", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, i2c_id);

static struct i2c_driver mpu6050_driver = {
	.probe = mpu6050_probe,
	.remove = mpu6050_remove,
	.id_table = i2c_id,
	.driver = {
		.name = "my_mpu6050_driver",
		.of_match_table = device_id,
	},
};

static struct proc_dir_entry *proc_file;
/*******************************************************************************/
/**
 * @brief Read mpu6050 data
 */
static ssize_t mpu6050_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[3];
	buffer[0] = i2c_smbus_read_byte_data(mpu6050_slave, 0xF6);
	buffer[1] = i2c_smbus_read_byte_data(mpu6050_slave, 0xF7);
	buffer[2] = i2c_smbus_read_byte_data(mpu6050_slave, 0xF8);
	i2c_smbus_write_byte_data(mpu6050_slave, 0xF4, 0x34 + (BMP085_OVERSAMPLING_SETTING<<6));
	copy_to_user(user_buffer, buffer, 3);
	return count;
}

static struct file_operations fops = {
	.write = NULL,
	.read = mpu6050_read,
};

/**
 * @brief This function is called on loading the driver 
 */
static int mpu6050_probe(struct i2c_client *client, const struct i2c_device_id *id) {
	u8 dummy;
	printk("mpu6050_driver - Now I am in the Probe function!\n");

	if(client->addr != MPU6050_ADDRESS) {
		printk("mpu6050_driver - Cannot find mpu6050 device!\n");
		return -1;
	}

	mpu6050_slave = client;
		
	/* Creating procfs file */
	proc_file = proc_create("my_mpu6050", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("mpu6050_driver - Error creating /proc/my_mpu6050\n");
		return -ENOMEM;
	}
	/* Write 0XB6 to 0xE0 to reset mpu6050 */
	i2c_smbus_write_byte_data(mpu6050_slave, 0xE0, 0XB6);
	/* Read chip ID from 0xD0, returned value should be 0x55 */
	dummy = i2c_smbus_read_byte_data(mpu6050_slave, 0xD0);
	i2c_smbus_write_byte_data(mpu6050_slave, 0xF4, 0x34 + (BMP085_OVERSAMPLING_SETTING<<6));
	printk("mpu6050 Chip ID: %d\n", dummy);
	return 0;
}

/**
 * @brief This function is called on unloading the driver 
 */
static int mpu6050_remove(struct i2c_client *client) {
	printk("mpu6050_driver - Now I am in the Remove function!\n");
	proc_remove(proc_file);
	i2c_smbus_write_byte_data(mpu6050_slave, 0xE0, 0XB6);
	return 0;
}
/*******************************************************************************/
/* This will create the init and exit function automatically */
module_i2c_driver(mpu6050_driver);
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A driver for reading mpu6050, using device tree");
/*******************************************************************************/