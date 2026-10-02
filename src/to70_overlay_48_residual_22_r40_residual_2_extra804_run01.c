#pragma force_active on
#include "global.h"
typedef struct {u8 pad[12];void*task;}Work;
extern void ov48_0225A650(Work*,int,int),ov48_0225A668(Work*,int,int),ov48_0225A680(void*,void*);
extern void*SysTask_CreateOnVWaitQueue(void(*)(void*,void*),void*,int);
void ov48_0225A5C4(Work*);
void ov48_0225A5C4(Work*w){*(volatile u32*)0x4001000=(*(volatile u32*)0x4001000&0xffff1fff)|0x6000;*(volatile u16*)0x400104a=(*(volatile u16*)0x400104a&~63)|31;*(volatile u16*)0x4001048=(*(volatile u16*)0x4001048&~63)|15;*(volatile u16*)0x4001048=(*(volatile u16*)0x4001048&0xffffc0ff)|0xf00;ov48_0225A650(w,0,0);ov48_0225A668(w,0,0);w->task=SysTask_CreateOnVWaitQueue(ov48_0225A680,w,0);}
