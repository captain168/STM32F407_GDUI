#ifndef __MUSIC_H
#define __MUSIC_H

#include <stm32f4xx.h>
#include "ff.h"


typedef enum
{
  LittleEndian,
  BigEndian
}Endianness;

typedef struct
{
  u32  RIFFchunksize;
  u16  FormatTag;
  u16  NumChannels;
  u32  SampleRate;
  u32  ByteRate;
  u16  BlockAlign;
  u16  BitsPerSample;
  u32  DataSize;
}wavdef;//wav头信息

typedef struct
{
	FIL  file;//音乐文件体
	DIR  dir;//音乐文件路径
	u32  hdlen;//头信息长度
	u32  PCMcnt;//WAV数据计数
	u32  pltime;//播放时长
	int  basex;//视图X轴基址
	int  basey;//视图Y轴基址
	UINT bytes;//读文件字节数	
	u16  filecnt;//音乐文件数
	u16  filecur;//当前播放的音乐文件
	u8   FillOK;//填充完成标志
	u8   aud_pl;//播放停止标志
	u8   plctrl;//播放控制,第一位播放、暂停；第二位上一曲；第三位下一曲
	u8   viewcur;//当前视图
	u8   buf_sw;//缓冲切换开关
}musdef;//音乐播放器用到的资源



#define  CHUNK_ID                            0x52494646//'RIFF'
#define  FILE_FORMAT                         0x57415645//'WAVE'
#define  FORMAT_ID                           0x666D7420//'fmt ' 
#define  DATA_ID                             0x64617461//'data' 
#define  FACT_ID                             0x66616374//'fact' 
#define  WAVE_FORMAT_PCM                     0x01
#define  FORMAT_CHNUK_SIZE                   0x10
#define  CHANNEL_MONO                        0x01
#define  CHANNEL_STEREO                      0x02
#define  SAMPLE_RATE_8000                    8000
#define  SAMPLE_RATE_11025                   11025
#define  SAMPLE_RATE_22050                   22050
#define  SAMPLE_RATE_44100                   44100
#define  BITS_PER_SAMPLE_8                   8
#define  BITS_PER_SAMPLE_16                  16


#define	FFTSize  128
#define	RDSize   1024

void App_music(void);
void Draw_prg(u16 x1,u16 y1,u16 x2,u16 y2,u8 prg);
void Draw_column(int xpos,u16 num,u16 extent,u8 which);
void Play_view(int xpos,int ypos);
int  Stick_view(int xpos,int ypos);
int  Stick_Mlist(int ypos);
void Show_time(u16 x,u16 y,u32 second);
void Info_board(void);
void Play_ctrl(u8 ctrl);

void Musicplay(u8* fname);
void Wavplay(u8* fname);
u8   WaveParsing(u8* buf);
void Play_next(void);
void Play_prev(void);
void Finish_music(void);
void AUDIO_TransferComplete(void);
void Mp3play(u8* fname);
void FFT_init(void);
void My_FFT(int* inbuf,int* outbuf);
u32  ReadUnit(u8 *buffer,u8 index,u8 Bytes,Endianness Format);





#endif

