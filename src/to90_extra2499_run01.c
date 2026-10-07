#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *main;u32 pad4;void *shadow;fx32 angle;u8 pad10,state,time;u8 pad13[9];s16 x,y;u8 tail20[0x10];u32 data[1];} Anim;
extern void ManagedSprite_SetDrawFlag(void*,int),ManagedSprite_GetPositionXYWithSubscreenOffset(void*,s16*,s16*,fx32),ManagedSprite_SetPositionXYWithSubscreenOffset(void*,s16,s16,fx32),ManagedSprite_SetAffineScale(void*,float,float),ov93_02261528(Anim*,int),ov93_0226249C(void*,Anim*,void*);
extern fx32 GF_SinDegFX32(fx32);
int ov93_02261554(void*,u32,Anim*);
int ov93_02261554(void *context,u32 unused,Anim *a){struct{s16 y,x;} position;position.x=a->x;position.y=a->y;int offset;switch(a->state){case 0:ManagedSprite_SetDrawFlag(a->main,1);ManagedSprite_SetDrawFlag(a->shadow,1);ManagedSprite_GetPositionXYWithSubscreenOffset(a->main,&position.x,&position.y,0x160000);a->x=position.x;a->y=position.y;a->state++;
case 1:switch(a->angle){case 0:{s16 delta=a->y;u8 time=a->time;delta+=32;delta=(s16)(time*delta/15);position.y=delta-32;}break;case 0x5a000:{s16 delta=a->x;u8 time=a->time;delta-=288;delta=(s16)(time*delta/15);position.x=delta+288;}break;case 0xb4000:{s16 delta=a->y;u8 time=a->time;delta-=228;delta=(s16)(time*delta/15);position.y=delta+228;}break;case 0x10e000:{s16 delta=a->x;u8 time=a->time;delta+=32;delta=(s16)(time*delta/15);position.x=delta-32;}break;default:GF_AssertFail();break;}
 if(a->time>=15){ManagedSprite_SetPositionXYWithSubscreenOffset(a->main,a->x,a->y,0x160000);ManagedSprite_SetPositionXYWithSubscreenOffset(a->shadow,a->x,(s16)(a->y+24),0x160000);ov93_02261528(a,2);ov93_0226249C(context,a,a->data);return 1;}
 offset= -((fx32)(((s64)GF_SinDegFX32(((180*a->time)<<12)/15)*0x18000+0x800)>>12))/4096;
 ManagedSprite_SetPositionXYWithSubscreenOffset(a->main,position.x,(s16)(position.y+offset),0x160000);ManagedSprite_SetPositionXYWithSubscreenOffset(a->shadow,position.x,(s16)(position.y+24),0x160000);{float scale=(float)(4096-((-offset/3)*4096)/24)/4096.0f;ManagedSprite_SetAffineScale(a->shadow,scale,scale);}break;
default:break;}a->time++;return 1;}
