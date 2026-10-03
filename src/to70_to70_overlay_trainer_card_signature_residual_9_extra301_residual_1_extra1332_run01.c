#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct{u8 p[0x5ca0];u16 a;s8 dx,dy;u32 b;int x,y;} Work;
extern void ov52_021E9424(void *);
void ov52_021E9364(Work *);
void ov52_021E9364(Work *w){int diff;if(gSystem.touchX!=65535&&gSystem.touchY!=65535&&w->x!=65535&&w->y!=65535){if(w->x>gSystem.touchX){diff=w->x-gSystem.touchX;w->dx=-1;}else{diff=gSystem.touchX-w->x;w->dx=1;}if(diff>=3&&diff<=40){if(w->y>gSystem.touchY){diff=w->y-gSystem.touchY;w->dy=-1;}else{diff=gSystem.touchY-w->y;w->dy=1;}if(diff<=40)ov52_021E9424(&w->a);}else if(diff<=40){if(w->y>gSystem.touchY){diff=w->y-gSystem.touchY;w->dy=-1;}else{diff=gSystem.touchY-w->y;w->dy=1;}if(diff>=3&&diff<=40)ov52_021E9424(&w->a);}}w->x=gSystem.touchX;w->y=gSystem.touchY;}
