#include<lis3dsh.h>

#include<include.h>

int X_value, Y_value, Z_value;

int main(void)
{
	//initialise the GPIO
	GPIO_set();
	// initialise lis3dsh 
	lis3dsh_init();

	static int Pushup[3], Situp[3], Jump[3], Squat[3];
	static int Situp_counts, Pushup_counts, Jumping_counts, Squat_counts;

	while(1)
	{
		Get_origin_value();
		
		if(Situp[0] == 0 && Z_value >= 900){
			Situp[0] = 1;
		}
		if(Situp[0] == 1 && X_value >= 900){
			Situp[0] = 0;
			Situp_counts++;
			gpio_set(GPIOD, GPIO15);
		}
		if(Situp_counts == Times){
			gpio_clear(GPIOD, GPIO15);
			Situp_counts = 0;
		}

		if(Pushup[0] == 0 && Pushup[1] == 0 && Z_value <= -900){
			Pushup[0] = 1;
		}
		else if(Pushup[0] == 1 && Pushup[1] == 0 && Z_value >= -600){
			Pushup[0] = 0;
			Pushup[1] = 1;
		}
		else if(Pushup[1] == 1 && Pushup[0] == 0 && Z_value <= -1200){
			Pushup[1] = 0;
			Pushup_counts++;
			gpio_set(GPIOD, GPIO12);
		}
		if(Pushup_counts == Times){
			gpio_clear(GPIOD, GPIO12);
			Pushup_counts = 0;
		}

		if(Jumping_counts == 0 && Squat[0] == 0 && Squat[1] == 0 && X_value >= 900){
			Squat[0] = 1;
		}
		else if(Squat[0] == 1 && Squat[1] == 0 && X_value <= 600){
			Squat[0] = 0;
			Squat[1] = 1;
		}
		else if(Squat[0] == 0 && Squat[1] == 1 && X_value >= 1200){
			Squat[1] = 0;
			Squat_counts++;
			gpio_set(GPIOD, GPIO13);
		}
		if(Squat_counts == Times || Jumping_counts){
			gpio_clear(GPIOD, GPIO13);
			Squat_counts = 0;
		}

		if(Jump[0] == 0 && Jump[1] == 0 && X_value >= 1800){
			Jump[0] = 1;
		}
		else if(Jump[0] == 1 && Jump[1] == 0 && X_value <= 0){
			Jump[0] = 0;
			Jump[1] = 1;
		}
		else if(Jump[0] == 0 && Jump[1] == 1 && X_value >= 900){
			Jump[1] = 0;
			Jumping_counts++;
			gpio_set(GPIOD, GPIO14);
		}
		if(Jumping_counts == Times){
			Jumping_counts = 0;
			gpio_clear(GPIOD, GPIO14);
		}
	}

	return 0;
}

static void lis3dsh_init(void)
{
	int i;
	SPI_set();
	i = READ_REG(REG_WHO_AM_I);
	if( i == 0X3F)
	{
		WRITE_REG(REG_CTRL_4, REG_CTRL_4_CFG);
		WRITE_REG(REG_CTRL_5, REG_CTRL_5_CFG);
		i = READ_REG(REG_CTRL_4);
		if( i != REG_CTRL_4_CFG )
			while(1);
	}
	else
		while(1);
}

static void Get_origin_value(){
		X_value = ((READ_REG(REG_OUT_X_H) << 8) | READ_REG(REG_OUT_X_L));
		X_value = data_transfer(X_value);
		X_value = X_value * SENS_RANGE;

		Y_value = ((READ_REG(REG_OUT_Y_H) << 8) | READ_REG(REG_OUT_Y_L));
		Y_value = data_transfer(Y_value);
		Y_value = Y_value * SENS_RANGE;

		Z_value = ((READ_REG(REG_OUT_Z_H) << 8) | READ_REG(REG_OUT_Z_L));
		Z_value = data_transfer(Z_value);
		Z_value = Z_value * SENS_RANGE;
}

static inline int data_transfer(int value)
{
	int i = 0;
	if(value > 32768)	i = -(((~value) & 0xFFFF) + 1);
	else	i = value;
	return i;
}




