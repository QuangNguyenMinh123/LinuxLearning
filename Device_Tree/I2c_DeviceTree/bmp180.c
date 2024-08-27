#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
/*******************************************************************************/
#define DRIVER_AUTHOR 		"QuangNM13"
#define DRIVER_DESC   		"Read bmp180 using device tree"
#define LOW					0
#define HIGH				1
/*******************************************************************************/

/*******************************************************************************/
static int bmp180_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int bmp180_remove(struct i2c_client *client);
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
		.name = "bmp_Driver",
		.of_match_table = match_id,
	},
};

// static struct file_operations file_ops = {
//     .owner = THIS_MODULE,
// 	.	
//     .write = bmp180_write,
// };

/*******************************************************************************/
/**
 * @brief bmp180_probe
 */
static int bmp180_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	pr_info("This is bmp180_probe\n");
	return 0;
}
/**
 * @brief bmp180_remove
 */
static int bmp180_remove(struct i2c_client *client)
{
	pr_info("This is bmp180_remove\n");
	return 0;
}

/*******************************************************************************/
module_i2c_driver(bmp180_driver);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
/*******************************************************************************/