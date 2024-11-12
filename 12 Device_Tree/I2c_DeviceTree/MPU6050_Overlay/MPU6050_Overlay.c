#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
/*******************************************************************************/
#define MPU6050_ADDRESS					0x68
#define Who_Am_I_Reg					117
/*******************************************************************************/
/* Declate the probe and remove functions */
static int mpu6050_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int mpu6050_remove(struct i2c_client *client);
/*******************************************************************************/
static struct i2c_client *mpu6050_slave;

static struct of_device_id device_id[] = {
	{
		.compatible = "QuangNM13,MPU6050",
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, device_id);			/* Tell OS to find device compatible with "Mympu6050" */

static struct i2c_device_id i2c_id[] = {
	{"mpu6050_device_id", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, i2c_id);

static struct i2c_driver mpu6050_driver = {
	.probe = mpu6050_probe,
	.remove = mpu6050_remove,
	.id_table = i2c_id,
	.driver = {
		.name = "mpu6050_driver",
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
	proc_file = proc_create("mpu6050", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("mpu6050_driver - Error creating /proc/mpu6050\n");
		return -ENOMEM;
	}
	/* Read mpu6050 from Who_Am_I_Reg, returned value should be 0x68 */
	dummy = i2c_smbus_read_byte_data(mpu6050_slave, Who_Am_I_Reg);
	if (dummy == MPU6050_ADDRESS)
		printk("mpu6050 is detected\n");
	else
	{
		printk("Cannot detect mpu6050\n");
		return -1;
	}
	/* MPU6050 init */
	i2c_smbus_write_byte_data(mpu6050_slave, 0x6B, 0x00);		/* 0x6B */
	i2c_smbus_write_byte_data(mpu6050_slave, 0x1B, 0x08);		/* 0x1B */
	i2c_smbus_write_byte_data(mpu6050_slave, 0x1C, 0x10);		/* 0x1C */
	i2c_smbus_write_byte_data(mpu6050_slave, 0x1A, 0x03);		/* 0x1C */
	return 0;
}

/**
 * @brief This function is called on unloading the driver 
 */
static int mpu6050_remove(struct i2c_client *client) {
	printk("mpu6050_driver - Now I am in the Remove function!\n");
	proc_remove(proc_file);
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