#ifndef TO54_W11_PRIVATE_H
#define TO54_W11_PRIVATE_H
#include "global.h"
#include "overlay07_linear_motion_private.h"
#include "sys_task.h"
#include "overlay07_script_slot_private.h"

typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteSystem SpriteSystem;
typedef struct Pokepic Pokepic;

void *BattleAnimUtil_AllocWork(void *, u32);
void BattleAnimUtil_InitCommonWork(void *, void *);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void BattleAnimSystem_GetSpriteTemplate(void *, void *);
void ov07_0222A8D8(ManagedSprite *);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_SetFlipMode(ManagedSprite *, int);
void ManagedSprite_OffsetPositionXY(ManagedSprite *, int, int);
void Sprite_DeleteAndFreeResources(ManagedSprite *);
void SpriteSystem_DrawSprites(SpriteSystem *);
void Heap_Free(void *);
ManagedSprite *SpriteSystem_NewSprite(void *, void *, void *);

void BattleAnimUtil_FreeWork(void *);
BOOL BattleAnimUtil_UpdateScaleMotionXY(void *);
void BattleAnimUtil_GetScaleFloat(void *, s32 *, s32 *);
void BattleAnimUtil_AlignSpriteToScale(ManagedSprite *, s32, s32, u32, u32);
void ManagedSprite_SetAffineScale(ManagedSprite *, s32, s32);
void ManagedSprite_TickFrame(ManagedSprite *);
void ManagedSprite_SetAffineOverwriteMode(ManagedSprite *, int);
void ManagedSprite_SetPriority(ManagedSprite *, int);
void ManagedSprite_SetDrawPriority(ManagedSprite *, int);
void BattleAnimUtil_InitScaleMotionXY(void *, int, int, int, int, int, int);
u32 BattleAnimSystem_GetDefender(void *);
Pokepic *BattleAnimSystem_GetBattlerPokepic(void *, u32);
u32 ov07_0221FAA0(void *, u32);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(void *, int);
int ov07_0221FAE8(void *);
u32 BattleAnimSystem_GetBattlerType(void *, u32);
u32 Pokepic_GetAttr(Pokepic *, int);
void BattleAnimUtil_InitLinearMotion(W02LinearMotion *, s16, s16, s16, s16, u16);
BOOL BattleAnimUtil_UpdateLinearMotion(W02LinearMotion *);
void Pokepic_SetAttr(Pokepic *, int, int);
void BattleAnimSystem_BuildBattlerPicParams(void *, void *, int);
void *BattleAnimSystem_GetHeapID(void *);
void BattleAnimUtil_RestorePic(void *, void *);
void BattleAnimUtil_LoadMonIntoPicOppositeFacing(void *, void *);
u32 BattleAnimSystem_GetAttacker(void *);
void BattleAnimUtil_GetBattlerPosition(void *, u32, s16 *);
u32 BattleAnimUtil_GetBattlerSide(void *, u32);

extern u32 BattleAnimSystem_GetArg(void *p, int i);
void BattleAnimUtil_CollectPicsByFlags(void *, u32, Ov07T10Slot *, int *);
#endif
