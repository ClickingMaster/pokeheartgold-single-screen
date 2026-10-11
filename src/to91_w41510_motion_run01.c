#include "global.h"
#include "sprite_system.h"
#pragma force_active on
#include "to91_ov07_motion_owner.h"
typedef Ov07MotionRow Motion;
typedef Ov07MotionWork Work;
typedef char MotionSize[(sizeof(Motion)==36)?1:-1];
typedef char WorkSize[(sizeof(Work)==308)?1:-1];
int BattleAnimUtil_UpdateSinMotion(struct S*);
BOOL ov07_0221FAE8(void*);
void ov07_0222FC44(void*);
void ov07_0222FC44(void*data){int i=0;Work*w=data;Motion*motion=w->motions;unsigned char*recordBytes=(unsigned char*)w;unsigned char*spriteBytes=(unsigned char*)w;
for(;i<6;recordBytes+=36,motion++,spriteBytes+=4,i++){s32 offset,sine,amplitude;BattleAnimUtil_UpdateSinMotion(&motion->interpolation);sine=FX_SinIdx((u16)(((struct S*)(recordBytes+0x30))->a*5u));amplitude=(s32)(40960u*(u32)w->amplitude);offset=(s32)(((s64)sine*amplitude+2048)>>12);ManagedSprite_SetPositionXY(*(ManagedSprite**)(spriteBytes+0x18),(s16)(w->x+((struct S*)(recordBytes+0x30))->x),(s16)(w->y+(s16)(offset>>12)));if((s32)((struct S*)(recordBytes+0x30))->a>=16383&&(s32)((struct S*)(recordBytes+0x30))->a<=48969)ManagedSprite_SetPriority(*(ManagedSprite**)(spriteBytes+0x18),1);else ManagedSprite_SetPriority(*(ManagedSprite**)(spriteBytes+0x18),ov07_0221FAE8(w->owner)+1);}}
