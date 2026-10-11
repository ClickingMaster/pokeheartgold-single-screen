#pragma force_active on
#include "global.h"
#pragma force_active on
struct A{u8 pad[12];void*pic;u8 anim[20];int z;u8 pad28[12];int delay,state,x,y;};
int BattleAnimUtil_UpdateScaleMotionPokepic(void*,void*);void BattleAnimUtil_AlignPokepicToScale(void*,s16,s16,int,int);void BattleAnimUtil_InitScaleMotion(void*,int,int,int,int);void Pokepic_SetAttr(void*,int,int);int ov07_0222E5E8(struct A*);
struct B{int a;void*context;void*pic;s16 x,y;u8 pad[0x54-16];volatile int side;};void*BattleAnimUtil_AllocWork(void*,int);int BattleAnimSystem_GetAttacker(void*);int ov07_02222004(void*,int);
extern u32 BattleAnimSystem_GetArg(void *p, int i);void*BattleAnimSystem_GetBattlerPokepic(void*,int);int Pokepic_GetAttr(void*,int);int BattleAnimSystem_GetDefender(void*);void ov07_0222E3A4(struct B*,int);void ov07_0222E4F8(void);void BattleAnimSystem_CreateTaskDefaultPriority(void*,void(*)(void),void*);void ov07_0222E54C(void*);
int ov07_0222E5E8(struct A*s){int done=0;switch(s->state){case 0:if(BattleAnimUtil_UpdateScaleMotionPokepic(s->anim,s->pic))BattleAnimUtil_AlignPokepicToScale(s->pic,(s16)s->x,(s16)s->y,s->z,0);else s->state++;break;case 1:if(--s->delay<0){s->state++;BattleAnimUtil_InitScaleMotion(s->anim,15,10,10,8);}break;case 2:if(BattleAnimUtil_UpdateScaleMotionPokepic(s->anim,s->pic))BattleAnimUtil_AlignPokepicToScale(s->pic,(s16)s->x,(s16)s->y,s->z,0);else s->state++;break;case 3:Pokepic_SetAttr(s->pic,12,256);Pokepic_SetAttr(s->pic,13,256);done=1;break;}return done;}
