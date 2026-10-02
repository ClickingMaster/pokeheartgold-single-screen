#pragma force_active on
#include "global.h"
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void*,int,int,int),ManagedSprite_SetDrawFlag(void*,int),ManagedSprite_OffsetPositionXY(void*,int,int);
typedef struct {void*s;s16 state;volatile s16 timer;int unused,x,y,a,b;} Work;
int ov93_02262374(void *unused,Work*w);
int ov93_02262374(void *unused,Work*w){switch(w->state){case 0:case 5:ManagedSprite_SetPositionXYWithSubscreenOffset(w->s,128,16,22<<16);ManagedSprite_SetDrawFlag(w->s,1);w->state++;break;case 1:case 6:w->timer++;if(w->timer>15){w->timer=0;w->a=1;w->b=1;w->x=128;w->y=32;w->state++;}break;case 2:case 7:w->b=0;w->y+=6;ManagedSprite_OffsetPositionXY(w->s,0,6);if(w->y>96)w->state++;break;case 3:case 8:w->a=0;ManagedSprite_SetDrawFlag(w->s,0);w->state++;case 4:case 9:w->timer++;if(w->timer>30){w->timer=0;w->state++;}break;case 10:return 1;}return 0;}
