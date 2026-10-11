#ifndef TO57_R2_03_PRIVATE_H
#define TO57_R2_03_PRIVATE_H
#include "global.h"
void *BattleAnimUtil_AllocWork(void *, int);
int BattleAnimSystem_GetAttacker(void *);
void *BattleAnimSystem_GetBattlerPokepic(void *, int);
int Pokepic_GetAttr(void *, int);
void BattleAnimSystem_GetSpriteTemplate(void *, void *);
void *SpriteSystem_NewSprite(void *, void *, void *);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetOamMode(void *, int);
void ManagedSprite_SetAnimateFlag(void *, int);
int ov07_02222004(void *, int);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
void BattleAnimUtil_StartBlendAlphaMotion(void *, int, int, int, int, int);
int BattleAnimUtil_IsBlendAlphaMotionDone(void *);
void Sprite_DeleteAndFreeResources(void *);
void Heap_Free(void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void SpriteSystem_DrawSprites(void *);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void ManagedSprite_SetPriority(void *, int);
int ov07_0221FAE8(void *);
void ov07_022220FC(void *, int, int, int, int, int, int, int);
int BattleAnimUtil_UpdateSinMotion(void *);
#endif
