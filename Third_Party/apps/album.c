#include <string.h>
#include <stdlib.h>
#include "arm_math.h"
#include "fsearch.h"
#include "at45db.h"
#include "album.h"
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "led.h"
#include "spi.h"

															  
u16* spic_buf;//预览图缓冲
u8*  pic_name;//图片文件名缓冲
u32* pic_size;//图片文件名缓冲
u16  piccnt;//图片计数器
u8   curpic;//当前选中的图片
u8*  linebuf;//数片行缓冲
int  basex=0,basea=0;//图片水平座标和旋转角度的基值
void App_album(void)
{
	u8  XX;
	DIR picdir;
	int xpos,temp;
	u16 i,ypos,times,angle=0;
	float scale=0;

	spic_buf=gui_buf;
	pic_name=namebuf;
	pic_size=sizebuf;
	linebuf=AT45_buffer;
	Dyna_clear(BLACK);
	if(f_opendir(&picdir,"0:/PICTURE"))
	{
		Dialog_box(40,180,"请插入有效的SD卡，并复位系统.");
		delay_ms(1200);
		Del_Dialog();
		Home_page();
	}
	Dialog_box(40,180,"正在检查缩略图...");
	piccnt=File_search("0:/PICTURE",T_IMAGE);//受缓冲区大小的限制,最多支持50个文件
	for(i=0;i<piccnt;i++)//缓存所有图片的缩略图
	{
		Convert_pic(pic_name+i*13);
	}
	Del_Dialog();
	Dyna_clear(BLACK);
	Draw_scroll(40);
	Set_Spic(basex,230);
	Fill_Mpic(pic_name+curpic*13);	
	for(i=0;i<=100;i++)Draw_Mpic(60,53,i);	
	ADS_SPI_Init();
	while(1)//界面主循环
	{
		if(PEN_int==RESET)
		{
			Read_X(&PENX);
			Read_Y(&PENY);
			xpos=PENX;
			ypos=PENY;
			if(ypos<40)
			{
				Scroll_ctrl(Finish_album);
				ADS_SPI_Init();
			}
			for(times=0;times<500;times++)//触屏操作识别
			{
				Read_X(&PENX);
				Read_Y(&PENY);
				temp=xpos-PENX;
				if(temp>6||temp<-6)break;
				temp=ypos-PENY;
				if(temp>6||temp<-6)break;
				if(PEN_int!=RESET)
				{
					delay_ms(50);
					if(PEN_int!=RESET)break;
				}					
			}
			if(PEN_int!=RESET)//点击操作
			{				
				if(ypos>230)//选中缩略图
				{					
					temp=curpic;
					if(xpos<60){curpic=-basex/60;XX=0;}
					else if(xpos<120){curpic=1-basex/60;XX=60;}
					else if(xpos<180){curpic=2-basex/60;XX=120;}
					else {curpic=3-basex/60;XX=180;}
					My_chdir("0:/PICTURE/temp");	
					Fill_Spic(pic_name+curpic*13);
					LCD_Fill(XX,230,XX+60,240,BLACK);
					Draw_Spic(XX,240);					
					Fill_Mpic(pic_name+temp*13);
					while(basea>0)//转回0度
					{
					 	basea/=2;
						Rotation(20,33,basea);
					}					
					for(i=0;i<=100;i++)Draw_Mpic(60,53,100-i);								
					Fill_Mpic(pic_name+curpic*13);	
					for(i=0;i<=100;i++)Draw_Mpic(60,53,i);//预览图片
					//f_chdir("0:/PICTURE/temp");
					My_chdir("0:/PICTURE/temp");
					Fill_Spic(pic_name+curpic*13);
					LCD_Fill(XX,310,XX+60,320,BLACK);
					Draw_Spic(XX,230);
				}	  	
			}
			else//滑屏操作
			{
				if(times==500)//长按操作
				{
					if(ypos>113&&ypos<153&&xpos>100&&xpos<140)//放大图片
					{
						//隐藏菜单
						Fill_Mpic(pic_name+curpic*13);	
						while(basea>0)//转回0度
						{
						 	basea/=2;
							Rotation(20,33,basea);
						}
						for(i=1;i<=10;i++)
						{
							Draw_scroll(40-4*i);
							LCD_Fill(0,40-4*i,240,45-4*i,BLACK);
							Set_Spic(basex,230+8*i);
							LCD_Fill(0,221+8*i,240,230+8*i,BLACK);
						}
						Set_Spic(basex,318);
						LCD_Fill(0,309,240,318,BLACK);
						Set_Spic(basex,326);
						LCD_Fill(0,317,240,326,BLACK);
						Fill_Mpic(pic_name+curpic*13);//重填缓冲
						while(PEN_int==RESET)
						{	
							Read_X(&PENX);
							Read_Y(&PENY);		
							xpos=abs(PENX-120);
							ypos=abs(PENY-133);
							arm_sqrt_f32(xpos*xpos+ypos*ypos,&scale);
							if(scale>100)scale=100;
							if(scale<0)scale=0;
							Zoom_Mpic(scale+100);
						}
						Zoomback(scale+100);
					}
					else if(ypos>230)//移动缩略图
					{
						Fill_Mpic(pic_name+curpic*13);	
						while(basea>0)//转回0度
						{
						 	basea/=2;
							Rotation(20,33,basea);
						}
						if(xpos<60)curpic=-basex/60;
						else if(xpos<120)curpic=1-basex/60;
						else if(xpos<180)curpic=2-basex/60;
						else curpic=3-basex/60;	
						for(i=0;i<=100;i++)Draw_Mpic(60,53,100-i);//淡去							
						Move_Spic(pic_name+curpic*13);
						Fill_Mpic(pic_name+curpic*13);	
						for(i=0;i<=100;i++)Draw_Mpic(60,53,i);//预览图片
					}
				}
				else//滑动操作
				{
					if(ypos>230)//滑动缩略图
					{			
						basex-=xpos;
						while(PEN_int==RESET)
						{	
							Read_X(&PENX);
							xpos=basex+PENX;
							if(xpos>0)xpos=0;//只能往左滑动,基址横座标小于0
							if(xpos<(240-60*piccnt))xpos=240-60*piccnt;
							Set_Spic(xpos,230);//缩略图显示
						}
						basex=Spic_slipback(xpos);//滑动定位
					}
					else if(ypos>40&&ypos<230&&xpos>180)//旋转预览图
					{
						Fill_Mpic(pic_name+curpic*13);//重填缓冲
						basea-=ypos;
						while(PEN_int==RESET)
						{	
							Read_Y(&PENY);
							ypos=basea+PENY;
							angle=ypos%360;
							Rotation(20,33,angle);
						}
						basea=Mpic_rotaback(angle);				
					}
				}
			}
		}
	}
}

//将图片文件转换成缩略图放入暂存文件夹
void Convert_pic(u8* filename)
{	
	u8 res;
	FIL file;
	UINT Bytes;	
	u16 i,j,color;
	BITMAPFILEHEADER* bmph;
	u8* readbuf;

	My_chdir("0:/PICTURE/temp");//切换到暂存文件夹
	Add_path(path_curr,filename);
	res=f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_close(&file);
	if(!res)return;//打开成功则说明缓冲文件已存在,直接返回
	My_chdir("0:/PICTURE");//切换到图片文件夹
	Add_path(path_curr,filename);	
	readbuf=work_buf+9600;//行缓冲
	f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_read(&file,readbuf,54,&Bytes);//读取54字节文件头
	bmph=(BITMAPFILEHEADER*)readbuf;//获取BMP头
	f_lseek(&file,bmph->bfOffBytes);//直接跳到图像数据
	for(j=0;j<80;j++)
	{		
		f_read(&file,readbuf,720,&Bytes);//读一行数据到缓冲
		f_lseek(&file,f_tell(&file)+2160);//跳过3行数据
		for(i=0;i<60;i++)
		{
			color=RGB888toRGB565(readbuf[12*i],readbuf[12*i+1],readbuf[12*i+2]);
			spic_buf[60*j+i]=color;
		}
	} 
	f_close(&file);
	//写文件
	My_chdir("0:/PICTURE/temp");//切换到暂存文件夹
	Add_path(path_curr,filename);
	f_open(&file,(TCHAR*)path_curr,FA_WRITE|FA_CREATE_ALWAYS);//创建并打开
	f_write(&file,spic_buf,9600,&Bytes);
	f_close(&file);			
}
//填充缩略图
void Fill_Spic(u8* filename)
{
	FIL file;
	UINT Bytes;

	My_chdir("0:/PICTURE/temp");//切换到暂存文件夹
	Add_path(path_curr,filename);
	f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_read(&file,spic_buf,9600,&Bytes);
}
//绘制缩略图
void Draw_Spic(int x,u16 y)
{
	int temp;
	u8  i,j;

	for(j=0;j<80;j++)
	{
		if(y+j<320)
		{
			for(i=0;i<60;i++)
			{
				temp=x+i;
				if(temp>=0&&temp<240)LCD_SetPoint(temp,y+j,spic_buf[60*j+i]);
			}
		}
	}	
}
//设置缩略图位置
void Set_Spic(int xpos,u16 ypos)
{
	u16 i;

	for(i=0;i<piccnt;i++)//缩略图显示
	{		
		if(xpos>(-60-60*i)&&xpos<(240-60*i))//在显示范围内才给于显示
		{
			Fill_Spic(pic_name+i*13);
			Draw_Spic(xpos+60*i,ypos);
		}					
	}
}
//缩略图定位
int Spic_slipback(int xpos)
{
	int temp,offset;

	temp=xpos/60;
	offset=xpos%60;
	if(offset<-30)//往左定位
	{
		temp=(temp-1)*60;
		while(xpos>temp)
		{			
			if(xpos>temp+1)xpos-=2;
			else xpos--;
			Set_Spic(xpos,230);
		}
	}
	else//往右定位
	{
		temp=temp*60;
		while(xpos<temp)
		{
			if(xpos<temp-1)xpos+=2;
			else xpos++;
			Set_Spic(xpos,230);
		}		
	}
	return xpos;
}
//将预览图填充至缓冲
void Fill_Mpic(u8* filename)
{
	FIL file;
	UINT Bytes;	
	u16 i,j,k,color;
	BITMAPFILEHEADER* bmph;
					  
	My_chdir("0:/PICTURE");//切换到暂存文件夹
	Add_path(path_curr,filename);
	f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_read(&file,linebuf,54,&Bytes);//读取54字节文件头
	bmph=(BITMAPFILEHEADER*)linebuf;//获取BMP头
	f_lseek(&file,bmph->bfOffBytes);//直接跳到图像数据
	for(j=0,k=0;j<160;j++)
	{		
		f_read(&file,linebuf,720,&Bytes);//读一行数据到缓冲
		f_lseek(&file,f_tell(&file)+720);//跳过1行数据
		for(i=0;i<120;i++,k++)
		{
			color=RGB888toRGB565(linebuf[6*i],linebuf[6*i+1],linebuf[6*i+2]);
			spic_buf[k]=color;
		}
	}
	f_close(&file);	
}
//绘制缓冲中的预览图
void Draw_Mpic(u16 x,u16 y,u8 trans)
{
	u16	i,color;
	u16 r=0,g=0,b=0;

	LCD_Window(x,y,x+119,y+159);
	LCD_Cursor(x,y);
	LCD_REG=0x22;
	for(i=0;i<19200;i++)
	{
		color=spic_buf[i];
		//黑色画透明点
		r=((color>>11)&0x1F)*trans/100;
		g=((color>>5)&0x3F)*trans/100;
		b=(color&0x1F)*trans/100;
		color=(r<<11)+(g<<5)+b;
		LCD_RAM=color;		
	} 
	LCD_Window(0,0,239,319);
}
//绘制原图
void Draw_Lpic(int xpos,u8* filename)
{
	FIL file;
	UINT Bytes;	
	u16 i,j,k,color;
	BITMAPFILEHEADER* bmph;
	int temp;

	My_chdir("0:/PICTURE");//切换到图片文件夹
	Add_path(path_curr,filename);
	f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_read(&file,linebuf,54,&Bytes);//读取54字节文件头
	bmph=(BITMAPFILEHEADER*)linebuf;//获取BMP头
	f_lseek(&file,bmph->bfOffBytes);//直接跳到图像数据
	for(j=0,k=0;j<320;j++)
	{		
		f_read(&file,linebuf,720,&Bytes);//读一行数据到缓冲
		for(i=0;i<240;i++,k++)
		{
			temp=xpos+i;
			color=RGB888toRGB565(linebuf[3*i],linebuf[3*i+1],linebuf[3*i+2]);
			if(temp>=0&&temp<240)LCD_SetPoint(temp,j,color);
		}
	}
	f_close(&file);	
}
//120*160图片旋转函数	
void Rotation(u16 x,u16 y,int angle)
{	
	int i,j;
	float sina,cosa,radian;
	u16* srcBuf=spic_buf;//原图像缓存
	int tX,tY,tXN,tYN;			

	radian=(360-angle)*PI/180.0f;//角度到弧度转化   
	//通过新图像的坐标，计算对应的原图像的坐标
	sina=arm_sin_f32(radian);
	cosa=arm_cos_f32(radian);
	for(i=7;i<197;i++)
	{        
		for(j=0;j<200;j++)
		{
			//转换到以图像为中心的坐标系，并进行逆旋转
			tX= (j-100)*cosa+(-i+100)*sina;
			tY=-(j-100)*sina+(-i+100)*cosa;
			//如果这个坐标不在原图像内，则不赋值
			if(tX>=60||tX<-60||tY>80||tY<=-80)
			{
				LCD_SetPoint(j+x,i+y,BLACK);
				continue;
			}
			//再转换到原坐标系下
			tXN=tX+60; 
			tYN=abs(tY-80);
			LCD_SetPoint(j+x,i+y,srcBuf[tYN*120+tXN]);
		}
	}
}
//角度定位
int Mpic_rotaback(int angle)
{
	int temp,offset;

	temp=angle/90;
	offset=abs(angle%90);
	if(offset<45)//往左定位
	{
		temp=temp*90;
		while(angle>temp)
		{			
			if(angle>temp+1)angle-=2;
			else angle--;
			Rotation(20,33,angle);
		}
	}
	else//往右定位
	{
		temp=(temp+1)*90;
		while(angle<temp)
		{
			if(angle<temp-1)angle+=2;
			else angle++;
			Rotation(20,33,angle);
		}		
	}
	return angle;	
}
//图片缩放
void Zoom_Mpic(u16 scale)
{
	u16 i,j,refw,refh,temp;
	POINT PLT,PRB;//左上和右下两个顶点

	refw=(120*scale/200)*2;//必须为偶数
	refh=(160*scale/200)*2;
	if(scale<100)scale=100;
	scale=(scale-100)*27/100;//用于随图片放大座标下移	
	PLT.x=120-refw/2;
	PLT.y=133-refh/2+scale;
	PRB.x=119+refw/2;
	PRB.y=132+refh/2+scale;
	LCD_Window(PLT.x,PLT.y,PRB.x,PRB.y);
	LCD_Cursor(PLT.x,PLT.y);
	LCD_REG=0x22;
	for(j=0;j<refh;j++)
	{
		temp=120*(160*j/refh);
		for(i=0;i<refw;i++)LCD_RAM=spic_buf[temp+(120*i/refw)];//行显示
	}
	LCD_Window(0,0,239,319);
	if(PLT.y>0)LCD_Fill(0,0,240,PLT.y,BLACK);
	if(PRB.y<320)LCD_Fill(0,PRB.y+1,240,320,BLACK);
	if(PLT.x>0)LCD_Fill(0,0,PLT.x,320,BLACK);
	if(PRB.x<240)LCD_Fill(PRB.x+1,0,240,320,BLACK);
}
//恢复界面
void RecallUI(u16 scale)
{
	u8 i,temp;

	temp=scale-85;
	while(temp>0)
	{
		temp=temp*94/100;
		scale=temp+85;
		Zoom_Mpic(scale);
	}
	while(scale<100)
	{
		scale++;
		Zoom_Mpic(scale);
	}
	Set_Spic(basex,318);					
	for(i=1;i<=10;i++)
	{
		Draw_scroll(4*i);
		Set_Spic(basex,320-8*i);
	}
	Set_Spic(basex,230);
	LCD_Fill(0,310,240,320,BLACK);
}
//缩放控制
void Zoomback(u16 scale)
{
	if(scale<150)RecallUI(scale);
	else
	{
		while(scale<200)
		{
		 	scale++;
		 	Zoom_Mpic(scale);		
		}
		Draw_Lpic(0,pic_name+curpic*13);
		while(PEN_int!=RESET);
		RecallUI(scale);			
	}
}
//缩略图移动
void Move_Spic(u8* filename)
{
	u8 which;

	OX=240;//赋成不可能的值
	OY=320;
	which=0;
	bufwidth=60;
    bufheight=80;
	FLASH_SPI_Init();
	Draw_icon(90,103,19,BLACK);
	My_chdir("0:/PICTURE/temp");//切换到暂存文件夹
	Fill_Spic(filename);//将要移动的缩略图缓存
	oldbuf=gui_buf+4800;
	newbuf=gui_buf+9600;		
	while(PEN_int==RESET)
	{		
		ADS_SPI_Init();
		Read_X(&PENX);
		Read_Y(&PENY);
		NX=PENX-30;
		NY=PENY-30;
		if(NX<0)NX=0;
		if(NY<0)NY=0;
		if(NX>180)NX=180;
		if(NY>240)NY=240;
		Fill_buff();//在绘制图标之前保存该区域
		Draw_menbuf(NX,NY,gui_buf,1);
		Draw_buff();//填回除当前图标区域以外的背景
		FLASH_SPI_Init();
		if(NX>60&&NX<120&&NY>73&&NY<133)Draw_icon(90,103,18,BLACK);
		else Draw_icon(90,103,19,BLACK);		
		OX=NX;
		OY=NY;
 		if(which)
		{
			oldbuf=gui_buf+4800;
			newbuf=gui_buf+9600;
			which=0;			
		}
		else 
		{
			oldbuf=gui_buf+9600;
			newbuf=gui_buf+4800;//切换缓冲区
			which=1;			
		}
	}
	if(NX>60&&NX<120&&NY>73&&NY<133)
	{
		delay_ms(400);
		LCD_Fill(60,73,181,214,BLACK);
		Dialog_box(40,180,"正在更新背景图片，请稍等...");
		My_chdir("0:/PICTURE");
		Save_pic(pic_name+curpic*13,BGD_addr,BGD_size);
		Del_Dialog();
	}
	else
	{	
		NX=240;//赋成不可能的值
		NY=320;
		Draw_buff();
	}
	bufwidth=240;
    bufheight=80;
	ADS_SPI_Init();
}
//结束浏览
void Finish_album(void)
{

}













