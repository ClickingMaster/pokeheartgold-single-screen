#pragma force_active on
#include "global.h"
extern void *Heap_Alloc(int,int);
extern void Heap_Free(void *);
extern void ov71_022476B4(void *,s32 *);
extern void ov71_022476C4(void *,s32 *);
extern void ov71_0224B068(void *);
extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,int);
typedef struct {void **task;int ticks;void *obj;s32 xyz[3];s32 step;} Work;
typedef struct {u8 p[0x24];void *obj;} Parent;
void ov71_0224AFD4(Parent *,void **);
void ov71_0224B028(void *,void *);
void ov71_0224AFD4(Parent *parent,void **task){Work *w=Heap_Alloc(57,28);if(w){w->task=task;w->obj=parent->obj;ov71_022476B4(w->obj,w->xyz);w->step=(0x32000-w->xyz[1])/16;w->ticks=16;*task=SysTask_CreateOnMainQueue(ov71_0224B028,w,0);if(!*task)Heap_Free(w);}}
void ov71_0224B028(void *task,void *data){Work *w=data;if(w->ticks){w->xyz[1]+=w->step;ov71_022476C4(w->obj,w->xyz);w->ticks--;return;}w->xyz[1]=0x32000;ov71_022476C4(w->obj,w->xyz);ov71_0224B068(task);}
