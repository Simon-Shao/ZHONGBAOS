/* ����˵��

�˳�����Ҫ��dac832��vref������5.3v  //5.
���˳���ʹ��240��a/d d/a��Ԫ����Ϊ�����̣�ÿ0.1����Ӧ2��Ԫ��

*/
#include <reg52.h>
#include <intrins.H>
#define  uchar unsigned char
#define  uint  unsigned int
#define  DA_AD_DATA  P0
#define  LED_PORT  P1

//===============================================================

uchar TAB1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xC1,0x88};//��������0-9���� ����
uchar buffer[]={0,0,0,0,0};
uint  buff_A,buff_V;                //������
void  jisuan( void );
void  Delay1(uint  delay_time) ;
void  display();
unsigned char caiji();
uchar disp_sn=0;
//---------------------------------------------------------------------
//---------------------------------------------------------------------
sbit    ADC_START =P3^3;
sbit    ADC_ALE   =P2^6; 
sbit    ADC_OE	  =P2^7;
sbit    DAC_CS	  =P3^7;
sbit    DAC_WR	  =P2^4;
sbit	ADC_EOC   =P3^2;
sbit    ADC_A 	  =P2^5;



sbit	LED_WK0	  =P3^1;
sbit	LED_WK1	  =P3^4;
sbit	LED_WK2   =P3^5;
sbit	LED_WK3   =P3^6;


sbit    Voltage_UP   =P2^0;	
sbit    Voltage_DOWN =P2^1;
sbit    Switch_A_V   =P2^2;
uchar   DA_data;				   
bit     sign=0;

/*���ܣ�����ͨ��0�ɼ�ģ������0809ת�����������ʾ��
START ADC_START  EOC ADC_EOC  OE ADC_OE  ALE ADC_ALE  A ADC_A  B ADC_B  
*/
//==============================================================
void jisuan()
{ 
	if(sign==0)		//��ѹ
	{
		buff_V=buff_V*5;	  				
		buffer[0]=buff_V/1000;				//ʮλ
        buffer[1]=(buff_V%1000)/100;		//��λ
        buffer[2]=((buff_V%1000)%100)/10;	//С������1λ
		buffer[3]=10;
	}
	else  		
	{
		buff_A=buff_A*(25/6);  				//���������Ϊ1Aʱ��AD0���ѹԼΪ3.63
		buffer[0]=buff_A/1000;				//��λ
        buffer[1]=(buff_A%1000)/100;		//С������1λ
        buffer[2]=((buff_A%1000)%100)/10;	//С������2λ
		buffer[3]=11;
	}
}
//===============================================================
                      /*�ɼ�����*/
//===============================================================
 unsigned char caiji()
{    
	uchar buff;
	DA_AD_DATA=0xff;  	 
	ADC_START=0;
	ADC_OE=0;         						//��ʼ������
	ADC_ALE=1;              
	_nop_();_nop_();_nop_();_nop_();
	ADC_ALE=0;        						//ͨ������������  
	ADC_START=1;
	_nop_();_nop_();_nop_();_nop_();
	ADC_START=0;        					//��ת������
	while(ADC_EOC==0);						//�ȴ�ת������
	ADC_OE=1;        						//�������������
	buff=DA_AD_DATA;
	ADC_OE=0;
	return(buff);//ADC0809�Ĺ���ԭ��
}
//===========================================================
void Delay1(uint delay_time)        // ��ʱ����
{
    uint n;
    while(delay_time--) 
    { 
        n = 125;
        while(--n);
    }
}
//===========================================================
void display()
{
	uchar i;
	for(disp_sn=0;disp_sn<4;disp_sn++)
	{
    	i=TAB1[buffer[disp_sn]];
		if((sign==0)&&(disp_sn==1))   i=i&0x7f;//��ʾС����  
		if((sign==1)&&(disp_sn==0))   i=i&0x7f; 
        LED_WK0=0;
        LED_WK1=0;
        LED_WK2=0;
        LED_WK3=0;
		LED_PORT=i;
		switch(disp_sn)
		{
            case 0:{LED_WK3=1;}break;
			case 1:{LED_WK2=1;}break;
			case 2:{LED_WK1=1;}break;
			case 3:{LED_WK0=1;}break;
		}
		Delay1(1);
	}
}
//===========================================================
 void main(void)
 {	   
    DAC_CS=0;
	DAC_WR=0;
	DA_data=0;
	while(1)	
	{	 
		if(Voltage_UP==0)
		{
			Delay1(30);
		   	if(Voltage_UP==0)
			{
				Delay1(30);				
				DA_data=DA_data+2;	  		//�� 0.1V ����
				if(DA_data>=240)
					DA_data=240;
				while(Voltage_UP==0);
			 }  
		}  
		if(Voltage_DOWN==0)
		{  
			Delay1(30);
			if(Voltage_DOWN==0)
			{
				Delay1(120);
			 	if(DA_data==2||DA_data==0)  DA_data=0;	 
			 	else  DA_data=DA_data-2;	//��  0.1V ����
			}
		}
		if (Switch_A_V==0)
		{
			Delay1(10);
			sign=~sign;
			while(Switch_A_V==0);
		}
		DAC_CS=0;
		DAC_WR=0;
		DA_AD_DATA=DA_data;
      	DAC_WR=1;   
        DAC_CS=1;
		if(sign==0)
		{
		  ADC_A=1;						//���õ�ѹͨ��    1
          buff_V=caiji();
		}
		else
		{
		  ADC_A=0;						//���õ���ͨ��   0
          buff_A=caiji();
		}
        jisuan();
		display();	
		Delay1(1);		
	}		
 }	 		