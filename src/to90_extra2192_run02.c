#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern const u8 ov18_021FA610[];extern void ov18_021F3E24(void*),ov18_021F69C0(void*,int),ov18_021F3CA8(void*,int,u8*,u8*),ov18_021F1534(void*,u16,u8,int),ov18_021F40E4(void*),ov18_021F40A0(void*),ov18_021F4188(void*);extern void* ov18_021F11EC(void*,const void*);
void ov18_021F3D98(void*);
#pragma force_active on
void ov18_021F3D98(void*w){u8 x,y;u32 i;ov18_021F3E24(w);for(i=1;i<=8;i++)P((u8*)w+i*4,0x670)=ov18_021F11EC(w,ov18_021FA610+(i-1)*52);
 ov18_021F69C0(w,1);ov18_021F3CA8(w,0,&x,&y);ov18_021F1534(w,H(w,0x18a2),x,1);ov18_021F40E4(w);ov18_021F40A0(w);ov18_021F4188(w);}
