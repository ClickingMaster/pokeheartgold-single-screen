#pragma force_active on
#include "global.h"
typedef struct{s16 id,index;int target,x,y,dx,dy;union{u32 flags;struct{u32 dirx:1;u32 diry:1;u32 rest:30;};};}Motion;
typedef struct{u8 pad[0xc];Motion*m;u8 pad2[0x318];void*sprite;}Sub;
typedef struct{u8 pad[0x34];Sub*sub;}Work;
extern void ManagedSprite_GetPositionXY(void*,s16*,s16*),PCBox_GetSlotPosition(int,s16*,s16*,int);
void PCBox_StartHeldItemMove(Work*,int,int,int,int);
void PCBox_StartHeldItemMove(Work*w,int index,int id,int target,int mode){struct{s16 y,x;}old;struct{s16 y,x;}dest;Motion*m=w->sub->m;int x,y,delta,destX,destY;ManagedSprite_GetPositionXY(w->sub->sprite,&old.x,&old.y);PCBox_GetSlotPosition(id,&dest.x,&dest.y,target);if(mode==1){dest.x+=8;dest.y+=8;}else dest.y+=4;m->id=id;m->index=index;m->flags&=3;m->target=target;destX=dest.x;x=old.x;if(x>destX){m->dirx=1;delta=x-destX;}else{m->dirx=0;delta=destX-x;}m->dx=(delta<<8)/8;destY=dest.y;y=old.y;if(y>destY){m->flags|=2;delta=y-destY;}else{m->flags&=~2;delta=destY-y;}m->dy=(delta<<8)/8;m->x=x<<8;m->y=y<<8;}
