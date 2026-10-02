#pragma force_active on
#include "global.h"
#include "sys_task_api.h"
#pragma force_active on
typedef struct Parent{u8 p[0x28];void*a,*b;}Parent;typedef struct W{Parent*owner;SysTask*task;void*a,*b;VecFx32 pos;u8 p1c[12];VecFx32 vec;s16 angles[3];u8 p3a[2];s32 scale[2],x,y;u8 p4c[8];s32 factor;u8 p58[4];u32 tick,zero[3];s16 values[2];}W;extern void ov71_022476EC(void*,s16*),ov71_02249A98(W*),ov71_022476C4(void*,VecFx32*),ov71_022476D4(void*,s16*),ov71_02249B64(SysTask*,void*);void ov71_022499A4(Parent*,W*);
void ov71_022499A4(Parent*p,W*w){w->owner=p;w->b=p->b;w->a=p->a;w->pos.x=0;w->pos.y=0xfff50e00;w->pos.z=0xe2000;w->values[0]= -10128;w->values[1]= -2736;w->scale[0]=0x134c4;w->scale[1]=0xffff4d56;w->zero[0]=0;w->zero[1]=0;w->zero[2]=0;w->angles[0]=0x2000;w->angles[1]= -8192;w->angles[2]=0;ov71_022476EC(w->b,w->angles);w->factor=4096;w->x=0x12800;w->y=0x18c00;w->tick=0;ov71_02249A98(w);ov71_022476C4(w->b,&w->vec);ov71_022476D4(w->b,w->angles);w->task=SysTask_CreateOnMainQueue(ov71_02249B64,w,0);}
