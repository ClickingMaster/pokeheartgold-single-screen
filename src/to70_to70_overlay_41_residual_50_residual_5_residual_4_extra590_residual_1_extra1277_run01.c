#pragma force_active on
#include "global.h"
typedef struct {u32 a; void *chars,*buffer;u32 c,d;} Work;
void *Heap_Alloc(int,u32);void *GfGfxLoader_GetCharData(int,int,int,void**,int);void ov41_0224A094(void*,void*);void *SysTask_CreateOnVWaitQueue(void(*)(void*,void*),void*,u32);
void *ov41_02249FFC(int,int,u32,u32,u32,int);
void *ov41_02249FFC(int a,int b,u32 c,u32 d,u32 e,int heap){Work*w=Heap_Alloc(heap,sizeof(*w)); memset(w,0,sizeof(*w));w->buffer=GfGfxLoader_GetCharData(a,b,0,&w->chars,heap);w->a=c;w->c=d;w->d=e;return SysTask_CreateOnVWaitQueue(ov41_0224A094,w,128);}
