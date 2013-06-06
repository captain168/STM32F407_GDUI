#include "rtc.h"
#include "lcd.h"
#include "key.h"

static RTC_TimeTypeDef RTC_TimeStructure;
static RTC_InitTypeDef RTC_InitStructure;
static RTC_AlarmTypeDef  RTC_AlarmStructure;
static __IO uint32_t AsynchPrediv=0,SynchPrediv=0;

u8 MyRTC_Init(void)
{
  	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)//未设置时间
  	{  
    	RTC_Config();
    	RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;//RTC 异步除数 （<0X7F）
    	RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;//RTC 同步除数 （<0X7FFF）
    	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//24小时制
    	if(RTC_Init(&RTC_InitStructure) == ERROR)return 1;
    	RTC_TimeRegulate();//设置时间 
  	}
  	else//已设置时间
  	{   
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	    PWR_BackupAccessCmd(ENABLE);//使能RTC操作
	    RTC_WaitForSynchro();//等待RTC APB寄存器同步
	    RTC_ClearFlag(RTC_FLAG_ALRAF);//清除RTC闹钟标志
	    EXTI_ClearITPendingBit(EXTI_Line17);//清除中断线17标志（内部连接至RTC闹钟）
  	}
	return 0;
}

void RTC_Config(void)
{
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  	PWR_BackupAccessCmd(ENABLE);//使能RTC操作
  	RCC_LSEConfig(RCC_LSE_ON);//使用外部晶振
  	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//等待外部晶振准备好
  	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择RTC时钟源  
  	SynchPrediv=0xFF;
  	AsynchPrediv=0x7F; 
  	RCC_RTCCLKCmd(ENABLE);//使能RTC时钟
  	RTC_WaitForSynchro();//等待RTC APB寄存器同步
}

void RTC_TimeShow(u16 x,u16 y)
{
	LCD_Charmd(1,WHITE);
  	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	LCD_Char(x+18,y,':',BLACK);
	LCD_Num(x,y,RTC_TimeStructure.RTC_Hours,2,BLACK);
	LCD_Num(x+30,y,RTC_TimeStructure.RTC_Minutes,2,BLACK);
	//LCD_Num(x+50,y,RTC_TimeStructure.RTC_Seconds,2,RED);
}

void RTC_TimeRegulate(void)
{
  	u32 tmp_hh = 0x01, tmp_mm = 0x01, tmp_ss = 0x01;
	RTC_TimeStructure.RTC_H12= RTC_H12_AM;		

   	RTC_TimeStructure.RTC_Hours = tmp_hh;
    RTC_TimeStructure.RTC_Minutes = tmp_mm;
    RTC_TimeStructure.RTC_Seconds = tmp_ss;	
  	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure)!=ERROR)
  	RTC_WriteBackupRegister(RTC_BKP_DR0,0x32F2);
	LCD_Clear(BLUE);
  	tmp_hh = 0x01;
  	tmp_mm = 0x01;
  	tmp_ss = 0x01;
  	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
  	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 2;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
  	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);//配置RTC闹钟寄存器
  	RTC_ITConfig(RTC_IT_ALRA, ENABLE);//使能闹钟A的中断
  	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);//使能闹钟A
	LCD_Clear(BLUE);
}

void RTC_AlarmShow(u16 x,u16 y)
{
  	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	LCD_String(x,y,"Alarm:  :  :",RED);
	LCD_Num(x+95,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours,2,RED);
	LCD_Num(x+145,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes,2,RED);
	LCD_Num(x+195,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds,2,RED);
}
