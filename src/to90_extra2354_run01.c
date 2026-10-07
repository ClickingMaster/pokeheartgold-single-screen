#pragma force_active on
#pragma force_active on
#include "global.h"
extern void ov01_021EFE34(void*,int,int,int),ov01_021F0C40(void*,void*);
extern void*SysTask_CreateOnVWaitQueue(void(*)(void*,void*),void*,int);
typedef struct{u8 pad[0x14];int state;u8 lines[192];void*bg;int a,active;int*result;}Work;
typedef struct{u8 pad[16];void*args;int a,result;}Manager;
void*ov01_021F0B78(Manager*,Work*,int,u32,u32,u32,u32);
void*ov01_021F0B78(Manager*m,Work*w,int kind,u32 period,volatile u32 width,u32 inside,u32 outside){int i;width=width;if(w->active)GF_AssertFail();m->result=0;w->bg=((void**)((void**)m->args)[1])[7];w->state=0;w->result=&m->result;ov01_021EFE34(w,255,0,kind);for(i=0;i<192;i++){u32 remainder=i%period;u32 product=remainder*width;u32 x=product/period;if((i/period&1)==0)w->lines[i]=x;else w->lines[i]=width-x;}*(volatile u16*)0x4000048=(*(volatile u16*)0x4000048&~63)|inside|32;*(volatile u16*)0x400004a=(*(volatile u16*)0x400004a&~63)|outside;*(volatile u16*)0x4000040=0;*(volatile u16*)0x4000044=192;*(volatile u32*)0x4000000=(*(volatile u32*)0x4000000&0xffff1fff)|0x2000;return SysTask_CreateOnVWaitQueue(ov01_021F0C40,w,1024);}
