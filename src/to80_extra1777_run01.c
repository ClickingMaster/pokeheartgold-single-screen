#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern int FontID_String_GetWidth(int,void*,int);extern u8 GetWindowWidth(void*);int ov40_022306C0(void*,void*);
int ov40_022306C0(void*w,void*s){int width=FontID_String_GetWidth(0,s,0);return (GetWindowWidth(w)*8-width)/2;}
