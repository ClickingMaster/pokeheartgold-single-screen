#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sys_task.h"
extern SysTask *SysTask_CreateOnMainQueue(void (*)(SysTask *,void *),void *,u32);
extern int ov07_0221DDB0(void *,void *,int);
extern void ov07_0221DD38(void *,void *,int),ov07_0221E600(SysTask *,void *);
typedef struct {void *object;s16 a,b,c,d;int e,f,g,h;u8 tail[12];} Params;
int ov07_0221E6D4(void *);
#define Q(n) (*(u32 *)(*(u8 **)(work+0x48)+(n)))
#pragma force_active on
#pragma opt_lifetimes off
int ov07_0221E6D4(void *argument){
 u8 *work=argument;Params *p=Heap_Alloc((enum HeapID)Q(0),0x28);
 p->object=(void *)Q(0xc4);p->a=(s16)Q(0x9c);p->b=(s16)Q(0xa0);p->c=(s16)Q(0x94);p->d=(s16)Q(0x98);p->e=3;p->f=3;
 if(ov07_0221DDB0(work,*(void **)(work+0x48),6)==1){p->c*=-1;p->d*=-1;p->a*=-1;p->b*=-1;}
 p->g=1;p->h=0;ov07_0221DD38(*(u8 **)(work+0x48)+0x180,p,1);*(u16 *)(work+0x44)=(*(u16 *)(work+0x44)&~1)|1;
 SysTask_CreateOnMainQueue(ov07_0221E600,p,0x1001);return 0;
}
