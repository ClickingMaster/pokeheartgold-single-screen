#pragma force_active on
#include "global.h"
#include "w19_particle_motion_owner.h"

extern u32 BattleAnimSystem_GetArg(void *p, int i);
extern void *BattleAnimSystem_GetEmitter(void*,int);
extern void *BattleAnimSystem_GetCurrentParticleSystem(void*); extern u16 BattleAnimSystem_GetAttacker(void*),BattleAnimSystem_GetDefender(void*);
void ov07_0222600C(void *p,Ov07Work104 *w);
void ov07_0222600C(void *p,Ov07Work104 *w){u32 v;w->arg0=BattleAnimSystem_GetArg(p, 0);w->argC=BattleAnimSystem_GetArg(p, 1);w->argE=BattleAnimSystem_GetArg(p, 2);w->delay=BattleAnimSystem_GetArg(p, 3);w->arg6=BattleAnimSystem_GetArg(p, 4);w->arg8=BattleAnimSystem_GetArg(p, 5);w->arg10=BattleAnimSystem_GetArg(p, 6);w->wave=BattleAnimSystem_GetArg(p, 8);v=BattleAnimSystem_GetArg(p, 7);w->unk1C=0;w->unk20=(v&0xffff0000)>>16;w->unk24=(u16)v;if(w->unk20==0)w->unk20=0;if(w->unk24==0)w->unk24=255;w->node=BattleAnimSystem_GetEmitter(p,w->arg0);w->resource=BattleAnimSystem_GetCurrentParticleSystem(p);if(w->arg10==0){w->handleA=BattleAnimSystem_GetAttacker(p);w->handleB=BattleAnimSystem_GetDefender(p);}else if(w->arg10==1){w->handleA=BattleAnimSystem_GetDefender(p);w->handleB=BattleAnimSystem_GetAttacker(p);}if(w->node==0)GF_AssertFail();}
