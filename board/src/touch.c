#include <stdlib.h>
#include "touch.h" 
#include "lcd.h"
#include "delay.h"	 
#include "spi.h"

u16 PENX,PENY;
	 	   
u16 ADS_Read(u8 CMD)	  
{ 	 	  
	u16 Num=0,temp;  	 
	TCS_LOW; 
	SPI1_RWByte(CMD);
	delay_us(6);	 
	temp=SPI1_RWByte(0x00); 
	Num=temp<<8; 
	delay_us(1); 
	temp=SPI1_RWByte(0x00); 
	Num|=temp;  	
	Num>>=4;
	TCS_HIGH; 
	return(Num);  
} 

u16 Read_XY(u8 CMD)
{
	u16 i, j;
	u16 buf[10];
	u16 sum=0;
	u16 temp;
	for(i=0;i<10;i++)buf[i]=ADS_Read(CMD);				    
	for(i=0;i<9; i++)
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	for(i=3;i<7;i++)sum+=buf[i];
	temp=sum/(4);
	return temp;   
} 
//触摸屏初始化
void Touch_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	EXTI_InitTypeDef EXTI_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//pen_int:PD6   
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//将其配置成中断
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;//优先级最低
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0E;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 	
			   
	SPI1_Init();
	ADS_SPI_Init();
	CSPin_init();
}

//连续读取2次有效的AD值,且这两次的偏差不能超过ERR_RANGE
#define ERR_RANGE 10 //误差范围 
#define MIN_VALUE 50 //AD最小值
u8 Read_X(u16* xpos)
{	
	int temp=0,v1,v2;

	while(1)
	{
		if(PEN_int!=RESET)return 1;//返回错误
	    v1=Read_XY(CMD_RDX);   
	    if(v1<MIN_VALUE)continue;
	    v2=Read_XY(CMD_RDX);   
	    if(v2<MIN_VALUE)continue;
		if(abs(v1-v2)>ERR_RANGE)continue;
		temp=(v1+v2)/2;
		break;
	}
	*xpos=240*(temp-ADx1)/(ADx2-ADx1);
	return 0; 
}

u8 Read_Y(u16* ypos)
{
	int temp=0,v1,v2;

	while(1)
	{
		if(PEN_int!=RESET)return 1;//返回错误
	    v1=Read_XY(CMD_RDY);   
	    if(v1<MIN_VALUE)continue;
	    v2=Read_XY(CMD_RDY);   
	    if(v2<MIN_VALUE)continue;
		if(abs(v1-v2)>ERR_RANGE)continue;
		temp=(v1+v2)/2;	
		break;		
	}
	*ypos=320*(temp-ADy1)/(ADy2-ADy1);
	return 0; 	
}
 


