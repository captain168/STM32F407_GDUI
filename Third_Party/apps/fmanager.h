#ifndef __FMANAGER_H__
#define __FMANAGER_H__

#include "stm32f4xx_conf.h"



void App_fmanager(void);
void File_list(int xpos,int ypos);
int  Stick_Flist(int ypos);
void Show_path(u8* path);
void Move_file(void);
void Select_file(void);






void Finish_fmanager(void);



#endif







