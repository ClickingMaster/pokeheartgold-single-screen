#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x1312];u8 value,input;u8 p2[0x1358-0x1314];u8 active,state,kind,ticks,a,b,x,y;u16 count,timer;}Work;

void ov72_0223AED0(Work*w,int x,int y,int value);

void ov72_0223AED0(Work*w,int x,int y,int value){w->active=1;w->ticks=0;w->state=0;w->kind=5;w->count=1;w->timer=0;w->x=x;w->y=y;w->a=6;w->b=6;w->input=value;w->value=10;}
