#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{s16 id;u16 a;int step,direction;};struct B{void*bg;};void ScheduleSetBgPosText(void*,int,int,int);void ov43_0222DAE8(struct S*,void*,struct B*,int,int,void*);void ov43_0222AD20(struct B*,int);void ov43_0222DB94(struct S*,struct B*,int);int ov43_0222D508(struct S*,void*,struct B*,void*);
int ov43_0222D508(struct S*s,void*x,struct B*b,void*y){int pos;if(s->step>4){ScheduleSetBgPosText(b->bg,2,0,0);ScheduleSetBgPosText(b->bg,3,0,0);ov43_0222DAE8(s,x,b,0,s->id,y);ov43_0222AD20(b,1);ov43_0222DB94(s,b,255);return 1;}pos=(s->step<<8)/4;if(s->direction==2)pos=-pos;ScheduleSetBgPosText(b->bg,3,0,pos);s->step++;return 0;}
