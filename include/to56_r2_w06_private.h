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

extern u32 ov07_0221C4A8(void *p, int i);
ManagedSprite *ov07_0221C4E8(void *, void *);
void ov07_0221C448(void *, void *);
void SpriteSystem_DrawSprites(SpriteSystem *);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_TickFrame(ManagedSprite *);
void Heap_Free(void *);
void ManagedSprite_SetPriority(ManagedSprite *, int);
void ManagedSprite_SetDrawPriority(ManagedSprite *, int);
int ov07_0221FAB0(void *);
int ov07_0221FB04(void *, int);
int ov07_0221C468(void *);
int ov07_0221C470(void *);
int BattleAnimUtil_GetBattlerType(void *, int);
void ov07_0221C410(void *, void (*)(void *, Ov07R2Work *), Ov07R2Work *);
void ov07_02226F80(void *);
#endif
