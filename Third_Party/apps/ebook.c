#include <string.h>
#include <stdlib.h>
//#include "arm_math.h"
//#include "MemPool.h"
#include "at45db.h"
#include "fsearch.h"
#include "ebook.h"  
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "spi.h"


ebkdef* ebk;//电子书结构体
u8*  	ebk_buf1;
u8*  	ebk_buf2;
u8*  	ebk_buf3;//三缓冲,用以存放当前页、前一页、下一页的显示内容
u32*	ebk_ptr;//用来存放每一页开头在文件中的位置
void App_ebook(void)
{
	u8  i;
	u16 times;
	int xpos,ypos;		
	//应用程序里面用到的内存空间均采用动态分配方式
	ebk=malloc(sizeof(ebkdef));
	if(ebk==NULL){Finish_ebook();return;}
	ebk_buf1=malloc(EBKPGSize*sizeof(u8));
	if(ebk_buf1==NULL){Finish_ebook();return;}
	ebk_buf2=malloc(EBKPGSize*sizeof(u8));
	if(ebk_buf2==NULL){Finish_ebook();return;}
	ebk_buf3=malloc(EBKPGSize*sizeof(u8));
	if(ebk_buf3==NULL){Finish_ebook();return;}	


	ebk->basex=0;
	ebk->basey=0;
	ebk->filecur=0;
	ebk->pagecur=0;		   
	LCD_Fill(0,40,240,320,GRAY[29]);
	LCD_Charmd(1,GRAY[29]);
	My_chdir("0:/EBOOK");//切换到电子书文件夹
	if(f_opendir(&ebk->dir,"0:/EBOOK"))
	{
		Dialog_box(40,180,"请插入有效的SD卡，并复位系统.");
		delay_ms(1200);
		Del_Dialog();
		Finish_ebook();
		Home_page();
	}
	ebk->filecnt=File_search("0:/EBOOK",T_BOOK);//受缓冲区大小的限制,最多支持50个文件	
	Add_path(path_curr,namebuf+ebk->filecur*13);
	f_open(&ebk->file,(TCHAR*)path_curr,FA_READ);
	ebk_ptr=malloc(sizebuf[ebk->filecur]/EBKRDSize*sizeof(u32));
	if(ebk_ptr==NULL){Finish_ebook();return;}	
	ebk_ptr[0]=0;
	Fill_paper(ebk_buf2);
	ebk_ptr[1]=f_tell(&ebk->file);
	Fill_paper(ebk_buf3);
	ebk_ptr[2]=f_tell(&ebk->file);
	for(i=0;i<11;i++)LCD_HLine(8,69+i*24,232,GRAY[20]);
	Slip_paper(0);//显示第一页	
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
				LCD_Charmd(1,WHITE);
				Scroll_ctrl(Finish_ebook);
				ADS_SPI_Init();
				LCD_Charmd(1,GRAY[29]);
			}
			for(times=0;times<600;times++)//触屏操作识别
			{
				Read_X(&PENX);
				Read_Y(&PENY);	
				if(abs(xpos-PENX)>8)break;
				if(abs(ypos-PENY)>8)break;
				if(PEN_int!=RESET)
				{
					delay_ms(50);
					if(PEN_int!=RESET)break;
				}					
			}
			if(PEN_int!=RESET)//点击操作
			{				
											
			}
			else//滑屏操作
			{
				if(times==600){}//长按操作
				else//滑动操作
				{
				 	if(ypos>40)
					{
						ebk->basex-=xpos;
						while(PEN_int==RESET)
						{						
							Read_X(&PENX);
							xpos=ebk->basex+PENX;
							if(ebk->pagecur==0&&xpos>0)xpos=0;
							Slip_paper(xpos);
						}
						Stick_paper(xpos);
						ebk->basex=0;						
					}
				}
			}
		}	
	}	
}
//交换缓冲,direction:左移或右移
void Change_buf(u8 direction)
{
	u8* tempbuf;

	if(direction)//右移,即下一页
	{		
		ebk->pagecur++;
		tempbuf=ebk_buf1;
		ebk_buf1=ebk_buf2;
		ebk_buf2=ebk_buf3;
		ebk_buf3=tempbuf;
		Fill_paper(ebk_buf3);
		ebk_ptr[ebk->pagecur+2]=f_tell(&ebk->file);//保存当前文件读数据指针			
	}
	else//左移,即上一页
	{
		ebk->pagecur--;
		if(ebk->pagecur>0)
		{
			f_lseek(&ebk->file,ebk_ptr[ebk->pagecur-1]);
			tempbuf=ebk_buf3;
			ebk_buf3=ebk_buf2;
			ebk_buf2=ebk_buf1;		
			ebk_buf1=tempbuf;
			Fill_paper(ebk_buf1);
			f_lseek(&ebk->file,ebk_ptr[ebk->pagecur+2]);
		}
		else//第一页
		{
			tempbuf=ebk_buf3;
			ebk_buf3=ebk_buf2;
			ebk_buf2=ebk_buf1;		
			ebk_buf1=tempbuf;
			f_lseek(&ebk->file,ebk_ptr[ebk->pagecur+2]);		
		}					
	}
}
//向缓冲填充一页纸的字模,一页共154个汉字(两个字符当作一个汉字,奇数个字符则填充空隔)
void Fill_paper(u8* buf)
{
 	u16 i,k,count;
	u8* Fonts;
	u32 offset;

	count=0;
	Fonts=ebk->page_buf;	
 	f_read(&ebk->file,ebk->page_buf,EBKRDSize,&ebk->bytes);//读取一页纸的内容	
	if(ebk->bytes<EBKRDSize)
	{
		if(!ebk->bytes)
		{
			Dialog_box(40,180,"已是最后一页!");
			delay_ms(1200);
			Del_Dialog();
			return;			
		}
		for(i=ebk->bytes;i<EBKRDSize;i++)ebk->page_buf[i]=' ';//将剩下位置填成空格	
	}
	for(i=0;i<154;i++)//循环一次消耗两个字节
	{			
		if(*Fonts<0x81)//是ASCII字符,把两个字符按汉字取模的方式复制到缓冲区
		{
			if(*Fonts<0x20)*Fonts=0x20;//控制字符当作空格显示
			for(k=0;k<8;k++)buf[k]=asc2_1608[*Fonts-' '][k];
			for(k=16;k<24;k++)buf[k]=asc2_1608[*Fonts-' '][k-8];
			*Fonts++;			
			if(*Fonts<0x81)//连续两个字符
			{
				if(*Fonts<0x20)*Fonts=0x20;//控制字符当作空格显示
				for(k=8;k<16;k++)buf[k]=asc2_1608[*Fonts-' '][k-8];
				for(k=24;k<32;k++)buf[k]=asc2_1608[*Fonts-' '][k-16];
				*Fonts++;//偏移地址加1									
			}
			else//奇个字符,填充空隔 
			{
				for(k=8;k<16;k++)buf[k]=asc2_1608[0][k-8];
				for(k=24;k<32;k++)buf[k]=asc2_1608[0][k-16];
				count++;//对填充的空格计数,以进行读指针的偏移				
			}
			buf+=32;
		}			
		else//是汉字 
		{	
			offset=Get_FontAdr(Fonts);
			SPI_Flash_Read(buf,FON_addr*528+offset,32);	
			buf+=32;					
			*Fonts++;//偏移地址加2
			*Fonts++;
		}
	}	  	
	f_lseek(&ebk->file,f_tell(&ebk->file)-count);//指针前移,避免因填充空格造成数据缺失
}
//显示第num列的文字
void Show_Vfonts(int xpos,u8 num,u8* Fonts)
{
	u8  i;

	for(i=0;i<11;i++)
	{
		Show_Hanzi(xpos,50+i*24,Fonts+i*448+num*32,GRAY[3]);
	}
}

//翻页
void Slip_paper(int xpos)
{
	u8  i,j;
	int temp;

	for(i=0;i<3;i++)
	{
		temp=xpos+i*240-240;
		if(temp>-8&&temp<240)for(j=0;j<11;j++)LCD_Char(temp,50+j*24,' ',GRAY[3]);
	}
	for(i=0;i<14;i++)
	{					  
		temp=i*16-232+xpos;
		if(temp>-16&&temp<240)Show_Vfonts(temp,i,ebk_buf1);
		temp=i*16+8+xpos;
		if(temp>-16&&temp<240)Show_Vfonts(temp,i,ebk_buf2);
		temp=i*16+248+xpos;
		if(temp>-16&&temp<240)Show_Vfonts(temp,i,ebk_buf3);
	}
	for(i=0;i<3;i++)
	{
		temp=xpos+i*240-8;
		if(temp>-8&&temp<240)
		for(j=0;j<11;j++)LCD_Char(temp,50+j*24,' ',GRAY[6]);	
	}
}
//纸张定位函数
int Stick_paper(int xpos)
{
	u16 temp;

	if(xpos>=120&&xpos<=240)
	{
		temp=240-xpos;
		while(xpos<240)
		{			
			temp/=2;
			xpos+=temp+1;
			Slip_paper(xpos);				
		}
		Change_buf(0);//缓冲左移
	}
	else if(xpos<120&&xpos>0)
	{	
		while(xpos>0)
		{				
			xpos/=2;
			Slip_paper(xpos);				
		}
	}
	else if(xpos<0&&xpos>-120)
	{
		while(xpos<0)
		{			
			xpos/=2;
			Slip_paper(xpos);				
		}
	}
	else if(xpos<=-120&&xpos>=-240)
	{
		temp=240+xpos;
		while(xpos>-240)
		{	
			temp/=2;	
			xpos-=temp+1;
			Slip_paper(xpos);				
		}
		Change_buf(1);//缓冲右移
	}
	return xpos;
}
//结束播放
void Finish_ebook(void)
{
	f_close(&ebk->file);
	//释放内存
	free(ebk);				   
	free(ebk_buf1);
	free(ebk_buf2);
	free(ebk_buf3);
	free(ebk_ptr);		
}




