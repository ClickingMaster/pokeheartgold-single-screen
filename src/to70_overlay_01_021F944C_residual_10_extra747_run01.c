#pragma force_active on
#include "global.h"
typedef struct {u32 a,b,c;void *used;} Slot;
typedef struct {s16 count,limit,active;u8 pad[6];Slot *slots;} Pool;
typedef struct {u8 pad[0xe0];void *archive;u8 pad2[12];void *model;u8 pad3[12];Pool *pool;} W;
extern int sub_020238F8(void *);extern void *FldObjSys_ReadMModelFromNarc(W *,u32,int);extern void ov01_021FA61C(W *,u32,void *,u32);
int ov01_021FA470(W *,u32,u32,u32);
int ov01_021FA470(W *w,u32 a,u32 b,u32 c){Pool *p=w->pool;if(p->active>=p->limit||sub_020238F8(w->archive)==1){int i;Slot *s=p->slots;for(i=0;i<p->count;i++,s++)if(!s->used){s->used=w->model;s->c=c;s->b=a;s->a=b;return 0;}GF_AssertFail();return 0;}else{void *model=FldObjSys_ReadMModelFromNarc(w,b,0);ov01_021FA61C(w,a,model,c);p->active++;return 1;}}
