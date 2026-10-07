#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x113e];u8 state,timer,index;union {u8 raw;struct {u8 active:4,other:4;} bits;} flags;u8 pad2[10];u8 arg;} Work;
extern void ov08_022247E4(Work*,u8,int,u8),ov08_022248D0(Work*,u8,int);
extern void ov08_02224858(void*,u8,int);
void ov08_02224974(Work*);
void ov08_02224974(Work *w){if(w->flags.bits.active){switch(w->state){case 0:ov08_022247E4(w,w->index,1,w->arg);ov08_02224858(w,w->index,1);ov08_022248D0(w,w->index,1);w->timer=0;w->state=1;break;case 1:ov08_022247E4(w,w->index,2,w->arg);ov08_02224858(w,w->index,2);ov08_022248D0(w,w->index,2);w->timer=0;w->state=2;break;case 2:ov08_022247E4(w,w->index,0,w->arg);ov08_02224858(w,w->index,0);ov08_022248D0(w,w->index,0);w->timer=0;w->state=0;w->flags.bits.active=0;break;}}}
