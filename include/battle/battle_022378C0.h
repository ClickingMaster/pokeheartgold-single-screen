#ifndef POKEHEARTGOLD_BATTLE_022378C0_H
#define POKEHEARTGOLD_BATTLE_022378C0_H

#include "battle.h"
#include "battle_setup.h"
#include "overlay_manager.h"

BOOL Battle_Run(OverlayManager *man, int *state);
void ov12_02237B0C(BattleSystem *battleSystem);
void ov12_02237B6C(BattleSystem *battleSystem);
void ov12_02237BB8(BattleSystem *battleSystem);
void ov12_02237CC4(BattleSystem *battleSystem);
void ov12_02237D00(BattleSystem *battleSystem);

// src/battle/battle_main.c
void Battle_LinkInit(OverlayManager *man, BattleSetup *setup);
BOOL Battle_LinkMain(OverlayManager *man);
void Battle_LinkExit(OverlayManager *man);
BOOL Battle_PartyPreviewInit(OverlayManager *man);
BOOL Battle_PartyPreviewMain(OverlayManager *man);
BOOL Battle_VsScreenInit(OverlayManager *man);
BOOL Battle_VsScreenMain(OverlayManager *man);
void Battle_BattleInit(OverlayManager *man);
BOOL Battle_BattleMain(OverlayManager *man);
void Battle_BattleExit(OverlayManager *man);
BOOL Battle_EndInit(OverlayManager *man);
BOOL Battle_EndMain(OverlayManager *man);
void BgConfig_CleanupBattleMenuBackgrounds(BgConfig *bgConfig);
void BattleSystem_SetupTextFlags(BattleSystem *battleSystem);

// src/battle/battle_022378C0.c, src/battle/battle_main.c
void BgConfig_CleanupBattleBackgrounds(BgConfig *config);
void ov12_02238A64(BattleSystem *battleSystem);
void ov12_02237ED0(BattleSystem *battleSystem, int a1);
void Battle_InitBackgrounds(BattleSystem *battleSystem, BgConfig *bgConfig);
void ov12_022389B8(BattleSystem *battleSystem);

#endif
