#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct S{void*task;s16 x,y;s8 frame,limit,repeats,pad;}S;typedef struct W{u8 pad[0x97];u8 low:6;u8 first:1;u8 second:1;}W;extern void SysTask_Destroy(void*);void ov80_0222F4F0(void*,S*),ov80_0222F5D0(W*,int),ov80_0222F5EC(W*,int);
void ov80_0222F4F0(void*task,S*s){s->frame++;if(s->frame>s->limit){s->frame=0;s->repeats--;if(s->repeats<0){SysTask_Destroy(task);MI_CpuFill8(s,0,12);return;}s->x=-s->x;s->y=-s->y;}}
