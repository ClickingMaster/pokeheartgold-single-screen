#ifndef POKEHEARTGOLD_BATTLE_BATTLE_ANIM_BATTLER_UTIL_H
#define POKEHEARTGOLD_BATTLE_BATTLE_ANIM_BATTLER_UTIL_H

// Overlay 7: battle animation helpers shared by the effect functions (battler types and positions, emitter
// positions, blend/window setup, target collection, work allocation).

#include "bg_window.h"
#include "overlay_07.h"
#include "palette.h"
#include "pokepic.h"
#include "spl.h"
#include "sprite_system.h"

// BattleAnimUtil_GetBattlerSide
#define BATTLE_ANIM_SIDE_PLAYER 3
#define BATTLE_ANIM_SIDE_ENEMY  4

typedef struct BattleAnimPosition {
    s16 x;
    s16 y;
} BattleAnimPosition;

// Common head of the effect work structs (BattleAnimUtil_InitCommonWork)
typedef struct BattleAnimCommonWork {
    u8 state[4];
    BattleAnimSystem *system;
    SpriteSystem *spriteSystem;
    SpriteManager *battlerSpriteManager;
    SpriteManager *spriteManager;
    BgConfig *bgConfig;
    PaletteData *paletteData;
} BattleAnimCommonWork; // size: 0x1C

// One target of an effect (BattleAnimUtil_CollectPicsByFlags / CollectSpritesByFlags)
typedef struct BattleAnimTarget {
    u32 unk_00;
    s16 pos[2];
    Pokepic *pokepic;
    ManagedSprite *sprite;
    int battler;
} BattleAnimTarget; // size: 0x14

int BattleAnimUtil_GetBattlerType(BattleAnimSystem *system, int battler);
int BattleAnimUtil_GetBattlerSide(BattleAnimSystem *system, int battler);
int BattleAnimUtil_FindBattlerByType(BattleAnimSystem *system, int battlerType);
int BattleAnimUtil_GetPartnerBattler(BattleAnimSystem *system, int battler);
int BattleAnimUtil_GetOpposingBattlerType(int battlerType);
void BattleAnimUtil_GetBattlerTypePosition(int battlerType, int unused, BattleAnimPosition *pos);
void BattleAnimUtil_GetBattlerPosition(BattleAnimSystem *system, int battler, BattleAnimPosition *pos);
void BattleAnimUtil_GetEmitterPosition(int battlerType, VecFx32 *pos, int unused, int perspective, int kind);
void ov07_02231AD0(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231AE0(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231AF0(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B00(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B10(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B20(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B30(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B40(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B50(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B60(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B70(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B80(int battlerType, VecFx32 *pos, int unused, int perspective);
void ov07_02231B90(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231BC0(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231BF0(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231C20(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231C50(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231C80(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231CB0(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231CE0(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231D10(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231D40(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231D70(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231DA0(BattleAnimSystem *system, int battler, VecFx32 *pos);
void ov07_02231DD0(VecFx32 *pos);
void ov07_02231DE0(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetCameraUp(BOOL flipped, VecFx32 *up);
void BattleAnimUtil_SetBlendWithBattleBg(BattleAnimSystem *system, int eva, int evb);
void BattleAnimUtil_SetBlendWithMask(BattleAnimSystem *system, int plane2, int eva, int evb);
void BattleAnimUtil_SetBlendBgOverBattleBg(BattleAnimSystem *system, int eva, int evb);
void BattleAnimUtil_SetWindowInsidePlanes(BattleAnimSystem *system, int window, BOOL effect);
void BattleAnimUtil_GetPicPosition(Pokepic *pokepic, s16 *pos);
void BattleAnimUtil_GetSpritePosition(ManagedSprite *sprite, s16 *pos);
void BattleAnimUtil_InitCommonWork(BattleAnimSystem *system, BattleAnimCommonWork *work);
void BattleAnimUtil_CollectPicsByFlags(BattleAnimSystem *system, u32 flags, BattleAnimTarget *targets, int *count);
void BattleAnimUtil_CollectSpritesByFlags(BattleAnimSystem *system, u32 flags, BattleAnimTarget *targets, int *count);
void *BattleAnimUtil_AllocWork(BattleAnimSystem *system, u32 size);
void BattleAnimUtil_FreeWork(void *work);

// overlay 7 functions defined in sources not merged yet (switch to battle/battle_anim_internal.h when it lands)
int ov07_0221BFC0(BattleAnimSystem *system);
enum HeapID BattleAnimSystem_GetHeapID(BattleAnimSystem *system);
u16 BattleAnimSystem_GetAttacker(BattleAnimSystem *system);
u16 BattleAnimSystem_GetDefender(BattleAnimSystem *system);
SPLEmitter *BattleAnimSystem_GetCurrentParticleSystem(BattleAnimSystem *system);
BgConfig *BattleAnimSystem_GetBgConfig(BattleAnimSystem *system);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(BattleAnimSystem *system, int index);
SpriteManager *BattleAnimSystem_GetBattlerSpriteManager(BattleAnimSystem *system);
SpriteManager *ov07_0221C528(BattleAnimSystem *system);
SpriteSystem *BattleAnimSystem_GetSpriteSystem(BattleAnimSystem *system);
u8 BattleAnimSystem_GetBattlerType(BattleAnimSystem *system, int battler);
Pokepic *BattleAnimSystem_GetBattlerPokepic(BattleAnimSystem *system, int battler);
PaletteData *BattleAnimSystem_GetPaletteData(BattleAnimSystem *system);
BOOL BattleAnimSystem_IsDoubleBattle(BattleAnimSystem *system);
u32 ov07_0221FAF8(BattleAnimSystem *system, int index);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_BATTLER_UTIL_H
