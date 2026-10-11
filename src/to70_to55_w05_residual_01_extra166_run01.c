#pragma force_active on
#include "global.h"
#pragma force_active on
void ManagedSprite_GetPositionXY(void *,s16 *,s16 *);
void BattleAnimUtil_InitLinearMotion(void *,int,int,int,int,int);
void BattleAnimUtil_InitScaleMotionXY(void *,int,int,int,int,int,int);
void BattleAnimUtil_GetScaleFloat(void *,float *,float *);
void ManagedSprite_SetAffineScale(void *,float,float);
void BattleAnimUtil_UpdateLinearMotionSprite(void *,void *);
int BattleAnimSystem_GetDefender(void *),BattleAnimUtil_GetBattlerType(void *,int);
void ManagedSprite_SetDrawFlag(void *,int);
void ov07_0222F408(void *,void *);
extern const u16 ov07_02236800[];
void ov07_0222F600(u8 *);
#define P(p,o) (*(void **)((p)+(o)))
void ov07_0222F600(u8 *w){
 s16 x,y;
 float sx,sy;
 ManagedSprite_GetPositionXY(P(w,28),&x,&y);
 BattleAnimUtil_InitLinearMotion(w+0x70,x,x,(s16)(y-64),y,8);
 BattleAnimUtil_InitScaleMotionXY(w+0x28,2,10,20,10,10,8);
 BattleAnimUtil_GetScaleFloat(w+0x28,&sx,&sy);
 if(*(int *)(w+0xcc)==1)sx=0.0f-sx;
 ManagedSprite_SetAffineScale(P(w,28),sx,sy);
 BattleAnimUtil_UpdateLinearMotionSprite(w+0x70,P(w,28));
 *(int *)(w+0x94)=0;*(int *)(w+0x98)=6;
 ManagedSprite_SetDrawFlag(P(w,36),ov07_02236800[BattleAnimUtil_GetBattlerType(P(w,0),BattleAnimSystem_GetDefender(P(w,0)))]);
 ManagedSprite_SetDrawFlag(P(w,28),1);ov07_0222F408(w,P(w,28));
}
