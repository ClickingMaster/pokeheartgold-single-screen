#pragma force_active on
#include "global.h"
typedef struct{int heap;void *b,*c,*a;int unused;void *fmt;u8 tail[36];} Work;
extern void *Heap_Alloc(int,int),*MessageFormat_New(int);
extern void FontID_Alloc(int,int),ov95_021E72B8(Work *);
Work *ov95_021E7020(void *,void *,void *,int);
Work *ov95_021E7020(void *a,void *b,void *c,int heap){Work *w;if(!a)GF_AssertFail();if(!b)GF_AssertFail();if(!c)GF_AssertFail();w=Heap_Alloc(heap,60);MI_CpuFill8(w,0,60);w->heap=heap;w->a=a;w->b=b;w->c=c;w->fmt=MessageFormat_New(heap);FontID_Alloc(4,w->heap);ov95_021E72B8(w);return w;}
