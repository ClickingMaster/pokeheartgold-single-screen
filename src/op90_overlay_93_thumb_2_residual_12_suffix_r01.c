#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "math_util.h"
typedef struct {u8 prefix[0xf0];u8 position,unused,remaining,elapsed,mode;} Cycle;
typedef struct{s32 x,y;}FixedPoint2;
typedef struct {ManagedSprite *sprite,*effect,*shadow;s32 angle;u8 unused,state,timer,half,animation,pad[3];s32 duration,spare;FixedPoint2 delta,position;} Motion;
typedef struct {s16 y,x;} Coordinates;
typedef struct {u8 value,pad[3];} TimingValue;
extern const u16 ov93_02262C7A[];
extern const TimingValue ov93_02262CC4[],ov93_02262CC5[],ov93_02262CC6[];
extern BOOL PlaySE(u16);
extern void ov93_02261528(void *,u8);
static inline void InitializeInterpolation(Motion *motion,fx32 sine,fx32 cosine,s16 positionX,s16 positionY,int duration)
{
  motion->delta.x=(s32)((u32)((s16)((s32)(((s64)sine*76+0x800)>>12)+128)-positionX)<<12)/duration;
  motion->delta.y=(s32)((u32)((s16)(98-(s32)(((s64)cosine*68+0x800)>>12))-positionY)<<12)/duration;
  motion->position.x=(s32)((u32)positionX<<12);motion->position.y=(s32)((u32)positionY<<12);
  motion->timer=duration;motion->duration=duration;motion->state=(u8)(motion->state+1);
}
int ov93_022618C4(void*,Cycle*,Motion*);
// @fn ov93_022618C4
int ov93_022618C4(void *unused,Cycle *cycle,Motion *motion)
{
 s16 originX,originY,positionX,positionY;
 switch(motion->state){
 case 0:
  PlaySE(1427);ManagedSprite_GetPositionXYWithSubscreenOffset(motion->sprite,&originX,&originY,0x160000);
  ManagedSprite_SetPositionXYWithSubscreenOffset(motion->effect,originX,originY-32,0x160000);
  ManagedSprite_SetAnim(motion->effect,33);ManagedSprite_SetDrawFlag(motion->effect,1);
  ManagedSprite_SetAnim(motion->sprite,ov93_02262C7A[motion->animation]+2);
  motion->timer=ov93_02262CC6[cycle->mode].value;motion->half=ov93_02262CC6[cycle->mode].value/2;motion->state=(u8)(motion->state+1);break;
 case 1:
  if(motion->timer==motion->half){ManagedSprite_SetAnim(motion->sprite,ov93_02262C7A[motion->animation]+1);ManagedSprite_SetDrawFlag(motion->effect,0);ManagedSprite_TickNFrames(motion->effect,0x4000);}
  if(motion->timer==0){ManagedSprite_SetDrawFlag(motion->effect,0);ManagedSprite_SetAnim(motion->sprite,ov93_02262C7A[motion->animation]);motion->state=(u8)(motion->state+1);}else motion->timer--;break;
 case 2:{
  int duration,start,value=0; s32 angle,sine,cosine;
  if(cycle->remaining==0){int period=ov93_02262CC4[cycle->mode].value;value+=period-cycle->elapsed;value+=ov93_02262CC5[cycle->mode].value;duration=value+period;start=cycle->position+2;}
  else{value+=cycle->remaining;if(cycle->position>=12){duration=value+ov93_02262CC4[cycle->mode+1].value;start=1;}else{duration=value+ov93_02262CC4[cycle->mode].value;start=cycle->position+1;}}
  start+=motion->angle/0x1e000;start%=12;angle=start*0x1e000;sine=GF_SinDegFX32(angle);cosine=GF_CosDegFX32(angle);
  ManagedSprite_GetPositionXYWithSubscreenOffset(motion->sprite,&positionX,&positionY,0x160000);positionY+=24;
  InitializeInterpolation(motion,sine,cosine,positionX,positionY,duration);
 }
 case 3:{
  s64 height;
  motion->position.x=(s32)((u32)motion->position.x+(u32)motion->delta.x);motion->position.y=(s32)((u32)motion->position.y+(u32)motion->delta.y);
  height=(s64)GF_SinDegFX32(((motion->timer*180)<<12)/motion->duration)*0xc000;
  ManagedSprite_SetPositionXYWithSubscreenOffset(motion->sprite,(s16)(motion->position.x/4096),(s16)(motion->position.y/4096-24+(-(s32)((height+0x800)>>12))/4096),0x160000);
  ManagedSprite_TickNFrames(motion->sprite,0x2c000/motion->duration);
  ManagedSprite_SetPositionXYWithSubscreenOffset(motion->shadow,(s16)(motion->position.x/4096),(s16)(motion->position.y/4096),0x160000);
  motion->timer--;if(motion->timer==0){ManagedSprite_SetAnim(motion->sprite,ov93_02262C7A[motion->animation]);ManagedSprite_SetAnimationFrame(motion->sprite,0);ov93_02261528(motion,2);return 1;}break;
 }
 }
 return 1;
}
