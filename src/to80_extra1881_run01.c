#pragma force_active on
#include "global.h"
typedef struct {void*p;s16 a,tick;s32 amount;} Work;
extern s32 FX_Div(s32,s32); extern s32 _s32_div_f(s32,s32);extern void ov49_0225D4C8(void*,s32),ov49_0225D4D0(void*,u8,u8),ov49_0225D4F0(void*,s32,s32,s32);
int ov49_0225EC30(Work*w);
#pragma force_active on
int ov49_0225EC30(Work*w){s32 x; register s32 alpha; if(w->tick<10){w->tick++;x=FX_Div((s32)(((s64)(w->tick<<12)*w->amount+2048)>>12),10<<12);ov49_0225D4C8(w->p,x+w->amount);alpha=w->tick*31/10;alpha=31-alpha;ov49_0225D4D0(w->p,alpha,31);x=FX_Div((s32)(((s64)(w->tick<<12)*737+2048)>>12),10<<12);ov49_0225D4F0(w->p,x+4096,x+4096,x+4096);return 0;}return 1;}
