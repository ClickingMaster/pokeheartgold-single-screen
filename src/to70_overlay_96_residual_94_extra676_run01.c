#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x154];void *sprites[10];union{volatile u32 word;struct{u32 ticks:16;u32 rest:16;}bits;} timer;union{u32 word;struct{u32 state:4;u32 index:4;u32 rest:24;}bits;}state;}Work;
extern int ov96_0220B164(void*);
extern void ov96_0220AED4(void*),ov96_0220AF64(void*),PlaySE(int);

void ov96_0220AD64(Work*w);

void ov96_0220AD64(Work*w){int i;u32 ticks;if(w->state.bits.index>=10){for(i=0;i<10;i++)if(ov96_0220B164(w->sprites[i])==1)break;if(i==10){PlaySE(0x8bd);w->state.bits.state=2;}}else{ticks=w->timer.bits.ticks;w->timer.bits.ticks++;if(ticks>=2){w->timer.bits.ticks=0;ov96_0220AED4(w->sprites[w->state.bits.index]);PlaySE(0x8c1);w->state.bits.index++;}}for(i=0;i<10;i++)ov96_0220AF64(w->sprites[i]);}
