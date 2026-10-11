#pragma force_active on
#include "global.h"
#pragma force_active on
u8 *BattleAnimUtil_AllocWork(void *,int);
void BattleAnimUtil_InitCommonWork(void *,void *);
int BattleAnimSystem_GetDefender(void *);
int ov07_02221F80(void *,int,int);
int BattleAnimSystem_GetAttacker(void *);
int BattleAnimUtil_GetBattlerSide(void *,int);
int ov07_0221FAE8(void *);
void ManagedSprite_SetPositionXY(void *,int,int);
void ManagedSprite_SetAnim(void *,int);
void ManagedSprite_SetPriority(void *,int);
void ManagedSprite_OffsetPositionXY(void *,int,int);
void BattleAnimSystem_CreateTaskDefaultPriority(void *,void (*)(void *,void *),void *);
void ov07_02226880(void *,void *);
void ov07_022269D8(void *,int,int,void *);
#define P(p,o) (*(void **)((p)+(o)))
void ov07_022269D8(void *ctx,int unused1,int unused2,void *sprite){
 u8 *w=BattleAnimUtil_AllocWork(ctx,56);
 int index,x,y;
 BattleAnimUtil_InitCommonWork(ctx,w);P(w,28)=sprite;*(int *)(w+52)=0;
 index=BattleAnimSystem_GetDefender(ctx);x=ov07_02221F80(ctx,index,0);y=ov07_02221F80(ctx,index,1);
 ManagedSprite_SetPositionXY(P(w,28),x,y);
 if(BattleAnimUtil_GetBattlerSide(ctx,BattleAnimSystem_GetAttacker(ctx))==4)ManagedSprite_SetAnim(P(w,28),1);
 else ManagedSprite_SetAnim(P(w,28),0);
 ManagedSprite_SetPriority(P(w,28),ov07_0221FAE8(P(w,4)));
 ManagedSprite_OffsetPositionXY(P(w,28),0,12);
 BattleAnimSystem_CreateTaskDefaultPriority(P(w,4),ov07_02226880,w);
}
