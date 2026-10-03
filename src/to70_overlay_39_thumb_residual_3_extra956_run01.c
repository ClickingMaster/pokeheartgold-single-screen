#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void*inner;u8 p[0x3e8-4];int timer;u8 p2[0x411-0x3ec];u8 state;}Work;
extern int ov39_02227DB8(Work*);
extern void ov39_022271A4(void*,void*,void*),ov39_02227060(void*,void*,void*);

int ov39_022273B0(Work*w,void*a,void*b);

int ov39_022273B0(Work*w,void*a,void*b){if(ov39_02227DB8(w)==0)return 0;ov39_022271A4(w->inner,a,(u8*)w+0x190);ov39_02227060(w->inner,b,(u8*)w+0x210);w->state=60;w->timer=20000;return 1;}
