#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define SH(p,n) (*(s16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
void PlaySE(u16);
void*Heap_Alloc(u32,u32);u32 ov96_021E5F24(void*);void ov96_0220D200(void*,u8,void*);void ov96_0220C490(void*,void*,void*);void ManagedSprite_GetPositionXYWithSubscreenOffset(void*,s16*,s16*,int);void ManagedSprite_GetPositionXY(void*,s16*,s16*);void*ov96_0220D1A0(void*,void*,int,int,int,int);void*ov96_0220D13C(void*,void*,int,int,int,int);void ManagedSprite_SetDrawFlag(void*,int);void ov96_0220C54C(void*,int,u8,int,int);void*ov96_0220B374(u32,void*,void*,void*);
void*ov96_0220B374(u32 heap,void*a,void*b,void*course){u8 index=ov96_021E5F24(course);void*s=Heap_Alloc(heap,72);s16 x,y,x0,y0;MI_CpuFill8(s,0,72);W(s,0)=heap;P(s,4)=a;P(s,8)=b;P(s,12)=course;W(s,0x44)&=~255;ov96_0220D200((u8*)s+0x34,index,course);ov96_0220C490((u8*)s+0x10,P(s,4),P(s,8));ManagedSprite_GetPositionXYWithSubscreenOffset(P(s,0x24),&x,&y,1<<20);y-=24;P(s,0x2c)=ov96_0220D1A0(a,b,x,y,17,0);ManagedSprite_SetDrawFlag(P(s,0x2c),0);ManagedSprite_GetPositionXYWithSubscreenOffset(P(s,0x28),&x,&y,1<<20);y-=24;P(s,0x30)=ov96_0220D1A0(a,b,x,y,17,0);ManagedSprite_SetDrawFlag(P(s,0x30),0);ManagedSprite_GetPositionXY(P(s,0x10),&x0,&y0);P(s,0x14)=ov96_0220D13C(a,b,x0,y0,17,26);ManagedSprite_SetDrawFlag(P(s,0x14),0);P(s,0x1c)=ov96_0220D13C(a,b,x0,y0,15,23);ManagedSprite_SetDrawFlag(P(s,0x1c),0);P(s,0x20)=ov96_0220D13C(a,b,x0,y0,27,24);ManagedSprite_SetDrawFlag(P(s,0x20),0);y0-=24;P(s,0x18)=ov96_0220D13C(a,b,x0,y0,24,25);ManagedSprite_SetDrawFlag(P(s,0x18),0);ov96_0220C54C(s,0,index,0,1);ov96_0220C54C(s,5,index,2,0);ov96_0220C54C(s,6,index,1,0);return s;}
