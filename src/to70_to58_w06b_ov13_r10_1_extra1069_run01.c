#pragma force_active on
#include "global.h"
typedef struct {int a,b;} Pair;
extern int SOC_Poll(Pair*,int,s64);
int ov13_02222998(void*,Pair*,int,int,Pair*);
int ov13_02222998(void*unused,Pair*p,int unused2,int unused3,Pair*time){Pair copy=*p;u64 ticks=0;ticks+=(u64)((s64)time->a*0x1ff6210)>>6;ticks+=(u64)((s64)time->b*0x1ff6210)>>6;return SOC_Poll(&copy,1,ticks);}
