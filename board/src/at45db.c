#include "at45db.h"
#include "spi.h"

u8 AT45_buffer[792];

void SPI_Flash_Init(void)
{
	SPI1_Init();
	FLASH_SPI_Init();
	CSPin_init();
}

void FlashReadID(u8 *Data)
{
	u8 i;
	AT_CS_LOW();	
  	SPI1_RWByte(0x9F);
  	for(i=0; i<4;i++)
  	{
  		Data[i]=SPI1_RWByte(Dummy);
  	}
  	AT_CS_HIGH();	
}

void FlashPageEarse(u16 page)
{	
	FlashWaitBusy();	
	AT_CS_LOW();
	SPI1_RWByte(PAGE_ERASE);
	SPI1_RWByte((u8)(page>>6));
	SPI1_RWByte((u8)(page<<2));
	SPI1_RWByte(Dummy);
	AT_CS_HIGH();	
}

void FlashPageRead(u16 page,u8 *Data)
{
	u16 i;	
	FlashWaitBusy();
	AT_CS_LOW();
	SPI1_RWByte(PAGE_READ);
	SPI1_RWByte((u8)(page>>6));
  	SPI1_RWByte((u8)(page<<2));
  	SPI1_RWByte(0x00);//3个字节
  	SPI1_RWByte(0x00);
  	SPI1_RWByte(0x00);
  	SPI1_RWByte(0x00);
  	SPI1_RWByte(0x00);
  	for(i=0;i<528;i++)
	{
		Data[i]=SPI1_RWByte(Dummy);
	}
	AT_CS_HIGH();	
}

void FlashPageWrite(u16 page,u8 *Data)		//写一整页，页范围0-4095
{
	u16 i;
	FlashWaitBusy();
	AT_CS_LOW();
	SPI1_RWByte(BUFFER_2_WRITE);
	SPI1_RWByte(0x00);
	SPI1_RWByte(0x00);
	SPI1_RWByte(0x00);
	for(i=0;i<528;i++)
	{
		SPI1_RWByte(Data[i]);
	}
	AT_CS_HIGH();		
	if ( page < 4096)
	{
		AT_CS_LOW();
		SPI1_RWByte(B2MM_PAGEERASE);
		SPI1_RWByte((u8)(page>>6));
		SPI1_RWByte((u8)(page<<2));
		SPI1_RWByte(0x00);
		AT_CS_HIGH();
		FlashWaitBusy();
	}	
}

void FlashWaitBusy(void)
{
	u8 state_reg=0x00;
	AT_CS_LOW();	
	SPI1_RWByte(FLASH_STATUS);	 
	while((state_reg&0x80)==0)state_reg=SPI1_RWByte(Dummy);
	AT_CS_HIGH();
}

void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)//写入flash
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/528;//页地址
	secoff=WriteAddr%528;//页内的偏移
	secremain=528-secoff;//页剩余空间大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于528个字节
	while(1) 
	{	
		FlashPageRead(secpos,&AT45_buffer[0]);//读出整页内容
		FlashPageEarse(secpos);//擦除该页
		for(i=0;i<secremain;i++)	   //复制
		{
			AT45_buffer[i+secoff]=pBuffer[i];	  
		}
		FlashPageWrite(secpos,&AT45_buffer[0]);//写入整个扇区  
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 
		   	pBuffer+=secremain;  //指针偏移  
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>528)secremain=528;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	}	 	 
}

void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)//读取flash	  
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=ReadAddr/528;//页地址
	secoff=ReadAddr%528;//页内的偏移
	secremain=528-secoff;//页剩余空间大小
	if(NumByteToRead<=secremain)secremain=NumByteToRead;
	while(1)
	{
		FlashPageRead(secpos,&AT45_buffer[0]);
		for(i=0;i<secremain;i++)
		{
			pBuffer[i]=AT45_buffer[i+secoff];
		}
		if(NumByteToRead==secremain)break;//读结束了
		else//读未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 
		   	pBuffer+=secremain;  //指针偏移  
		   	NumByteToRead-=secremain;				//字节数递减
			if(NumByteToRead>528)secremain=528;	//下一个扇区还是读不完
			else secremain=NumByteToRead;		//下一个扇区可以读完了
		}
	}
}

