#include <stdlib.h>
#include <stdio.h>


static int pushup_flag[2], situp_flag[2], jkjump_flag[2], squat_flag[2];
int x_last, y_last, z_last;
static int Situps, Pushups, Jack_Jumpings, Squats;	
int AccX, AccY, AccZ;


#define SENS_RANGE		((float)0.06)
#define ADD_REG_WHO_AM_I				0x0F
#define ADD_REG_CTRL_4					0x20
#define ADD_REG_CTRL_5					0x24

static void         Get_Acc_Value(void);
static void			gpio_initialnize			(void);
static void 		spi_setup			(void);
static void 		LIS3DSH_INIT		(void);
static void 		LIS3DSH_write_reg	(int, int);
static int 			LIS3DSH_read_reg	(int);

void                ALL_LED_ON(void);

#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

#define ADD_REG_OUT_X_L					0x28
#define ADD_REG_OUT_X_H					0x29


#define SET_READ_SINGLE_CMD(x)			(x | 0x80)

#define SET_READ_MULTI_CMD(x)			(x | 0xC0)

#define SET_WRITE_SINGLE_CMD(x)			(x & (~(0xC0)))

#define SET_WRITE_MULTI_CMD(x)			(x & (~(0x80))	\
										 x |= 0x40)

#define GREEN_ON()					(gpio_set(GPIOD, GPIO12))
#define ORANGE_ON()					(gpio_set(GPIOD, GPIO13))
#define RED_ON()					(gpio_set(GPIOD, GPIO14))
#define BLUE_ON()					(gpio_set(GPIOD, GPIO15))

#define ADD_REG_OUT_Y_L					0x2A
#define ADD_REG_OUT_Y_H					0x2B
#define ADD_REG_OUT_Z_L					0x2C
#define ADD_REG_OUT_Z_H					0x2D

#define GREEN_OFF()					(gpio_clear(GPIOD, GPIO12))
#define ORANGE_OFF()				(gpio_clear(GPIOD, GPIO13))
#define RED_OFF()					(gpio_clear(GPIOD, GPIO14))
#define BLUE_OFF()					(gpio_clear(GPIOD, GPIO15))

static void gpio_initialnize(void)
{
	rcc_periph_clock_enable(RCC_GPIOD);

	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

	rcc_periph_clock_enable(RCC_GPIOA);


    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);

	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO6 | GPIO7);
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO5 | GPIO6 | GPIO7);
	gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO6 | GPIO7);

	rcc_periph_clock_enable(RCC_GPIOE);
	gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
	gpio_set_output_options(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO3);
	gpio_set(GPIOE, GPIO3);
}

static inline int Data_Convert(int j)
{
	int i = 0;

	if(j > 32768)
	{
		i = -(((~j) & 0xFFFF) + 1);
	}
	else
	{
		i = j;
	}

	return i;
}

static void LIS3DSH_INIT(void)
{
	int intRegValue;

	spi_setup();

	intRegValue = LIS3DSH_read_reg(ADD_REG_WHO_AM_I);

	if( intRegValue == WHO_AM_I )
	{
		
		LIS3DSH_write_reg(ADD_REG_CTRL_4, REG_CTRL_4_CFG_VALUE);
		LIS3DSH_write_reg(ADD_REG_CTRL_5, REG_CTRL_5_CFG_VALUE);
		
		intRegValue = LIS3DSH_read_reg(ADD_REG_CTRL_4);
		
		if( intRegValue != REG_CTRL_4_CFG_VALUE )
		{
			while(1);
		}
	}
	else
	{
		while(1);
	}
}

static void Get_Acc_Value(){
        AccX = ((LIS3DSH_read_reg(ADD_REG_OUT_X_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_X_L));
		AccY = ((LIS3DSH_read_reg(ADD_REG_OUT_Y_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_Y_L));
		AccZ = ((LIS3DSH_read_reg(ADD_REG_OUT_Z_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_Z_L));

		AccX = Data_Convert(AccX);
		AccX = AccX * SENS_RANGE;

		AccY = Data_Convert(AccY);
		AccY = AccY * SENS_RANGE;

		AccZ = Data_Convert(AccZ);
		AccZ = AccZ * SENS_RANGE;
}

static void LIS3DSH_write_reg(int reg, int data)
{
	gpio_clear(GPIOE, GPIO3);
	spi_xfer(SPI1, SET_WRITE_SINGLE_CMD(reg));
	spi_xfer(SPI1, data);
	gpio_set(GPIOE, GPIO3);
}

static int LIS3DSH_read_reg(int reg)
{
	int regValue;
	gpio_clear(GPIOE, GPIO3);
	regValue = spi_xfer(SPI1, SET_READ_SINGLE_CMD(reg));
	regValue = spi_xfer(SPI1, 0xFF);
	gpio_set(GPIOE, GPIO3);

	return regValue;
}

static void spi_setup(void)
{
	rcc_periph_clock_enable(RCC_SPI1);

	spi_reset(SPI1);
	spi_init_master(SPI1,
					SPI_CR1_BAUDRATE_FPCLK_DIV_64,
					SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
					SPI_CR1_CPHA_CLK_TRANSITION_1,
					SPI_CR1_DFF_8BIT,
					SPI_CR1_MSBFIRST);
	spi_enable(SPI1);
}

void ALL_LED_ON(void){
    BLUE_ON();
	ORANGE_ON();
	RED_ON();
	GREEN_ON();
}