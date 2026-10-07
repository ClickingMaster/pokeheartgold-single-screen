#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

void FillBgTilemapRect(void*,u8,u16,u8,u8,u8,u8,u8),CopyToBgTilemapRect(void*,u8,u8,u8,u8,u8,const void*,u8,u8,u8,u8),ScheduleBgTilemapBufferTransfer(void*,int);void ov108_021E767C(void*,int);
void ov108_021E767C(void*s,int value){FillBgTilemapRect(P(s,0x340),0,0,0,19,24,5,17);void*data=P(s,0x4d8);CopyToBgTilemapRect(P(s,0x340),0,0,(u8)(21u-(u32)value*2),24,(u8)((u32)value*2+3),(u8*)data+12,0,(u8)((u32)value*3),(u8)(H(data,0)>>3),(u8)(H(data,2)>>3));ScheduleBgTilemapBufferTransfer(P(s,0x340),0);}
