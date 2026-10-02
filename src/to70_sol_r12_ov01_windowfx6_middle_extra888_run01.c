#pragma force_active on
#include "global.h"
typedef struct{int offset;u8 p[20];u32 period;} Work;void ov01_021F01D0(void *,Work *);
static inline void planes(u32 mask,int enable){u32 value=*(volatile u16 *)0x0400004a;value=(value&~63)|mask;if(enable)value|=32;*(volatile u16 *)0x0400004a=value;}
void ov01_021F01D0(void *task,Work *w){int offset;u32 scroll;if(!((*(volatile u16 *)0x04000006/w->period)&1))offset=w->offset;else offset=-w->offset;offset>>=12;scroll=offset&511;*(volatile u32 *)0x04000010=scroll;*(volatile u32 *)0x04000014=scroll;*(volatile u32 *)0x04000018=scroll;*(volatile u32 *)0x0400001c=scroll;if(offset>=0){*(volatile u32 *)0x04000000=(*(volatile u32 *)0x04000000&~0xe000)|0x2000;planes(0,0);}else{*(volatile u32 *)0x04000000=(*(volatile u32 *)0x04000000&~0xe000)|0x4000;planes(31,1);}}
