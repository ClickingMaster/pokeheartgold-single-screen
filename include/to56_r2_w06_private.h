#ifndef TO56_R2_W06_PRIVATE_H
#define TO56_R2_W06_PRIVATE_H
#include "global.h"
typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteSystem SpriteSystem;
typedef struct Ov07R2Work Ov07R2Work;
void ov07_02226CB0(Ov07R2Work *);
void ov07_02226F04(void *, Ov07R2Work *);
void *BattleAnimUtil_AllocWork(void *, u32);
void BattleAnimUtil_InitCommonWork(void *, void *);

extern u32 BattleAnimSystem_GetArg(void *p, int i);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(void *, void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void SpriteSystem_DrawSprites(SpriteSystem *);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_TickFrame(ManagedSprite *);
void Heap_Free(void *);
void ManagedSprite_SetPriority(ManagedSprite *, int);
void ManagedSprite_SetDrawPriority(ManagedSprite *, int);
int BattleAnimSystem_IsDoubleBattle(void *);
int BattleAnimSystem_GetBgPriority(void *, int);
int BattleAnimSystem_GetAttacker(void *);
int BattleAnimSystem_GetDefender(void *);
int BattleAnimUtil_GetBattlerType(void *, int);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void (*)(void *, Ov07R2Work *), Ov07R2Work *);
void ov07_02226F80(void *);
#endif
