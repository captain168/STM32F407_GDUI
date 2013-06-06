#ifndef __ALBUM_H__
#define __ALBUM_H__

#include "stm32f4xx_conf.h"

typedef struct
{
	int x;
	int y;
	u16 color;
}POINT;

void App_album(void);
void Convert_pic(u8* filename);
void Fill_Spic(u8* filename);
void Draw_Spic(int x,u16 y);
void Set_Spic(int xpos,u16 ypos);
int  Spic_slipback(int xpos); 
void Fill_Mpic(u8* filename);
void Draw_Mpic(u16 x,u16 y,u8 trans);
void Rotation(u16 x,u16 y,int angle);
int  Mpic_rotaback(int angle);
void Draw_Lpic(int xpos,u8* filename);
void Zoom_Mpic(u16 scale);
void Zoomback(u16 scale);
void Move_Spic(u8* filename);
void Finish_album(void);

#endif


