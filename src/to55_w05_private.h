#ifndef TO55_W05_PRIVATE_H
#define TO55_W05_PRIVATE_H
#include "global.h"
void BattleAnimUtil_InitScaleMotionXY(void *, int, int, int, int, int, int);
void BattleAnimUtil_InitScaleMotion(void *, int, int, int, int);
int BattleAnimSystem_GetAttacker(void *);
void BattleAnimSystem_GetDefender(void *);
int BattleAnimUtil_GetBattlerType(void *, int);
void ManagedSprite_SetDrawFlag(void *, int);
void ManagedSprite_SetAffineScale(void *, float, float);
void BattleAnimUtil_GetScaleFloat(void *, float *, float *);
void ov07_0222F408(void *, void *);
void BattleAnimUtil_InitSpriteSequence(void *, void *, void *, int, int, int, int, int, int, int, int, int);
int BattleAnimUtil_UpdateScaleMotion(void *);
void Sprite_DeleteAndFreeResources(void *);
void SpriteSystem_DrawSprites(void *);
void Heap_Free(void *);
void BattleAnimSystem_DestroyTask(void *, void *);
int ov07_0222FFD0(void *);
void BattleAnimUtil_InitSinMotion(void *, int, int, int, int, int, int, int);
void BattleAnimUtil_UpdateSinMotion(void *);
void ov07_0222F434(void *);
void ov07_0222F9B8(void *);
void ov07_0222FA08(void *);
void ov07_0222FBEC(void *, int);
void ov07_02230058(void *, void *);
void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
void BattleAnimUtil_InitLinearMotion(void *, int, int, int, int, int);
void ov07_0222FEB0(void *, void *, int, int);
int BattleAnimUtil_UpdateSpriteSequence(void *);
void BattleAnimUtil_InitShake(void *, int, int, int, int);
int BattleAnimUtil_UpdateShakePokepic(void *, int, int, int);
void ov07_0222FA64(void *, void *);
int BattleAnimUtil_UpdateLinearMotionSprite(void *, void *);
int BattleAnimUtil_UpdateScaleMotionXY(void *);
int ov07_0222F6C0(void *);
extern u16 ov07_02236800[];
void BattleAnimUtil_AlignSpriteToScale(void *, int, int, int, int);
int ov07_0222F4D4(void *);
#endif
