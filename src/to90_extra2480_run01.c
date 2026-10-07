#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x10];u16 state,count;u8 q[0x2f0-0x14];void*bg;u8 r[44];void*first;u32 unused;void*second;}W31716;typedef struct{u8 p[0x34];W31716*w;}W31716Owner;
extern void sub_02019978(void*,int),ov14_021F391C(void*,int),ov14_021F29E4(void*,int,int),ov14_021F2A18(void*,int,int),ManagedSprite_GetPositionXY(void*,s16*,s16*),ManagedSprite_SetPositionXY(void*,s16,s16);extern int ov14_021F2A04(void*,int);
int ov14_021EAA04(W31716Owner*);
int ov14_021EAA04(W31716Owner*o){W31716*w=o->w;struct{s16 y,x;}pos;sub_02019978(w->bg,15);switch(w->state){case 0:ov14_021F391C(o->w,1);ov14_021F29E4(o->w,11,1);ov14_021F2A18(o->w,11,1);w->state++;break;case 1:if(!ov14_021F2A04(o->w,11)){ov14_021F391C(o->w,0);w->state++;}break;case 2:ov14_021F29E4(o->w,9,9);w->state++;case 3:if(w->count==4){w->count=0;w->state++;}else{ManagedSprite_GetPositionXY(o->w->first,&pos.x,&pos.y);ManagedSprite_SetPositionXY(o->w->first,pos.x,(s16)(pos.y+2));w->count++;}break;case 4:ov14_021F29E4(o->w,9,10);w->state++;case 5:if(w->count==4){w->count=0;w->state=0;return 0;}ManagedSprite_GetPositionXY(o->w->first,&pos.x,&pos.y);ManagedSprite_SetPositionXY(o->w->first,pos.x,(s16)(pos.y-2));ManagedSprite_GetPositionXY(o->w->second,&pos.x,&pos.y);ManagedSprite_SetPositionXY(o->w->second,pos.x,(s16)(pos.y-2));w->count++;break;default:break;}return 1;}
