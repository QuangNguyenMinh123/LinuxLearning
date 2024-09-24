#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
/*******************************************************************************/
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("QuangNM13");
MODULE_DESCRIPTION("A SPI driver for Nokia5110 LCD");
/*******************************************************************************/
#define LOW					0
#define HIGH				1
#define COMPATIBLE			"QuangNM13,Nokia5110"
/*******************************************************************************/
#define POWER_ON					0
#define POWER_OFF					1 << 2
#define HORIZONTAL_ADDRESS			0
#define VERTICAL_ADDRESS			1 << 1
#define BASIC_INSTRUCTION			0
#define EXTEND_INSTRUCTION			1 << 0
#define DISPLAY_BLANK				0
#define NORMAL_MODE					0b100
#define ALL_SEG_ON					0b001
#define INVERSED_MODE				0b101
/*******************************************************************************/
typedef enum DisplayModeType{
	BLANK 	= 0b00001000,
	NORMAL 	= 0b00001100,
	ALL_SEG = 0b00001001,
	INVERSE	= 0b00001101,
}DisplayModeType;
/*******************************************************************************/
char image[504] = {
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111, 
	0b00011111, 0b00000101, 0b00000111, 
	0b00000000, 
	0b00011111, 0b00000100, 0b00011111
};
/*******************************************************************************/

/*******************************************************************************/
static struct gpio_desc *resetPin = NULL;
static struct gpio_desc *dcPin = NULL;
static struct spi_device *nokia5110 = NULL;
/*******************************************************************************/
static int Nokia5110_probe(struct spi_device *pdev);
static int Nokia5110_remove(struct spi_device *pdev);
/*******************************************************************************/
static void Nokia5110_Write(struct spi_device *pdev, bool isCommand, char* buff, int size)
{
	if (isCommand)						/* If command is sent */
		gpiod_set_value(dcPin, LOW);
	else								/* If data is sent */
		gpiod_set_value(dcPin, HIGH);
	spi_write(pdev, buff, size);
	gpiod_set_value(dcPin, LOW);
}

static void Nokia5110_Reset(void)
{
	gpiod_set_value(resetPin, LOW);
	mdelay(150);
	gpiod_set_value(resetPin, HIGH);
}

static void Nokia5110_goto(struct spi_device *pdev, int x, int y)
{
	char buff[3] = {
		0b00100000 | POWER_OFF | HORIZONTAL_ADDRESS | BASIC_INSTRUCTION,
		0b10000000 | x,		/* Set x address, 0 <= x <= 83*/
		0b01000000 | y		/* Set x address, 0 <= y <= 5*/
	};
	Nokia5110_Write(pdev, true, buff, sizeof(buff));
}

static void Nokia5110_print(struct spi_device *pdev, char* ch, int size)
{
	Nokia5110_Write(pdev, false, ch, size);
}

static void Nokia5110_ClearScreen(struct spi_device *pdev)
{
	char buff[504] = {0};
	Nokia5110_goto(pdev, 0, 0);
	Nokia5110_print(pdev, buff, 504);
}

static void Nokia5110_Init(struct spi_device *pdev)
{
	char buff[] = {
		0x21,		/* Function set */
		0b10000000 | 100,
		0x20,			/* Basic command */
		0x0C
	};
	Nokia5110_Reset();
	Nokia5110_Write(pdev, true, buff, sizeof(buff));
}

static void Nokia5110_Deinit(struct spi_device *pdev)
{
	char buff[] = {
		0b00100000 | POWER_OFF | HORIZONTAL_ADDRESS | EXTEND_INSTRUCTION,
		
	};
	
}



/*******************************************************************************/

/*******************************************************************************/
static struct of_device_id nokia5110_id[] = {
	{
		.compatible = COMPATIBLE,
	},
	{}
};
MODULE_DEVICE_TABLE(spi0, nokia5110_id);
static struct spi_driver nokia5110_driver = {
	.probe = Nokia5110_probe,
	.remove = Nokia5110_remove,
	.driver = {
		.name = "Nokia5110_Driver",
		.of_match_table = nokia5110_id,
	}
};
module_spi_driver(nokia5110_driver);
/*******************************************************************************/
static int Nokia5110_probe(struct spi_device *pdev)
{
	struct device *dev = &pdev->dev;
	printk("Nokia5110 Probe\n");
	/* Check device properties */
	if (!device_property_present(dev, "commandData-gpio"))
	{
		printk("dt_probe - Error! Device property 'commandData-gpio' not found\n");
	}
	if (!device_property_present(dev, "reset-gpio"))
	{
		printk("dt_probe - Error! Device property 'reset-gpio' not found\n");
	}
	/* Obtain GPIO */
	resetPin = gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(resetPin))
	{
		printk("Nokia5110 Probe - Error! cannot setup the reset-gpio\n");
		gpiod_put(resetPin);
	}
	gpiod_set_value(resetPin, HIGH);
	dcPin = gpiod_get(dev, "commandData", GPIOD_OUT_HIGH);
	if (IS_ERR(dcPin))
	{
		printk("Nokia5110 Probe - Error! cannot setup the dcPin\n");
		gpiod_put(dcPin);
	}
	gpiod_set_value(dcPin, HIGH);
	/* Obtain SPI device */
	nokia5110 = pdev;
	Nokia5110_Init(nokia5110);
	mdelay(1000);
	Nokia5110_ClearScreen(nokia5110);
	
	Nokia5110_goto(nokia5110, 0, 0);
	Nokia5110_print(nokia5110, image, 200);
	return 0;
}
/**
 * @brief This function is called, when the module is removed
 */
static int Nokia5110_remove(struct spi_device *pdev)
{
	printk("Nokia5110 Remove\n");
	gpiod_put(resetPin);
	gpiod_put(dcPin);
	return 0;
}
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/
