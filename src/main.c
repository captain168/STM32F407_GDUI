#include "main.h"
#include <stdlib.h>

void TEST_Char(u16 x,u16 y,u8 num,u16 color);
int main(void)
{         		  
	u8 	  res;
	FATFS fatfs;
	FIL   file;
	u8*   gifbuf;
	UINT  bytes;

	Key_Init();//按键初始化		 
	LED_Init();//LED灯初始化
	LCD_Init();//显示屏初始化
	COM1Init(115200);
	MyRTC_Init();//RTC时钟初始化
	Touch_Init();//触屏初始化			
	SPI_Flash_Init();//串行FLASH用于存储系统图片、字库及关键参数
	Load_param();//读出所有系统参数到缓冲中.
	PWM_Init();//PWM用于控制屏幕背光亮度
	TIM2_Init(8400,5000);//LED闪烁
	mem_init();//动态内存分配初始化
	Set_Keyint(KEY4);//按键中断用于截屏
  	f_mount(0,&fatfs);//初始化磁盘	
  	res=f_open(&file,"0:/GIF/nokia.gif",FA_READ);
	if(!res)
	{
		gifbuf=malloc(95*1024);
		f_read(&file,gifbuf,94018,&bytes);
		ShowGif(gifbuf,94018,0,0,80);//开机动画
		free(gifbuf);
		delay_ms(500);
	}					
	GDGUI_start();
}

