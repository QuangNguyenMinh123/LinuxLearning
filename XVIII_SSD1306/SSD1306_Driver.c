#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/of_device.h>
#include "SSD1306.h"
/*******************************************************************************/
#define SSD1306_ADDRESS				0x3c
/*******************************************************************************/
/* Declate the probe and remove functions */
static int SSD1306_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int SSD1306_remove(struct i2c_client *client);
/*******************************************************************************/
static struct i2c_client *ssd1306_slave;

static struct of_device_id device_id[] = {
	{
		.compatible = "QuangNM13,SSD1306",
	}, { /* sentinel */ }
};

static struct i2c_device_id i2c_id[] = {
	{"QuangNM13,SSD1306", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, i2c_id);

static struct i2c_driver ssd1306_driver = {
	.probe = SSD1306_probe,
	.remove = SSD1306_remove,
	.id_table = i2c_id,
	.driver = {
		.name = "ssd1306_driver",
		.of_match_table = device_id,
	},
};
/* This will create the init and exit function automatically */
module_i2c_driver(ssd1306_driver);
static struct proc_dir_entry *proc_file;
/*******************************************************************************/
/**
 * @brief Read ssd1306 data
 */
static ssize_t SSD1306_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[3];
	int cnt;
	printk("SSD1306_read: \n");
	cnt = copy_to_user(user_buffer, buffer, 3);
	return count;
}

static ssize_t SSD1306_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	u8 buffer[3];
	int cnt;
	printk("SSD1306_write: \n");
	cnt = copy_to_user(user_buffer, buffer, 3);
	return count;
}

static struct file_operations fops = {
	.write = SSD1306_write,
	.read = SSD1306_read,
};
/*******************************************************************************/
/**
 * @brief This function is called on loading the driver 
 */
static int SSD1306_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("SSD1306_probe\n");
	if(client->addr != SSD1306_ADDRESS) {
		printk("SSD1306_probe: Cannot find ssd1306 device!\n");
		return -1;
	}
	ssd1306_slave = client;
	/* Creating procfs file */
	proc_file = proc_create("ssd1306", 0666, NULL, &fops);
	if(proc_file == NULL) {
		printk("SSD1306_probe: Error creating /proc/ssd1306\n");
		return -ENOMEM;
	}
	return 0;
}

/**
 * @brief This function is called on unloading the driver 
 */
static int SSD1306_remove(struct i2c_client *client) {
	printk("SSD1306_remove\n");
	proc_remove(proc_file);
	return 0;
}
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A driver for reading ssd1306, using device tree");
/*******************************************************************************/