#pragma force_active on
#include "global.h"
extern void *Heap_Alloc(int,u32),*NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New(int);extern void ov96_022148A4(void *);void *ov96_0221464C(int,void *,void *);
void *ov96_0221464C(int heap,void *a,void *b){u8 *w=Heap_Alloc(heap,80);MI_CpuFill8(w,0,80);*(int*)w=heap;*(void**)(w+16)=a;*(void**)(w+4)=b;ov96_022148A4(w);*(void**)(w+52)=NewMsgDataFromNarc(1,27,309,heap);*(void**)(w+56)=MessageFormat_New(heap);return w;}
