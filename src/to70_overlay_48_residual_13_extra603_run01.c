#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct{int result,state,x,y,a,b,timer;} Work;
extern void ov48_02258CE4(int,int,int *,int *,int *,int *);
void ov48_02259DA0(Work *);
void ov48_02259DA0(Work *w){int x1,y1,x2,y2;w->result=0;if(gSystem.touchNew){if(gSystem.touchX>=200&&gSystem.touchX<=248&&gSystem.touchY>=168&&gSystem.touchY<=184){w->result=2;return;}w->state=0;w->a=0;w->b=0;w->timer=0;w->result=0;w->x=gSystem.touchX;w->y=gSystem.touchY;w->timer=4;}if(gSystem.touchHeld){switch(w->state){case 0:if(!w->timer)w->state++;else w->timer--;case 1:ov48_02258CE4(w->x,w->y,&x1,&y1,&x2,&y2);w->result=x1|x2;w->a=y1;w->b=y2;w->x=gSystem.touchX;w->y=gSystem.touchY;break;}}else{if(w->timer)w->result=1;w->state=0;w->a=0;w->b=0;w->timer=0;}}
