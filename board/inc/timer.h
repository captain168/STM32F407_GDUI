#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx_conf.h"


void TIM2_Init(u32 TIM_scale, u32 TIM_Period);
void TIM3_Init(void);
void PWM_Init(void);
void PWM_SetPulse(u16 Pulse);

#endif
