#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x240];u32 lo:8;u32 current:8;u32 pending:8;u32 top:8;} Entity;
typedef struct {u8 a[0xb0];int task;Entity*entities[128];u8 b[0x73c-0x2b4];u8 state,active;} Work;
extern int (*ov89_0225CD10[])(Work*,Work*,void*);
typedef struct{int (*fn)(Work*,Entity*);int x,y;} Entry;extern Entry ov89_0225CDB0[];
extern void ov89_0225BE84(void*,void*);extern void ov89_0225AC24(Work*,Entity*,int);

void ov89_0225A4F4(Work*w,void*ctx);

void ov89_0225A4F4(Work*w,void*ctx){int i;if(w->active)ov89_0225BE84(w,&w->task);if(w->state){if(ov89_0225CD10[w->state](w,w,ctx)==1)w->state=0;}for(i=0;i<128;i++){Entity*e=w->entities[i];if(e){if(e->current==0 && e->pending!=0)e->current=e->pending;if(ov89_0225CDB0[e->current].fn(w,e)==1)ov89_0225AC24(w,e,i);}}}
