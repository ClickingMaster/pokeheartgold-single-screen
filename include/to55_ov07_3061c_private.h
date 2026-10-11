#ifndef TO55_SOL_R3_W10_PRIVATE_H
#define TO55_SOL_R3_W10_PRIVATE_H

#include "global.h"

typedef struct Ov07W10Motion {
    u8 raw[0x24];
} Ov07W10Motion;

typedef struct SpriteSystem SpriteSystem;
typedef struct ManagedSprite ManagedSprite;

typedef struct Ov07W10Small {
    void *owner;
    void *arg;
    SpriteSystem *spriteSystem;
    int state;
    ManagedSprite *sprite;
    Ov07W10Motion motion;
    u32 pad38;
} Ov07W10Small;

typedef struct Ov07W10Large {
    void *owner;
    int state;
    s16 x;
    s16 y;
    s16 field0C;
    s16 field0E;
    void *pokepic;
    u8 motion[0x24];
    u8 anim[0x24];
    int field5C;
    int field60;
    int field64;
    int field68;
    int field6C;
    int timer;
    int field74;
} Ov07W10Large;

void *BattleAnimUtil_AllocWork(void *, u32);
void BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
void BattleAnimUtil_StartBlendAlphaMotion(void *, int, int, int, int, int);
BOOL BattleAnimUtil_IsBlendAlphaMotionDone(void *);
BOOL ManagedSprite_IsAnimated(void *);
void ManagedSprite_SetAnimateFlag(void *, BOOL);
void ManagedSprite_SetOamMode(void *, int);
void Sprite_DeleteAndFreeResources(void *);
void SpriteSystem_DrawSprites(void *);
void Heap_Free(void *);
void BattleAnimUtil_InitScaleMotionXY(void *, s32, s32, s32, s32, s32, s32);
void BattleAnimUtil_UpdateShakePokepic(void *, s16, s16, void *);
BOOL BattleAnimUtil_UpdateScaleMotionXYPokepic(void *, void *);
void BattleAnimUtil_AlignPokepicToScale(void *, s16, s16, int, int);
void Pokepic_SetAttr(void *, int, int);
u32 BattleAnimSystem_GetDefender(void *);
u32 ov07_0221FAA0(void *, u32);
void *BattleAnimSystem_GetBattlerPokepic(void *, u32);
u32 Pokepic_GetAttr(void *, int);
void BattleAnimUtil_InitShake(void *, int, int, int, int);

void ov07_0223061C(void *, Ov07W10Small *);
void ov07_022306D8(void *, void *, SpriteSystem *, ManagedSprite *);
void ov07_02230714(void *, Ov07W10Large *);
void ov07_022308E4(void *);

#endif
