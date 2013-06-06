#ifndef __GDGUI_H
#define __GDGUI_H

#include <stm32f4xx.h>
#include "ff.h"

#pragma pack(1)//按字节进行内存对齐
typedef struct tagBITMAPFILEHEADER 
{ 
	WORD bfType;   
	DWORD bfSize; 
	WORD bfReserved1; 
	WORD bfReserved2; 
	DWORD bfOffBytes;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize; 
	LONG biWidth; 
	LONG biHeight; 
	WORD biPlanes; 
	WORD biBitCount; 
	DWORD biCompression; 
	DWORD biSizeImage; 
	LONG biXPelsPerMeter; 
	LONG biYPelsPerMeter; 
	DWORD biClrUsed; 
	DWORD biClrImportant;
} BITMAPINFOHEADER;

//图片存放起始页
#define BGD_addr  	0	//背景图片
#define CLK_addr  	291	//时钟图片
#define LCK_addr  	332	//锁屏图片
#define GDL_addr  	354	//金龙标志
#define SCR_addr  	361 //卷轴图片
#define ICO_addr  	367	//图标图片
#define TYP_addr  	647	//文件类型图片

#define FON_addr  	2628//16*16汉字保存页
#define PAR_addr  	4095//系统参数保存页

//图片占用页数
#define BGD_size	291
#define CLK_size	41
#define LCK_size	22
#define ICO_size	14
#define GDL_size	7
#define SCR_size	6
#define TYP_size	3  	
#define FON_size   	1467

#define Rdirection  1	
#define Ldirection	-1
//背景图片某一行在SPI FLASH中所在页和页内偏移
#define BGDpage(Line)	((Line*480)/528+BGD_addr)
#define BGDoffset(Line)	((Line*480)%528)


static const u16 Icoaddr[20]={ICO_addr,ICO_addr+14,ICO_addr+28,ICO_addr+42,ICO_addr+56,
							ICO_addr+70,ICO_addr+84,ICO_addr+98,ICO_addr+112,ICO_addr+126,
							ICO_addr+140,ICO_addr+154,ICO_addr+168,ICO_addr+182,ICO_addr+196,
							ICO_addr+210,ICO_addr+224,ICO_addr+238,ICO_addr+252,ICO_addr+266};//20个系统图标保存起始页

static const u16 Typaddr[12]={TYP_addr,TYP_addr+3,TYP_addr+6,TYP_addr+9,TYP_addr+12,TYP_addr+15,
							 TYP_addr+18,TYP_addr+21,TYP_addr+24,TYP_addr+27,TYP_addr+30,TYP_addr+33};


static const u8 Icotext[][12]=
{
 	{"  办公  "},
	{" 电子书 "},	
	{" 记事本 "},
	{"  皮肤  "},
	{"  游戏1 "},
	{"  游戏2 "},
	{"  日历  "},
	{" 播放器 "},
	{"  相册  "},
	{"  相机  "},
	{"  设置  "},
	{"  音乐  "},
	{"  工具  "},
	{"  屏幕  "},
	{"  时间  "},
	{"  关于  "},
	{"文件管理"},
	{"  连接  "},
	{"  保留  "},
};


extern u8*  work_buf;
extern u16* gui_buf;
extern u8   GBK_Buf[32];
extern u16  bufwidth;
extern u16  bufheight;
extern u8   appcur;
//用于图片移动的变量
extern int  NX,NY;
extern u16  OX,OY;
extern u16* oldbuf;
extern u16* newbuf;


void UpdateFont(u8 *path);
u32  Get_FontAdr(u8* ch);
void Show_Hanzi(int x0,u16 y0,u8* word,u16 color);
void Show_Fonts(u16 x0,u16 y0,u8* Fonts,u16 color);
void Save_pic(u8* fname,u16 addr,u16 num);
void Update_patterns(void);
void Load_bground(void);
void Load_clock(u16 x,u16 y);
void Win_SetPoint(u16 x,u16 y,u16 color);
u16  Win_Getpoint(u16 x,u16 y);
void Win_TransPoint(u16 x,u16 y,u16 fcolor,u8 Trans);
void Win_HTLine(u16 x1, u16 y, u16 x2 ,u16 color,u8 trans);
void Win_VTLine(u16 y1, u16 x, u16 y2 ,u16 color,u8 trans);
void Win_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u16 trans);
void Win_SFill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u8 trans1,u8 trans2);
void WIN_TCircle(u16 x0, u16 y0, u16 r,u16 color,u8 trans);
void Win_Char(int x,u16 y,u8 num,u16 color);
void Win_Hanzi(int x0,u16 y0,u8 *word,u16 color);
void Win_Fonts(int x0,u16 y0,u8* Fonts,u16 color);
void Win_Num(u16 x,u16 y,u32 num,u8 len,u16 color);
void Fill_back(u16 LINE1,u16 LINE2);
void Dip_guibuf(u16 LINE1,u16 LINE2);
void Load_icon(int x,u16 y,u8 iconum);
void Load_type(int x,int y,u8 typenum);
void Draw_icon(int x,u16 y,u8 iconum,u16 bcolor);
void Dip_slipbuf(u8 which);
void Set_mico(int xpos);
void Set_eico(int xpos);
void ICO_Select(u16 xpos,u16 ypos,u8 iconum);
int  MICO_slipback(int xpos);
int  EICO_slipback(int xpos);
void Load_lock(u16 x);
void Draw_lock(u8 xpos);
void Trans_bground(u8 trans);
int  LCK_Slipback(int xpos);
void Load_lockpage(void);
u8   Prt_Screen(u8* fpath);
void WriteHeader(FIL* file);
void WriteBMPInfo(FIL* file);
void Dyna_clear(u16 color);
void Draw_BGDline(u16 linenum,u8 mode);
void Dyna_bground(void);
void Draw_RRect(u16 x1,u16 y1,u16 x2,u16 y2);
void Dialog_box(u8 x,u8 y,u8* Fonts);
void Del_Dialog(void);
void Save_mono(u8* fname,u16 addr,u16 num);
void Draw_mono(u16 x,u16 y,u16 width,u16 height,u16 addr,u16 color,u8 trans);
void Draw_scroll(u16 ypos);
void Set_scroll(u8 mode);
u16  Scroll_back(u16 ypos,u8 mode);
void Draw_busy(u16 time);
void Fill_buff(void);
void Draw_buff(void);
void Buf_icon(u16* buf,u8 iconum);
void Draw_menbuf(int x,u16 y,u16* buf,u8 mode);
void Move_ICO(u8 iconum);
void Trans_shadow(u16 x,u16 y,u8 width,u8 height,u16 page);



void GDGUI_start(void);
void Home_page(void);
void Slip_mico(int xpos);
void Slip_eico(int xpos);
void Copy_ico(u16 xpos,u16 ypos);
void Click_ico(u16 xpos,u16 ypos,u8 mode);
void Ch_BTcolor(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
u8   Scroll_ctrl(void(*finishfun)(void));
void Select_app(u8 iconum);




void Save_param(void);
void Load_param(void);
void Draw_pagepointer(void);
u8   Draw_bmp(u16 xpos,u16 ypos,u8* path);
void Trans_HLine(u16 x1,u16 y,u16 x2,u16 color,u8 trans);
void Trans_VLine(u16 y1,u16 x,u16 y2,u16 color,u8 trans);
void Trans_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u8 trans);
void Trans_SFill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u8 trans1,u8 trans2);
void Trans_triangle(u16 x,u16 y,u8 size,u16 color,u8 trans,int direc);


#endif

