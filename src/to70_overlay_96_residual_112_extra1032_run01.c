#pragma force_active on
#include "global.h"
typedef struct{void *a,*b,*task;s32 value,step;u8 state,duration,initial,unused;int ticks;} Work;
extern void SysTask_Destroy(void *);extern s32 FX_Div(s32,s32);extern void ov96_02215710(void *,Work *);extern void *SysTask_CreateOnMainQueue(void (*)(void *,Work *),Work *,int);
void ov96_02215884(Work *,int,int);
void ov96_02215884(Work *w,int initial,int duration){if(!w->b)GF_AssertFail();if(!w->a)GF_AssertFail();if(w->task)SysTask_Destroy(w->task);w->ticks=0;w->state=0;w->duration=duration;w->value=initial<<12;w->initial=initial;w->step=FX_Div(initial<<12,duration<<12);w->task=SysTask_CreateOnMainQueue(ov96_02215710,w,2);}
