#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x74];void*value;u32 p2;void*task;} Work;
typedef struct{Work*w;void*value;u8 first[12];u8 second[8];int a,b,c,d;u16 e;u16 pad;} Task;
extern void*Heap_Alloc(int,int);
extern void TradeSequence_GetModelPosition(void*,void*),TradeSequence_GetModelRotation(void*,void*),ov71_0224820C(void*,void*);
extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);
void ov71_0224817C(Work*);
void ov71_0224817C(Work*w){Task*t=Heap_Alloc(57,sizeof(*t));if(t){t->w=w;t->value=w->value;TradeSequence_GetModelPosition(t->value,t->first);TradeSequence_GetModelRotation(t->value,t->second);t->a=0x2f60;t->b=0;t->e=0;t->c=0;t->d=0;w->task=SysTask_CreateOnMainQueue(ov71_0224820C,t,0);}}
