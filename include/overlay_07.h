#ifndef POKEHEARTGOLD_OVERLAY_07_H
#define POKEHEARTGOLD_OVERLAY_07_H

#include "battle/battle.h"

void UnkBallData_SetBallAnimation(UnkBallData *data, s32 ballAnim);
BOOL UnkBallData_IsBallAnimationPlaying(UnkBallData *data, s32 ballAnim_Unused); // IsBallAnimationPlaying?
UnkBallData *UnkBallData_New(UnkStruct_134 *unkStruct_134);                      // unkBallData_Init?
void UnkBallData_Delete(UnkBallData *data);                                      // unkBallData_Destroy?
s32 UnkBallData_GetMode(UnkBallData *data);

// src/battle/battle_anim_system.c: the move animation system

typedef struct BattleAnimSystem BattleAnimSystem;
typedef struct BattleAnimBattlerSprites BattleAnimBattlerSprites;
struct MoveAnimData;

// The battle background's graphics (BG3), restored after a move background
typedef struct BattleAnimBgGraphics {
    NarcId narcId;
    int charMember;
    int paletteMember;
    int screenMember;
    int palettePos;
    int paletteCount;
} BattleAnimBgGraphics;

// What the battle passes to BattleAnimSystem_StartMove besides the move packet
typedef struct BattleAnimSetup {
    SpriteSystem *spriteSystem;
    BgConfig *bgConfig;
    PaletteData *paletteData;
    UnkBattleSystemSub1D0 *picData[4];
    u8 battlerTypes[4];
    Pokepic *pokepics[4];
    u32 battleType;
    u16 species[4];
    u8 genders[4];
    u8 shiny[4];
    u8 forms[4];
    u32 personalities[4];
    u32 moveEffectFlags[4];
    NarcId scriptNarc;
    BattleAnimBgGraphics battleBg;
    SOUND_CHATOT *chatotCry;
    u8 *battleBgTiles;
    u16 *battleBgPalette;
} BattleAnimSetup; // size: 0x90

BattleAnimSystem *BattleAnimSystem_New(enum HeapID heapId);
BOOL BattleAnimSystem_Delete(BattleAnimSystem *system);
BOOL BattleAnimSystem_StartMove(BattleAnimSystem *system, struct MoveAnimData *animData, u16 move, BattleAnimSetup *setup);
BOOL BattleAnimSystem_Execute(BattleAnimSystem *system);
BOOL BattleAnimSystem_IsActive(BattleAnimSystem *system);
BOOL BattleAnimSystem_EndMove(BattleAnimSystem *system);
enum HeapID BattleAnimSystem_GetHeapID(BattleAnimSystem *system);
BattleAnimBattlerSprites *BattleAnimBattlerSprites_New(BattleSystem *battleSystem, enum HeapID heapId, int battler);
BattleAnimBattlerSprites *BattleAnimBattlerSprites_NewAll(BattleSystem *battleSystem, enum HeapID heapId);
void BattleAnimBattlerSprites_Free(BattleAnimBattlerSprites *sprites);
void BattleAnimBattlerSprites_Free2(BattleAnimBattlerSprites *sprites, int battler);

#endif // POKEHEARTGOLD_OVERLAY_07_H
