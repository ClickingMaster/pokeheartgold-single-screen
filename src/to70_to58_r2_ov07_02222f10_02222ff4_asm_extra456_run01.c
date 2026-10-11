#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{int state;void*task;void*data;u16 a,b,c;u8 start,end,current;s8 delta,mode,last;void*ctx;}W;extern void*Heap_Alloc(int,u32),*SysTask_CreateOnMainQueue(void(*)(void*,W*),W*,int);extern void ov07_02222E74(void*,W*);extern void*BattleAnimSystem_GetPaletteData(void*);extern u16*PaletteData_GetFadedBuf(void*,int);extern int ov07_0221BFC0(void*);W*ov07_02222F10(void*,int,void*,u16,u16,s8,s8,u8,u8,u16,int);void ov07_02222F7C(u16*,int),ov07_02222FC4(void*);
W*ov07_02222F10(void*ctx,int heap,void*data,u16 a,u16 b,s8 mode,s8 delta,u8 start,u8 end,u16 c,int priority){W*w=Heap_Alloc(heap,28);if(w==0)GF_AssertFail();w->ctx=ctx;w->data=data;w->a=a;w->b=b;w->c=c;w->start=start;w->end=end;w->current=start;w->mode=mode;w->last=mode;if(w->start<w->end)w->delta=delta;else w->delta=-delta;w->state=1;w->task=SysTask_CreateOnMainQueue(ov07_02222E74,w,priority);return w;}
void ov07_02222F7C(u16*palette,int count){int i=0;if(count>0){do{int color=*palette;int b=(color>>10)&31;int r=color&31;int g=(color>>5)&31;int level=(b*29+(r*76+g*151))>>8;*palette=(level<<10)|(level<<5)|level;i++;palette++;}while(i<count);}}
void ov07_02222FC4(void*ctx){u16*buf=PaletteData_GetFadedBuf(BattleAnimSystem_GetPaletteData(ctx),0);if(ov07_0221BFC0(ctx)==1)ov07_02222F7C(buf,48);else ov07_02222F7C(buf,128);}
