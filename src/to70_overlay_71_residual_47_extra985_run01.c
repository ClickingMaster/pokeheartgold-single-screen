#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x7c];void*task;} Work;
typedef struct{Work*w;int value,target,delta,count;} Transition;
extern void*Heap_Alloc(int,int);
extern void ov71_0224B990(void*,void*);
extern void*SysTask_CreateOnVBlankQueue(void(*)(void*,void*),void*,int);
void ov71_0224B910(Work*,int,int,int);
void ov71_0224B910(Work*w,int start,int target,int ticks){Transition*t=Heap_Alloc(57,sizeof(*t));if(t){t->w=w;t->value=start<<12;t->target=target<<12;t->delta=(t->target-t->value)/ticks;t->count=ticks;w->task=SysTask_CreateOnVBlankQueue(ov71_0224B990,t,0);G2x_SetBlendBrightness_((void*)0x4000050,1,start);}}
