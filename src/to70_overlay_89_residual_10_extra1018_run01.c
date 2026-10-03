#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x240];u32 low:4;u32 other:4;u32 current:8;u32 pending:8;u32 top:8;}Work;

int ov89_0225AFC0(Work*w,int state);

int ov89_0225AFC0(Work*w,int state){if(w->current!=0 || w->pending!=0 || w->low==0)return 0;w->pending=state;MI_CpuFill8((u8*)w+0x94,0,0x240-0x94);return 1;}
