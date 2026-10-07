#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 flags;u16 min,max,freq,mode;u8 a,b,c,pad;} MPParams;
int WMi_StartMP(void*,void*,u32,void*,u16,MPParams*);
int WM_StartMPEx(void*,void*,u32,void*,u16,u16,u16,u32,u32,u32,u32);
int WM_StartMPEx(void*cb,void*recv,u32 size,void*send,u16 sendsize,u16 freq,u16 mode,u32 a,u32 b,u32 enable,u32 c){MPParams p;MI_CpuClear32(&p,16);p.flags=0x1e03;p.min=freq;p.max=freq;p.mode=mode;p.a=a;p.b=b;p.c=c;if(enable&&freq){p.flags|=4;p.freq=freq;}return WMi_StartMP(cb,recv,size,send,sendsize,&p);}
