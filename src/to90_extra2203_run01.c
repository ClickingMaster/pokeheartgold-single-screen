#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#include "system.h"
extern int ov18_021F7B94(void*);int ov18_021F7D00(void*,int*);
int ov18_021F7D00(void*w,int*out){int value=ov18_021F7B94(w),fallback=-1;if(value==fallback){value=0;*out=0;if(!(gSystem.newAndRepeatedKeys&32)){if(gSystem.newAndRepeatedKeys&16)fallback=2;value=fallback;}}return value;}
