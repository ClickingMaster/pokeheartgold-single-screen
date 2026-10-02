#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sys_task_api.h"
typedef struct {u8 pad[0xc4];SysTask *tasks[8];u8 pad2[0x10];volatile int count,index,mode;} Work;
typedef struct {Work *w;int value,state,tick,index,mode;} Job;
extern void ov71_02246F90(SysTask *,void *);
void ov71_02246EAC(Work *);
void ov71_02246EAC(Work *w){Job *j;if(w->tasks[w->index]!=NULL)return;j=Heap_Alloc((enum HeapID)56,sizeof(Job));if(j){j->w=w;j->value=w->mode==1?-1:15;j->tick=0;j->state=2;j->index=w->index;j->mode=w->mode;w->tasks[w->index]=SysTask_CreateOnMainQueue(ov71_02246F90,j,10);if(w->tasks[w->index]){w->count++;w->index++;if(w->index>=8)w->index=0;}else Heap_Free(j);}}
