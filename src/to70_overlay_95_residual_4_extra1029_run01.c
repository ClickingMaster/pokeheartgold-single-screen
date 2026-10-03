#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int heap;void *arg;int dummy;void *object;}Work;
extern void *Heap_Alloc(int,u32),*ov95_021E5E18(int,int,void*);
extern void sub_02015528(void*,int),sub_02015494(void*,int,void(*)(void),void*),ov95_021E5E58(void);

Work *ov95_021E5E90(Work*src);
void ov95_021E5EC0(Work*w,int a);

Work *ov95_021E5E90(Work*src){Work*w=Heap_Alloc(src->heap,16);if(w==0)GF_AssertFail();{volatile Work *input=src;int heap=input->heap;void *arg=input->arg;w->heap=heap;w->arg=arg;w->object=ov95_021E5E18(w->heap,116,arg);}sub_02015528(w->object,1);return w;}
void ov95_021E5EC0(Work*w,int a){sub_02015494(w->object,a,ov95_021E5E58,w);sub_02015528(w->object,1);}
