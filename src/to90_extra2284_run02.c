#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x10];u16 hp,max;u8 pad2[0x50-0x14];} Mon;
typedef struct{u32 unk;Mon mons[6];u8 pad[0x2070-4-6*0x50];void*windows;} Work;
extern u8 CalculateHpBarPixelsLength(u16,u16,u8),CalculateHpBarColor(u16,u16,u8);extern void ScheduleWindowCopyToVram(void*),FillWindowPixelRect(void*,u8,u16,u16,u16,u16);
void ov08_0221E048(Work*,int,int,u16,u8);
void ov08_0221E048(Work*w,int window,int mon,u16 x,volatile u8 y){Mon *m=&w->mons[mon];u8 color=1;u8 width=CalculateHpBarPixelsLength(m->hp,m->max,48);int second;switch(CalculateHpBarColor(m->hp,m->max,48)){case 0:ScheduleWindowCopyToVram((u8*)w->windows+window*16);return;case 3:case 4:color=1;break;case 2:color=3;break;case 1:color=5;break;}{u8 line=y;second=color+1;FillWindowPixelRect((u8*)w->windows+window*16,(u8)second,x,(u16)(line+1),width,1);FillWindowPixelRect((u8*)w->windows+window*16,color,x,(u16)(line+2),width,2);FillWindowPixelRect((u8*)w->windows+window*16,(u8)second,x,(u16)(line+4),width,1);ScheduleWindowCopyToVram((u8*)w->windows+window*16);}}
