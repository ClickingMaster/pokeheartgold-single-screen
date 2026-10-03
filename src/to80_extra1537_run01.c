#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int state;void*a,*b;}S;extern void ov57_0223B890(void*,int,int),ov57_0223B8B8(void*,void*);extern void *Heap_Alloc(int,u32);extern void *SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,u32);
void*ov57_0223B90C(void*a,void*b);
void*ov57_0223B90C(void*a,void*b){S*s=Heap_Alloc(52,12);s->state=1;s->a=a;s->b=b;ov57_0223B890(s->b,0,-1);return SysTask_CreateOnMainQueue(ov57_0223B8B8,s,1000);}
