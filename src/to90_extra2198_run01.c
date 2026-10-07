#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern const u8 ov18_021FB828[],ov18_021FB968[],ov18_021FB668[];void*GridInputHandler_Create(const void*,const void*,const void*,void*,int,int,int);void ov18_021F11C0(void*,int,int);void ov18_021F6F38(void*,int);
void ov18_021F6EC0(u8*w);
void ov18_021F6EC0(u8*w){P(w,0x1864)=GridInputHandler_Create(ov18_021FB828,ov18_021FB968,ov18_021FB668,w,1,0,37);ov18_021F11C0(w,0,1);ov18_021F6F38(w,0);}
