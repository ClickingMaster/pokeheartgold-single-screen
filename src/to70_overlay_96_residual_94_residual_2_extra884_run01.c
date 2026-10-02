#pragma force_active on
#include "global.h"
typedef struct{void *a,*b;u16 count;u8 p[6];u32 low:4;u32 field:8;u32 part:8;u32 p2:7;u32 active:1;u32 high:4;} Work;
extern void ManagedSprite_SetAnim(void *,int),ManagedSprite_SetPositionXY(void *,int,int),ManagedSprite_SetDrawFlag(void *,int),ov96_0220AFF8(Work *),ov96_0220B068(Work *);
void ov96_0220AED4(Work *);
void ov96_0220AED4(Work *w){if(!w)GF_AssertFail();w->count=100;w->field=0;w->part=1;w->active=1;ManagedSprite_SetAnim(w->a,1);ManagedSprite_SetPositionXY(w->a,120,0);ManagedSprite_SetDrawFlag(w->a,1);ov96_0220AFF8(w);ov96_0220B068(w);ManagedSprite_SetDrawFlag(w->b,1);}
