#pragma force_active on
#include "global.h"
#pragma force_active on
extern void SetLCRNGSeed(u32),ov92_0225D8D4(void*);
void ov92_0225C540(void*a,void*b,volatile u32*p,u8*w);
void ov92_0225C540(void*a,void*b,volatile u32*p,u8*w){u32 first=p[0],second=p[1];*(u32*)(w+20)=first;*(u32*)(w+24)=second;SetLCRNGSeed(second);ov92_0225D8D4(w);}
