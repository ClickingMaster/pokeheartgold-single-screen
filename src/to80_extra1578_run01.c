#pragma force_active on
#include "global.h"
typedef struct {int heap,mask,param;void *man[4];u8 rest[0x98];}Work;
extern void *Heap_Alloc(int,u32),*Create2DGfxResObjMan(int,int,int);
extern void ov96_021E9D10(Work *);
Work *ov96_021E9A78(int,int,int);
#define ADD(bit) if(mask&(bit)){counts[0]++;counts[2]++;counts[3]++;}
Work *ov96_021E9A78(int heap,int mask,int param){u8 counts[4];Work *w;int i;for(i=0;i<4;i++)counts[i]=0;ADD(1);ADD(4);ADD(2);ADD(8);ADD(16);ADD(32);ADD(64);ADD(128);ADD(256);ADD(512);ADD(1024);ADD(2048);counts[1]=2;w=Heap_Alloc(heap,0xb4);w->heap=heap;w->mask=mask;w->param=param;for(i=0;i<4;i++)w->man[i]=Create2DGfxResObjMan(counts[i],i,w->heap);ov96_021E9D10(w);return w;}
