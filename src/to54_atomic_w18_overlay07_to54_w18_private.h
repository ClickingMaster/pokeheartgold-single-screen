#ifndef TO54_W18_PRIVATE_H
#define TO54_W18_PRIVATE_H

#include "global.h"

typedef void *Handle;

void *BattleAnimUtil_AllocWork(void *, u32);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void (*)(void *, void *), void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void *BattleAnimSystem_GetAttacker(void *);
void *BattleAnimSystem_GetDefender(void *);
void *BattleAnimSystem_GetBattlerPokepic(void *, void *);
void *ov07_0221FAA0(void *, void *);
void *ov07_02222004(void *, void *);
void BattleAnimUtil_SetPokepicPosWithOffset(void *, void *, s16, s16);
BOOL BattleAnimUtil_UpdateSinMotionPokepic(void *, s16, s16, void *);
void BattleAnimUtil_InitLinearMotion(void *, s16, s16, int, int, int);
BOOL BattleAnimUtil_UpdateLinearMotion(void *);
void BattleAnimUtil_InitScaleMotion(void *, s16, int, s16, int);
BOOL BattleAnimUtil_UpdateScaleMotionSprite(void *, void *);
void ov07_02222A44(void *, int, int);
void BattleAnimUtil_StartBlendAlphaMotion(void *, int, int, int, int, int);
BOOL BattleAnimUtil_IsBlendAlphaMotionDone(void *);
void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);

void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ManagedSprite_SetAffineOverwriteMode(void *, int);
void ManagedSprite_SetAffineScale(void *, float, float);
void ManagedSprite_SetOamMode(void *, int);
void ManagedSprite_SetAnimationFrame(void *, int);
void ManagedSprite_SetAnim(void *, int);
void ManagedSprite_SetAnimateFlag(void *, BOOL);
void ManagedSprite_SetDrawFlag(void *, BOOL);
BOOL ManagedSprite_IsAnimated(void *);
int Pokepic_GetAttr(void *, int);
void Pokepic_SetAttr(void *, int, int);
void SpriteSystem_DrawSprites(void *);
void Sprite_DeleteAndFreeResources(void *);
void StartBrightnessTransition(int, int, int, int, int);
BOOL IsBrightnessTransitionActive(int);
void Heap_Free(void *);

#endif
