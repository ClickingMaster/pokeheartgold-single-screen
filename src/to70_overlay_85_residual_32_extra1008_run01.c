#pragma force_active on
#include "global.h"
extern void ov85_021E82E4(void *,void *);extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,u32);void ov85_021E82F8(void *);
void ov85_021E82F8(void *w){u8 *p=(u8*)w+0xc44;p[0]=0;p[1]=0;p[2]=0;p[3]=0;p[4]=0;p[5]=0;p[6]=0;p[7]=0;*(void**)((u8*)w+0xdc0)=SysTask_CreateOnMainQueue(ov85_021E82E4,w,259);if(*(void**)((u8*)w+0xdc0)==0)GF_AssertFail();}
