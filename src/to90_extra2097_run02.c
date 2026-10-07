#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(u8 **)((u8 *)(p)+(n)))
#define H(p,n) (*(u16 *)((u8 *)(p)+(n)))
typedef struct{u16 move;u8 pp,maxpp;u32 other;}Slot;extern int ov08_02224FE0[];
extern void FillWindowPixelBuffer(void *,u8),ScheduleWindowCopyToVram(void *),ov08_0221DDCC(void *,int,int,int,int,int),ov08_0221E244(void *,u16,int,int,int,int,u32),ov08_0221F284(void *,Slot *,int);
extern u32 GetMoveAttr(u16,int);
void ov08_0221FC7C(u8 *);
void ov08_0221FC7C(u8 *p){u32 i;Slot slot;FillWindowPixelBuffer(P(p,0x2070),0);FillWindowPixelBuffer(P(p,0x2070)+16,0);FillWindowPixelBuffer(P(p,0x2070)+32,0);FillWindowPixelBuffer(P(p,0x2070)+48,0);FillWindowPixelBuffer(P(p,0x2070)+64,0);FillWindowPixelBuffer(P(p,0x2070)+80,0);ov08_0221DDCC(p,0,0,P(p,0)[17],0,0);for(i=0;i<4;i++){Slot *current=(Slot *)(p+52+P(p,0)[17]*80);if(current[i].move){ov08_0221E244(p,current[i].move,i+1,ov08_02224FE0[i],4,7,0x70809);ov08_0221F284(p,&current[i],i+1);}}ov08_0221E244(p,H(P(p,0),36),5,73,4,7,0x70809);slot.pp=GetMoveAttr(H(P(p,0),36),5);slot.maxpp=slot.pp;ov08_0221F284(p,&slot,5);ScheduleWindowCopyToVram(P(p,0x2070)+16);ScheduleWindowCopyToVram(P(p,0x2070)+32);ScheduleWindowCopyToVram(P(p,0x2070)+48);ScheduleWindowCopyToVram(P(p,0x2070)+64);ScheduleWindowCopyToVram(P(p,0x2070)+80);}
