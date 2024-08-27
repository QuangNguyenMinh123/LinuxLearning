#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		"Read bmp180 using device tree"
#define LOW					0
#define HIGH				1
#define I2C_BUS				2
#define BMP180_ADDRESS		0x77
#define BMP180_NAME			"BMP180"
#define DRIVER_NAME			"bmp180"
#define DRIVER_CLASS		"bmp180Class"
/*******************************************************************************/
static int bmp180_probe(struct i2c_client *client, const struct i2c_device_id *pid);
static int bmp180_remove(struct i2c_client *client);
static ssize_t bmp180_read(struct file *File, char *user_buffer, size_t count, loff_t *offs);
/*******************************************************************************/
/* Variables for temperature calculation */
s32 dig_T1, dig_T2, dig_T3;
/*******************************************************************************/
static struct of_device_id match_id[] = {
	{
		.compatible = "Mybmp180",
	},
	{}
};

static struct i2c_driver bmp180_driver = {
	.probe = bmp180_probe,
	.remove = bmp180_remove,
	.driver = {
		.name = "bmp180_Driver",
		.of_match_table = match_id,
		.owner = THIS_MODULE,
	},
};
/* Map the file operations */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = NULL,
	.release = NULL,
	.read = bmp180_read,
};
static struct i2c_adapter * bmp180_i2c_adapter = NULL;
static struct i2c_client  * bmp180_i2c_client  = NULL;
/* Variables for Device and Deviceclass*/
static dev_t myDeviceNr;
static struct class *myClass;
static struct cdev myDevice;
static struct i2c_board_info bmp_i2c_board_info = {
	I2C_BOARD_INFO(BMP180_NAME, BMP180_ADDRESS),
};
/*******************************************************************************/
/*
 * @brief Read current temperature from BMP180 sensor
 *
 * @return temperature in degree
 */
static s32 read_temperature(void) {
	int var1, var2;
	s32 raw_temp;
	s32 d1, d2, d3;

	/* Read Temperature */
	d1 = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xFA);
	d2 = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xFB);
	d3 = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xFC);
	raw_temp = ((d1<<16) | (d2<<8) | d3) >> 4;

	/* Calculate temperature in degree */
	var1 = ((((raw_temp >> 3) - (dig_T1 << 1))) * (dig_T2)) >> 11;

	var2 = (((((raw_temp >> 4) - (dig_T1)) * ((raw_temp >> 4) - (dig_T1))) >> 12) * (dig_T3)) >> 14;
	return ((var1 + var2) *5 +128) >> 8;
}

/**
 * @brief This function is used to initialize bmp180
 */
// static int __init bmp180_init(void)	
// {
// 	int ret = -1;
// 	u8 id;
// 	pr_info("This is bmp180_probe, initializing bmp180\n");

// 	/* Allocate myDeviceNr */
// 	if ( alloc_chrdev_region(&myDeviceNr, 0, 1, DRIVER_NAME) < 0) {
// 		printk("myDeviceNr could not be allocated!\n");
// 	}
// 	printk("myDeviceNr - myDeviceNr %d was registered\n", myDeviceNr);

// 	/* Create Device Class */
// 	if ((myClass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
// 		printk("Device Class can not be created!\n");
// 		goto ClassError;
// 	}

// 	/* Create Device file */
// 	if (device_create(myClass, NULL, myDeviceNr, NULL, DRIVER_NAME) == NULL) {
// 		printk("Can not create device file!\n");
// 		goto FileError;
// 	}

// 	/* Initialize Device file */
// 	cdev_init(&myDevice, &fops);

// 	/* register device to kernel */
// 	if (cdev_add(&myDevice, myDeviceNr, 1) == -1) {
// 		printk("Registering of device to kernel failed!\n");
// 		goto KernelError;
// 	}

// 	bmp180_i2c_adapter = i2c_get_adapter(I2C_BUS);

// 	if(bmp180_i2c_adapter != NULL) {
// 		bmp180_i2c_client = i2c_new_client_device(bmp180_i2c_adapter, &bmp_i2c_board_info);
// 		if(bmp180_i2c_client != NULL) {
// 			if(i2c_add_driver(&bmp180_driver) != -1) {
// 				ret = 0;
// 			}
// 			else
// 				printk("Can't add driver...\n");
// 		}
// 		i2c_put_adapter(bmp180_i2c_adapter);
// 	}
// 	printk("BMP180 Driver added!\n");
// 	/* Read Chip ID */
// 	id = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xD0);
// 	printk("ID: 0x%x\n", id);

// 	/* Read Calibration Values */
// 	dig_T1 = i2c_smbus_read_word_data(bmp180_i2c_client, 0x88);
// 	dig_T2 = i2c_smbus_read_word_data(bmp180_i2c_client, 0x8a);
// 	dig_T3 = i2c_smbus_read_word_data(bmp180_i2c_client, 0x8c);

// 	if(dig_T2 > 32767)
// 		dig_T2 -= 65536;

// 	if(dig_T3 > 32767)
// 		dig_T3 -= 65536;

// 	/* Initialice the sensor */
// 	i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf5, 5<<5);
// 	i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));
// 	return ret;
// KernelError:
// 	device_destroy(myClass, myDeviceNr);
// FileError:
// 	class_destroy(myClass);
// ClassError:
// 	unregister_chrdev(myDeviceNr, DRIVER_NAME);
// 	return (-1);
// }

// static void __exit bmp180_exit(void)
// {
	// pr_info("This is bmp180_remove, Goodbye, Kernel\n");
	// i2c_unregister_device(bmp180_i2c_client);
	// i2c_del_driver(&bmp180_driver);
	// cdev_del(&myDevice);
    // device_destroy(myClass, myDeviceNr);
    // class_destroy(myClass);
    // unregister_chrdev_region(myDeviceNr, 1);
// }

/**
 * @brief This function is used to initialize bmp180
 */
static int bmp180_probe(struct i2c_client *client, const struct i2c_device_id *pid)
{
	u8 id;
	int ret = -1;
	pr_info("This is bmp180_probe, initializing bmp180\n");

	/* Allocate myDeviceNr */
	if ( alloc_chrdev_region(&myDeviceNr, 0, 1, DRIVER_NAME) < 0) {
		printk("myDeviceNr could not be allocated!\n");
	}
	printk("myDeviceNr - myDeviceNr %d was registered\n", myDeviceNr);

	/* Create Device Class */
	if ((myClass = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device Class can not be created!\n");
		goto ClassError;
	}

	/* Create Device file */
	if (device_create(myClass, NULL, myDeviceNr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize Device file */
	cdev_init(&myDevice, &fops);

	/* register device to kernel */
	if (cdev_add(&myDevice, myDeviceNr, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto KernelError;
	}

	bmp180_i2c_adapter = i2c_get_adapter(I2C_BUS);

	if(bmp180_i2c_adapter != NULL) {
		bmp180_i2c_client = i2c_new_client_device(bmp180_i2c_adapter, &bmp_i2c_board_info);
		if(bmp180_i2c_client != NULL) {
			if(i2c_add_driver(&bmp180_driver) != -1) {
				ret = 0;
			}
			else
				printk("Can't add driver...\n");
		}
		i2c_put_adapter(bmp180_i2c_adapter);
	}
	printk("BMP180 Driver added!\n");
	/* Read Chip ID */
	id = i2c_smbus_read_byte_data(bmp180_i2c_client, 0xD0);
	printk("ID: 0x%x\n", id);

	/* Read Calibration Values */
	dig_T1 = i2c_smbus_read_word_data(bmp180_i2c_client, 0x88);
	dig_T2 = i2c_smbus_read_word_data(bmp180_i2c_client, 0x8a);
	dig_T3 = i2c_smbus_read_word_data(bmp180_i2c_client, 0x8c);

	if(dig_T2 > 32767)
		dig_T2 -= 65536;

	if(dig_T3 > 32767)
		dig_T3 -= 65536;

	/* Initialice the sensor */
	i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf5, 5<<5);
	i2c_smbus_write_byte_data(bmp180_i2c_client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));
	return ret;
KernelError:
	device_destroy(myClass, myDeviceNr);
FileError:
	class_destroy(myClass);
ClassError:
	unregister_chrdev(myDeviceNr, DRIVER_NAME);
	return (-1);
}

/**
 * @brief This function is used to read bmp180 data
 */
static ssize_t bmp180_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
	return count;
}

/**
 * @brief This function is used to deinitialize bmp180
 */
static int bmp180_remove(struct i2c_client *client)
{
	pr_info("This is bmp180_remove, Goodbye, Kernel\n");
	i2c_unregister_device(bmp180_i2c_client);
	i2c_del_driver(&bmp180_driver);
	cdev_del(&myDevice);
    device_destroy(myClass, myDeviceNr);
    class_destroy(myClass);
    unregister_chrdev_region(myDeviceNr, 1);
	return 0;
}
/*******************************************************************************/
// module_init(bmp180_init);
// module_exit(bmp180_exit);
module_i2c_driver(bmp180_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
/*******************************************************************************/