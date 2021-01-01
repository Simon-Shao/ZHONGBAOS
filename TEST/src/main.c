#include<LIS3DSH.h>

int main(void)
{
	
	int ValueMgX, ValueMgY, ValueMgZ;
	int x_filter[3], y_filter[3], z_filter[3];
	int i;
	static int COUNT;
	static int Fit;
	static int pushup[4], situp[4], jk_jump[4], squat[4];
	int x_old, y_old, z_old;
	static int Situps, Pushups, Jumping_Jacks, Squats;

	/* setup all GPIOs */
	gpio_setup();

	/* initialise LIS3DSH */
	LIS3DSH_init();

	/* infinite loop */
	while(1)
	{
		// if(KEY_Scan(0))
		// 	gpio_toggle( GPIOD, GPIO12);
		if(!Situps && !Pushups && !Jumping_Jacks && !Squats)
		{
			COUNT = KEY_Scan(0);

		switch (COUNT % 4)
		{
		case 0:
			LED_BLUE_ON();
			LED_ORANGE_OFF();
			LED_GREEN_OFF();
			LED_RED_OFF();
			Fit = 1;	//蓝灯亮代表pushup即俯卧撑
			break;
		case 1:
			LED_BLUE_OFF();
			LED_ORANGE_OFF();
			LED_GREEN_OFF();
			LED_RED_ON();
			Fit = 2;	//红灯亮代表仰卧起坐
			break;
		case 2:
			LED_BLUE_OFF();
			LED_ORANGE_OFF();
			LED_GREEN_ON();
			LED_RED_OFF();
			Fit = 3;	//绿灯亮代表开合跳
			break;	
		case 3:
			LED_BLUE_OFF();
			LED_ORANGE_ON();
			LED_GREEN_OFF();
			LED_RED_OFF();
			Fit = 4;	//橙灯亮代表上下蹲
			break;
		// default:
		// 	LED_BLUE_OFF();
		// 	LED_ORANGE_OFF();
		// 	LED_GREEN_OFF();
		// 	LED_RED_OFF();
		// 	break;
		}
		}
		/* get X, Y, Z values */
		ValueMgX = ((LIS3DSH_read_reg(ADD_REG_OUT_X_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_X_L));
		ValueMgY = ((LIS3DSH_read_reg(ADD_REG_OUT_Y_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_Y_L));
		ValueMgZ = ((LIS3DSH_read_reg(ADD_REG_OUT_Z_H) << 8) | LIS3DSH_read_reg(ADD_REG_OUT_Z_L));

		/* transform X value from two's complement to 16-bit int */
		ValueMgX = CONVERT(ValueMgX);
		/* convert X absolute value to mg value */
		ValueMgX = ValueMgX * SENS_2G_RANGE_MG_PER_DIGIT;

		ValueMgY = CONVERT(ValueMgY);
		ValueMgY = ValueMgY * SENS_2G_RANGE_MG_PER_DIGIT;

		ValueMgZ = CONVERT(ValueMgZ);
		ValueMgZ = ValueMgZ * SENS_2G_RANGE_MG_PER_DIGIT;

		x_filter[0] = x_filter[1];
		x_filter[1] = x_filter[2];
		x_filter[2] = ValueMgX;

		y_filter[0] = y_filter[1];
		y_filter[1] = y_filter[2];
		y_filter[2] = ValueMgY;

		z_filter[0] = z_filter[1];
		z_filter[1] = z_filter[2];
		z_filter[2] = ValueMgZ;

		ValueMgX = Mid_filter(x_filter);
		ValueMgY = Mid_filter(y_filter);
		ValueMgZ = Mid_filter(z_filter);

		ValueMgX = 0.4 * ValueMgX + 0.6 * x_old;
		ValueMgY = 0.4 * ValueMgY + 0.6 * y_old;
		ValueMgZ = 0.4 * ValueMgZ + 0.6 * z_old;

		x_old = ValueMgX;
		y_old = ValueMgY;
		z_old = ValueMgZ;

		
		

		if(Fit == 1){
		if(pushup[0] == 0 && ValueMgY >= LED_TH_MG){
			pushup[0] = 1;
			LED_BLUE_OFF();
			LED_RED_OFF();
			LED_GREEN_OFF();
			LED_ORANGE_ON();
		}
		if(pushup[0] == 1 && ValueMgX >= LED_TH_MG){
			LED_BLUE_OFF();
			LED_ORANGE_OFF();
			LED_GREEN_OFF();
			LED_RED_ON();
			pushup[0] = 0;
			Pushups++;
		}
		if(Pushups == Regulater){
			Pushups = 0;
		}
		}
		if(Fit == 2){
		if(situp[0] == 0 && ValueMgZ <= -LED_TH_MG){
			LED_BLUE_OFF();
			LED_ORANGE_OFF();
			LED_RED_OFF();
			LED_GREEN_OFF();
			situp[0] = 1;
		}
		if(situp[0] == 1 && ValueMgX <= -LED_TH_MG){
			LED_BLUE_OFF();
			LED_ORANGE_OFF();
			LED_RED_OFF();
			LED_GREEN_ON();
			situp[0] = 0;
			Situps++;
		}
		if(Situps == Regulater){
			Situps = 0;
		}
		}

		if(Fit == 3){
		if(jk_jump[0] == 0 && ValueMgY >= LED_TH_MG){
			LED_BLUE_OFF();
			LED_RED_OFF();
			LED_GREEN_OFF();
			LED_ORANGE_ON();
			jk_jump[0] = 1;
		}
		if(jk_jump[0] == 1 && ValueMgY <= -LED_TH_MG){
			LED_RED_OFF();
			LED_GREEN_OFF();
			LED_ORANGE_OFF();
			LED_BLUE_ON();
			jk_jump[0] = 0;
			Jumping_Jacks++;
		}
		if(Jumping_Jacks == Regulater){
			Jumping_Jacks = 0;
		}
		}
		if(Fit == 4){
		if(squat[0] == 0 && ValueMgZ <= 300){
			LED_BLUE_OFF();
			LED_ORANGE_OFF();
			LED_RED_OFF();
			LED_GREEN_OFF();
			squat[0] = 1;
		}
		if(squat[0] == 1 && ValueMgZ >= 1600){
			LED_BLUE_ON();
			LED_ORANGE_ON();
			LED_RED_ON();
			LED_GREEN_ON();
			squat[0] = 0;
			Squats++;
		}
		if(Squats == Regulater){
			Squats = 0;
		}
		}

		// if(pushup[0] == 0 && ValueMgY >= LED_TH_MG){
		// 	pushup[0] = 1;
		// }
		// if(pushup[0] == 1 && ValueMgX >= LED_TH_MG){
		// 	pushup[0] = 0;
		// 	Pushups++;
			
		// }
		// if(Pushups == 4){
		// 	gpio_toggle(GPIOD, GPIO12);
		// 	Pushups = 0;
		// }

		// if(situp[0] == 0 && ValueMgZ <= -LED_TH_MG){
		// 	situp[0] = 1;
		// }
		// if(situp[0] == 1 && ValueMgX <= -LED_TH_MG){
		// 	situp[0] = 0;
		// 	Situps++;
		// }
		// if(Situps == 4){
		// 	gpio_toggle(GPIOD, GPIO14);
		// 	Situps = 0;
		// }

		// if(jk_jump[0] == 0 && ValueMgY >= LED_TH_MG){
		// 	jk_jump[0] = 1;
		// }
		// if(jk_jump[0] == 1 && ValueMgY <= -LED_TH_MG){
		// 	jk_jump[0] = 0;
		// 	Jumping_Jacks++;
		// }
		// if(Jumping_Jacks == 4){
		// 	gpio_toggle(GPIOD, GPIO14);
		// 	Jumping_Jacks = 0;
		// }

		// if(squat[0] == 0 && ValueMgZ <= 300){
		// 	squat[0] = 1;
		// }
		// if(squat[0] == 1 && ValueMgZ >= 1600){
		// 	squat[0] = 0;
		// 	Squats++;
		// }
		// if(Squats == 4){
		// 	gpio_toggle(GPIOD, GPIO14);
		// 	Squats = 0;
		// }

			

		// if(Situps[0] == 0 && Situps[2] == 0 && ValueMgZ <= -1000){
		// 	Situps[0] = 1;
		// 	LED_ORANGE_ON();
		// }
			
		// if(Situps[0] == 1 && ValueMgZ >= -500){
		// 	Situps[1]++;
		// 	Situps[0] = 0;
		// 	LED_GREEN_ON();
		// }
			
		// if(Situps[1] >= 1 && ValueMgZ <= -1500){
		// 	Situps[2]++;
		// 	Situps[1] = 0;
		// 	LED_RED_ON();
		// }
			
		// if(Situps[2] >= 1 && ValueMgZ <= -100){
		// 	Situps_count++;
		// 	Situps[2] = 0;
		// 	LED_BLUE_ON();
		// }
		// if(Situps_count%2)
		// 	gpio_toggle(GPIOD, GPIO12);

		for (i = 0; i < 100; i++)
		{
			__asm__("nop");
		}
	}

	return 0;
}

static inline int CONVERT(int twoComplValue)
{
	int int16Value = 0;

	if(twoComplValue > 32768)
	{
		int16Value = -(((~twoComplValue) & 0xFFFF) + 1);
	}
	else
	{
		int16Value = twoComplValue;
	}

	return int16Value;
}

int Mid_filter(int *array){
	if(MAX(array[0],array[1]) < array[2])        return MIN(array[0],array[1]);
  		else if( array[2] < MIN(array[0],array[1]) ) return MIN(array[0],array[1]);
  			else    return array[2];
}

void Limit_filter(int* array){
	if(array[2] - array[1] > GROWTH_LIMIT)
		array[2] = array[1] + GROWTH_LIMIT;
	else if(array[2] - array[1] < -GROWTH_LIMIT)
			array[2] = array[1] - GROWTH_LIMIT;
}

static int KEY_Scan(int mode)

{  
    static int key_up = 1;//按键按松开标志
    static int KEY_COUNT;

    if(mode)key_up = 1; //支持连按   

    if(key_up && gpio_get(GPIOA,GPIO0))

    {

        delay_ms(10);//去抖动

        key_up=0;

        if(gpio_get(GPIOA,GPIO0) == 1)return ++KEY_COUNT;
    }
    else 
        if(gpio_get(GPIOA,GPIO0) == 0)  key_up=1;

    return ;// 无按键按下

}

//毫秒级的延时
void delay_ms(int time)
{    
   int i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
}

static void gpio_setup(void)
{
	/* Enable GPIOD clock. */
	rcc_periph_clock_enable(RCC_GPIOD);

	/* Set GPIO12, GPIO13, GPIO14, GPIO15 (in GPIO port D) to 'output push-pull'. */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

	/* Enable GPIOD clock. */
	rcc_periph_clock_enable(RCC_GPIOA);

    //初始化KEY PA0  上拉输入

    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);

	/* LIS3DSH pins map:
	PA5 - SPI1_SCK
	PA6 - SPI1_MISO
	PA7 - SPI1_MOSI
	PE3 - CS_SPI
	*/

	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);
	/* set SPI pins as CLK, MOSI, MISO */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO6 | GPIO7);
	/* Push Pull, Speed 100 MHz */
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO5 | GPIO6 | GPIO7);
	/* Alternate Function: SPI1 */
	gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO6 | GPIO7);

	/* Enable GPIOE clock. */
	rcc_periph_clock_enable(RCC_GPIOE);
	/* set CS as OUTPUT */
	gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
	/* Push Pull, Speed 100 MHz */
	gpio_set_output_options(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO3);
	/* set CS high */
	gpio_set(GPIOE, GPIO3);
}

static void spi_setup(void)
{
	/* Enable SPI1 clock. */
	rcc_periph_clock_enable(RCC_SPI1);

	/* reset SPI1 */
	spi_reset(SPI1);
	/* init SPI1 master */
	spi_init_master(SPI1,
					SPI_CR1_BAUDRATE_FPCLK_DIV_64,
					SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
					SPI_CR1_CPHA_CLK_TRANSITION_1,
					SPI_CR1_DFF_8BIT,
					SPI_CR1_MSBFIRST);
	/* enable SPI1 first */
	spi_enable(SPI1);
}

static void LIS3DSH_init(void)
{
	int intRegValue;

	/* init SPI1 */
	spi_setup();

	/* get WHO AM I value */
	intRegValue = LIS3DSH_read_reg(ADD_REG_WHO_AM_I);

	/* if WHO AM I value is the expected one */
	if( intRegValue == UC_WHO_AM_I_DEFAULT_VALUE )
	{
		
		LIS3DSH_write_reg(ADD_REG_CTRL_4, UC_ADD_REG_CTRL_4_CFG_VALUE);
		LIS3DSH_write_reg(ADD_REG_CTRL_5, UC_ADD_REG_CTRL_5_CFG_VALUE);
		
		intRegValue = LIS3DSH_read_reg(ADD_REG_CTRL_4);
		
		if( intRegValue != UC_ADD_REG_CTRL_4_CFG_VALUE )
		{
			/* ERROR: stay here... */
			while(1);
		}
	}
	else
	{
		/* ERROR: stay here... */
		while(1);
	}
}

static void LIS3DSH_write_reg(int reg, int data)
{
	/* set CS low */
	gpio_clear(GPIOE, GPIO3);
	/* discard returned value */
	spi_xfer(SPI1, SET_WRITE_SINGLE_CMD(reg));
	spi_xfer(SPI1, data);
	/* set CS high */
	gpio_set(GPIOE, GPIO3);
}

static int LIS3DSH_read_reg(int reg)
{
	int regValue;
	/* set CS low */
	gpio_clear(GPIOE, GPIO3);
	regValue = spi_xfer(SPI1, SET_READ_SINGLE_CMD(reg));
	regValue = spi_xfer(SPI1, 0xFF);
	/* set CS high */
	gpio_set(GPIOE, GPIO3);

	return regValue;
}


/* End of file */