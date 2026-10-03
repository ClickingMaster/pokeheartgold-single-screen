#pragma force_active on
#include "global.h"
typedef struct {u32 a,b;void*tasks[4];u32 c,d;} Work;
typedef struct {Work*w;u32 c,d,index;volatile u32 slot;u32 state;} Env;
extern void *Heap_Alloc(int,int);extern void *SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);extern void (*volatile ov102_021EC788[])(void*,void*);

void ov102_021E940C(Work*w,u32 index);

void ov102_021E940C(Work*w,u32 index){if(index<32){Env*e=Heap_Alloc(35,24);if(e){int i=0;Work*cursor;e->state=0;e->index=index;e->w=w;e->c=w->c;e->d=w->d;cursor=w;for(;i<4;i++){if(cursor->tasks[0]==0){void(*fn)(void*,void*)=ov102_021EC788[index];e->slot=i;w->tasks[i]=SysTask_CreateOnMainQueue(fn,e,1);return;}cursor=(Work*)((u8*)cursor+4);}}}else GF_AssertFail();}
