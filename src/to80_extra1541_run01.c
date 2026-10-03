#pragma force_active on
#include "global.h"
#define PTR(off) (*(void **)(w+(off)))
extern void FillWindowPixelBuffer(void *,int),ScheduleWindowCopyToVram(void *),ov67_021E61E8(void *),ov67_021E6164(void *,int,int,int,int,int,int,int),ov67_021E61A0(void *,int,int,int,int,int,int,int),BufferIntegerAsString(void *,int,int,int,int,int);
extern u8 GetWindowWidth(void *);
extern int sub_0202D5DC(void *,int,int),sub_0202D488(void *,int),FrontierSave_GetStat(void *,int,int);
void ov67_021E6220(u8 *);
void ov67_021E6220(u8 *w){u32 i,j;u8 width;for(i=0;i<10;i++)FillWindowPixelBuffer(w+0x6c+i*16,0);ov67_021E61E8(w);width=GetWindowWidth(w+0x7c);ov67_021E6164(w,1,2,(width*8)/2,0,0,0xf0200,2);ov67_021E6164(w,2,5,0,0,0,0x10200,0);ov67_021E6164(w,3,8,0,0,0,0x10200,0);ov67_021E6164(w,4,12,0,0,0,0x10200,0);if(sub_0202D5DC(PTR(0x20),12,0)==0)ov67_021E6164(w,6,9,0,0,0,0x10200,0);else ov67_021E6164(w,6,10,0,0,0,0x10200,0);ov67_021E6164(w,8,11,0,0,0,0x10200,0);BufferIntegerAsString(PTR(0x2c),0,sub_0202D488(PTR(0x20),0),2,0,1);ov67_021E61A0(w,5,14,GetWindowWidth(w+0xbc)*8,0,0,0x10200,1);BufferIntegerAsString(PTR(0x2c),0,FrontierSave_GetStat(PTR(0x1c),9,255),4,1,1);width=GetWindowWidth(w+0xdc);ov67_021E61A0(w,7,15,(width*8)/2,0,0,0x10200,2);BufferIntegerAsString(PTR(0x2c),0,FrontierSave_GetStat(PTR(0x1c),8,255),4,1,1);width=GetWindowWidth(w+0xfc);ov67_021E61A0(w,9,15,(width*8)/2,0,0,0x10200,2);for(j=0;j<10;j++)ScheduleWindowCopyToVram(w+0x6c+j*16);}
