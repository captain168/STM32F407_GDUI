#include "timer.h"

//对84MHz（TIM3时钟源为168MHz/2）进行TIM_scale分频后作为计数时钟
void TIM2_Init(u32 TIM_scale, u32 TIM_Period)//TIM_Period为16位的数
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	NVIC_InitTypeDef  NVIC_InitStructure; 
	 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
  	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);					
  	TIM_TimeBaseStructure.TIM_Period = TIM_Period;//计数器重装值
  	TIM_TimeBaseStructure.TIM_Prescaler = 0;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  	TIM_PrescalerConfig(TIM2, (TIM_scale-1), TIM_PSCReloadMode_Immediate);
  	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  	TIM_Cmd(TIM2, ENABLE);
}
/******************************************************************/
//配置定时器3输出通道
void TIM3_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//GPIOA时钟使能
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//配置PB5为定时器3第2通道输出引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);//连接复用引脚
}

static TIM_OCInitTypeDef  TIM_OCInitStructure;
void PWM_Init(void)
{
	uint16_t PrescalerValue=0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	

	TIM3_Init();  
	//计算预分频因子	
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 28000000) - 1;
  	//配置时基
  	TIM_TimeBaseStructure.TIM_Period = 665;
  	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//PWM1模式配置：通道2
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStructure.TIM_Pulse = 500;
  	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  	TIM_ARRPreloadConfig(TIM3, ENABLE);
  	TIM_Cmd(TIM3, ENABLE);//计数使能
}

void PWM_SetPulse(u16 Pulse)
{
	TIM_Cmd(TIM3, DISABLE);//计数禁止
	TIM_OCInitStructure.TIM_Pulse = Pulse;
  	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  	TIM_ARRPreloadConfig(TIM3, ENABLE);
  	TIM_Cmd(TIM3, ENABLE);//计数使能
}


