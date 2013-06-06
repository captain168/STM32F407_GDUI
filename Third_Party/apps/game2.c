#include "game2.h"
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "CS4334.h"
#include "lcd.h"
#include "led.h"
#include "spi.h"

void App_game2(void)
{
	u16 xpos,ypos;

	Dyna_clear(BLACK);
	//Draw_busy(1);
	if(Draw_bmp(0,40,"0:/apps/piano/piano.bmp"))
	{
		Dialog_box(40,180,"请插入有效的SD卡，并复位系统.");
		delay_ms(1200);
		Del_Dialog();
		Home_page();	
	}	
 	ADS_SPI_Init();	
	I2S_GPIO_Init();
	Audio_I2S_Init(I2S_AudioFreq_96k);
	I2S_Cmd(SPI2,ENABLE);
	while(1)
	{
		if(PEN_int==RESET)
		{
			Read_X(&PENX);
			Read_Y(&PENY);
		  	xpos=PENX;
			ypos=PENY;
			if(ypos<40)
			{
				Scroll_ctrl(Finish_game2);
				ADS_SPI_Init();	
			}
			if(xpos>40&&ypos>40)
			{	
				if(ypos<80)
				{
					Buf_back(40);
					LCD_Fill(40,40,240,59,GRAY[20]);  
					LCD_Fill(151,58,240,77,GRAY[20]);
					Play_sound(110);
					while(PEN_int==RESET);											
					Redraw_back(40);
				}
				else if(ypos<120)
				{
					Buf_back(80);
					LCD_Fill(151,80,240,84,GRAY[20]);				
					LCD_Fill(40,84,240,104,GRAY[20]);
					LCD_Fill(151,104,240,117,GRAY[20]);
					Play_sound(125);
					while(PEN_int==RESET);
					Redraw_back(80);
				}
				else if(ypos<160)
				{
					Buf_back(120);
					LCD_Fill(151,120,240,131,GRAY[20]);				
					LCD_Fill(40,131,240,151,GRAY[20]);
					LCD_Fill(151,151,240,157,GRAY[20]);
					Play_sound(140);
					while(PEN_int==RESET);
					Redraw_back(120);
				}
				else if(ypos<200)
				{
					Buf_back(160);
					LCD_Fill(151,160,240,177,GRAY[20]);
					LCD_Fill(40,177,240,197,GRAY[20]);
					Play_sound(155);
					while(PEN_int==RESET);			
					Redraw_back(160);
				}
				else if(ypos<240)
				{
					Buf_back(200);
					LCD_Fill(40,200,240,221,GRAY[20]);
					LCD_Fill(151,221,240,237,GRAY[20]);
					Play_sound(170);
					while(PEN_int==RESET);		
					Redraw_back(200);
				}
				else if(ypos<280)
				{
					Buf_back(240);
					LCD_Fill(151,240,240,249,GRAY[20]);				
					LCD_Fill(40,249,240,269,GRAY[20]);
					LCD_Fill(151,269,240,277,GRAY[20]);
					Play_sound(185);
					while(PEN_int==RESET);
					Redraw_back(240);
				}
				else
				{
					Buf_back(280);
					LCD_Fill(151,280,240,300,GRAY[20]);
					LCD_Fill(40,298,240,320,GRAY[20]);
					Play_sound(200);
					while(PEN_int==RESET);								
					Redraw_back(280);
				} 
			}	
		}
	}
}

void Buf_back(u16 ypos)
{
	u16 i,j,k;

	for(j=0,k=0;j<40;j++)
	{
		for(i=0;i<240;i++,k++)
		{
			gui_buf[k]=LCD_GetPoint(i,ypos+j);
		}	
	}
}

void Redraw_back(u16 ypos)
{
	u16 i,j,k;

	for(j=0,k=0;j<40;j++)
	{
		for(i=0;i<240;i++,k++)
		{
			LCD_SetPoint(i,ypos+j,gui_buf[k]);
		}	
	}
}

void Play_sound(u8 tone)
{
	u16 i,j;
	
	for(i=0;i<(1000-tone*3);i++)
	{
		for(j=0;j<tone;j++)
		{	
			while((SPI2->SR&SPI_I2S_FLAG_TXE)==RESET);
			SPI2->DR = sinewave[200*j/tone];			
		}
	}	
}

//结束game2
void Finish_game2(void)
{

}



