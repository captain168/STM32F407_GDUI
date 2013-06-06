#include "lcd.h"
#include "delay.h"
#include "math.h"


u8 	Fsize;
u8 	Charmd;  
u16 Bcolor; 

void LCD_LineConf(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                   GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                 GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//(RS)
  	GPIO_Init(GPIOD, &GPIO_InitStructure);  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);	   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//(CS) 
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
}

void LCD_FSMCConf(void)
{
  	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  p1,p2;

  	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
  	p1.FSMC_AddressSetupTime = 5;
  	p1.FSMC_AddressHoldTime = 0;
  	p1.FSMC_DataSetupTime = 9;
  	p1.FSMC_BusTurnAroundDuration = 0;
  	p1.FSMC_CLKDivision = 0;
  	p1.FSMC_DataLatency = 0;
  	p1.FSMC_AccessMode = FSMC_AccessMode_A;

  	p2.FSMC_AddressSetupTime = 5;
  	p2.FSMC_AddressHoldTime = 0;
  	p2.FSMC_DataSetupTime = 12;
  	p2.FSMC_BusTurnAroundDuration = 2;
  	p2.FSMC_CLKDivision = 0;
  	p2.FSMC_DataLatency = 0;
	p2.FSMC_AccessMode = FSMC_AccessMode_A;

  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p2;
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p1;
  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);   
  	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void LCD_WriteReg(u8 Reg, u16 Value)
{
  	LCD_REG=Reg;
  	LCD_RAM=Value;
}

u16 LCD_ReadReg(u8 Reg)
{
  	LCD_REG=Reg;
  	return (LCD_RAM);
}

void LCD_WriteRAM(u16 RGB_Code)
{
  	LCD_RAM=RGB_Code;
}

u16 LCD_ReadRAM(void)
{
  	u16 dummy;
  	LCD_REG=0x22; 
	dummy=LCD_RAM;
  	return LCD_RAM;
}

u32 lcdid=0;
void LCD_Init(void)
{  	
   	LCD_LineConf();
  	LCD_FSMCConf();
  	delay_ms(5); 
  	lcdid=LCD_ReadReg(0x00);  
  	if(lcdid == 0x9320)
  	{
		LCD_WriteReg(0xE5,0x8000);
		LCD_WriteReg(0x00,0x0001);
		LCD_WriteReg(0x01,0x0100);
		LCD_WriteReg(0x02,0x0700);
		LCD_WriteReg(0x03,0x1030);
		LCD_WriteReg(0x04,0x0000);
		LCD_WriteReg(0x08,0x0202);
		LCD_WriteReg(0x09,0x0000);
		LCD_WriteReg(0x0A,0x0000);
		LCD_WriteReg(0x0B,0x0000);
		LCD_WriteReg(0x0C,0x0000);
		LCD_WriteReg(0x0F,0x0000);
		LCD_WriteReg(0x10,0x0000);
		LCD_WriteReg(0x11,0x0000);
		LCD_WriteReg(0x12,0x0000);
		LCD_WriteReg(0x13,0x0000);
		delay_ms(20);
		LCD_WriteReg(0x10,0x17B0);
		LCD_WriteReg(0x11,0x0137);
		delay_ms(5);
		LCD_WriteReg(0x12,0x0139);
		delay_ms(5);
		LCD_WriteReg(0x13,0x1d00);
		LCD_WriteReg(0x29,0x0013);
		delay_ms(5);
		LCD_WriteReg(0x20,0x0000);
		LCD_WriteReg(0x21,0x0000);
		LCD_WriteReg(0x30,0x0006);
		LCD_WriteReg(0x31,0x0101);
		LCD_WriteReg(0x32,0x0003);
		LCD_WriteReg(0x35,0x0106);
		LCD_WriteReg(0x36,0x0b02);
		LCD_WriteReg(0x37,0x0302);
		LCD_WriteReg(0x38,0x0707);
		LCD_WriteReg(0x39,0x0007);
		LCD_WriteReg(0x3C,0x0600);
		LCD_WriteReg(0x3D,0x020b);
		LCD_WriteReg(0x50,0x0000);
		LCD_WriteReg(0x51,0x00EF);
		LCD_WriteReg(0x52,0x0000);
		LCD_WriteReg(0x53,0x013F);
		LCD_WriteReg(0x60,0x2700);
		LCD_WriteReg(0x61,0x0001);
		LCD_WriteReg(0x6A,0x0000);
		LCD_WriteReg(0x80,0x0000);
		LCD_WriteReg(0x81,0x0000);
		LCD_WriteReg(0x82,0x0000);
		LCD_WriteReg(0x83,0x0000);
		LCD_WriteReg(0x84,0x0000);
		LCD_WriteReg(0x85,0x0000);
		LCD_WriteReg(0x90,0x0010);
		LCD_WriteReg(0x92,0x0000);
		LCD_WriteReg(0x93,0x0003);
		LCD_WriteReg(0x95,0x0110);
		LCD_WriteReg(0x97,0x0000);
		LCD_WriteReg(0x98,0x0000);
		LCD_WriteReg(0x03,0x1018);
		LCD_WriteReg(0x07,0x0173); 
  	}
  	else if(lcdid == 0x9325)
  	{
		LCD_WriteReg(0x00,0x0001);
		LCD_WriteReg(0x01,0x0100);
		LCD_WriteReg(0x02,0x0700);
		LCD_WriteReg(0x03,0x1018);
		LCD_WriteReg(0x04,0x0000);
		LCD_WriteReg(0x08,0x0202);
		LCD_WriteReg(0x09,0x0000);
		LCD_WriteReg(0x0A,0x0000);
		LCD_WriteReg(0x0C,0x0000);
		LCD_WriteReg(0x0D,0x0000);
		LCD_WriteReg(0x0F,0x0000);
		LCD_WriteReg(0x10,0x0000);
		LCD_WriteReg(0x11,0x0000);
		LCD_WriteReg(0x12,0x0000);
		LCD_WriteReg(0x13,0x0000);
		delay_ms(20);
		LCD_WriteReg(0x10,0x17B0);
		LCD_WriteReg(0x11,0x0137);
		delay_ms(5);
		LCD_WriteReg(0x12,0x0139);
		delay_ms(5);
		LCD_WriteReg(0x13,0x1d00);
		LCD_WriteReg(0x29,0x0013);
		delay_ms(5);
		LCD_WriteReg(0x20,0x0000);
		LCD_WriteReg(0x21,0x0000);
		LCD_WriteReg(0x30,0x0007);
		LCD_WriteReg(0x31,0x0302);
		LCD_WriteReg(0x32,0x0105);
		LCD_WriteReg(0x35,0x0206);
		LCD_WriteReg(0x36,0x0808);
		LCD_WriteReg(0x37,0x0206);
		LCD_WriteReg(0x38,0x0504);
		LCD_WriteReg(0x39,0x0007);
		LCD_WriteReg(0x3C,0x0105);
		LCD_WriteReg(0x3D,0x0808);
		LCD_WriteReg(0x50,0x0000);
		LCD_WriteReg(0x51,0x00EF);
		LCD_WriteReg(0x52,0x0000);
		LCD_WriteReg(0x53,0x013F);
		LCD_WriteReg(0x60,0xA700);
		LCD_WriteReg(0x61,0x0001);
		LCD_WriteReg(0x6A,0x0000);
		LCD_WriteReg(0x80,0x0000);
		LCD_WriteReg(0x81,0x0000);
		LCD_WriteReg(0x82,0x0000);
		LCD_WriteReg(0x83,0x0000);
		LCD_WriteReg(0x84,0x0000);
		LCD_WriteReg(0x85,0x0000);
		LCD_WriteReg(0x90,0x0010);
		LCD_WriteReg(0x92,0x0000);
		LCD_WriteReg(0x93,0x0003);
		LCD_WriteReg(0x95,0x0110);
		LCD_WriteReg(0x97,0x0000);
		LCD_WriteReg(0x98,0x0000);
		LCD_WriteReg(0x03,0x1018);
		LCD_WriteReg(0x07,0x0133);
  	} 
	else if(lcdid==0x5408)
	{
	 	LCD_WriteReg(0x0000,0x0000);
	    LCD_WriteReg(0x0001,0x0100);//0000	     
	    LCD_WriteReg(0x0002,0x0700);
		LCD_WriteReg(0x0003,0x1030);//d0a0 
		LCD_WriteReg(0x0004,0x0000);
		LCD_WriteReg(0x0008,0x0207);		
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000a,0x0000);	
		LCD_WriteReg(0x000C,0x0000);
		LCD_WriteReg(0x000D,0x0000);
		LCD_WriteReg(0x000F,0x0000);		
		LCD_WriteReg(0x0007,0x0101);
	    LCD_WriteReg(0x0010,0x12b0);
		LCD_WriteReg(0x0011,0x0007);
		LCD_WriteReg(0x0017,0x0001);
		delay_ms(100);//200		
		LCD_WriteReg(0x0012,0x01bb);
		LCD_WriteReg(0x0013,0x1c00);
		delay_ms(25);		
		LCD_WriteReg(0x0029,0x0019);//0012
		delay_ms(25);		
		LCD_WriteReg(0x0030,0x0102);
		delay_ms(25);			
	    LCD_WriteReg(0x0031,0x0c21);//0c20
	    LCD_WriteReg(0x0032,0x0b22);//0b21	    
	    LCD_WriteReg(0x0033,0x2610);//250f	    	    
	    LCD_WriteReg(0x0034,0x1e0b);//1d0b
	   	LCD_WriteReg(0x0035,0x0a04);
	    LCD_WriteReg(0x0036,0x1701);
	    LCD_WriteReg(0x0037,0x0617);
	    LCD_WriteReg(0x0038,0x0305);
	    LCD_WriteReg(0x0039,0x0a05);	    
	    LCD_WriteReg(0x003a,0x0c04);//0f04
	    LCD_WriteReg(0x003b,0x0c00);//0f00
	    LCD_WriteReg(0x003c,0x000c); //000f
		LCD_WriteReg(0x003d,0x050c);//050f	
	    LCD_WriteReg(0x003e,0x0204); 
		LCD_WriteReg(0x003f,0x0404);
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00ef);
		LCD_WriteReg(0x0052,0x0000);	
		LCD_WriteReg(0x0053,0x013F);		
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0061,0x0001);	
		LCD_WriteReg(0x006A,0x0000);		
		LCD_WriteReg(0x0080,0x0000);
		LCD_WriteReg(0x0081,0x0000);
		LCD_WriteReg(0x0082,0x0000);
		LCD_WriteReg(0x0083,0x0000);
		LCD_WriteReg(0x0084,0x0000);
		LCD_WriteReg(0x0085,0x0000);	
		LCD_WriteReg(0x0090,0x0010);
		LCD_WriteReg(0x0092,0x0000);
		LCD_WriteReg(0x0093,0x0103);
		LCD_WriteReg(0x0095,0x0210);
		LCD_WriteReg(0x0097,0x0000);
		LCD_WriteReg(0x0098,0x0000);
		LCD_WriteReg(0x00f0,0x5408);
		LCD_WriteReg(0x00f3,0x0005);
	   	LCD_WriteReg(0x00f4,0x001f);
	   	LCD_WriteReg(0x00f0,0x0000);
	   	LCD_WriteReg(0x0007,0x0173);    
	}
	else
	{
	   LCD_WriteReg(0x01,0x0006);           //IDMON=0, INVON=1, NORON=1, PTLON=0
	   LCD_WriteReg(0x16,0x00c8);           //scan 00c8
	   LCD_WriteReg(0x18,0x0000);           //vertical scrolling function enable
	   LCD_WriteReg(0x23,0x0095);           //N_DC=1001 0101
	   LCD_WriteReg(0x24,0x0095);           //PI_DC=1001 0101
	   LCD_WriteReg(0x25,0x00FF);           //I_DC=1111 1111
	   LCD_WriteReg(0x27,0x0002);           //N_BP=0000 0010
	   LCD_WriteReg(0x28,0x0002);           //N_FP=0000 0010
	   LCD_WriteReg(0x29,0x0002);           //PI_BP=0000 0010
	   LCD_WriteReg(0x2a,0x0002);           //PI_FP=0000 0010
	   LCD_WriteReg(0x2c,0x0002);           //I_BP=0000 0010
	   LCD_WriteReg(0x2d,0x0002);           //I_FP=0000 0010
	   LCD_WriteReg(0x3a,0x0000);           //N_RTN=0000, N_NW=001
	   LCD_WriteReg(0x3b,0x0001);           //PI_RTN=0000, PI_NW=001
	   LCD_WriteReg(0x3c,0x00F0);           //I_RTN=1111, I_NW=000
	   LCD_WriteReg(0x3d,0x0000);           //DIV=00
	   delay_ms(20);
	   LCD_WriteReg(0x35,0x0008);           //EQS=38hSet_LCD_8B_REG(0x0036,0x0078); // EQP=78h
	   LCD_WriteReg(0x3E,0x0038);           //SON=38h
	   LCD_WriteReg(0x40,0x000F);           //GDON=0Fh
	   LCD_WriteReg(0x41,0x00F0);           //GDOFF   
	   //240X320 windown setting
	   LCD_WriteReg(0x02,0x0000);           //Column address start2
	   LCD_WriteReg(0x03,0x0000);           //Column address start1
	   LCD_WriteReg(0x04,0x0000);           //Column address end2
	   LCD_WriteReg(0x05,0x00ef);           //Column address end1
	   LCD_WriteReg(0x06,0x0000);           //Row address start2
	   LCD_WriteReg(0x07,0x0000);           //Row address start1
	   LCD_WriteReg(0x08,0x0001);           //Row address end2
	   LCD_WriteReg(0x09,0x003f);           //Row address end1
	   LCD_WriteReg(0x0a,0x0000);           //partical area start row register 2
	   LCD_WriteReg(0x0b,0x0000);           //partical area start row register 1
	   LCD_WriteReg(0x0c,0x0001);           //partical area end row register 2
	   LCD_WriteReg(0x0d,0x003f);           //partical area end row register 1
	   //Power Supply Setting
	   LCD_WriteReg(0x19,0x0049);           //internal oscillator frequency adjust,60Hz
	   LCD_WriteReg(0x93,0x0003);           //internal oscillator frequency;100%
	   LCD_WriteReg(0x1C,0x0004);           //AP=100
	   delay_ms(2);
	   LCD_WriteReg(0x42,0x0018);           //set VBGP output=n*VBGP
	   LCD_WriteReg(0x1D,0x0001);           //set DDVDH=5.47*VBGP
	   LCD_WriteReg(0x1E,0x0000);           //set VREG3=DDVDH
	   LCD_WriteReg(0x20,0x0020);           //set VGH=3*VREG3,VGL=-(VREG3*2)+VCI
	
	   LCD_WriteReg(0x1F,0x0001);           //VREG1=VBGP*3.3
	   LCD_WriteReg(0x45,0x0014);           //VCOM=1.02*VREG1
	   LCD_WriteReg(0x44,0x0050);           //VCMH=
	   delay_ms(2);
	   LCD_WriteReg(0x43,0x0080);           //set VCOMG=1
	   delay_ms(2);
	   LCD_WriteReg(0x1B,0x0008);           //GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=0, STB=0
	   delay_ms(2);
	   LCD_WriteReg(0x1B,0x0010);           //GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=0, STB=0
	   delay_ms(2);   
	   // gamma set
	   LCD_WriteReg(0x46,0x00a4);//0x0091
	   LCD_WriteReg(0x47,0x0053);//0x0011
	   LCD_WriteReg(0x48,0x0000);
	   LCD_WriteReg(0x49,0x0044);//0x0066
	   LCD_WriteReg(0x4a,0x0004);//0x0037
	   LCD_WriteReg(0x4b,0x0067);//0x0004
	   LCD_WriteReg(0x4c,0x0033);//0x0011
	   LCD_WriteReg(0x4d,0x0077);
	   LCD_WriteReg(0x4e,0x0012);//0x0000
	   LCD_WriteReg(0x4f,0x004c);//0x001f
	   LCD_WriteReg(0x50,0x0046);//0x000f
	   LCD_WriteReg(0x51,0x0044);//0x0000
	   //Display ON Setting
	   LCD_WriteReg(0x90,0x007F);           //SAP=0111 1111
	   LCD_WriteReg(0x26,0x0004);           //GON=0, DTE=0, D=01
	   delay_ms(4);
	   LCD_WriteReg(0x26,0x0024);           //GON=1, DTE=0, D=01
	   LCD_WriteReg(0x26,0x002C);           //GON=1, DTE=0, D=11
	   delay_ms(4);
	   LCD_WriteReg(0x26,0x003C);           //GON=1, DTE=1, D=11
	   
	   LCD_WriteReg(0x57,0x0002);           //test Mode enable
	   LCD_WriteReg(0x55,0x0000);           //VDC_SL=000,VDDD=1.95V
	   LCD_WriteReg(0x57,0x0000);	
	}
	LCD_Window(0,0,239,319);
	LCD_Clear(WHITE);
	LCD_Font(16);
	LCD_Dispmd(7);
	LCD_Charmd(1,WHITE);							 
}

void LCD_Font(u8 font)
{
  	Fsize=font;
}

void LCD_Charmd(u8 mode,u16 color)//0为叠加方式
{
  	Charmd=mode;
	Bcolor=color;
}

void LCD_Cursor(u16 Xpos, u16 Ypos)
{
  	LCD_WriteReg(0x20, Xpos);
  	LCD_WriteReg(0x21, Ypos);
}

//LCD开启显示
void LCD_DisplayOn(void)
{					   
	LCD_WriteReg(0x07,0x0133); //26万色显示开启
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(0x07,0x0000);//关闭显示 
}  

//X,Y为窗口左下角座标,注意设置窗口后座标不要超出该范围
void LCD_Window(u16 x1, u16 y1, u16 x2, u16 y2)
{
  	LCD_WriteReg(0x50,x1);
  	LCD_WriteReg(0x51,x2);
  	LCD_WriteReg(0x52,y1); 
  	LCD_WriteReg(0x53,y2);
}

//设置刷屏模式
void LCD_Dispmd(u8 mode)
{
	switch(mode)//常用4(横屏)和7(竖屏)
	{
		case 1:LCD_WriteReg(0x03,0x1000);break;//向左向上:←↖
		case 2:LCD_WriteReg(0x03,0x1008);break;//向上向左:↑↖
		case 3:LCD_WriteReg(0x03,0x1010);break;//向右向上:→↗
		case 4:LCD_WriteReg(0x03,0x1018);break;//向上向右:↑↗
		case 5:LCD_WriteReg(0x03,0x1020);break;//向左向下:←↙
		case 6:LCD_WriteReg(0x03,0x1028);break;//向下向左:↓↙
		case 7:LCD_WriteReg(0x03,0x1030);break;//向右向下:→↘
		case 8:LCD_WriteReg(0x03,0x1038);break;//向下向右:↓↘
	}
}

void LCD_Clear(u16 Color)
{
  	u32 index=0;
 
  	LCD_Cursor(0,0); 
  	LCD_REG=0x22;//WriteRAM_Prepare 
  	for(index=0;index<76800;index++)LCD_RAM=Color;
}

void LCD_SetPoint(u16 xpos, u16 ypos,u16 color)
{
	LCD_Cursor(xpos,ypos);
	LCD_REG=0x22;//WriteRAM_Prepare 
	LCD_RAM=color;
}

void Draw_Bigpiont(u16 x,u16 y,u16 color)
{
	u8 i;

	LCD_Window(x,y,x+4,y+4);
	LCD_Cursor(x,y);
	LCD_REG=0x22;
	for(i=0;i<25;i++)LCD_RAM=RED;
	LCD_Window(0,0,239,319);
}

u16 LCD_GetPoint(u16 x,u16 y)
{
	LCD_Cursor(x,y);
	return BGR565toRGB565(LCD_ReadRAM());
}

void LCD_Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_SetPoint(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

void LCD_Rect(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	LCD_Line(x1,y1,x2,y1,color);
	LCD_Line(x1,y1,x1,y2,color);
	LCD_Line(x1,y2,x2,y2,color);
	LCD_Line(x2,y1,x2,y2,color);
}

void LCD_Circle(u16 Xpos, u16 Ypos, u16 Radius, u16 color)
{
  	s32  Dec;
  	u32  CurX;
  	u32  CurY;
	 
  	CurX=0;
  	CurY=Radius;   
  	Dec=3-(Radius<<1);
  	while(CurX<=CurY)
  	{
		LCD_SetPoint(Xpos+CurX,Ypos+CurY,color);
		LCD_SetPoint(Xpos+CurX,Ypos-CurY,color);
		LCD_SetPoint(Xpos-CurX,Ypos+CurY,color);
		LCD_SetPoint(Xpos-CurX,Ypos-CurY,color);
		LCD_SetPoint(Xpos+CurY,Ypos+CurX,color);
		LCD_SetPoint(Xpos+CurY,Ypos-CurX,color);
		LCD_SetPoint(Xpos-CurY,Ypos+CurX,color);
		LCD_SetPoint(Xpos-CurY,Ypos-CurX,color);
		if(Dec<0)Dec+=(CurX<<2)+6;
	    else
	    {
	      	Dec+=((CurX-CurY)<<2)+10;
	      	CurY--;
	    }
	    CurX++;
  	}
}

void LCD_HLine(u16 x1, u16 y, u16 x2 ,u16 color)
{
	u16 i;

	LCD_Cursor(x1,y);
	LCD_REG=0x22;
	for(i=x1;i<x2;i++)LCD_RAM=color;
}

void LCD_VLine(u16 y1, u16 x, u16 y2 ,u16 color)
{
	u16 i;

	for(i=y1;i<y2;i++)LCD_SetPoint(x,i,color);
}

void Fill_Circle(u16 x0, u16 y0, u16 r,u16 color)
{
  	u16 i,x=r;
  	u16 imax=(r*707)/1000+1;
  	u16 sqmax=r*r+r/2;

  	LCD_HLine(x0-r,y0,x0+r,color);
  	for(i=1;i<=imax;i++)
	{
    	if((i*i+x*x)>sqmax)
		{
      		if(x>imax) 
			{
        		LCD_HLine (x0-i+1,y0+x, x0+i-1,color);
        		LCD_HLine (x0-i+1,y0-x, x0+i-1,color);
      		}
      		x--;
    	}
    	LCD_HLine(x0-x,y0+i, x0+x,color);
    	LCD_HLine(x0-x,y0-i, x0+x,color);
  	}
}

void LCD_Fill(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{          	
	u16 i,j;
	u16 xlen=0;

	xlen=x2-x1;	   
	for(i=y1;i<y2;i++)
	{
	 	LCD_Cursor(x1,i);      	//设置光标位置 
		LCD_REG=0x22;      			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_RAM=color;//设置光标位置 	    
	}  						  	    
}  

u16 Llimit=0;
u16 Rlimit=240;
void LCD_Char(int x,u16 y,u8 num,u16 color)
{     
	u8  m,n;
	u16 temp;
	int refx;

	num=num-' ';//得到偏移后的值
	for(m=0;m<8;m++)
	{
		refx=m+x;
		if(refx>=Llimit&&refx<Rlimit)
		{
			temp=(asc2_1608[num][m+8]<<8)+asc2_1608[num][m];
			for(n=0;n<16;n++) 
			{				
				if(temp&0x0001)LCD_SetPoint(refx,y+n,color); 
				else if(Charmd)LCD_SetPoint(refx,y+n,Bcolor);
				temp>>=1;
			}
		}
	} 	   	 	  
} 

void LCD_String(u16 x,u16 y,const u8 *p,u16 color)
{         
    while(*p!='\0')
    {       
        LCD_Char(x,y,*p,color);
        x+=Fsize/2;
        p++;
    }  
}

//m^n函数
u32 mypow(u8 m,u8 n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
	 
void LCD_Num(u16 x,u16 y,u32 num,u8 len,u16 color)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_Char(x+(Fsize/2)*t,y,'0',color);
				continue;
			}else enshow=1; 		 	 
		}
	 	LCD_Char(x+(Fsize/2)*t,y,temp+'0',color); 
	}
} 
//绘制数组中的位图
void Draw_Bmp(u16 x,u16 y,u16 width,u16 height,u16* pic)
{
  	u16 i,bytes;

	bytes=width*height;
	LCD_Window(x,y,x+width-1,y+height-1);
	LCD_Cursor(x,y);
  	LCD_REG=0x22; 
	for(i=0;i<bytes;i++)LCD_RAM=pic[i];
}
//绘制一个透明度为Transparency（0~100）的点
void TransPoint(u16 x,u16 y,u16 Color,u8 Trans)
{
	u8  Trans_Temp = Trans;			
	u16 r1=0,g1=0,b1=0,r2=0,g2=0,b2=0;
	u16   RGB_BootomLayer[1] = {0};
	u16   *RGB_Buf = &RGB_BootomLayer[0];

	//对顶层覆盖色做不透明度系数为Trans_Temp的渲染
	r2=((Color>>11)&0x1F)*Trans_Temp/100;
	g2=((Color>>5)&0x3F)*Trans_Temp/100;
	b2=(Color&0x1F)*Trans_Temp/100;

	*RGB_Buf = LCD_GetPoint(x,y);//读点		
	r1 = ((*RGB_Buf>>11)&0x1F)*(100-Trans_Temp)/100;//根据透明度参数对该点进行渲染
	g1 = ((*RGB_Buf>>5)&0x3F)*(100-Trans_Temp)/100;
	b1 = (*RGB_Buf&0x1F)*(100-Trans_Temp)/100;
	*RGB_Buf = ((r1+r2)<<11)+((g1+g2)<<5)+(b1+b2);
	LCD_SetPoint(x,y,*RGB_Buf);//画点			
}


void TransWhite(u16 x,u16 y,u8 Trans)
{		
	u16 r1=0,g1=0,b1=0,r2=0,g2=0,b2=0;
	u16 color;

	r2=0x1F*Trans/100;
	g2=0x3F*Trans/100;
	b2=0x1F*Trans/100;

	color = LCD_GetPoint(x,y);//读点		
	r1 = ((color>>11)&0x1F)*(100-Trans)/100;//根据透明度参数对该点进行渲染
	g1 = ((color>>5)&0x3F)*(100-Trans)/100;
	b1 = (color&0x1F)*(100-Trans)/100;
	color = ((r1+r2)<<11)+((g1+g2)<<5)+(b1+b2);//白色透明
	LCD_SetPoint(x,y,color);//画点			
}

void TransBlack(u16 x,u16 y,u8 trans)
{		
	u16 r1,g1,b1;
	u16 color;

	color = LCD_GetPoint(x,y);//读点		
	r1 = ((color>>11)&0x1F)*trans/100;//根据透明度参数对该点进行渲染
	g1 = ((color>>5)&0x3F)*trans/100;
	b1 = (color&0x1F)*trans/100;
	color = (r1<<11)+(g1<<5)+b1;//黑色透明
	LCD_SetPoint(x,y,color);//画点			
}


/**************************************************************************
*                 3D显示主程序
***************************************************************************/
/*int sx,sy,ex,ey;
float rotx=0.00;                     // starting amount of x rotation
float roty=0.00;                     // starting amount of y rotation
float rotz=0.00;                     // starting amount of z rotation
unsigned char OFFSETX = 120;        
#define OFFSETY 200         
#define OFFSETZ 20
/////////////////////////////////立方体加文字/////////////////////////////////////////////////////////
const signed int aa[23]={8,-8,-8,8,8,-8,-8,8,8,8,8,8,8,0,4,-4,-8,-8,-8,-8,-8,-8,-8}; // x 
const signed int bb[23]={8,8,-8,-8,8,8,-8,-8,0,-4,4,-2,2,8,8,8,4,4,4,-4,-4,-4,0};   // y 
const signed int cc[23]={-8,-8,-8,-8,8,8,8,8,6,-6,-6,0,0,-6,6,6,-6,0,6,6,0,-6,0};   // z
const int ff[22]={1,2,3,4,5,6,7,8,1,2,3,4,9,9,12,14,14,17,19,20,21,22};         // start
const int gg[22]={2,3,4,1,6,7,8,5,5,6,7,8,10,11,13,15,16,19,20,21,18,23};       // end 
void Disply3D(float xpos,float ypos,float zpos,u16 color,u16 color_back,u8 width,u8 speed)   
{
    int newx[23];                     // translated screen x co-ordinates for vertex
    int newy[23];                     // translated screen y co-ordinates for vertex
    int i,loop;                         // temp variable for loops
    int vertex;
    float xt,yt,zt,x,y,z,sinax,cosax,sinay,cosay,sinaz,cosaz;   // lots of work variables
    for (loop=0;loop<1;loop++)
    {
        xpos=xpos+0.00;               // move the object
        ypos=ypos+0.00;               // it would wander off screen
        zpos=zpos+0.00;               // really quick, so leave it centered
		rotx=rotx+0.02;                 // rotate the cube on X axis
		roty=roty+0.1;                 // and on its y axis
		rotz=rotz+0.02;                 // dont bother with z or it gets confusing
		
		sinax=sin(rotx);               // precalculate the sin and cos values
		cosax=cos(rotx);             // for the rotation as this saves a 
		
		sinay=sin(roty);             // little time when running as we
		cosay=cos(roty);             // call sin and cos less often
		
		sinaz=sin(rotz);             // they are slow routines
		cosaz=cos(rotz);           // and we dont want slow!
		
		for (i=0; i<23; i++)         // translate 3d vertex position to 2d screen position
		{
			x=aa[i];                   // get x for vertex i
			y=bb[i];                   // get y for vertex i
			z=cc[i];                   // get z for vertex i
			
			yt = y * cosax - z * sinax;         // rotate around the x axis
			zt = y * sinax + z * cosax;       // using the Y and Z for the rotation
			y = yt;
			z = zt;
			
			yt = y * cosax - z * sinax;         // rotate around the x axis
			zt = y * sinax + z * cosax;       // using the Y and Z for the rotation
			y = yt;
			z = zt;
			
			xt = x * cosay - z * sinay;       // rotate around the Y axis
			zt = x * sinay + z * cosay;       // using X and Z
			x = xt;
			z = zt;
			
			xt = x * cosaz - y * sinaz;         // finaly rotate around the Z axis
			yt = x * sinaz + y * cosaz;       // using X and Y
			x = xt;
			y = yt;
			
			x=x+xpos;                         // add the object position offset
			y=y+ypos;                         // for both x and y
			z=z+OFFSETZ-zpos;           // as well as Z
			
			newx[i]=(x*64/z)+OFFSETX;       // translate 3d to 2d coordinates for screen
			newy[i]=(y*64/z)+OFFSETY;       // drawing so we can see the cube
		}
		
		for (i=0; i<22; i++)           // draw the lines that make up the object
		{
			vertex=ff[i]-1;               // temp = start vertex for this line
			sx=newx[vertex];           // set line start x to vertex[i] x position
			sy=newy[vertex];           // set line start y to vertex[i] y position
			vertex=gg[i]-1;             // temp = end vertex for this line
			ex=newx[vertex];           // set line end x to vertex[i+1] x position
			ey=newy[vertex];           // set line end y to vertex[i+1] y position
			LCD_Line(sx,sy,ex,ey,color);
		}
		delay_ms(speed);
		for (i=0; i<22; i++)           // draw the lines that make up the object
		{
			vertex=ff[i]-1;               // temp = start vertex for this line
			sx=newx[vertex];           // set line start x to vertex[i] x position
			sy=newy[vertex];           // set line start y to vertex[i] y position
			vertex=gg[i]-1;             // temp = end vertex for this line
			ex=newx[vertex];           // set line end x to vertex[i+1] x position
			ey=newy[vertex];           // set line end y to vertex[i+1] y position
			LCD_Line(sx,sy,ex,ey,color_back);
		}
	}
}*/





