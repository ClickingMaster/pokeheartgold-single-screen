#pragma force_active on
#include "global.h"
typedef struct{void*sprite;int unknown;u16 angle;u8 pad0[0xa];s16 x,y;int radiusX,radiusY;u8 pathX[0x18],pathY[0x18];u8 tail[0x50];}S;
typedef struct{S a,b;u8 pad[4];void*managerB,*managerA;u8 pad1[0x1c];int state;u8 pad2[0x10];void*g;}W;
typedef struct{u8 pad[0x34];u8 flag;}G;
extern int IsPaletteFadeFinished(void),GF_SinDeg(u16),GF_CosDeg(u16),ov92_022619C4(void*),ov92_022620D0(S*,void*,int,int),ov92_02262018(S*,void*,int,int),ov92_02261E88(S*,void*,int,int),ov92_02261F60(S*,void*,int,int);
extern void SysTask_Destroy(void*),ManagedSprite_SetDrawFlag(void*,int),ManagedSprite_GetPositionFxXYWithSubscreenOffset(void*,int*,int*,int),ManagedSprite_SetPositionFxXYWithSubscreenOffset(void*,int,int,int),ov92_02260860(void*,int,int,int),ov92_02261E80(S*);
void ov92_02261BA0(void*,W*);
#define DONE() ov92_02261E80(&w->b);ov92_02261E80(&w->a);w->state++
void ov92_02261BA0(void*task,W*w){int x,y;int ax,ay,bx,by,a,b;if(!IsPaletteFadeFinished()||((G*)w->g)->flag==1){SysTask_Destroy(task);return;}
switch(w->state){case 0:w->a.angle=0;w->a.x=128;w->a.y=160;w->a.radiusX=64;w->a.radiusY=24;ManagedSprite_SetDrawFlag(w->a.sprite,1);w->b.angle=0;w->b.x=128;w->b.y=160;w->b.radiusX=64;w->b.radiusY=24;ManagedSprite_SetDrawFlag(w->b.sprite,1);
ManagedSprite_GetPositionFxXYWithSubscreenOffset(w->a.sprite,&x,&y,1<<20);ax=(w->a.x<<12)+GF_SinDeg(w->a.angle)*w->a.radiusX;ay=(w->a.y<<12)+GF_CosDeg(w->a.angle)*w->a.radiusY;ManagedSprite_SetPositionFxXYWithSubscreenOffset(w->a.sprite,ax,ay,1<<20);ov92_02260860(w->a.pathX,ax,ax+(45<<14),ov92_022619C4(w->managerA));ov92_02260860(w->a.pathY,ay,ay-(6<<14),ov92_022619C4(w->managerA));
ManagedSprite_GetPositionFxXYWithSubscreenOffset(w->b.sprite,&x,&y,1<<20);bx=(w->b.x<<12)+GF_SinDeg(w->b.angle)*w->b.radiusX;by=(w->b.y<<12)-GF_CosDeg(w->b.angle)*w->b.radiusY;ManagedSprite_SetPositionFxXYWithSubscreenOffset(w->b.sprite,bx,by,1<<20);ov92_02260860(w->b.pathX,bx,bx-(45<<14),ov92_022619C4(w->managerB));ov92_02260860(w->b.pathY,by,by+(6<<14),ov92_022619C4(w->managerB));DONE();
case 1:a=ov92_022620D0(&w->b,w->managerB,0,1);b=ov92_02262018(&w->a,w->managerA,1,3);if(a&&b){DONE();}return;
case 2:a=ov92_02261E88(&w->b,w->managerB,0,3);b=ov92_02261E88(&w->a,w->managerA,1,2);if(a&&b){DONE();}return;
case 3:a=ov92_02261F60(&w->b,w->managerB,0,2);b=ov92_02262018(&w->a,w->managerA,1,3);if(a&&b){DONE();}return;
case 4:a=ov92_022620D0(&w->b,w->managerB,0,1);b=ov92_02261E88(&w->a,w->managerA,1,2);if(a&&b){DONE();}return;
default:SysTask_Destroy(task);break;}}
