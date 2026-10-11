#pragma force_active on
#include "global.h"
typedef struct{u32 a;void *ctx,*pic;s16 x,y;u8 p[0x44];volatile int mode;} Work;
extern Work *BattleAnimUtil_AllocWork(void *,int);extern int BattleAnimSystem_GetAttacker(void *),BattleAnimSystem_GetDefender(void *),ov07_02222004(void *,int),Pokepic_GetAttr(void *,int);
extern u32 BattleAnimSystem_GetArg(void *p, int i);
extern void *BattleAnimSystem_GetBattlerPokepic(void *,int);extern void Pokepic_SetAttr(void *,int,int),ov07_0222E3A4(Work *,int),BattleAnimSystem_CreateTaskDefaultPriority(void *,void (*)(void),Work *),ov07_0222E4F8(void);
void ov07_0222E54C(void *);
void ov07_0222E54C(void *ctx){Work *w=BattleAnimUtil_AllocWork(ctx,88);int side;w->ctx=ctx;side=ov07_02222004(w->ctx,BattleAnimSystem_GetAttacker(ctx));if(((s32)BattleAnimSystem_GetArg(ctx, 0))==0){w->pic=BattleAnimSystem_GetBattlerPokepic(w->ctx,BattleAnimSystem_GetAttacker(ctx));w->x=Pokepic_GetAttr(w->pic,0);w->y=Pokepic_GetAttr(w->pic,1);}else{w->pic=BattleAnimSystem_GetBattlerPokepic(w->ctx,BattleAnimSystem_GetDefender(ctx));w->x=Pokepic_GetAttr(w->pic,0);w->y=Pokepic_GetAttr(w->pic,1);}Pokepic_SetAttr(w->pic,11,50);{int mode;switch(side){case 1:mode=0;break;default:mode=1;break;}w->mode=mode;}ov07_0222E3A4(w,w->mode);BattleAnimSystem_CreateTaskDefaultPriority(w->ctx,ov07_0222E4F8,w);}
