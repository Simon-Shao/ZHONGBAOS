#include<include.h>
#include<define.h>

#define FIT_COUNT 5

int main(void)
{
	
	gpio_initialnize();
	LIS3DSH_INIT();

	while(1)
	{
		// Get the Accelorometer Value from lis3dsh on the Board
		Get_Acc_Value();

		// low pass filter
		AccX = 0.4 * AccX + 0.6 * x_last;
		AccY = 0.4 * AccY + 0.6 * y_last;
		AccZ = 0.4 * AccZ + 0.6 * z_last;

		x_last = AccX;
		y_last = AccY;
		z_last = AccZ;

		if(situp_flag[0] == 0 && AccX <= -900){
			situp_flag[0] = 1;
		}
		if(situp_flag[0] == 1 && AccZ >= 900){
			situp_flag[0] = 0;
			Situps++;
			gpio_toggle(GPIOD, GPIO15);
		}
		if(Situps == FIT_COUNT){
			ALL_LED_ON();
			Situps = 0;
		}

		if(pushup_flag[0] == 0 && pushup_flag[1] == 0 && AccZ <= -900){
			pushup_flag[0] = 1;
		}
		else if(pushup_flag[0] == 1 && pushup_flag[1] == 0 && AccZ >= -600){
			pushup_flag[0] = 0;
			pushup_flag[1] = 1;
		}
		else if(pushup_flag[1] == 1 && pushup_flag[0] == 0 && AccZ <= -1200){
			pushup_flag[1] = 0;
			Pushups++;
			gpio_toggle(GPIOD, GPIO12);
		}
		if(Pushups == FIT_COUNT){
			ALL_LED_ON();
			Pushups = 0;
		}

		if(Jack_Jumpings == 0 && squat_flag[0] == 0 && squat_flag[1] == 0 && AccX <= -900){
			squat_flag[0] = 1;
		}
		else if(squat_flag[0] == 1 && squat_flag[1] == 0 && AccX >= -600){
			squat_flag[0] = 0;
			squat_flag[1] = 1;
		}
		else if(squat_flag[0] == 0 && squat_flag[1] == 1 && AccX <= -1200){
			squat_flag[1] = 0;
			Squats++;
			gpio_toggle(GPIOD, GPIO13);
		}
		if(Squats == FIT_COUNT){
			ALL_LED_ON();
			Squats = 0;
		}

		if(jkjump_flag[0] == 0 && jkjump_flag[1] == 0 && AccX <= -1600){
			jkjump_flag[0] = 1;
		}
		else if(jkjump_flag[0] == 1 && jkjump_flag[1] == 0 && AccX >= -200){
			jkjump_flag[0] = 0;
			jkjump_flag[1] = 1;
		}
		else if(jkjump_flag[0] == 0 && jkjump_flag[1] == 1 && AccX <= -900){
			jkjump_flag[1] = 0;
			Jack_Jumpings++;
			gpio_toggle(GPIOD, GPIO14);
		}
		if(Jack_Jumpings == FIT_COUNT){
			Jack_Jumpings = 0;
			ALL_LED_ON();
		}

		if(Jack_Jumpings){
			gpio_clear(GPIOD, GPIO13);
		}
	}
	return 0;
}

void Blink(int i){
	switch (i)
	{
	case 1:
		gpio_set(GPIOD, GPIO12);
		delay_ms(1);
		gpio_clear(GPIOD, GPIO12);
		break;
	case 2:
		gpio_set(GPIOD, GPIO13);
		delay_ms(1);
		gpio_clear(GPIOD, GPIO13);
		break;
	case 3:
		gpio_set(GPIOD, GPIO14);
		delay_ms(1);
		gpio_clear(GPIOD, GPIO14);
		break;
	case 4:
		gpio_set(GPIOD, GPIO15);
		delay_ms(1);
		gpio_clear(GPIOD, GPIO15);
		break;
	default:
		break;
	}
}
