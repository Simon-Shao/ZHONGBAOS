

#define REG_CTRL_4_CFG		0x77

#define SENS_RANGE		(float)0.06
#define REG_CTRL_4					0x20
#define REG_CTRL_5					0x24
#define REG_CTRL_5_CFG		0x40




#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/sdio.h>
#include <libopencm3/stm32/usart.h>

static void			GPIO_set			(void);
#define UC_WHO_AM_I		0x3F
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

static void GPIO_set(void)
{
	rcc_periph_clock_enable(RCC_GPIOD);
	gpio_mode_setup(GPIOD, 0x1, 0x0, GPIO12 | GPIO13 | GPIO14 | GPIO15);

	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_mode_setup(GPIOA, 0x2, 0x0, GPIO5 | GPIO6 | GPIO7);
	gpio_set_output_options(GPIOA, 0x0, 0x3, GPIO5 | GPIO6 | GPIO7);
	gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO6 | GPIO7);

	rcc_periph_clock_enable(RCC_GPIOE);
	gpio_mode_setup(GPIOE, 0x1, 0x0, GPIO3);
	gpio_set_output_options(GPIOE, 0x0, 0x3, GPIO3);
	gpio_set(GPIOE, GPIO3);
}

static inline int 	data_transfer		(int);
static void Get_origin_value(void);