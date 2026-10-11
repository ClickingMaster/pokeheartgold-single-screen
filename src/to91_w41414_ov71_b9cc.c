#include "global.h"
#include "heap.h"
#include <string.h>
#include "to91_ov71_bounce_private.h"
#include "overlay71_polygon_attr_r9_private.h"
extern void TradeSequence_SetModelVisible(Ov71Cam *,void *);
extern void TradeSequence_GetModelPosition(Ov71Cam *,Ov71Vec3 *);
extern void TradeSequence_GetModelRotation(Ov71Cam *,Ov71S16Vec3 *);
#pragma force_active on
static inline void InitJob(Ov71BounceJob*j,void*w,SysTask**out){u32 zero=0;j->state=zero;j->out=out;j->pic=*(void *const *)((const u8*)w+0x74);j->sprite=*(void *const *)((const u8*)w+0x78);j->object=w;j->angle=zero;j->delta=0x18cc;j->count=20;j->scale=0x1000;j->frames=zero;}
void ov71_0224B9CC(void *w,SysTask **out){Ov71BounceJob *j=Heap_Alloc((enum HeapID)57,0x44);if(j){InitJob(j,w,out);TradeSequence_SetModelAlpha(j->sprite,0);TradeSequence_SetModelVisible(j->sprite,(void *)1);TradeSequence_GetModelPosition(j->sprite,&j->pos);TradeSequence_GetModelRotation(j->sprite,&j->rotation);*out=SysTask_CreateOnMainQueue(ov71_0224BABC,j,0);if(*out==NULL)Heap_Free(j);}else *out=NULL;}
