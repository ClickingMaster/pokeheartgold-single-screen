#pragma force_active on
#include "global.h"
typedef struct {u16 a,t; int b,x,y,z,limit;} Work;
int ov91_0225E6F8(Work *w);
void ov91_0225E728(Work*w,int*out);
int ov91_0225E6F8(Work *w){int v=w->limit-(int)(((s64)(w->t<<12)*0x670+0x800)>>12);if(v<0)v=0;return v;}
void ov91_0225E728(Work*w,int*out){int v=ov91_0225E6F8(w);out[0]=(int)(((s64)w->x*v+0x800)>>12);out[1]=(int)(((s64)w->y*v+0x800)>>12);out[1]+=(int)(((s64)(w->t<<12)*-0x1334+0x800)>>12);out[2]=(int)(((s64)w->z*v+0x800)>>12);}
