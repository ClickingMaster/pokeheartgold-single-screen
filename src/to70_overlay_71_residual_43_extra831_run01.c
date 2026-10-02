#pragma force_active on
#include "global.h"
extern void *Heap_Alloc(int,int);
extern void Heap_Free(void *);
extern void ov71_02247708(void *,int);
extern void ov71_0224B0E8(void *,void *);
extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,int);
typedef struct {void **task;int x,step,target,ticks;void *obj;} Work;
void ov71_0224B084(void *obj,int start,int target,int ticks,void **task);
void ov71_0224B084(void *obj,int start,int target,int ticks,void **task){Work *w=Heap_Alloc(57,24);if(w){w->task=task;w->x=start<<12;w->target=target<<12;w->step=(w->target-w->x)/ticks;w->ticks=ticks;w->obj=obj;ov71_02247708(obj,start);*task=SysTask_CreateOnMainQueue(ov71_0224B0E8,w,0);if(!*task)Heap_Free(w);}else *task=0;}
