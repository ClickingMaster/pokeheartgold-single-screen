#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern int Save_WriteFileAsync(void*);int ov70_0224089C(void*);
int ov70_0224089C(void*w){if(Save_WriteFileAsync(P(P(w,0),0x20))==1)U(w,0x2c)=H(w,0x11d4);return 3;}
