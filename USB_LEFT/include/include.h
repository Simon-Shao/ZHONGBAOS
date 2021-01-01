#define SET_READ_SINGLE_CMD(x)			(x | 0x80)

#define SET_READ_MULTI_CMD(x)			(x | 0xC0)

#define SET_WRITE_SINGLE_CMD(x)			(x & (~(0xC0)))

#define SET_WRITE_MULTI_CMD(x)			(x & (~(0x80))	\
										 x |= 0x40)


/* lis3dsh registers */
#define REG_WHO_AM_I				0x0F
#define REG_OUT_X_L					0x28
#define REG_OUT_X_H					0x29
#define REG_OUT_Y_L					0x2A
#define REG_OUT_Y_H					0x2B
#define REG_OUT_Z_L					0x2C
#define REG_OUT_Z_H					0x2D

static void 		SPI_set			(void);
static void 		lis3dsh_init		(void);
static void 		WRITE_REG	(int, int);
static int 			READ_REG	(int);



static void WRITE_REG(int reg, int data)
{
	gpio_clear(GPIOE, GPIO3);
	spi_xfer(SPI1, SET_WRITE_SINGLE_CMD(reg));
	spi_xfer(SPI1, data);
	gpio_set(GPIOE, GPIO3);
}

static int READ_REG(int reg)
{
	int i;
	gpio_clear(GPIOE, GPIO3);
	i = spi_xfer(SPI1, SET_READ_SINGLE_CMD(reg));
	i = spi_xfer(SPI1, 0xFF);
	gpio_set(GPIOE, GPIO3);

	return i;
}

static void SPI_set(void)
{
	rcc_periph_clock_enable(RCC_SPI1);
	spi_reset(SPI1);
	spi_init_master(SPI1,SPI_CR1_BAUDRATE_FPCLK_DIV_64,SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,SPI_CR1_CPHA_CLK_TRANSITION_1,
					SPI_CR1_DFF_8BIT,
					SPI_CR1_MSBFIRST);
	spi_enable(SPI1);
}

#define Times 5