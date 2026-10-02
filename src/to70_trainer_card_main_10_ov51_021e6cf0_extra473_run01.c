#pragma force_active on
#include "global.h"
void ov51_021E6CF0(u8 *,u8 *);
void ov51_021E6CF0(u8 *source,u8 *dest){int count=0x3000;int i=0;do{u8 col=(i/8)%8;u8 bit=i%8;dest[i]=(source[(i/64)*8+col]>>bit)&1;i++;}while(i<count);}
extern void BG_LoadCharTilesData(void *,u8,int,int,int);extern u16 *GetBgTilemapBuffer(void *,u8);extern void ScheduleBgTilemapBufferTransfer(void *,u8);
void ov51_021E6D44(void *,int,int);
