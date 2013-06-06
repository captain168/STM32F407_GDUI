#ifndef __RTC_H
#define __RTC_H

#include "stm32f4xx_conf.h"

//void RTC_Config(void);
//void RTC_AlarmCfg(void);
u8 MyRTC_Init(void);
void RTC_Config(void);
void RTC_TimeShow(u16 x,u16 y);
void RTC_AlarmShow(u16 x,u16 y);
void RTC_TimeRegulate(void);

#endif

