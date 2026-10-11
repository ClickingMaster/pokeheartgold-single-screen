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
void ov07_0221C410(void *, void *, void *);
void ov07_0221C448(void *, void *);
void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
void ov07_02222AC4(void *, int, int, int, int, int);
BOOL ov07_02222AF4(void *);
BOOL ManagedSprite_IsAnimated(void *);
void ManagedSprite_SetAnimateFlag(void *, BOOL);
void ManagedSprite_SetOamMode(void *, int);
void Sprite_DeleteAndFreeResources(void *);
void SpriteSystem_DrawSprites(void *);
void Heap_Free(void *);
void ov07_02222590(void *, s32, s32, s32, s32, s32, s32);
void ov07_0222283C(void *, s16, s16, void *);
BOOL ov07_02222788(void *, void *);
void ov07_022226C4(void *, s16, s16, int, int);
void Pokepic_SetAttr(void *, int, int);
u32 ov07_0221C470(void *);
u32 ov07_0221FAA0(void *, u32);
void *ov07_0221FA48(void *, u32);
u32 Pokepic_GetAttr(void *, int);
void ov07_022227A8(void *, int, int, int, int);

void ov07_0223061C(void *, Ov07W10Small *);
void ov07_022306D8(void *, void *, SpriteSystem *, ManagedSprite *);
void ov07_02230714(void *, Ov07W10Large *);
void ov07_022308E4(void *);

#endif
