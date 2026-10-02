#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x214];u8 object[20];volatile s32 value;u16 index,mode;} Work;
extern u8 ov91_02261C12[],ov91_02261C13[];extern void sub_02018198(void *,u32);
void ov91_02260378(Work *);
void ov91_02260378(Work *w){if(w->mode==1){int v=w->value+8192;if(v<163840)w->value=w->value+8192;else w->value=4096;}else{int i=w->index==4?0:w->index+1;int lo=ov91_02261C12[i*2]<<12;int v=w->value;if(v<lo)w->value=lo;else{int next=v+8192;if(next<(ov91_02261C13[i*2]<<12))w->value=next;else w->value=lo;}}sub_02018198(w->object,w->value);}
