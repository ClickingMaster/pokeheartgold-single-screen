#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#include "system.h"
extern int PokedexApp_HandleEntryTabTouch(void*);int PokedexApp_HandleSizeCheckInput(void*,int*);
int PokedexApp_HandleSizeCheckInput(void*w,int*out){int value=PokedexApp_HandleEntryTabTouch(w),fallback=-1;if(value==fallback){value=0;*out=0;if(!(gSystem.newAndRepeatedKeys&32)){if(gSystem.newAndRepeatedKeys&16)fallback=2;value=fallback;}}return value;}
