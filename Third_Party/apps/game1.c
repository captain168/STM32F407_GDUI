#include <stdlib.h>
#include "arm_math.h"
#include "game1.h"
#include "gdgui.h"
#include "mma7455.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "spi.h"

#define HOLENUM		19
const u16 holepos[HOLENUM][2]={{50,90},{50,130},{50,170},{50,210},{50,250},{50,290},
								{120,65},{120,105},{120,145},{120,185},{120,225},{120,265},
								{190,90},{190,130},{190,170},{190,210},{190,250},{190,290},{224,304}};
int Sx,Sy;
u8  Pbuf_sw;
u16* ballbuf1;
u16* ballbuf2;								
void App_game1(void)
{
	u8 i;
	signed char Ax,Ay;
	int xaj,yaj;
	float Vx,Vy;
	
	ballbuf1=malloc(30*30*sizeof(u16));
	if(ballbuf1==NULL){Finish_game1();return;}
	ballbuf2=malloc(30*30*sizeof(u16));
	if(ballbuf2==NULL){Finish_game1();return;}

	FLASH_SPI_Init();
	Trans_bground(50);
	Draw_scroll(40);
	Dialog_box(40,180,"请将板水平放置，然后按下触摸屏继续.");
	while(PEN_int!=RESET);
	Del_Dialog();
	MMA_Init();	
	xaj=-Get_MMAval(CMD_GETY);//读Y轴的值
	yaj=-Get_MMAval(CMD_GETX);//读X轴的值	

	OX=240;//赋成不可能的值
	OY=320;
	Sx=0;//滚球初始位置
	Sy=300;
	Vx=0;//滚球初始速度
	Vy=0;	
	Pbuf_sw=0;
	bufwidth=30;
    bufheight=30;	
	oldbuf=ballbuf1;
	newbuf=ballbuf2; 
	//MMA_SPI_Init(); 
	//LCD_Charmd(1,WHITE); 
	for(i=0;i<HOLENUM-1;i++)
	{
		Fill_Circle(holepos[i][0],holepos[i][1],16,BLACK);
	}
	Fill_Circle(holepos[HOLENUM-1][0],holepos[HOLENUM-1][1],15,RED);
	while(1)
	{
		if(PEN_int==RESET)
		{
			ADS_SPI_Init();
			Read_Y(&PENY);
			if(PENY<40)
			{
				Scroll_ctrl(Finish_game1);
				ADS_SPI_Init();
			}	
			MMA_SPI_Init();
		}
		else
		{
			Ax=Get_MMAval(CMD_GETY)+xaj;//读Y轴的值
			Ay=Get_MMAval(CMD_GETX)+yaj;//读X轴的值
			Vx-=(float)Ax/3;//速度在加速度的作用下改变
			Vy+=(float)Ay/3;
			Vx=Vx*9/10;//速度损耗
			Vy=Vy*9/10;
			Sx+=Vx;//位移改变
			Sy+=Vy;
			if(Sx<0){Sx=0;Vx=-Vx*0.8f;Vy=Vy*0.8f;}//碰壁反弹
			if(Sy<40){Sy=40;Vx=Vx*0.8f;Vy=-Vy*0.8f;}
			if(Sx>209){Sx=209;Vx=-Vx*0.8f;Vy=Vy*0.8f;}
			if(Sy>289){Sy=289;Vx=Vx*0.8f;Vy=-Vy*0.8f;}
			NX=Sx;
			NY=Sy;			
			Set_ball(100);
			Check_hole(NX+15,NY+15);
		}
	}
}
//绘制数组中的位图
void Draw_pattern(u16 x,u16 y,u16 width,u16 height,u16* pic,u16 Tcolor,u8 trans)
{
  	u16 i,j,k;
	
	for(j=0,k=0;j<height;j++)
	{
		for(i=0;i<width;i++,k++)
		{
			if(pic[k]==Tcolor)LCD_SetPoint(i+x,j+y,newbuf[j*30+i]);
			else 
			{
				if(trans==100)LCD_SetPoint(i+x,j+y,pic[k]);
				else TransPoint(i+x,j+y,Bcolor,trans);
			}
		}
	}
}
//根据位置判断是否掉进洞里
void Check_hole(u16 x,u16 y)
{
	u8 i;
	int temp1,temp2;
	float distance;
	
	for(i=0;i<HOLENUM;i++)
	{
		temp1=x-holepos[i][0];
		temp2=y-holepos[i][1];
		arm_sqrt_f32(temp1*temp1+temp2*temp2,&distance);
		if(distance<15)//
		{
			while(temp1||temp2)
			{
				temp1=holepos[i][0]-x;
				temp2=holepos[i][1]-y;			
				if(temp1%2)temp1++;
				if(temp2%2)temp2++;
				x+=temp1/2;
				y+=temp2/2;
				NX=x-15;
				NY=y-15;
				Set_ball(100);
			}	
			FLASH_SPI_Init();
			if(i==HOLENUM-1)
			{
				Bcolor=RED;
				for(i=0;i<10;i++)Set_ball(10*i);				
				Dialog_box(40,180,"   恭喜,您已过关!");
			}
			else
			{
				Bcolor=BLACK;
				for(i=0;i<10;i++)Set_ball(10*i);
				Dialog_box(40,180,"      请重试!");						
			}
			delay_ms(500);
			Del_Dialog();			
			MMA_SPI_Init();
			Sx=0;//滚球初始位置
			Sy=300;				
			return;
		}
	}
}

void Set_ball(u8 trans)
{
	Fill_buff();//在绘制图标之前保存该区域
	Draw_pattern(NX,NY,30,30,(u16*)Image_ball,RED,trans);			
	Draw_buff();//填回除当前图标区域以外的背景
	OX=NX;
	OY=NY;			
	delay_ms(40);
	if(Pbuf_sw)
	{
		oldbuf=ballbuf1;
		newbuf=ballbuf2;
		Pbuf_sw=0;			
	}
	else 
	{
		oldbuf=ballbuf2;
		newbuf=ballbuf1;//切换缓冲区
		Pbuf_sw=1;			
	}
}









//结束game1
void Finish_game1(void)
{
	bufwidth=240;
    bufheight=80;
	free(ballbuf1);
	free(ballbuf2);
}







