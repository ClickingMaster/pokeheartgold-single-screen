#pragma force_active on
#include "global.h"
extern u8 ov86_021E81DC[];typedef struct{u8 gap[0x24c];u8 a:1,b:7;u8 c:4,d:4;u8 e,f,x,y,w,h;}Work;
void ov86_021E7688(Work*w,int index);
static inline void initFlags(Work*w){w->a=1;w->b=2;w->c=2;w->d=0;}

#pragma force_active on
#pragma opt_lifetimes off
void ov86_021E7688(Work*w,int index){u32 v=index;initFlags(w);v=ov86_021E81DC[v];w->x=((v%7)*32+24)/8;w->y=((v/7)*32+40)/8+32;w->w=2;w->h=2;w->e=0;w->f=0;}
