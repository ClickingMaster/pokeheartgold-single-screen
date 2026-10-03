#pragma force_active on
#include "global.h"
typedef struct {u32 pad[3];void *sprite;u8 pad2[0xd2];s8 count;u8 pad3;u32 lo:8,state:4,hi:20;} W;
typedef struct {u32 a:17,flag:2,b:13;} Flags;
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void*,s16,s16,int),ManagedSprite_SetDrawFlag(void*,int),ManagedSprite_GetPositionXY(void*,s16*,s16*),ov96_022193CC(int,void*);
void ov96_02216934(W*,s16*,int,int,Flags*,int,void*);
void ov96_02216934(W*w,s16*pos,int unused2,int unused3,Flags*flag,int enable,void*arg){s16 x,y;switch(w->state){case 0:ManagedSprite_SetPositionXYWithSubscreenOffset(w->sprite,pos[0],pos[1]-400,0x1e0000);ManagedSprite_SetDrawFlag(w->sprite,1);w->count=0;ov96_022193CC(2229,arg);w->state++;break;case 1:ManagedSprite_GetPositionXY(w->sprite,&x,&y);y+=40;ManagedSprite_SetPositionXYWithSubscreenOffset(w->sprite,x,y,0x1e0000);w->count++;if(w->count>=10)w->state++;break;case 2:if(enable)flag->flag=2;w->state++;break;case 3:break;default:GF_AssertFail();break;}}
