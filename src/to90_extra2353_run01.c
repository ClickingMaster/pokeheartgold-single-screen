#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

u32 ov01_021EE2E4(void*);void AddWindowParameterized(void*,void*,u8,u8,u8,u8,u8,u8,u16),LoadUserFrameGfx1(void*,int,int,int,int,int),DrawFrameAndWindow1(void*,int,int,int),ov01_021EE324(void*),ov01_021EE634(void*),ov01_021EE49C(void*,void*);void*ListMenuInit(void*,u16,u16,int);void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);void ov01_021EE0EC(void*,u16*,u16*);
typedef struct {u8 rest:6;u8 first:1;u8 second:1;} Flags;
void ov01_021EE0EC(void*s,u16*a,u16*b){u32 n=ov01_021EE2E4(s),width;if((n&7)==0)width=n>>3;else width=(n>>3)+1;if(((Flags*)((u8*)s+0x97))->first)B(s,0x98)-=width;if(((Flags*)((u8*)s+0x97))->second){if(B(s,0x9b)>8)B(s,0x99)-=16;else B(s,0x99)-=B(s,0x9b)*2;}if(B(s,0x9b)>8)AddWindowParameterized(P(P(s,0),8),(u8*)s+8,3,B(s,0x98),B(s,0x99),(u8)width,16,13,61);else AddWindowParameterized(P(P(s,0),8),(u8*)s+8,3,B(s,0x98),B(s,0x99),(u8)width,(u8)(B(s,0x9b)*2),13,61);LoadUserFrameGfx1(P(P(s,0),8),3,985,11,0,4);DrawFrameAndWindow1((u8*)s+8,1,985,11);ov01_021EE324(s);P(s,0xa4)=a;P(s,0xa8)=b;H(s,0x2dc)=H(P(s,0xa4),0)+H(P(s,0xa8),0);P(s,0x1bc)=ListMenuInit((u8*)s+0x19c,*a,*b,4);ov01_021EE634(s);P(s,4)=SysTask_CreateOnMainQueue(ov01_021EE49C,s,0);}
