#ifndef __EBOOK_H__
#define __EBOOK_H__

#include "stm32f4xx_conf.h"

#define EBKPGSize	4928//一页纸的字模大小
#define EBKRDSize	308 //一页纸的数据量


typedef struct
{
	FIL  file;//电子书文件体
	DIR  dir;//电子书文件路径
	u32  hdlen;//头信息长度
	int  basex;//视图X轴基址
	int  basey;//视图Y轴基址
	UINT bytes;//读文件字节数	
	u16  filecnt;//文本文件数
	u16  filecur;//当前的文件
	u8   plctrl;//播放控制,第一位播放、暂停；第二位上一曲；第三位下一曲
	u8   pagecur;//当前页
	u8   buf_sw;//缓冲切换开关
	u8   page_buf[EBKRDSize];//页缓冲
}ebkdef;//电子书用到的资源

void App_ebook(void);
void Change_buf(u8 direction);
void Fill_paper(u8* buf);
void Show_Vfonts(int xpos,u8 num,u8* Fonts);
void Slip_paper(int xpos);
int  Stick_paper(int xpos);
void Finish_ebook(void);


#endif


