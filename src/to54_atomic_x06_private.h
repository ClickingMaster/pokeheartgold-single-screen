#ifndef TO54_X06_PRIVATE_H
#define TO54_X06_PRIVATE_H
#include "global.h"
void BattleAnimUtil_InitValueMotionFx(void *, s32, u32, s32);
BOOL BattleAnimUtil_UpdateValueMotionFx(void *);
void BattleAnimUtil_InitLinearMotion(void *, s32, s32, s32, s32, s32);
void ov07_0222E3A4(void *, u32);
BOOL ov07_0222E3E8(void *);
BOOL BattleAnimUtil_UpdateLinearMotionPokepic(void *, void *);
void Pokepic_SetAttr(void *, int, int);
void *BattleAnimUtil_AllocWork(void *, u32);
void *BattleAnimSystem_GetDefender(void *);
void *BattleAnimSystem_GetBattlerPokepic(void *, void *);
void ManagedSprite_SetDrawPriority(void *, u32);
void ManagedSprite_SetPriority(void *, u32);
void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void ov07_0222E108(void *, void *);
void ov07_0222E020(void *, void *, void *, void *);
void GF_AssertFail(void);
void ov07_0222E07C(void *);
void ov07_0222DE30(void *);
#endif
