#include "battle/battle_main.h"

#include "global.h"

#include "constants/battle.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "battle/battle.h"
#include "battle/battle_controller_opponent.h"
#include "battle/battle_controller_player.h"
#include "battle/battle_input.h"
#include "battle/battle_setup.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "battle/overlay_12_0226BEC4.h"

#include "bag.h"
#include "bag_cursor.h"
#include "battle_link_internal.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "game_stats.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_06.h"
#include "palette.h"
#include "party.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_mood.h"
#include "pokepic_anim_internal.h"
#include "render_text.h"
#include "render_window.h"
#include "sound_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "sound_playback.h"
#include "unk_0200FA24.h"
#include "particle_system.h"
#include "palette_blink.h"
#include "touchpad.h"
#include "unk_02026E30.h"
#include "unk_02026E84.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"
#include "unk_0208805C.h"
#include "vram_transfer_manager.h"

FS_EXTERN_OVERLAY(OVY_5);
FS_EXTERN_OVERLAY(OVY_6);
FS_EXTERN_OVERLAY(OVY_18);

// Work of the link data exchange stage (BSTATE_LINK_INIT/MAIN). battle_link*.c sees it as LinkWork, which
// declares only the members those functions use.
typedef struct BattlerInitData {
    u8 battler;
    u8 battlerType;
} BattlerInitData;

typedef struct BattleLinkStageWork {
    BattleSetup *setup;       // 0x00
    BgConfig *bgConfig;       // 0x04
    Window *window;           // 0x08
    PaletteData *palette;     // 0x0C
    u8 unk_10[0x1010];        // 0x10, LinkWork.friends / exchange buffer
    u8 syncCount;             // 0x1020
    u8 state;                 // 0x1021
    u16 timer;                // 0x1022
    WaitingIcon *waitingIcon; // 0x1024
} BattleLinkStageWork;

// Arguments of the overlay 5 link battle screen (0x30 bytes).
typedef struct BattleLinkScreenArgs {
    BattleSetup *setup; // 0x00
    Party *parties[4];  // 0x04
    String *names[4];   // 0x14
    enum HeapID heapId; // 0x24
    u8 mode;            // 0x28
    u8 multiOrSide;     // 0x29
    u8 outcome;         // 0x2A
    u8 done;            // 0x2B
    u8 unk_2C;          // 0x2C
    u8 unk_2D[3];       // 0x2D
} BattleLinkScreenArgs;

// BattleSystem.unk1C4 holds the overlay 5 link battle screen arguments while that screen runs. A macro, not an
// inline function: the field is reloaded at each use, after the calls in the same expression.
#define LINK_SCREEN_ARGS(battleSystem) ((BattleLinkScreenArgs *)(battleSystem)->unk1C4)

extern const OamManagerParam sBattleOamManagerParam;
extern const OamCharTransferParam sBattleOamCharTransferParam;
extern SpriteResourceCountsListUnion sBattleSpriteResourceCounts;
extern const u32 sBattleBgFillColors[23][3];
extern const GraphicsBanks ov12_0226C0D0;
extern const GraphicsBanks ov12_0226C0F8;
extern const GraphicsBanks sLinkGraphicsBanks;
extern const GraphicsModes sLinkGraphicsModes;
extern const BgTemplate sLinkBgTemplate;
extern const u8 sSinglesBattlerTypes[4];
extern const u8 sLinkSinglesBattlerTypes[4];
extern const u8 sLinkDoublesBattlerTypes[2][4];
extern const u8 sLinkMultiBattlerTypes[4][4];
extern const u8 sDoublesBattlerTypes[4];

// Declared by no header yet.
extern u32 *BattleAnimSystem_New(enum HeapID heapId);
extern void BattleAnimSystem_Delete(u32 *animSystem);
extern void LinkBattleScreen_Start(BattleLinkScreenArgs *args);
extern void sub_020302A4(BattleSetup *setup);
extern void ParticleSystem_FreeAll(void);
extern void sub_02037930(int mode);
extern void sub_020399FC(int a0, BgConfig *bgConfig);
extern void sub_0203A930(int linkLevel);
extern int ov00_021EC9D4(void);
extern BOOL sub_02035650(void);
extern int WM_GetLinkLevel(void);

#pragma force_active on

void Battle_BattleInit(OverlayManager *manager) {
    BattleSystem *battleSystem = OverlayManager_GetData(manager);
    BattleSetup *setup = OverlayManager_GetArgs(manager);
    NARC *archive7;
    NARC *archive8;
    int paletteSize;
    int i;
    int colorIndex;
    u32 gender;

    paletteSize = sub_02026E8C();
    MIi_CpuClear16(0, GetMainBgPlttAddr(), paletteSize);
    paletteSize = sub_02026E9C();
    MIi_CpuClear16(0, GetSubBgPlttAddr(), paletteSize);
    battleSystem->unk0 = (void *)Battle_Create3DVramMan();
    HBlankInterruptDisable();
    FontID_Alloc(4, HEAP_ID_BATTLE);
    battleSystem->hpNumPrinter = (void *)MessagePrinter_New(14, 2, 15, HEAP_ID_BATTLE);
    battleSystem->levelNumPrinter = battleSystem->hpNumPrinter;
    battleSystem->palette = PaletteData_Init(HEAP_ID_BATTLE);
    PaletteData_SetAutoTransparent(battleSystem->palette, TRUE);
    PaletteData_AllocBuffers(battleSystem->palette, PLTTBUF_MAIN_BG, 512, HEAP_ID_BATTLE);
    PaletteData_AllocBuffers(battleSystem->palette, PLTTBUF_SUB_BG, 512, HEAP_ID_BATTLE);
    PaletteData_AllocBuffers(battleSystem->palette, PLTTBUF_MAIN_OBJ, 448, HEAP_ID_BATTLE);
    PaletteData_AllocBuffers(battleSystem->palette, PLTTBUF_SUB_OBJ, 512, HEAP_ID_BATTLE);
    battleSystem->bgConfig = BgConfig_Alloc(HEAP_ID_BATTLE);
    battleSystem->window = AllocWindows(HEAP_ID_BATTLE, 3);
    for (i = 0; i < 4; i++) {
        battleSystem->unk1D0[i].unk0 = Heap_Alloc(HEAP_ID_BATTLE, 3200);
    }
    GF_CreateVramTransferManager(64, HEAP_ID_BATTLE);

    archive7 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
    archive8 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
    gender = BattleSystem_GetTrainerGender(battleSystem, ov12_0223BFC0(battleSystem));
    battleSystem->battleInput = BattleInput_NewInit(archive7, archive8, battleSystem, gender, battleSystem->unk1C0);
    NARC_Delete(archive7);
    NARC_Delete(archive8);

    Battle_InitBackgrounds(battleSystem, battleSystem->bgConfig);
    AddWindowParameterized(battleSystem->bgConfig, battleSystem->window, 1, 2, 19, 27, 4, 11, 31);
    FillWindowPixelBuffer(battleSystem->window, 255);
    DrawFrameAndWindow2(battleSystem->window, FALSE, 1, 10);
    battleSystem->spriteRenderer = SpriteSystem_Alloc(HEAP_ID_BATTLE);
    SpriteSystem_Init(battleSystem->spriteRenderer, &sBattleOamManagerParam, &sBattleOamCharTransferParam, 32);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, (GXOBJVRamModeChar)0x100010);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    battleSystem->gfxHandler = SpriteManager_New(battleSystem->spriteRenderer);
    SpriteSystem_InitSprites(battleSystem->spriteRenderer, battleSystem->gfxHandler, 128);
    SpriteSystem_InitManagerWithCapacities(battleSystem->spriteRenderer, battleSystem->gfxHandler, &sBattleSpriteResourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(battleSystem->spriteRenderer), 0, 0x110000);
    BattleInput_LoadDefaultResources(battleSystem->battleInput);

    archive7 = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
    archive8 = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);
    BattleInput_ChangeMenu(archive7, archive8, battleSystem->battleInput, 0, 1, NULL);
    BattleInput_LoadBallGaugeResources(archive8, battleSystem->battleInput);
    NARC_Delete(archive7);
    NARC_Delete(archive8);

    battleSystem->pokepicManager = PokepicManager_Create(HEAP_ID_BATTLE);
    PokepicManager_SetPlttBaseAddrAndSize(battleSystem->pokepicManager, 0, 192);
    BattleSystem_HpBar_Init(battleSystem);
    Battle_ReserveTexAndPlttVram();
    battleSystem->unk8C = BattleAnimSystem_New(HEAP_ID_BATTLE);
    ov12_022389B8(battleSystem);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    battleSystem->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 197, HEAP_ID_BATTLE);
    battleSystem->unk10 = (void *)NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 3, HEAP_ID_BATTLE);
    battleSystem->msgFormat = MessageFormat_New(HEAP_ID_BATTLE);
    battleSystem->msgBuffer = String_New(320, HEAP_ID_BATTLE);
    MIi_CpuCopy16(PaletteData_GetUnfadedBuf(battleSystem->palette, PLTTBUF_MAIN_BG), battleSystem->unk2228, 224);
    MIi_CpuCopy16(PaletteData_GetUnfadedBuf(battleSystem->palette, PLTTBUF_MAIN_OBJ), battleSystem->unk2308, 224);
    colorIndex = ov12_0223B52C(battleSystem);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, (u16)sBattleBgFillColors[battleSystem->backgroundId][colorIndex], 0, 112);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, (u16)sBattleBgFillColors[battleSystem->backgroundId][colorIndex], 192, 256);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_MAIN_OBJ, PLTTSEL_BOTH, (u16)sBattleBgFillColors[battleSystem->backgroundId][colorIndex], 0, 223);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_MAIN_BG, PLTTSEL_TRANSPARENT, 0, 160, 192);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_SUB_BG, PLTTSEL_TRANSPARENT, 0, 0, 255);
    PaletteData_FillPaletteInBuffer(battleSystem->palette, PLTTBUF_SUB_OBJ, PLTTSEL_TRANSPARENT, 65535, 0, 255);
    battleSystem->msgIcon = PaletteBlink_New(battleSystem->palette, (PMLCDTarget)0, 11, HEAP_ID_BATTLE);
    PaletteBlink_SetState(battleSystem->msgIcon, 1);
    battleSystem->unk1C = (u32)SysTask_CreateOnMainQueue(Task_DrawBattleGraphics, battleSystem, 60000);
    battleSystem->unk20 = (u32)SysTask_CreateOnMainQueue(Task_UpdateCriticalHpMusic, battleSystem, 50000);
    battleSystem->unk24 = (u32)SysTask_CreateOnVBlankQueue(VBlankTask_ScrollBattleBackground, battleSystem, 1200);
    battleSystem->unk2438 = -51;
    BattleSystem_SetupTextFlags(battleSystem);
    BagCursor_Battle_Init(BattleSystem_GetBagCursor(battleSystem));
    battleSystem->unk1C8 = PokepicAnimManager_New(HEAP_ID_BATTLE, 4, 0);
    battleSystem->unk1CC = (void *)sub_02020654(4, HEAP_ID_BATTLE);
    if (battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING) {
        for (i = 0; i < 4; i++) {
            battleSystem->chatotVoiceParam[i] = setup->unk1BC[i];
        }
    }
}

#pragma force_active reset

BOOL Battle_BattleMain(OverlayManager *man) {
    BattleSystem *battleSystem = OverlayManager_GetData(man);
    int i;

    if ((battleSystem->battleType & BATTLE_TYPE_LINK) && !(battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING)) {
        if (battleSystem->unk23FC) {
            BattleContext_Main(battleSystem, battleSystem->ctx);
        }
        for (i = 0; i < battleSystem->maxBattlers; i++) {
            OpponentData_RunCommand(battleSystem, battleSystem->opponentData[i]);
        }
    } else {
        if (battleSystem->unk23FC) {
            battleSystem->unk23FE = BattleContext_Main(battleSystem, battleSystem->ctx);
            BattleController_ProcessQueue(battleSystem, 1);
        }
        for (i = 0; i < battleSystem->maxBattlers; i++) {
            OpponentData_RunCommand(battleSystem, battleSystem->opponentData[i]);
            BattleController_ProcessQueue(battleSystem, 0);
        }
        if (!battleSystem->unk23FE) {
            if (battleSystem->unk23FC) {
                battleSystem->unk23FE = BattleContext_Main(battleSystem, battleSystem->ctx);
                BattleController_ProcessQueue(battleSystem, 1);
            }
            for (i = 0; i < battleSystem->maxBattlers; i++) {
                OpponentData_RunCommand(battleSystem, battleSystem->opponentData[i]);
                BattleController_ProcessQueue(battleSystem, 0);
            }
        }
    }

    return battleSystem->unk23FE;
}

#pragma force_active on

void Battle_BattleExit(OverlayManager *man) {
    BattleSystem *battleSystem = OverlayManager_GetData(man);
    BattleSetup *setup = OverlayManager_GetArgs(man);
    int color;
    int i;

    setup->unk_19C = battleSystem->randTemp;
    setup->battleSpecial = battleSystem->battleSpecial;
    if (!(battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING)) {
        sub_020302A4(setup);
    }
    if (battleSystem->unk2445) {
        ov12_02237ED0(battleSystem, 0);
    }
    if (setup->battleType & 0xAA4) {
        color = 0;
    } else if ((u8)(battleSystem->battleOutcomeFlag + 254) <= 1) {
        color = 0x7FFF;
    } else {
        color = 0;
    }
    SetMasterBrightnessByColor(0, color);
    SetMasterBrightnessByColor(1, color);
    BattleSystem_TryChangeForm(battleSystem);
    if (battleSystem->battleOutcomeFlag != 4) {
        ov12_0223B870(battleSystem, Party_GetMonByIndex(battleSystem->trainerParty[1], 0));
    }
    for (i = 0; i < 4; i++) {
        Party_Copy(battleSystem->trainerParty[i], setup->party[i]);
        Heap_Free(battleSystem->trainerParty[i]);
        PlayerProfile_Copy(battleSystem->playerProfile[i], setup->profile[i]);
        Heap_Free(battleSystem->playerProfile[i]);
    }
    PaletteBlink_Delete(battleSystem->msgIcon);
    Save_Bag_Copy(battleSystem->bag, setup->bag);
    Heap_Free(battleSystem->bag);
    Pokedex_Copy(battleSystem->pokedex, setup->pokedex);
    Heap_Free(battleSystem->pokedex);
    setup->storagePC = battleSystem->storage;
    setup->bagCursor = battleSystem->bagCursor;
    setup->unk1B8 = battleSystem->unk1C0;
    setup->unk_128 = battleSystem->unk98;
    setup->unk_134 = (struct UnkStruct_02067A60 *)battleSystem->unk9C;
    setup->safariBalls = battleSystem->safariBallCnt;
    setup->winFlag = battleSystem->battleOutcomeFlag & 0x3F;
    setup->unk_170 = battleSystem->unk243C;
    setup->levelUpFlag = ov12_022581D4(battleSystem, battleSystem->ctx, 4, 0);
    setup->unk138.unk_0 += ov12_022581D4(battleSystem, battleSystem->ctx, 3, 0);
    setup->unk138.unk_4 += ov12_022581D4(battleSystem, battleSystem->ctx, 6, 0) + ov12_022581D4(battleSystem, battleSystem->ctx, 6, 2);
    setup->unk138.unk_8 += ov12_022581D4(battleSystem, battleSystem->ctx, 7, 0) + ov12_022581D4(battleSystem, battleSystem->ctx, 7, 2);
    setup->unk1B4 = ov12_022581D4(battleSystem, battleSystem->ctx, 3, 0);
    setup->unk1C4 = battleSystem->isRecordingPaused;
    for (i = 0; i < 4; i++) {
        Heap_Free(battleSystem->unk1D0[i].unk0);
    }
    Heap_Free(battleSystem->msgBuffer);
    PaletteData_FreeBuffers(battleSystem->palette, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(battleSystem->palette, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(battleSystem->palette, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(battleSystem->palette, PLTTBUF_SUB_OBJ);
    PaletteData_Free(battleSystem->palette);
    DestroyMsgData(battleSystem->msgData);
    DestroyMsgData((MsgData *)battleSystem->unk10);
    MessageFormat_Delete(battleSystem->msgFormat);
    PokepicAnimManager_Delete(battleSystem->unk1C8);
    ParticleSystem_FreeAll();
    BattleAnimSystem_Delete(battleSystem->unk8C);
    BattleContext_Delete(battleSystem->ctx);
    for (i = 0; i < battleSystem->maxBattlers; i++) {
        OpponentData_Free(battleSystem, battleSystem->opponentData[i], battleSystem->unk23FD);
    }
    PokepicManager_Delete(battleSystem->pokepicManager);
    if (battleSystem->unk23FD != 2) {
        ov12_02237B6C(battleSystem);
    }
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    WindowArray_Delete(battleSystem->window, 3);
    Heap_Free(battleSystem->bgConfig);
    Heap_Free(battleSystem->unk220);
    Heap_Free(battleSystem->unk224);
    MessagePrinter_Delete((MessagePrinter *)battleSystem->hpNumPrinter);
    SysTask_Destroy((SysTask *)battleSystem->unk1C);
    SysTask_Destroy((SysTask *)battleSystem->unk20);
    GF_TouchpadStopSampling();
    Battle_Delete3DVramMan((GF3DVramMan *)battleSystem->unk0);
    SetLCRNGSeed(battleSystem->unk2434);
    if (BattleSystem_GetCriticalHpMusicFlag(battleSystem)) {
        StopSE(SEQ_SE_DP_HINSI, 0);
    }
    sub_0202067C((NNSG2dCellTransferState *)battleSystem->unk1CC);
    if (BattleSystem_IsRecordingPaused(battleSystem)) {
        Sound_SetMasterVolume(127);
    }
    if (battleSystem->unk247C) {
        ov12_0226BEF0(battleSystem->unk247C);
    }
    Heap_Free(battleSystem);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_6));
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_7));
    if (!sub_02039998()) {
        UnloadOverlayByID(FS_OVERLAY_ID(OVY_18));
    }
}

#pragma force_active reset

void Battle_InitBackgrounds(BattleSystem *battleSystem, BgConfig *bgConfig) {
    BattleBgTemplateSet templates;
    GraphicsBanks banks;
    GraphicsModes modes;
    u32 frame;

    GfGfx_DisableEngineAPlanes();

    banks = sBattleGraphicsBanks;
    GfGfx_SetBanks(&banks);

    MI_CpuClear32((void *)0x06000000, 0x80000);
    MI_CpuClear32((void *)0x06200000, 0x20000);
    MI_CpuClear32((void *)0x06400000, 0x40000);
    MI_CpuClear32((void *)0x06600000, 0x20000);

    SetMasterBrightnessNeutral(0);
    SetMasterBrightnessNeutral(1);

    modes = sBattleGraphicsModes;
    SetBothScreensModesAndDisable(&modes);

    battleSystem->unk240F_0 = 1;

    templates = ov12_0226C174;

    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &templates.templates[0], 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &templates.templates[1], 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &templates.templates[2], 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);

    G2_SetBG0Priority(1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);

    BgConfig_InitBattleMenuBackgrounds(bgConfig);

    frame = BattleSystem_GetFrame(battleSystem);
    LoadUserFrameGfx2WithFill(bgConfig, GF_BG_LYR_MAIN_1, 1, 10, frame, HEAP_ID_BATTLE);

    GfGfxLoader_LoadCharData(NARC_a_0_0_7, battleSystem->backgroundId + 3, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_BATTLE);
    PaletteData_LoadNarc(battleSystem->palette, NARC_a_0_0_7, battleSystem->backgroundId * 3 + 0xB0 + ov12_0223B52C(battleSystem), HEAP_ID_BATTLE, (PaletteBufferId)0, 0, 0);
    PaletteData_LoadNarc(battleSystem->palette, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_BATTLE, (PaletteBufferId)0, 0x20, 0xA0);
    PaletteData_LoadNarc(battleSystem->palette, NARC_graphic_font, 8, HEAP_ID_BATTLE, (PaletteBufferId)0, 0x20, 0xB0);
    GfGfxLoader_LoadScrnData(NARC_a_0_0_7, 2, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_BATTLE);

    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_NONE, FALSE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);

    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);

    Main_SetVBlankIntrCB(Battle_VBlankCallback, battleSystem);
}

void ov12_022389B8(BattleSystem *battleSystem) {
    BattleContext *ctx;
    int i;
    Terrain terrain = BattleSystem_GetTerrainId(battleSystem);

    ov12_02265FD4(&battleSystem->unk17C[0], battleSystem, 0, terrain);
    ov12_02265FD4(&battleSystem->unk17C[1], battleSystem, 1, terrain);

    ctx = BattleSystem_GetBattleContext(battleSystem);

    for (i = 0; i < battleSystem->maxBattlers; i++) {
        int battlerId = ov12_022581D4(battleSystem, ctx, 2, i);
        ov12_02258DB0(battleSystem, battleSystem->opponentData[i], BattleSystem_GetMonBall(battleSystem, BattleSystem_GetPartyMon(battleSystem, i, battlerId)), battlerId);
    }

    ov12_0223A7A0();
}

void BgConfig_CleanupBattleBackgrounds(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    BgConfig_CleanupBattleMenuBackgrounds(bgConfig);
}

void ov12_02238A64(BattleSystem *battleSystem) {
}

void BattleSystem_InitFromSetup(BattleSystem *battleSystem, BattleSetup *setup);

void BattleSystem_InitFromSetup(BattleSystem *battleSystem, BattleSetup *setup) {
    int i, slot;
    BattlerInitData battlerInitData;
    Pokemon *mon;

    battleSystem->battleType = setup->battleType;

    for (i = 0; i < BATTLER_MAX; i++) {
        battleSystem->playerProfile[i] = PlayerProfile_New(HEAP_ID_BATTLE);
        PlayerProfile_Copy(setup->profile[i], battleSystem->playerProfile[i]);
        battleSystem->chatotVoice[i] = setup->chatot[i];
    }

    battleSystem->unk2446 = setup->unk1B0;

    for (i = 0; i < BATTLER_MAX; i++) {
        battleSystem->unk2464[i] = setup->unk_1A0[i];
    }

    battleSystem->unk2434 = GetLCRNGSeed();
    battleSystem->rand = setup->unk_19C;
    battleSystem->randTemp = setup->unk_19C;
    battleSystem->battleSpecial = setup->battleSpecial;
    battleSystem->bag = Save_Bag_New(HEAP_ID_BATTLE);
    Save_Bag_Copy(setup->bag, battleSystem->bag);
    battleSystem->pokedex = Pokedex_New(HEAP_ID_BATTLE);
    Pokedex_Copy(setup->pokedex, battleSystem->pokedex);

    battleSystem->storage = setup->storagePC;
    battleSystem->options = setup->options;
    battleSystem->unk1B8 = (u32 *)setup->palPad;
    battleSystem->bagCursor = setup->bagCursor;
    battleSystem->unk1C0 = setup->unk1B8;
    battleSystem->unk98 = setup->unk_128;
    battleSystem->unk2424 = setup->evolutionLocation;
    battleSystem->unk9C = (u32 *)setup->unk_134;
    battleSystem->safariBallCnt = setup->safariBalls;
    battleSystem->terrain = setup->terrain;
    battleSystem->backgroundId = setup->battleBg;
    battleSystem->location = setup->mapSection;
    battleSystem->timezone = setup->timeOfDay;
    battleSystem->unk241C = setup->fixedDamaageMovesBanned;
    battleSystem->unk2428 = setup->unk_164;
    battleSystem->metBill = setup->metBill;
    battleSystem->unk2474_1 = setup->momsSavingsActive;
    battleSystem->isFishing = setup->unk_1D0;
    battleSystem->weather = setup->weatherType;
    battleSystem->gameStats = setup->gameStats;

    GF_ASSERT(setup->gameStats != NULL);
    battleSystem->bugContestCaughtMon = setup->bugContestMon;

    for (i = 0; i < BATTLER_MAX; i++) {
        battleSystem->trainerId[i] = setup->trainerId[i];
        battleSystem->trainers[i] = setup->trainer[i];
    }

    battleSystem->ctx = BattleContext_New(battleSystem);

    for (i = 0; i < BATTLER_MAX; i++) {
        battleSystem->trainerParty[i] = SaveArray_Party_Alloc(HEAP_ID_BATTLE);
        battleSystem->unk248C[i] = setup->unk1CC[i];
    }

    int gender;

    for (i = 0; i < BATTLER_MAX; i++) {
        for (slot = 0; slot < Party_GetCount(setup->party[i]); slot++) {
            mon = Party_GetMonByIndex(setup->party[i], slot);
            gender = GetMonGender(mon);
            SetMonData(mon, MON_DATA_GENDER, &gender);
        }
    }

    if (battleSystem->battleType & BATTLE_TYPE_LINK) {
        sub_02074E5C(battleSystem);
        u8 networkID = ov12_0223BFC0(battleSystem);

        ov12_0223A664(battleSystem, setup);

        if (battleSystem->battleType & BATTLE_TYPE_FRONTIER) {
            for (i = 0; i < BATTLER_MAX; i++) {
                battlerInitData.battler = i;
                battlerInitData.battlerType = sDoublesBattlerTypes[i];
                battleSystem->opponentData[i] = OpponentData_New(battleSystem, (u8 *)&battlerInitData);
            }

            battleSystem->maxBattlers = i;

            for (i = 0; i < BATTLER_MAX; i++) {
                OpponentData_SetInputTasks(battleSystem, battleSystem->opponentData[i]);
                Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
            }

            for (i = 0; i < battleSystem->maxBattlers; i++) {
                for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                    mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                    if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                }
                ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            }
            ov12_02256F28(battleSystem, battleSystem->ctx);
        } else if (battleSystem->battleType & BATTLE_TYPE_MULTI) {
            for (i = 0; i < BATTLER_MAX; i++) {
                battlerInitData.battler = i;
                battlerInitData.battlerType = sLinkMultiBattlerTypes[ov12_0223BFCC(battleSystem, networkID)][ov12_0223BFCC(battleSystem, i)];
                battleSystem->opponentData[i] = OpponentData_New(battleSystem, (u8 *)&battlerInitData);
            }
            battleSystem->maxBattlers = i;
            for (i = 0; i < BATTLER_MAX; i++) {
                OpponentData_SetInputTasks(battleSystem, battleSystem->opponentData[i]);
                Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
            }
            for (i = 0; i < battleSystem->maxBattlers; i++) {
                for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                    mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                    if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                }
                ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            }
            ov12_02256F28(battleSystem, battleSystem->ctx);
        } else if (battleSystem->battleType & BATTLE_TYPE_DOUBLES) {
            for (i = 0; i < BATTLER_MAX; i++) {
                battlerInitData.battler = i;
                battlerInitData.battlerType = sLinkDoublesBattlerTypes[networkID][i];
                battleSystem->opponentData[i] = OpponentData_New(battleSystem, (u8 *)&battlerInitData);
                OpponentData_SetInputTasks(battleSystem, battleSystem->opponentData[i]);
            }
            battleSystem->maxBattlers = i;
            for (i = 0; i < BATTLER_MAX; i++) {
                Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
            }
            for (i = 0; i < battleSystem->maxBattlers; i++) {
                for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i & 1]); slot++) {
                    mon = Party_GetMonByIndex(battleSystem->trainerParty[i & 1], slot);
                    if (i > 1) {
                        if (ov12_022581D4(battleSystem, battleSystem->ctx, 2, i & 1) != slot
                            && GetMonData(mon, MON_DATA_SPECIES, NULL)
                            && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                            && GetMonData(mon, MON_DATA_HP, NULL)) {
                            break;
                        }
                    } else if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                }
                ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            }
        } else {
            for (i = 0; i < BATTLER_MAX / 2; i++) {
                battlerInitData.battler = i;
                battlerInitData.battlerType = sLinkSinglesBattlerTypes[networkID * 2 + i];
                battleSystem->opponentData[i] = OpponentData_New(battleSystem, (u8 *)&battlerInitData);
                OpponentData_SetInputTasks(battleSystem, battleSystem->opponentData[i]);
            }
            battleSystem->maxBattlers = i;
            for (i = 0; i < BATTLER_MAX; i++) {
                Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
                for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                    mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                    if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                }
                ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            }
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
    } else if (battleSystem->battleType & BATTLE_TYPE_TAG) {
        int playerSlot;
        for (i = 0; i < BATTLER_MAX; i++) {
            battlerInitData.battler = i;
            battlerInitData.battlerType = sDoublesBattlerTypes[i];
            battleSystem->opponentData[i] = OpponentData_New(battleSystem, (u8 *)&battlerInitData);
            OpponentData_SetInputTasks(battleSystem, battleSystem->opponentData[i]);
        }
        battleSystem->maxBattlers = i;
        for (i = 0; i < BATTLER_MAX; i++) {
            Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
        }
        for (i = 0; i < battleSystem->maxBattlers; i++) {
            for (slot = 0; slot < BattleSystem_GetPartySize(battleSystem, i); slot++) {
                mon = BattleSystem_GetPartyMon(battleSystem, i, slot);
                if (i == 2) {
                    if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)
                        && playerSlot != slot) {
                        break;
                    }
                } else if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                    && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                    && GetMonData(mon, MON_DATA_HP, NULL)) {
                    break;
                }
            }
            ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
            if (i == 0) {
                playerSlot = slot;
            }
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
        battleSystem->unk23FC = 1;
    } else if (battleSystem->battleType & BATTLE_TYPE_MULTI) {
        for (i = 0; i < BATTLER_MAX; i++) {
            battlerInitData.battler = i;
            battlerInitData.battlerType = sDoublesBattlerTypes[i];
            battleSystem->opponentData[i] = OpponentData_New(battleSystem, (u8 *)&battlerInitData);
            OpponentData_SetInputTasks(battleSystem, battleSystem->opponentData[i]);
        }
        battleSystem->maxBattlers = i;
        for (i = 0; i < BATTLER_MAX; i++) {
            Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
        }
        for (i = 0; i < battleSystem->maxBattlers; i++) {
            for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                    && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                    && GetMonData(mon, MON_DATA_HP, NULL)) {
                    break;
                }
            }
            ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
        battleSystem->unk23FC = 1;
    } else if (battleSystem->battleType & BATTLE_TYPE_DOUBLES) {
        for (i = 0; i < BATTLER_MAX; i++) {
            battlerInitData.battler = i;
            battlerInitData.battlerType = sDoublesBattlerTypes[i];
            battleSystem->opponentData[i] = OpponentData_New(battleSystem, (u8 *)&battlerInitData);
            OpponentData_SetInputTasks(battleSystem, battleSystem->opponentData[i]);
        }
        battleSystem->maxBattlers = i;
        for (i = 0; i < BATTLER_MAX; i++) {
            Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
        }
        for (i = 0; i < battleSystem->maxBattlers; i++) {
            for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i & 1]); slot++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[i & 1], slot);
                if (i > 1) {
                    if (ov12_022581D4(battleSystem, battleSystem->ctx, 2, i & 1) != slot
                        && GetMonData(mon, MON_DATA_SPECIES, NULL)
                        && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                        && GetMonData(mon, MON_DATA_HP, NULL)) {
                        break;
                    }
                } else if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                    && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                    && GetMonData(mon, MON_DATA_HP, NULL)) {
                    break;
                }
            }
            ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
        battleSystem->unk23FC = 1;
    } else {
        for (i = 0; i < BATTLER_MAX / 2; i++) {
            battlerInitData.battler = i;
            battlerInitData.battlerType = sSinglesBattlerTypes[i];
            battleSystem->opponentData[i] = OpponentData_New(battleSystem, (u8 *)&battlerInitData);
            OpponentData_SetInputTasks(battleSystem, battleSystem->opponentData[i]);
        }
        battleSystem->maxBattlers = i;
        for (i = 0; i < BATTLER_MAX; i++) {
            Party_Copy(setup->party[i], battleSystem->trainerParty[i]);
            for (slot = 0; slot < Party_GetCount(battleSystem->trainerParty[i]); slot++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[i], slot);
                if (GetMonData(mon, MON_DATA_SPECIES, NULL)
                    && GetMonData(mon, MON_DATA_IS_EGG, NULL) == FALSE
                    && GetMonData(mon, MON_DATA_HP, NULL)) {
                    break;
                }
            }
            ov12_022582B8(battleSystem, battleSystem->ctx, 2, i, slot);
        }
        ov12_02256F28(battleSystem, battleSystem->ctx);
        battleSystem->unk23FC = 1;
    }

    if (battleSystem->battleType & BATTLE_TYPE_PAL_PARK) {
        mon = Party_GetMonByIndex(battleSystem->trainerParty[1], 0);
        GetMonData(mon, MON_DATA_OT_NAME, (u8 *)&battleSystem->trainers[1].name);
    }

    if (battleSystem->battleType & BATTLE_TYPE_TRAINER) {
        if (Battle_IsLeagueTrainerClass(battleSystem->trainers[1].data.trainerClass) == TRUE
            || Battle_IsLeagueTrainerClass(battleSystem->trainers[3].data.trainerClass) == TRUE) {
            for (i = 0; i < Party_GetCount(battleSystem->trainerParty[0]); i++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[0], i);
                MonApplyFriendshipMod(mon, FRIENDSHIP_EVENT_LEAGUE_BATTLE, battleSystem->location);
                ApplyMonMoodModifier(mon, 2);
            }
            for (i = 0; i < Party_GetCount(battleSystem->trainerParty[2]); i++) {
                mon = Party_GetMonByIndex(battleSystem->trainerParty[2], i);
                MonApplyFriendshipMod(mon, FRIENDSHIP_EVENT_LEAGUE_BATTLE, battleSystem->location);
                ApplyMonMoodModifier(mon, 2);
            }
        }
    }

    battleSystem->unk2478 = 0;
    battleSystem->unk2474_2 = 0;
    int incompatible = 0;
    for (i = 0; i < battleSystem->maxBattlers; i++) {
        int version = PlayerProfile_GetVersion(BattleSystem_GetPlayerProfile(battleSystem, i));
        if (version != GAME_VERSION) {
            incompatible = 1;
        }
        if (version != VERSION_HEARTGOLD && version != VERSION_SOULSILVER) {
            battleSystem->unk2474_2 = 1;
        }
    }
    if (battleSystem->unk23FC != 0) {
        battleSystem->unk2478 = incompatible;
        return;
    }
    battleSystem->unk2478 = 0;
}

BOOL Battle_IsLeagueTrainerClass(int trainerClass) {
    switch (trainerClass) {
    case 0x42:
    case 0x43:
    case 0x46:
    case 0x48:
    case 0x49:
    case 0x4A:
    case 0x4B:
    case 0x4C:
    case 0x56:
    case 0x57:
    case 0x58:
    case 0x59:
    case 0x62:
    case 0x67:
    case 0x68:
    case 0x69:
    case 0x6A:
    case 0x6B:
    case 0x6C:
    case 0x6E:
    case 0x70:
        return TRUE;
    default:
        return FALSE;
    }
}

GF3DVramMan *Battle_Create3DVramMan(void) {
    return GF_3DVramMan_Create(HEAP_ID_BATTLE, 0, 2, 0, 2, Battle_Setup3D);
}

#pragma force_active on

void Battle_Setup3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7FFF, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}

#pragma force_active reset

void Battle_Delete3DVramMan(GF3DVramMan *vramMan) {
    GF_3DVramMan_Delete(vramMan);
}

void Battle_ReserveTexAndPlttVram(void) {
    u32 tex = NNS_GfdDefaultFuncAllocTexVram(0x8000, FALSE, 0);
    u32 pltt = NNS_GfdDefaultFuncAllocPlttVram(0xC0, FALSE, 0);
    if (tex == 0) {
        GF_AssertFail();
    }
    if (pltt == 0) {
        GF_AssertFail();
    }
    ParticleSystem_ZeroAll();
}

// Sets an IRQ check flag in the DTCM system area through its literal address (OS_SetIrqCheckFlag addresses
// it through the DTCM section symbol).
static inline void SetIrqCheckFlag(u8 *dtcm, u32 mask) {
    *(volatile u32 *)(dtcm + 0x3FF8) |= mask;
}

#pragma force_active on

void Battle_VBlankCallback(void *data) {
    BattleSystem *battleSystem = data;
    GraphicsBanks banks;
    GraphicsBanks banks2;

    if (battleSystem->unk240F_0) {
        battleSystem->unk240F_0 = 0;
        reg_G2_BLDCNT = 0;
        reg_G2S_DB_BLDCNT = 0;
    }
    if (battleSystem->unk240F_1) {
        battleSystem->unk240F_1 = 0;
        banks = ov12_0226C0D0;
        GfGfx_SetBanks(&banks);
    }
    if (battleSystem->unk240E_F) {
        battleSystem->unk240E_F = 0;
        banks2 = ov12_0226C0F8;
        GfGfx_SetBanks(&banks2);
    }
    PokepicManager_HandleLoadImgAndOrPltt(battleSystem->pokepicManager);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(battleSystem->palette);
    DoScheduledBgGpuUpdates(battleSystem->bgConfig);
    SetIrqCheckFlag((u8 *)0x027E0000, OS_IE_VBLANK);
}

void Battle_LinkVBlankCallback(void *data) {
    BattleLinkStageWork *work = data;

    PaletteData_PushTransparentBuffers(work->palette);
    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(work->bgConfig);
    SetIrqCheckFlag((u8 *)0x027E0000, OS_IE_VBLANK);
}

#pragma force_active reset

void Task_DrawBattleGraphics(SysTask *task, void *data) {
    BattleSystem *battleSystem = data;

    sub_020399FC(5, battleSystem->bgConfig);
    if (battleSystem->unk23FD == 0 || battleSystem->unk23FD == 3) {
        if (battleSystem->unk23FD == 0) {
            ov06_0221BAF0();
        }
        PokepicManager_DrawAll(battleSystem->pokepicManager);
        SpriteSystem_DrawSprites(battleSystem->gfxHandler);
        SpriteSystem_UpdateTransfer();
        RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
    }
}

#pragma force_active on

void Task_UpdateCriticalHpMusic(SysTask *task, void *data) {
    BattleSystem *battleSystem = data;
    int maxBattlers = BattleSystem_GetMaxBattlers(battleSystem);
    u32 criticalMask = 0;
    int flag = BattleSystem_GetCriticalHpMusicFlag(battleSystem);
    int i;

    if (BattleSystem_GetBattleType(battleSystem) & (BATTLE_TYPE_SAFARI | BATTLE_TYPE_PAL_PARK)) {
        return;
    }
    if (flag & 2) {
        if (flag & 1) {
            StopSE(SEQ_SE_DP_HINSI, 0);
            BattleSystem_SetCriticalHpMusicFlag(battleSystem, 2);
        }
        return;
    }
    for (i = 0; i < maxBattlers; i++) {
        OpponentData *opponentData = BattleSystem_GetOpponentData(battleSystem, i);
        if ((!OpponentData_GetInputMode(opponentData) && !(BattleSystem_GetBattleSpecial(battleSystem) & BATTLE_SPECIAL_RECORDING))
            || (!BattleSystem_GetFieldSide(battleSystem, i) && (BattleSystem_GetBattleSpecial(battleSystem) & BATTLE_SPECIAL_RECORDING))) {
            BattleHpBar *hpBar = OpponentData_GetHpBar(opponentData);
            if (hpBar && CalculateHpBarColor(hpBar->hp, hpBar->maxHp, 48) == 1) {
                criticalMask |= MaskOfFlagNo(i);
            }
        }
    }
    if (criticalMask && !BattleSystem_GetCriticalHpMusicFlag(battleSystem)) {
        PlaySE(SEQ_SE_DP_HINSI);
        BattleSystem_SetCriticalHpMusicFlag(battleSystem, 1);
        BattleSystem_SetCriticalHpMusicDelay(battleSystem, 4);
    } else if (!criticalMask && BattleSystem_GetCriticalHpMusicFlag(battleSystem)) {
        StopSE(SEQ_SE_DP_HINSI, 0);
        BattleSystem_SetCriticalHpMusicFlag(battleSystem, 0);
    }
    if (BattleSystem_GetCriticalHpMusicFlag(battleSystem)) {
        int delay = BattleSystem_GetCriticalHpMusicDelay(battleSystem);
        if (!IsSEPlaying(SEQ_SE_DP_HINSI)) {
            if (--delay == 0) {
                PlaySE(SEQ_SE_DP_HINSI);
                BattleSystem_SetCriticalHpMusicDelay(battleSystem, 4);
            } else {
                BattleSystem_SetCriticalHpMusicDelay(battleSystem, delay);
            }
        }
    }
}

#pragma force_active reset

void VBlankTask_ScrollBattleBackground(SysTask *task, void *data) {
    BattleSystem *battleSystem = data;

    battleSystem->unk2438 += 3;
    BgSetPosTextAndCommit(battleSystem->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, battleSystem->unk2438);
    if (battleSystem->unk2438 == 0) {
        SysTask_Destroy(task);
    }
}

void Battle_StaticInit(void) {
    if (!sub_02039998()) {
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_18), OVY_LOAD_ASYNC);
    }
}

// The overlay's single static initializer entry.
#pragma define_section SINIT ".sinit" abs32 RWX
#pragma section SINIT begin
SDK_FORCE_EXPORT static void (*NitroStaticInit_[])(void) = { Battle_StaticInit };
#pragma section SINIT end

void Battle_LinkInit(OverlayManager *man, BattleSetup *setup) {
    GraphicsBanks banks;
    GraphicsModes modes;
    BgTemplate bgTemplate;
    BattleLinkStageWork *work = OverlayManager_CreateAndGetData(man, sizeof(BattleLinkStageWork), HEAP_ID_BATTLE);
    MsgData *msgData;
    String *string;
    u32 frame;

    work->setup = setup;
    work->syncCount = 0;
    work->state = 0;
    work->timer = 0;
    work->palette = PaletteData_Init(HEAP_ID_BATTLE);
    PaletteData_SetAutoTransparent(work->palette, TRUE);
    PaletteData_AllocBuffers(work->palette, (PaletteBufferId)0, 0x200, HEAP_ID_BATTLE);
    PaletteData_FillPaletteInBuffer(work->palette, (PaletteBufferId)0, (PaletteSelector)2, 0, 0, 0x100);
    work->bgConfig = BgConfig_Alloc(HEAP_ID_BATTLE);
    work->window = AllocWindows(HEAP_ID_BATTLE, 1);
    sub_02074EC4((LinkWork *)work);
    GfGfx_DisableEngineAPlanes();
    banks = sLinkGraphicsBanks;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (void *)0x06000000, 0x80000);
    MIi_CpuClear32(0, (void *)0x06200000, 0x20000);
    MIi_CpuClear32(0, (void *)0x06400000, 0x40000);
    MIi_CpuClear32(0, (void *)0x06600000, 0x20000);
    MIi_CpuClear16(0, (void *)0x05000000, 0x200);
    modes = sLinkGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    bgTemplate = sLinkBgTemplate;
    InitBgFromTemplate(work->bgConfig, 1, &bgTemplate, 0);
    BgClearTilemapBufferAndCommit(work->bgConfig, 1);
    frame = Options_GetFrame(setup->options);
    LoadUserFrameGfx2WithFill(work->bgConfig, 1, 1, 10, (u8)frame, HEAP_ID_BATTLE);
    PaletteData_LoadNarc(work->palette, (NarcId)0x10, 8, HEAP_ID_BATTLE, (PaletteBufferId)0, 0x20, 0xB0);
    PaletteData_LoadNarc(work->palette, (NarcId)0x26, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_BATTLE, (PaletteBufferId)0, 0x20, 0xA0);
    PaletteData_FillPaletteInBuffer(work->palette, (PaletteBufferId)0, (PaletteSelector)0, 0, 0, 0x100);
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(0x10, TRUE);
    *(volatile u16 *)0x04000304 |= 0x8000;
    AddWindowParameterized(work->bgConfig, work->window, 1, 2, 0x13, 0x1B, 4, 0xB, 0x1F);
    FillWindowPixelBuffer(work->window, 0xFF);
    DrawFrameAndWindow2(work->window, FALSE, 1, 0xA);
    msgData = NewMsgDataFromNarc((MsgDataLoadType)1, (NarcId)0x1B, 0xC5, HEAP_ID_BATTLE);
    string = String_New(0x100, HEAP_ID_BATTLE);
    ReadMsgDataIntoString(msgData, 0x39B, string);
    AddTextPrinterParameterized(work->window, 1, string, 0, 0, 0, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    Main_SetVBlankIntrCB(Battle_LinkVBlankCallback, work);
    PaletteData_BeginPaletteFade(work->palette, 5, 0xFFFF, 0, 0x10, 0, 0);
    work->waitingIcon = WaitingIcon_New(work->window, 1);
    ov12_0223A7A0();
}

BOOL Battle_LinkMain(OverlayManager *man) {
    BattleLinkStageWork *work = OverlayManager_GetData(man);
    BOOL done;
    int i;

    sub_020399FC(5, work->bgConfig);
    done = FALSE;
    switch (work->state) {
    case 0:
        SetMasterBrightnessNeutral(0);
        sub_02037930(1);
        work->state++;
        break;
    case 1:
        if (PaletteData_GetSelectedBuffersBitmask(work->palette) == 0) {
            work->state++;
        }
        break;
    case 2:
        sub_02037AC0(0x32);
        work->state++;
        break;
    case 3:
        if (sub_02037B38(0x32)) {
            sub_02037AC0(0x33);
            work->timer = 0;
            work->state++;
        } else {
            work->timer++;
            if (work->timer > 0x708) {
                sub_02039AD8(1);
            }
        }
        break;
    case 4:
        if (sub_02075074((LinkWork *)work, 0x140) == TRUE) {
            work->state++;
        }
        break;
    case 5:
        goto common;
    case 6:
        if (sub_020750E0((LinkWork *)work) == TRUE) {
            sub_02037AC0(0x34);
            work->state++;
        }
        break;
    case 7:
        if (sub_02075108((LinkWork *)work) == TRUE) {
            work->state++;
        }
        break;
    case 8:
        goto common;
    case 9:
        if (sub_0207514C((LinkWork *)work) == TRUE) {
            sub_02037AC0(0x35);
            work->state++;
        }
        break;
    case 10:
        if (sub_02075178((LinkWork *)work) == TRUE) {
            work->state++;
        }
        break;
    case 11:
        goto common;
    case 12:
        if (sub_020751B8((LinkWork *)work) == TRUE) {
            sub_02037AC0(0x36);
            work->state++;
        }
        break;
    case 13:
        if (sub_020751DC((LinkWork *)work) == TRUE) {
            work->state++;
        }
        break;
    case 14:
        goto common;
    case 15:
        if (sub_02075220((LinkWork *)work) == TRUE) {
            sub_02037AC0(0x37);
            work->state++;
        }
        break;
    case 16:
        if (sub_02075248((LinkWork *)work) == TRUE) {
            work->state++;
        }
        break;
    case 17:
        goto common;
    case 18:
        if (sub_0207527C((LinkWork *)work) == TRUE) {
            sub_02037AC0(0x38);
            work->state++;
        }
        break;
    case 19:
        if (sub_020752D8((LinkWork *)work) == TRUE) {
            work->state++;
        }
        break;
    case 20:
        goto common;
    case 21:
        if (!(work->setup->battleType & 0x80)) {
            work->state = 33;
        } else if (Comm_GetLocalNetId() != 0) {
            sub_02037AC0(0x39);
            work->state++;
        } else if (sub_0207531C((LinkWork *)work, 1) == TRUE) {
            sub_02037AC0(0x39);
            work->state++;
        }
        break;
    case 22:
        work->syncCount = 1;
        if (Comm_GetLocalNetId() != 0) {
            if (sub_02037B38(0x39) == TRUE) {
                work->state++;
            }
        } else if (sub_02075350((LinkWork *)work, 1, 0x39) == TRUE) {
            work->state++;
        }
        break;
    case 23:
        goto common;
    case 24:
        if (Comm_GetLocalNetId() != 0) {
            sub_02037AC0(0x3A);
            work->state++;
        } else if (sub_0207531C((LinkWork *)work, 3) == TRUE) {
            sub_02037AC0(0x3A);
            work->state++;
        }
        break;
    case 25:
        work->syncCount = 1;
        if (Comm_GetLocalNetId() != 0) {
            if (sub_02037B38(0x3A) == TRUE) {
                work->state++;
            }
        } else if (sub_02075350((LinkWork *)work, 3, 0x3A) == TRUE) {
            work->state++;
        }
        break;
    case 26:
        goto common;
    case 27:
        if (Comm_GetLocalNetId() != 0) {
            sub_02037AC0(0x3B);
            work->state++;
        } else if (sub_020753A8((LinkWork *)work, 1) == TRUE) {
            sub_02037AC0(0x3B);
            work->state++;
        }
        break;
    case 28:
        work->syncCount = 1;
        if (Comm_GetLocalNetId() != 0) {
            if (sub_02037B38(0x3B) == TRUE) {
                work->state++;
            }
        } else if (sub_020753D4((LinkWork *)work, 1, 0x3B) == TRUE) {
            work->state++;
        }
        break;
    case 29:
        goto common;
    case 30:
        if (Comm_GetLocalNetId() != 0) {
            sub_02037AC0(0x3C);
            work->state++;
        } else if (sub_020753A8((LinkWork *)work, 3) == TRUE) {
            sub_02037AC0(0x3C);
            work->state++;
        }
        break;
    case 31:
        work->syncCount = 1;
        if (Comm_GetLocalNetId() != 0) {
            if (sub_02037B38(0x3C) == TRUE) {
                work->state++;
            }
        } else if (sub_020753D4((LinkWork *)work, 3, 0x3C) == TRUE) {
            work->state++;
        }
        break;
    case 32:
common:
        if (work->syncCount == Comm_CountConnected()) {
            if (work->state == 20) {
                for (i = 0; i < 4; i++) {
                    Heap_Free(((LinkWork *)work)->friends[i]);
                }
            }
            work->syncCount = 0;
            work->timer = 0;
            work->state++;
            if (work->state == 33) {
                PaletteData_BeginPaletteFade(work->palette, 5, 0xFFFF, 0, 0, 0x10, 0);
            }
        } else {
            work->timer++;
            if (work->timer > 0x708) {
                sub_02039AD8(1);
            }
        }
        break;
    case 33:
        if (PaletteData_GetSelectedBuffersBitmask(work->palette) == 0) {
            done = TRUE;
            WaitingIcon_Delete(work->waitingIcon);
            sub_02037930(0);
        }
        break;
    default:
        goto finish;
    }
finish:
    return done;
}

void Battle_LinkExit(OverlayManager *man) {
    BattleLinkStageWork *work = OverlayManager_GetData(man);

    Main_SetVBlankIntrCB(NULL, NULL);
    SetMasterBrightnessByColor(0, 0);
    PaletteData_FreeBuffers(work->palette, PLTTBUF_MAIN_BG);
    PaletteData_Free(work->palette);
    WindowArray_Delete(work->window, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(work->bgConfig, GF_BG_LYR_MAIN_1);
    Heap_Free(work->bgConfig);
    Heap_Free(work);
}

#pragma force_active on

BOOL ov12_0223A0D4(OverlayManager *man) {
    BattleSystem *battleSystem = OverlayManager_CreateAndGetData(man, sizeof(BattleSystem), HEAP_ID_BATTLE);
    BattleSetup *setup = OverlayManager_GetArgs(man);
    u8 netId;

    MIi_CpuClearFast(0, (u32 *)battleSystem, sizeof(BattleSystem));
    BattleSystem_InitFromSetup(battleSystem, setup);
    if (!(battleSystem->battleType & BATTLE_TYPE_LINK) || (battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING) || (battleSystem->battleType & BATTLE_TYPE_FRONTIER)) {
        WirelessStatusIcon_Destroy();
        return FALSE;
    }
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_5), OVY_LOAD_ASYNC);
    if (!(battleSystem->battleType & BATTLE_TYPE_MULTI)) {
        return FALSE;
    }
    battleSystem->unk1C4 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleLinkScreenArgs));
    MIi_CpuClearFast(0, battleSystem->unk1C4, sizeof(BattleLinkScreenArgs));
    netId = Comm_GetLocalNetId();
    switch (sub_020378AC(netId)) {
    case 0:
    case 3:
        LINK_SCREEN_ARGS(battleSystem)->parties[0] = battleSystem->trainerParty[netId];
        LINK_SCREEN_ARGS(battleSystem)->parties[2] = battleSystem->trainerParty[BattleSystem_GetBattlerIdPartner(battleSystem, netId)];
        break;
    case 1:
    case 2:
        LINK_SCREEN_ARGS(battleSystem)->parties[0] = battleSystem->trainerParty[BattleSystem_GetBattlerIdPartner(battleSystem, netId)];
        LINK_SCREEN_ARGS(battleSystem)->parties[2] = battleSystem->trainerParty[netId];
        break;
    }
    LINK_SCREEN_ARGS(battleSystem)->heapId = HEAP_ID_BATTLE;
    LINK_SCREEN_ARGS(battleSystem)->mode = 0;
    switch (sub_020378AC(netId)) {
    case 0:
    case 3:
        LINK_SCREEN_ARGS(battleSystem)->multiOrSide = 0;
        break;
    case 1:
    case 2:
        LINK_SCREEN_ARGS(battleSystem)->multiOrSide = 1;
        break;
    }
    LinkBattleScreen_Start(LINK_SCREEN_ARGS(battleSystem));
    return TRUE;
}

#pragma force_active reset

BOOL ov12_0223A218(OverlayManager *man) {
    BattleSystem *battleSystem = OverlayManager_GetData(man);
    int i;

    if (LINK_SCREEN_ARGS(battleSystem)->done) {
        for (i = 0; i < 4; i++) {
            if (LINK_SCREEN_ARGS(battleSystem)->names[i] != NULL) {
                Heap_Free(LINK_SCREEN_ARGS(battleSystem)->names[i]);
            }
        }
        Heap_Free(battleSystem->unk1C4);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL ov12_0223A260(OverlayManager *man) {
    BattleSystem *battleSystem = OverlayManager_GetData(man);
    u8 netId;
    int i;
    int slot;

    OverlayManager_GetArgs(man);
    if (!(battleSystem->battleType & BATTLE_TYPE_LINK) || (battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING) || (battleSystem->battleType & BATTLE_TYPE_FRONTIER)) {
        WirelessStatusIcon_Destroy();
        return FALSE;
    }
    netId = Comm_GetLocalNetId();
    battleSystem->unk1C4 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleLinkScreenArgs));
    MI_CpuClearFast(battleSystem->unk1C4, sizeof(BattleLinkScreenArgs));
    if (battleSystem->battleType & BATTLE_TYPE_MULTI) {
        BattleSystem *cursor;
        for (i = 0, cursor = battleSystem; i < 4; cursor = (BattleSystem *)((u8 *)cursor + 4), i++) {
            LINK_SCREEN_ARGS(battleSystem)->parties[sub_020378AC(i)] = cursor->trainerParty[0];
            slot = sub_020378AC(i);
            LINK_SCREEN_ARGS(battleSystem)->names[slot] = PlayerProfile_GetPlayerName_NewString(cursor->playerProfile[0], HEAP_ID_BATTLE);
        }
        LINK_SCREEN_ARGS(battleSystem)->heapId = HEAP_ID_BATTLE;
        LINK_SCREEN_ARGS(battleSystem)->mode = 1;
        LINK_SCREEN_ARGS(battleSystem)->multiOrSide = 1;
    } else {
        int other;
        LINK_SCREEN_ARGS(battleSystem)->parties[sub_020378AC(netId)] = battleSystem->trainerParty[netId];
        other = netId ^ 1;
        LINK_SCREEN_ARGS(battleSystem)->parties[sub_020378AC(other)] = battleSystem->trainerParty[other];
        slot = sub_020378AC(netId);
        LINK_SCREEN_ARGS(battleSystem)->names[slot] = PlayerProfile_GetPlayerName_NewString(battleSystem->playerProfile[netId], HEAP_ID_BATTLE);
        slot = sub_020378AC(other);
        LINK_SCREEN_ARGS(battleSystem)->names[slot] = PlayerProfile_GetPlayerName_NewString(battleSystem->playerProfile[other], HEAP_ID_BATTLE);
        LINK_SCREEN_ARGS(battleSystem)->heapId = HEAP_ID_BATTLE;
        LINK_SCREEN_ARGS(battleSystem)->mode = 1;
        LINK_SCREEN_ARGS(battleSystem)->multiOrSide = 0;
    }
    LinkBattleScreen_Start(LINK_SCREEN_ARGS(battleSystem));
    return TRUE;
}

#pragma force_active reset

BOOL ov12_0223A3A8(OverlayManager *man) {
    BattleSystem *battleSystem = OverlayManager_GetData(man);
    int i;

    if (LINK_SCREEN_ARGS(battleSystem)->done) {
        for (i = 0; i < 4; i++) {
            if (LINK_SCREEN_ARGS(battleSystem)->names[i] != NULL) {
                Heap_Free(LINK_SCREEN_ARGS(battleSystem)->names[i]);
            }
        }
        Heap_Free(battleSystem->unk1C4);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL Battle_EndInit(OverlayManager *man) {
    BattleSetup *setup = OverlayManager_GetArgs(man);
    int netId;
    int i;
    int offset;
    int other;
    int slot;
    BattleLinkScreenArgs *args;

    if (!(setup->battleType & BATTLE_TYPE_LINK) || (setup->battleSpecial & BATTLE_SPECIAL_RECORDING) || (setup->battleType & BATTLE_TYPE_FRONTIER)) {
        return FALSE;
    }
    netId = (u8)Comm_GetLocalNetId();
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_5), OVY_LOAD_ASYNC);
    args = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleLinkScreenArgs));
    setup->evolutionTaskData = (EvolutionTaskData *)args;
    MIi_CpuClearFast(0, (u32 *)args, sizeof(BattleLinkScreenArgs));
    args->setup = setup;
    switch (setup->winFlag) {
    case 1:
        if (!sub_02039998()) {
            GameStats_Inc(setup->gameStats, 22);
        } else {
            GameStats_Inc(setup->gameStats, 27);
        }
        break;
    case 2:
        if (!sub_02039998()) {
            GameStats_Inc(setup->gameStats, 23);
        } else {
            GameStats_Inc(setup->gameStats, 28);
        }
        break;
    case 3:
    case 5:
        if (!sub_02039998()) {
            GameStats_Inc(setup->gameStats, 24);
        } else {
            GameStats_Inc(setup->gameStats, 29);
        }
        break;
    case 0:
    case 4:
        break;
    }
    if (setup->battleType & 8) {
        for (i = 0; i < 4; i++) {
            args->parties[sub_020378AC(i)] = setup->party[i];
            slot = sub_020378AC(i);
            args->names[slot] = PlayerProfile_GetPlayerName_NewString(setup->profile[i], HEAP_ID_BATTLE);
        }
        args->heapId = HEAP_ID_BATTLE;
        args->mode = 2;
        args->multiOrSide = 1;
        if (setup->winFlag != 5) {
            args->outcome = setup->winFlag;
        } else {
            args->outcome = 3;
        }
    } else {
        offset = netId;
        args->parties[sub_020378AC(netId)] = setup->party[offset];
        other = netId ^ 1;
        args->parties[sub_020378AC(other)] = setup->party[other];
        slot = sub_020378AC(netId);
        args->names[slot] = PlayerProfile_GetPlayerName_NewString(setup->profile[offset], HEAP_ID_BATTLE);
        slot = sub_020378AC(other);
        args->names[slot] = PlayerProfile_GetPlayerName_NewString(setup->profile[other], HEAP_ID_BATTLE);
        args->heapId = HEAP_ID_BATTLE;
        args->mode = 2;
        args->multiOrSide = 0;
        if (setup->winFlag != 5) {
            args->outcome = setup->winFlag;
        } else {
            args->outcome = 3;
        }
    }
    args->unk_2C = setup->unk1B2;
    LinkBattleScreen_Start(args);
    return TRUE;
}

#pragma force_active reset

BOOL Battle_EndMain(OverlayManager *man) {
    BattleSetup *setup = OverlayManager_GetArgs(man);
    BattleLinkScreenArgs *args = (BattleLinkScreenArgs *)setup->evolutionTaskData;
    int i;

    if (args->done) {
        for (i = 0; i < 4; i++) {
            if (args->names[i] != NULL) {
                Heap_Free(args->names[i]);
            }
        }
        Heap_Free(args);
        return TRUE;
    }
    return FALSE;
}

void BattleSystem_SetupTextFlags(BattleSystem *battleSystem) {
    if ((battleSystem->battleType & (BATTLE_TYPE_LINK | BATTLE_TYPE_TUTORIAL)) != 0 || (battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING) != 0) {
        TextFlags_SetAutoScrollParam(1);
        TextFlags_SetCanABSpeedUpPrint(TRUE);
        TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    } else {
        TextFlags_SetAutoScrollParam(3);
        TextFlags_SetCanABSpeedUpPrint(TRUE);
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    }
}

static inline void SwapInts(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

#pragma force_active on

void ov12_0223A664(BattleSystem *battleSystem, BattleSetup *setup) {
    int ids[4];
    int values[4];
    int netId;
    int count;
    int i;
    int j;

    if (battleSystem->battleSpecial & BATTLE_SPECIAL_RECORDING) {
        battleSystem->unk23FC = 1;
        return;
    }
    for (i = 0; i < 4; i++) {
        ids[i] = i;
        values[i] = setup->aiFlags[i];
    }
    netId = Comm_GetLocalNetId();
    count = Comm_CountConnected();
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (values[i] < values[j]) {
                SwapInts(&ids[i], &ids[j]);
                SwapInts(&values[i], &values[j]);
            }
        }
    }
    if (ids[0] == netId) {
        battleSystem->unk23FC = 1;
        return;
    }
    if (!(battleSystem->battleType & BATTLE_TYPE_FRONTIER)) {
        if (battleSystem->battleType & BATTLE_TYPE_MULTI) {
            int side = sub_020378AC(netId);
            switch (sub_020378AC(ids[0])) {
            case 0:
            case 2:
                if (side & 1) {
                    battleSystem->battleSpecial |= 0x20;
                }
                break;
            case 1:
            case 3:
                if (!(side & 1)) {
                    battleSystem->battleSpecial |= 0x20;
                }
                break;
            }
        } else {
            battleSystem->battleSpecial |= 0x20;
        }
    }
}

#pragma force_active reset

void ov12_0223A7A0(void) {
    WirelessStatusIcon_Create();
    if (sub_02039998()) {
        sub_0203A930(3 - ov00_021EC9D4());
    } else if (sub_02035650()) {
        sub_0203A930(3 - WM_GetLinkLevel());
    }
}
