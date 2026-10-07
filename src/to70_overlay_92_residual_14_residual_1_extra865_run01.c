#pragma force_active on
#include "global.h"
#include <string.h>
#include "heap.h"
#include "sys_task_api.h"
#include "to45_overlay92_tail_private.h"
#pragma force_active on
typedef struct Job {u32 state,tick;void*x,*a,*b;u8 p14[0x18];void*owner;}Job;
typedef Ov92SpriteTask J2;
typedef struct W {u8 p[0x14];void*owner;u8 p18[4];void*a[3],*b[3],*c[3];u8 p40[0xb4];u16 index[4];}W;extern void ov92_0225F254(SysTask*,void*),ov92_0225F338(SysTask*,void*);SysTask*ov92_0225EDB4(W*,int);void ov92_0225EE04(W*,int);
SysTask*ov92_0225EDB4(W*w,int index){Job*j=Heap_Alloc((enum HeapID)113,0x30);memset(j,0,0x30);j->a=w->a[w->index[index]];j->b=w->b[w->index[index]];j->state=0;j->tick=0;j->owner=w->owner;return SysTask_CreateOnMainQueue(ov92_0225F254,j,4096);}
void ov92_0225EE04(W*w,int i){J2*j=(J2*)((u8*)w+0x29d0+i*0x44);if(j->active==1)return;j->extra=*(u32*)((u8*)w+0x2bb4);j->active=1;j->current=(s32*)((u8*)w+0x2b08+i*4);j->previous=*j->current;j->sprite=w->c[w->index[i]];j->state=0;j->mode=0;j->parent=w->owner;SysTask_CreateOnMainQueue(ov92_0225F338,j,4096);}
