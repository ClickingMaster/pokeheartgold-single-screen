#pragma force_active on
#include "global.h"
typedef struct {int a,b;u8 counts[4];} Args;
typedef struct {u32 heap,x;Args args;void *renderer;u8 render[0x140-0x18];void *man[4],*arr1,*arr2;} Work;
extern void *Heap_Alloc(u32,u32);extern void *G2dRenderer_Init(u32,void*,u32);extern void *Create2DGfxResObjMan(u32,u32,u32);Work *ov96_021EB180(u32,Args*);
Work *ov96_021EB180(u32 heap,Args *args){Work*w=Heap_Alloc(heap,0x158);int i;MIi_CpuClearFast(0,(u32*)w,0x158);w->heap=heap;w->args=*args;w->arr1=Heap_Alloc(heap,w->args.b*20);MIi_CpuClear32(0,w->arr1,w->args.b*20);w->arr2=Heap_Alloc(w->heap,w->args.a*12);MIi_CpuClear32(0,w->arr2,w->args.a*12);w->renderer=G2dRenderer_Init(w->args.a,w->render,w->heap);for(i=0;i<4;i++)w->man[i]=Create2DGfxResObjMan(w->args.counts[i],i,w->heap);return w;}
