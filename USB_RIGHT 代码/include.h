#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/sdio.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>


#define GROWTH_LIMIT					(200)

#define WHO_AM_I		0x3F

#define REG_CTRL_4_CFG_VALUE		0x77
#define REG_CTRL_5_CFG_VALUE		0x40

//Fitness regulater  wechatid v3168370549
#define Regulater 5

void delay_ms(int );
void Blink(int);
static inline int 	Data_Convert		(int);

