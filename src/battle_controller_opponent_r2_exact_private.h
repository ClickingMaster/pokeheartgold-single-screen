#ifndef TO50_BATTLE_CONTROLLER_OPPONENT_R2_PRIVATE_H
#define TO50_BATTLE_CONTROLLER_OPPONENT_R2_PRIVATE_H

#include "global.h"

#include "constants/heap.h"

#include "battle/battle_system.h"

#include "heap.h"
#include "move.h"
#include "sprite_system.h"
#include "sys_task_api.h"

void ov12_02261E40(SysTask *task, s32 *data);

typedef struct BattleAnimWork {
    u8 pad00[0xC];
    ManagedSprite *sprite;
    u8 pad10[6];
    u16 active : 1;
    u16 flagsRest : 15;
} BattleAnimWork;

typedef struct BattleSourceR2 {
    u8 pad00[0x18];
    u16 values18[4];
    u8 values20[4];
    u8 values24[4];
    u8 values28[4];
    u32 values2C[4];
} BattleSourceR2;

typedef struct BattleParamsR2 {
    u32 value0;
    u32 value4;
    UnkBattleSystemSub1D0 *battlerValues[4];
    void *objects[4];
    u16 values28[4];
    u8 values30[4];
    u8 values34[4];
    u8 values38[4];
    u32 values3C[4];
    u8 mapping[4];
} BattleParamsR2;

typedef struct BattleSourceR1 {
    u16 unk0;
    u16 unk2;
    u8 pad04[0x14];
    u16 values18[4];
    u8 values20[4];
    u8 values24[4];
    u8 values28[4];
    u32 values2C[4];
    u32 values3C[4];
    u32 mode4C;
    u32 value50;
} BattleSourceR1;

typedef struct BattleParamsR1 {
    SpriteSystem *spriteSystem;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    UnkBattleSystemSub1D0 *battlerValues[4];
    u8 mapping[4];
    void *objects[4];
    u32 battleType;
    u16 values34[4];
    u8 values3C[4];
    u8 values40[4];
    u8 values44[4];
    u32 values48[4];
    u32 values58[4];
    u32 value68;
    u32 value6C;
    u32 value70;
    u32 value74;
    u32 value78;
    u32 value7C;
    u32 value80;
    SOUND_CHATOT *chatot;
    u8 *bad0;
    u16 *bad8;
} BattleParamsR1;

void ov12_02261B80(BattleSystem *bsys, u8 *ctx, void *arg2, BattleSourceR1 *src);
void ov12_02261CA8(BattleSystem *bsys, BattleSourceR2 *src, BattleParamsR2 *dst, u32 value);
void ov12_02261D30(u8 *outA, u8 *outB, BOOL useKind, int kind, u16 move);
BOOL ov12_02261DC8(BattleAnimWork *work, ManagedSprite *unused);

extern void ov07_0221C01C(void *arg2, BattleSourceR1 *src, u16 selected, BattleParamsR1 *params);

#endif
