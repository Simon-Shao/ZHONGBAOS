#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>


/* LIS3DSH registers addresses */
#define ADD_REG_WHO_AM_I				0x0F
#define ADD_REG_CTRL_4					0x20
#define ADD_REG_CTRL_5					0x24
#define ADD_REG_OUT_X_L					0x28
#define ADD_REG_OUT_X_H					0x29
#define ADD_REG_OUT_Y_L					0x2A
#define ADD_REG_OUT_Y_H					0x2B
#define ADD_REG_OUT_Z_L					0x2C
#define ADD_REG_OUT_Z_H					0x2D

/* WHO AM I register default value */
#define UC_WHO_AM_I_DEFAULT_VALUE		0x3F

/* ADD_REG_CTRL_4 register configuration value: X,Y,Z axis enabled and 400Hz of output data rate */
#define UC_ADD_REG_CTRL_4_CFG_VALUE		0x77
#define UC_ADD_REG_CTRL_5_CFG_VALUE		0x40

/* Sensitivity for 2G range [mg/digit] */
#define SENS_2G_RANGE_MG_PER_DIGIT		((float)0.06)

#define LED_TH_MG						(900)	/* 1000mg (1G) */

//Limit_filter growth limit
#define GROWTH_LIMIT					(200)


#define SET_READ_SINGLE_CMD(x)			(x | 0x80)

#define SET_READ_MULTI_CMD(x)			(x | 0xC0)

#define SET_WRITE_SINGLE_CMD(x)			(x & (~(0xC0)))

#define SET_WRITE_MULTI_CMD(x)			(x & (~(0x80))	\
										 x |= 0x40)

#define LED_GREEN_ON()					(gpio_set(GPIOD, GPIO12))
#define LED_ORANGE_ON()					(gpio_set(GPIOD, GPIO13))
#define LED_RED_ON()					(gpio_set(GPIOD, GPIO14))
#define LED_BLUE_ON()					(gpio_set(GPIOD, GPIO15))

#define LED_GREEN_OFF()					(gpio_clear(GPIOD, GPIO12))
#define LED_ORANGE_OFF()				(gpio_clear(GPIOD, GPIO13))
#define LED_RED_OFF()					(gpio_clear(GPIOD, GPIO14))
#define LED_BLUE_OFF()					(gpio_clear(GPIOD, GPIO15))

#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

//Fitness regulater  wechatid v3168370549
#define Regulater 5


static void			gpio_setup			(void);
static void 		spi_setup			(void);
static void 		LIS3DSH_init		(void);
static void 		LIS3DSH_write_reg	(int, int);
static int 			LIS3DSH_read_reg	(int);
static inline int 	CONVERT		(int);
int Mid_filter(int *);
void Limit_filter(int*);
static int KEY_Scan(int );
void delay_ms(int );
void LED_TEST(int intValueMgX, int intValueMgY, int intValueMgZ);