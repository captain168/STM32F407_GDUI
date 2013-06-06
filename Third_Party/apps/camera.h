#ifndef __CAMERA_H
#define __CAMERA_H

#include <stm32f4xx.h>

																	
#define RGB565toRGB555(RGB) ((RGB&0xF800)>>1)+((RGB&0x7C0)>>1)+(RGB&0x1f)
#define BGR565toRGB555(BGR) ((BGR&0x1f)<<10)+((BGR&0x7C0)>>1)+((BGR&0xF800)>>11)


typedef struct
{
	FIL  file;//相片文件体
	DIR  dir;//相片文件路径
	u32  hdlen;//头信息长度
	int  basex;//视图X轴基址
	int  basey;//视图Y轴基址
	UINT bytes;//读文件字节数	
	u16  filecnt;//文本文件数
	u16  filecur;//当前的文件
	u8   plctrl;//播放控制
	u8   pagecur;//当前页
	u8   buf_sw;//缓冲切换开关
	u16  line_buf[240];//行缓冲
}camdef;//相机用到的资源

void App_camera(void);
void Finish_camera(void);

#endif

