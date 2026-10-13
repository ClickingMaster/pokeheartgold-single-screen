#include "battle/battle_022378C0.h"

#include "constants/game_stats.h"

#include "battle/battle_input.h"
#include "battle/battle_system.h"

#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "poke_overlay.h"
#include "render_text.h"
#include "sound_system.h"
#include "system.h"
#include "palette_vram_addr.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "evolution_scene.h"
#include "vram_transfer_manager.h"

FS_EXTERN_OVERLAY(OVY_5);
FS_EXTERN_OVERLAY(OVY_6);
FS_EXTERN_OVERLAY(OVY_7);
FS_EXTERN_OVERLAY(OVY_8);
FS_EXTERN_OVERLAY(OVY_10);

typedef enum BattleState {
    BSTATE_INIT,
    BSTATE_LINK_INIT,
    BSTATE_LINK_MAIN,
    BSTATE_UNK_A_INIT,
    BSTATE_UNK_A_MAIN,
    BSTATE_UNK_B_INIT,
    BSTATE_UNK_B_MAIN,
    BSTATE_SYNC,
    BSTATE_BATTLE_INIT,
    BSTATE_BATTLE_MAIN,
    BSTATE_END_INIT,
    BSTATE_END_MAIN,
    BSTATE_END_WAIT,
    BSTATE_EVOLUTION_INIT,
    BSTATE_EVOLUTION_MAIN,
    BSTATE_EXIT
} BattleState;

BOOL Battle_Run(OverlayManager *man, int *state) {
    BattleSetup *setup = OverlayManager_GetArgs(man);

    switch (*state) {
    case BSTATE_INIT:
        Heap_Create(HEAP_ID_3, HEAP_ID_BATTLE, 0xB0000);
        if ((setup->battleType & BATTLE_TYPE_LINK) && !(setup->battleSpecial & BATTLE_SPECIAL_RECORDING)) {
            *state = BSTATE_LINK_INIT;
        } else {
            *state = BSTATE_UNK_A_INIT;
        }
        break;
    case BSTATE_LINK_INIT:
        Battle_LinkInit(man, setup);
        Comm_RecordPlayerLocations(setup->wifiHistory);
        if (!CommState_IsWifi()) {
            GameStats_Inc(setup->gameStats, GAME_STAT_LOCAL_LINK_BATTLES);
            if (setup->battleType != 0x8F) {
                Sound_SetSelectSEMuted(1);
            }
        } else {
            GameStats_Inc(setup->gameStats, GAME_STAT_WIFI_BATTLES);
        }
        *state = BSTATE_LINK_MAIN;
        break;
    case BSTATE_LINK_MAIN:
        if (Battle_LinkMain(man) == TRUE) {
            Battle_LinkExit(man);
            *state = BSTATE_UNK_A_INIT;
        }
        break;
    case BSTATE_UNK_A_INIT:
        if (Battle_PartyPreviewInit(man) == TRUE) {
            *state = BSTATE_UNK_A_MAIN;
        } else {
            *state = BSTATE_UNK_B_INIT;
        }
        break;
    case BSTATE_UNK_A_MAIN:
        if (Battle_PartyPreviewMain(man) == TRUE) {
            *state = BSTATE_UNK_B_INIT;
        }
        break;
    case BSTATE_UNK_B_INIT:
        if (Battle_VsScreenInit(man) == TRUE) {
            *state = BSTATE_UNK_B_MAIN;
        } else {
            *state = BSTATE_BATTLE_INIT;
        }
        break;
    case BSTATE_UNK_B_MAIN:
        if (Battle_VsScreenMain(man) == TRUE) {
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_5));
            *state = BSTATE_SYNC;
            LinkSync_StartSync(0x3D);
        }
        break;
    case BSTATE_SYNC:
        if (LinkSync_IsSynced(0x3D)) {
            *state = BSTATE_BATTLE_INIT;
        }
        break;
    case BSTATE_BATTLE_INIT:
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_6), OVY_LOAD_ASYNC);
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_7), OVY_LOAD_ASYNC);
        Battle_BattleInit(man);
        *state = BSTATE_BATTLE_MAIN;
        break;
    case BSTATE_BATTLE_MAIN:
        if (Battle_BattleMain(man) == TRUE) {
            Battle_BattleExit(man);
            Sound_SetSelectSEMuted(0);
            *state = BSTATE_END_INIT;
        }
        break;
    case BSTATE_END_INIT:
        if (Battle_EndInit(man) == TRUE) {
            *state = BSTATE_END_MAIN;
        } else {
            Heap_Destroy(HEAP_ID_BATTLE);
            *state = BSTATE_EVOLUTION_INIT;
        }
        break;
    case BSTATE_END_MAIN:
        if (Battle_EndMain(man) == TRUE) {
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_5));
            Heap_Destroy(HEAP_ID_BATTLE);
            *state = BSTATE_END_WAIT;
        }
        break;
    case BSTATE_END_WAIT:
        *state = BSTATE_EXIT;
        break;
    case BSTATE_EVOLUTION_INIT: {
        int selectedMonIndex;
        int evolutionSpecies;
        int evolutionCondition;
        Pokemon *mon;

        evolutionSpecies = BattleSystem_CheckEvolution(setup, &selectedMonIndex, &evolutionCondition);

        if (evolutionSpecies) {
            Heap_Create(HEAP_ID_3, HEAP_ID_EVOLUTION, 0x30000);
            mon = Party_GetMonByIndex(setup->party[BATTLER_PLAYER], selectedMonIndex);
            setup->evolutionTaskData = EvolutionScene_New(setup->party[BATTLER_PLAYER], mon, evolutionSpecies, setup->options, setup->unk_164, setup->pokedex, setup->bag, setup->gameStats, evolutionCondition, 3, HEAP_ID_EVOLUTION);
            *state = BSTATE_EVOLUTION_MAIN;
        } else {
            *state = BSTATE_EXIT;
        }
        break;
    }
    case BSTATE_EVOLUTION_MAIN: {
        void *data = setup->evolutionTaskData;
        if (EvolutionScene_IsDone(data) == TRUE) {
            EvolutionScene_Delete(data);
            Heap_Destroy(HEAP_ID_EVOLUTION);
            *state = BSTATE_EVOLUTION_INIT;
        }
        break;
    }
    case BSTATE_EXIT:
        return TRUE;
    }
    return FALSE;
}

void BattleSystem_FreeInputForSubMenu(BattleSystem *battleSystem) {
    BattleInput_Free(battleSystem->battleInput);
    BgConfig_CleanupBattleMenuBackgrounds(battleSystem->bgConfig);
    battleSystem->subMenuBanksRequested = 1;
    FontID_Release(4);
    ov12_0223BBF0(battleSystem, 3);

    if (battleSystem->trainerAIOverlayLoaded == 0) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_7));
    } else {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_10));
    }
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_8), OVY_LOAD_ASYNC);
}

void BattleSystem_FreeGraphics(BattleSystem *battleSystem) {
    Main_SetVBlankIntrCB(NULL, NULL);
    BattleInput_Free(battleSystem->battleInput);
    RemoveWindow(battleSystem->window);
    BgConfig_CleanupBattleBackgrounds(battleSystem->bgConfig);
    ov12_02238A64(battleSystem);
    SpriteSystem_FreeResourcesAndManager(battleSystem->spriteRenderer, battleSystem->gfxHandler);
    SpriteSystem_Free(battleSystem->spriteRenderer);
    GF_DestroyVramTransferManager();
    FontID_Release(4);
}

void BattleSystem_RestoreInputAfterSubMenu(BattleSystem *battleSystem) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_8));

    if (battleSystem->trainerAIOverlayLoaded == 0) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_7), OVY_LOAD_ASYNC);
    } else {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_10), OVY_LOAD_ASYNC);
    }

    ov12_0223BBF0(battleSystem, 0);

    int size = GetSubBgPlttSize();
    void *data = GetSubBgPlttAddr();
    MIi_CpuClear16(0, data, size);

    NARC *unkNarcA = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
    NARC *unkNarcB = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);

    battleSystem->battleInput = BattleInput_NewInit(unkNarcA, unkNarcB, battleSystem, BattleSystem_GetTrainerGender(battleSystem, BattleSystem_GetNetworkId(battleSystem)), battleSystem->keyPressedSave);

    FontID_Alloc(4, HEAP_ID_BATTLE);

    battleSystem->battleBanksRequested = 1;

    BgConfig_InitBattleMenuBackgrounds(battleSystem->bgConfig);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    BattleInput_LoadDefaultResources(battleSystem->battleInput);
    BattleInput_ChangeMenu(unkNarcA, unkNarcB, battleSystem->battleInput, 0, TRUE, NULL);
    BattleInput_LoadBallGaugeResources(unkNarcB, battleSystem->battleInput);

    NARC_Delete(unkNarcA);
    NARC_Delete(unkNarcB);

    TextPrinter_SetDownArrowBaseTile(1);
    BattleSystem_SetupTextFlags(battleSystem);

    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(battleSystem->spriteRenderer), 0, FX32_CONST(272));
}

void BattleSystem_FreeMainBgLayers(BattleSystem *battleSystem) {
    RemoveWindow(battleSystem->window);

    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(battleSystem->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(battleSystem->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(battleSystem->bgConfig, GF_BG_LYR_MAIN_3);
    BattleSystem_SetHpBarDisabled(battleSystem);
}

#include "constants/battle.h"

#include "battle/battle_022378C0.h"
#include "battle/battle_main.h"
#include "battle/battle_controller_player.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"

#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "render_window.h"
#include "unk_0200FA24.h"

BOOL Battle_BattleMain(OverlayManager *man);
void Battle_InitBackgrounds(BattleSystem *battleSystem, BgConfig *bgConfig);
void ov12_022389B8(BattleSystem *battleSystem);

void BattleSystem_ReinitMainBgLayers(BattleSystem *battleSystem) {
    u32 frame;
    BattleBgTemplateSet templates;

    battleSystem->resetBlendRequested = 1;

    templates = ov12_0226C120;

    InitBgFromTemplate(battleSystem->bgConfig, GF_BG_LYR_MAIN_1, &templates.templates[0], 0);
    BgClearTilemapBufferAndCommit(battleSystem->bgConfig, GF_BG_LYR_MAIN_1);
    InitBgFromTemplate(battleSystem->bgConfig, GF_BG_LYR_MAIN_2, &templates.templates[1], 0);
    BgClearTilemapBufferAndCommit(battleSystem->bgConfig, GF_BG_LYR_MAIN_2);
    InitBgFromTemplate(battleSystem->bgConfig, GF_BG_LYR_MAIN_3, &templates.templates[2], 0);
    BgClearTilemapBufferAndCommit(battleSystem->bgConfig, GF_BG_LYR_MAIN_3);

    G2_SetBG0Priority(1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);

    frame = BattleSystem_GetFrame(battleSystem);
    LoadUserFrameGfx2WithFill(battleSystem->bgConfig, GF_BG_LYR_MAIN_1, 1, 10, frame, HEAP_ID_BATTLE);

    GfGfxLoader_LoadCharData(NARC_a_0_0_7, battleSystem->backgroundId + 3, battleSystem->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_BATTLE);
    PaletteData_LoadNarc(battleSystem->palette, NARC_a_0_0_7, battleSystem->backgroundId * 3 + 0xB0 + BattleSystem_GetBgTimeOfDay(battleSystem), HEAP_ID_BATTLE, (PaletteBufferId)0, 0, 0);
    PaletteData_LoadNarc(battleSystem->palette, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_BATTLE, (PaletteBufferId)0, 0x20, 0xA0);
    PaletteData_LoadNarc(battleSystem->palette, NARC_graphic_font, 8, HEAP_ID_BATTLE, (PaletteBufferId)0, 0x20, 0xB0);
    GfGfxLoader_LoadScrnData(NARC_a_0_0_7, 2, battleSystem->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_BATTLE);

    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);

    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);

    Main_SetVBlankIntrCB(Battle_VBlankCallback, battleSystem);

    battleSystem->battleBanksRequested = 1;

    AddWindowParameterized(battleSystem->bgConfig, battleSystem->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 11, 31);
    FillWindowPixelBuffer(battleSystem->window, 0xFF);
    DrawFrameAndWindow2(battleSystem->window, FALSE, 1, 10);

    BattleSystem_SetupTextFlags(battleSystem);
}

void BattleSystem_SetTrainerAIOverlayLoaded(BattleSystem *battleSystem, int a1) {
    if (battleSystem->trainerAIOverlayLoaded == a1) {
        GF_AssertFail();
    }

    battleSystem->trainerAIOverlayLoaded = a1;

    if (a1 == 0) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_10));
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_7), OVY_LOAD_ASYNC);
    } else {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_7));
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_10), OVY_LOAD_ASYNC);
    }
}
