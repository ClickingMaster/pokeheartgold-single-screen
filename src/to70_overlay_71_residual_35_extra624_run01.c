#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void *parent,*task,*a,*b;int x,y,z;u8 p[0x28-0x1c];int matrix[3];s16 rot[3];u8 p2[2];int vx,vy,ax,ay;u8 p3[0x5c-0x4c];int speed;u8 p4[8];int mode,step,ticks,spare;s16 angle1,angle2;}Work;
typedef struct{u8 p[0x28];void*a,*b;}Parent;
extern void TradeSequence_SetModelRotation(void*,s16*),ov71_0224A694(Work*,int,int,int,int),ov71_0224A6D8(Work*),TradeSequence_SetModelPosition(void*,int*),TradeSequence_GetModelRotation(void*,s16*),ov71_0224A7D0(void*,void*);
extern void *SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);

void ov71_0224A5C4(Parent*p,Work*w);

void ov71_0224A5C4(Parent*p,Work*w){w->parent=p;w->b=p->b;w->a=p->a;w->x=0;w->y=0x3d6c00;w->z=(int)0xffe44000;w->angle1=-10128;w->angle2=-2736;w->vx=(int)0xfffef2c6;w->vy=0x936d;w->step=0;w->ticks=0;w->spare=0;w->rot[0]=8192;w->rot[1]=-8192;w->rot[2]=0;TradeSequence_SetModelRotation(w->b,w->rot);w->speed=512;w->ax=2048;w->ay=2048;*(int*)((u8*)w+0x68)=0;ov71_0224A694(w,74<<10,99<<10,4096,12);ov71_0224A6D8(w);TradeSequence_SetModelPosition(w->b,w->matrix);TradeSequence_GetModelRotation(w->b,w->rot);w->task=SysTask_CreateOnMainQueue(ov71_0224A7D0,w,0);}
