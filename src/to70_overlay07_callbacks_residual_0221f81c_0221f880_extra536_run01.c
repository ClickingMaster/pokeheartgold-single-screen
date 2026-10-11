#pragma force_active on
#include "global.h"
typedef struct {u32 heap;u8 pad[0x18-4];u32*pc;u8 q[0x8d-0x1c];u8 busy;u8 r[0xc0-0x8e];void*data;}Work;
typedef struct {u8 pad[20];u16 index;u8 q[0xd8-22];u16 species[8];u8 form[8];u8 r[0x10c-0xf0];void*arg;u8 s[8];signed int flags:1;unsigned int unused:31;}Data;
extern int IsCryFinished(void);
extern void StopCry(u32),sub_02071FDC(void*,u32,u16,u8,int,u32,int,u32);
extern int ov07_0221F8C8(Work*,s8);
void ov07_0221F81C(Work*),ov07_0221F880(Work*);
void ov07_0221F81C(Work*w){u32 mode,param;int v;Data*d;w->pc++;mode=*w->pc;w->pc++;v=ov07_0221F8C8(w,(s8)(s16)*w->pc);w->pc++;param=*w->pc;w->pc++;d=w->data;sub_02071FDC(d->arg,mode,d->species[d->index],d->form[d->index],v,param,d->flags,w->heap);}
void ov07_0221F880(Work*w){if(IsCryFinished()==0){u32 arg;w->pc++;arg=*w->pc;w->pc++;w->busy=0;StopCry(arg);}else w->busy=1;}
