#include <string.h>
#include "arm_math.h"
#include "stdlib.h"
#include "at45db.h"
#include "fsearch.h"
#include "apps.h"
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "spi.h"

u16  file_cnt;
u8*  file_name;
u32* file_size;
u8*  file_trib;
u16	 file_curr;


void App_fmanager(void)
{
	DIR filedir;
	int temp,xpos,ypos,basey,i;
	u16 times;

	file_name=namebuf;
	file_size=sizebuf;
	file_trib=tribbuf;
	My_chdir("0:");
	Load_bground();
	Draw_scroll(40);
fmstart:
	basey=0;
	if(f_opendir(&filedir,(TCHAR*)path_curr))
	{
		Dialog_box(40,180,"请插入有效的SD卡，并复位系统.");
		delay_ms(1200);
		Del_Dialog();
		Home_page();
	}
	file_cnt=File_search(path_curr,T_ANY);//受缓冲区大小的限制,最多支持50个文件
	FLASH_SPI_Init();
	i=300;
	LCD_Charmd(0,WHITE);
	while(i>0)
	{
		i/=2;
		File_list(i,basey);//显示列表		
	}		
	Show_path(path_curr);//显示当前目录	
	ADS_SPI_Init();		
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
				if(Scroll_ctrl(Finish_fmanager)&&path_curr[2])//返回上级
				{
					FLASH_SPI_Init();
					Cut_path(path_curr);
					Show_path(path_curr);
					goto fmstart;
				}
				ADS_SPI_Init();
			}
			for(times=0;times<1000;times++)//触屏操作识别
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
				if(ypos>70&&ypos<310&&ypos<(74+file_cnt*26))
				{
					for(i=0;i<9&&i<file_cnt;i++)//一屏显示9个文件
					{
						if(ypos>(74+i*26)&&ypos<(100+i*26))
						{
							Trans_SFill(0,74+i*26,240,100+i*26,WHITE,100,0);
							file_curr=i-basey/26;
						}
					}
					FLASH_SPI_Init();
					switch(Get_type(file_name+file_curr*13,file_trib[file_curr]))
					{
					 	case T_ANY:	Dialog_box(40,180,"文件格式不支持.");
									delay_ms(1200);
									Del_Dialog();
									break;
						case T_DIR:	Add_path(path_curr,file_name+file_curr*13);//打开目录									
									Show_path(path_curr);
									goto fmstart;	
						case T_AUDIO:appcur=11;
									App_music();
									break;
						case T_IMAGE:appcur=8;
									App_album();
									break;
						case T_VIDO:break;
						case T_BOOK:break;
						case T_GAME:break;
						case T_SYS: Dialog_box(40,180,"系统文件，不可操作.");
									delay_ms(1200);
									Del_Dialog();
									break;
						default:break;
					}
					delay_ms(300);
					File_list(0,basey);
					ADS_SPI_Init();				
				}									
			}
			else//滑屏操作
			{
				if(times==1000)//长按操作
				{
					if(ypos>70&&ypos<310&&ypos<(74+file_cnt*26))
					{
						for(i=0;i<9&&i<file_cnt;i++)//一屏显示9个文件
						{
							if(ypos>(74+i*26)&&ypos<(100+i*26))file_curr=i-basey/26;
						}				 	
						Move_file();
					}
				}
				else//滑动操作
				{
					if(ypos>70&&ypos<300)
					{
						if(xpos>40&&xpos<180)//滑动列表
						{
							basey-=ypos;
							while(PEN_int==RESET)
							{	
								ADS_SPI_Init();														
								Read_Y(&PENY);
								ypos=basey+PENY;
								if(file_cnt<10||ypos>0)ypos=0;//只能往上滑动,基址纵座标小于0
								else if(ypos<(230-26*file_cnt))ypos=230-26*file_cnt;
								FLASH_SPI_Init();
								File_list(0,ypos);//显示列表							
							}
							basey=Stick_Flist(ypos);
							ADS_SPI_Init();
						}
						else if(xpos<40)
						{
							Select_file();
						}
					}
				}
			}
		}	
	}
}
//显示文件列表
void File_list(int xpos,int ypos)
{
	u16 i;
	int temp;

	bufheight=80;
	Fill_back(70,150);//填充第一段背景
	for(i=0;i<file_cnt;i++)//文件列表显示
	{		
		if(ypos>(-26-26*i)&&ypos<(80-26*i))//在显示范围内才给予显示
		{
			temp=ypos+i*26+8;//temp=ypos+i*26+78-70; 78为第一个文件显示的位置
			Load_type(xpos+20,temp-4,Get_type(file_name+i*13,file_trib[i]));//显示文件类型图标
			Win_Fonts(xpos+50,temp,file_name+i*13,WHITE);//显示文件名			
		}					
	}
	Dip_guibuf(70,150);//显示第一段内容
	Fill_back(150,230);//填充第二段背景		   
	for(i=0;i<file_cnt;i++)//文件列表显示
	{		
		if(ypos>(54-26*i)&&ypos<(160-26*i))//在显示范围内才给予显示
		{
			temp=ypos+i*26-72;//temp=ypos+i*26+78-150;
			Load_type(xpos+20,temp-4,Get_type(file_name+i*13,file_trib[i]));//显示文件类型图标
			Win_Fonts(xpos+50,temp,file_name+i*13,WHITE);//显示文件名			
		}					
	}
	Dip_guibuf(150,230);//显示第二段内容
	Fill_back(230,310);//填充第三段背景		   
	for(i=0;i<file_cnt;i++)//文件列表显示
	{		
		if(ypos>(134-26*i)&&ypos<(240-26*i))//在显示范围内才给予显示
		{
			temp=ypos+i*26-152;//temp=ypos+i*26+78-230;
			Load_type(xpos+20,temp-4,Get_type(file_name+i*13,file_trib[i]));//显示文件类型图标
			Win_Fonts(xpos+50,temp,file_name+i*13,WHITE);//显示文件名			
		}					
	}
	Dip_guibuf(230,310);//显示第三段内容
} 
//列表定位
int Stick_Flist(int ypos)
{
	int temp,offset;

	temp=ypos/26;
	offset=ypos%26;
	if(offset<-13)//往上定位
	{
		temp=(temp-1)*26;
		while(ypos>temp)
		{			
			if(ypos>temp+1)ypos-=2;
			else ypos--;
			File_list(0,ypos);
		}
	}
	else//往下定位
	{
		temp=temp*26;
		while(ypos<temp)
		{
			if(ypos<temp-1)ypos+=2;
			else ypos++;
			File_list(0,ypos);
		}		
	}
	return ypos;
}
//显示当前路径
void Show_path(u8* path)
{
	u8 i;

	LCD_Charmd(0,WHITE);
	for(i=45;i<70;i++)Draw_BGDline(i,1);
	Show_Fonts(23,48,path,WHITE);
}
//移动文件
void Move_file(void)
{
	u8 which;

	OX=240;//赋成不可能的值
	OY=320;
	which=0;
	FLASH_SPI_Init();
	bufwidth=135;
	bufheight=24;
	memset(gui_buf,0,7200);
	Load_type(0,0,Get_type(file_name+file_curr*13,file_trib[file_curr]));//显示文件类型图标
	Win_Fonts(30,4,file_name+file_curr*13,WHITE);//显示文件名	
	oldbuf=gui_buf+3600;
	newbuf=gui_buf+7200;		
	while(PEN_int==RESET)
	{		
		ADS_SPI_Init();
		Read_X(&PENX);
		Read_Y(&PENY);
		NX=PENX-13;
		NY=PENY-13;
		if(NX<0)NX=0;
		if(NY<0)NY=0;
		if(NX>215)NX=215;
		if(NY>295)NY=295;
		Fill_buff();//在绘制图标之前保存该区域
		Draw_menbuf(NX,NY,gui_buf,0);
		Draw_buff();//填回除当前图标区域以外的背景
		FLASH_SPI_Init();
		//if(NX>60&&NX<120&&NY>73&&NY<133)Draw_icon(90,103,18,BLACK);
		//else Draw_icon(90,103,19,BLACK);		
		OX=NX;
		OY=NY;
 		if(which)
		{
			oldbuf=gui_buf+3600;
			newbuf=gui_buf+7200;
			which=0;			
		}
		else 
		{
			oldbuf=gui_buf+7200;
			newbuf=gui_buf+3600;//切换缓冲区
			which=1;			
		}
	}
//	if(NX>60&&NX<120&&NY>73&&NY<133)
//	{
//
//	}
//	else
//	{	
		NX=240;//赋成不可能的值
		NY=320;
		Draw_buff();
//	}
	bufwidth=240;
	ADS_SPI_Init();
}
//选中文件或文件夹
void Select_file(void)
{
	while(PEN_int==RESET)
	{
		Read_Y(&PENY);
		Read_X(&PENX);
		if(PENY<70)PENY=70;
		if(PENY>305)PENY=305;
		Draw_Bigpiont(PENX,PENY,RED);
	}
}
















//结束文件管理器
void Finish_fmanager(void)
{

}








