#pragma force_active on
#include "global.h"
typedef struct{u8 p;u8 mode;u8 pad[0x14a];void *obj;} Work;
extern void ov49_0225B99C(Work *,int,int),ov49_0225BBD0(Work *,int,void *,int,int,int,void *,int,int,int,int,int,int,int);
void ov49_0225B944(Work *,int,int,int,int,int,int,int,int,int,int,int);
void ov49_0225B944(Work *w,int a,int b,int c,int d,int e,int f,int g,int h,int i,int j,int k){if(w->mode==2)ov49_0225B99C(w,a,b);ov49_0225BBD0(w,a,(u8*)w+8,b,c,d,w->obj,e,f,g,h,i,j,k);w->mode=2;}
