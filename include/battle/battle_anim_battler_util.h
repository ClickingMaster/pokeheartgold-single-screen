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
void BattleAnimUtil_GetEmitterPositionKind0(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind1(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind2(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind3(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind4(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind5(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind6(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind7(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind8(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind9(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind10(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetEmitterPositionKind11(int battlerType, VecFx32 *pos, int unused, int perspective);
void BattleAnimUtil_GetBattlerEmitterPositionKind0(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind1(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind2(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind3(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind4(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind5(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind6(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind7(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind8(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind9(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind10(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetBattlerEmitterPositionKind11(BattleAnimSystem *system, int battler, VecFx32 *pos);
void BattleAnimUtil_GetEmitterHighPosition(VecFx32 *pos);
void BattleAnimUtil_GetEmitterPositionKind0Alt(int battlerType, VecFx32 *pos, int unused, int perspective);
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
SpriteManager *BattleAnimSystem_GetFirstSpriteManager(BattleAnimSystem *system);
SpriteSystem *BattleAnimSystem_GetSpriteSystem(BattleAnimSystem *system);
u8 BattleAnimSystem_GetBattlerType(BattleAnimSystem *system, int battler);
Pokepic *BattleAnimSystem_GetBattlerPokepic(BattleAnimSystem *system, int battler);
PaletteData *BattleAnimSystem_GetPaletteData(BattleAnimSystem *system);
BOOL BattleAnimSystem_IsDoubleBattle(BattleAnimSystem *system);
u32 BattleAnimSystem_GetBgMaskLayer(BattleAnimSystem *system, int index);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_ANIM_BATTLER_UTIL_H
