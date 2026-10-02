#pragma force_active on
#include "global.h"
#pragma force_active on
struct Pair{int a,b;};extern const struct Pair ov93_02262CF0[];extern const int ov93_02262FD4[][3];void ov93_0225FD8C(int,int,int,int*,int*);
struct S{int active,col,value,pad,jitter;u8 gap[20];int toggle;};u32 LCRandom(void);void ScheduleSetBgAffineScale(void*,int,int,int);void ScheduleSetBgPosText(void*,int,int,int);void ov93_0225FCA4(void*,struct S*,int);
void ov93_0225FCA4(void*bg,struct S*s,int row){int x,y,dx,dy,jitter,invy,invx;if(s->active){ov93_0225FD8C(row,s->col,s->value,&x,&y);if(x==4096)dx=0;else if(x<4096)dx=128-((x<<7)/4096);else dx=-(((x-4096)<<7)/4096);if(y==4096)dy=0;else if(y<4096)dy=128-((y<<7)/4096);else dy=-(((y-4096)<<7)/4096);if(s->jitter==0)jitter=0;else{jitter=(LCRandom()&7)+1;if(s->toggle&1)jitter=-jitter;s->toggle^=1;}invx=FX_Inv(x);invy=FX_Inv(y);ScheduleSetBgAffineScale(bg,7,3,invx);ScheduleSetBgAffineScale(bg,7,6,invy);ScheduleSetBgPosText(bg,7,0,jitter-dx);ScheduleSetBgPosText(bg,7,3,39-dy);}}
void ov93_0225FD8C(int row,int col,int value,int*x,int*y){int q=ov93_02262CF0[col].a*value/ov93_02262FD4[row][col];int scale=q+768;*x=scale;*y=scale;if(scale>4096)*x+= (int)(((s64)(q-3328)*4506+2048)>>12);}
