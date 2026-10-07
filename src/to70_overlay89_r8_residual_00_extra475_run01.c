#pragma force_active on
#include "global.h"
#include "overlay89_r8_private.h"
typedef struct {u8 pad[0x240];u32 lo:8;u32 current:8;u32 pending:8;u32 top:8;} Entity;
typedef struct {u8 a[0xb0];int task;Entity*entities[128];u8 b[0x73c-0x2b4];u8 state,active;} Work;
extern int (*ov89_0225CD10[])(Work*,Work*,void*);

extern void ov89_0225BE84(void*,void*);extern void ov89_0225AC24(Work*,Entity*,int);

void ov89_0225A4F4(void*,void*,u32,u32);

void ov89_0225A4F4(void*opaque,void*ctx,u32 phase,u32 column){Work*w=opaque;int i;if(w->active)ov89_0225BE84(w,&w->task);if(w->state){if(ov89_0225CD10[w->state](w,w,ctx)==1)w->state=0;}for(i=0;i<128;i++){Entity*e=w->entities[i];if(e){if(e->current==0 && e->pending!=0)e->current=e->pending;if(ov89_0225CDB0[e->current].update((Ov89Work *)w,(Ov89Object *)e)==1)ov89_0225AC24(w,e,i);}}}
