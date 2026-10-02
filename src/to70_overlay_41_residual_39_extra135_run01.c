#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Owner {u8 pad[0x3c];int busy,side;}Owner;
typedef struct Work{Owner*owner;int a,b,c,d,done,rate,state,count1,count2;void*array;int count;}Work;
extern void*CreateSysTaskAndEnvironment(void(*)(void*,void*),int,int,int),*SysTask_GetData(void*);extern int ov41_02248AE0(Owner*,int,int);extern void*Heap_Alloc(int,int);extern void ov41_02248BFC(void*,void*);
void ov41_02248B48(int,int*,int*),ov41_02248B84(Owner*,int,int,int,int);
void ov41_02248B84(Owner*o,int a,int b,int c,int d){Work*w=SysTask_GetData(CreateSysTaskAndEnvironment(ov41_02248BFC,48,0,13));w->owner=o;w->a=a;w->b=b;w->c=c;w->d=d;w->state=0;w->count1=ov41_02248AE0(o,a,b);w->count2=ov41_02248AE0(o,c,d);w->count=w->count1+w->count2;w->array=Heap_Alloc(13,12*w->count);if(!w->array)GF_AssertFail();memset(w->array,0,12*w->count);o->busy=1;}
