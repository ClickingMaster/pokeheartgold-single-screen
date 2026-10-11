#ifndef POKEHEARTGOLD_BATTLE_BATTLE_MAIN_H
#define POKEHEARTGOLD_BATTLE_BATTLE_MAIN_H

#include "battle/battle.h"
#include "battle/battle_022378C0.h"

#include "bg_window.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "sys_task_api.h"

// The battle overlay's main module: src/battle/battle_022378C0.c and src/battle/battle_main.c.
// (Kept apart from battle_022378C0.h: more declarations there renumber function-local statics in its includers.)

FS_EXTERN_OVERLAY(OVY_7);
FS_EXTERN_OVERLAY(OVY_10);

typedef struct BattleBgTemplateSet {
    BgTemplate templates[3];
} BattleBgTemplateSet;

extern const BattleBgTemplateSet ov12_0226C120;
extern const BattleBgTemplateSet ov12_0226C174;
extern const GraphicsBanks sBattleGraphicsBanks;
extern const GraphicsModes sBattleGraphicsModes;

void ov12_022389B8(BattleSystem *battleSystem);
void BattleSystem_InitFromSetup(BattleSystem *battleSystem, BattleSetup *setup);
BOOL Battle_IsLeagueTrainerClass(int trainerClass);
GF3DVramMan *Battle_Create3DVramMan(void);
void Battle_Setup3D(void);
void Battle_Delete3DVramMan(GF3DVramMan *vramMan);
void Battle_ReserveTexAndPlttVram(void);
void Battle_VBlankCallback(void *data);
void Battle_LinkVBlankCallback(void *data);
void Task_DrawBattleGraphics(SysTask *task, void *data);
void Task_UpdateCriticalHpMusic(SysTask *task, void *data);
void VBlankTask_ScrollBattleBackground(SysTask *task, void *data);
void Battle_StaticInit(void);
void ov12_0223A664(BattleSystem *battleSystem, BattleSetup *setup);
void ov12_0223A7A0(void);

// Defined in other modules that have no header declaring them yet.
void ov12_02258DB0(BattleSystem *battleSystem, OpponentData *opponentData, u16 ball, int a3);
void ov12_02258E54(BattleSystem *battleSystem, OpponentData *opponentData);
void ov12_02258E7C(BattleSystem *battleSystem, OpponentData *opponentData, int a2);
OpponentData *ov12_02258D74(BattleSystem *battleSystem, const u8 *data);
void ov12_02260EA4(BattleSystem *battleSystem, OpponentData *opponentData);
void ov12_022621C4(BattleSystem *battleSystem, int a1);
void ov12_02265FD4(UnkBattleSystemSub17C *dest, BattleSystem *battleSystem, s32 idx, s32 terrain);
void BgConfig_InitBattleMenuBackgrounds(BgConfig *bgConfig);
void LoadUserFrameGfx2WithFill(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 color, u8 type, enum HeapID heapId);
u32 GetUserFrameGfx2PaletteMember(u32 frame);

#endif // POKEHEARTGOLD_BATTLE_BATTLE_MAIN_H
