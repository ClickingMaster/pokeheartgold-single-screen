// Pokewalker connection app (main menu option): overlay manager callbacks, graphics/message setup and teardown,
// and the start of the IR link with the player's identity.

#include "application/pokewalker/pokewalker_app.h"

#include "global.h"

#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "main.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "system.h"
#include "title_screen.h"
#include "touchpad.h"
#include "vram_transfer_manager.h"

typedef int (*PokewalkerAppModeFunc)(PokewalkerApp *app);

// Mode table: {enter, update, exit} for each mode (asm data). Each column is reached through its own label.
typedef struct PokewalkerAppModeEntry {
    PokewalkerAppModeFunc func;
    PokewalkerAppModeFunc other[2];
} PokewalkerAppModeEntry;

extern const PokewalkerAppModeEntry ov112_021FF54C[]; // enter
extern const PokewalkerAppModeEntry ov112_021FF550[]; // update
extern const PokewalkerAppModeEntry ov112_021FF554[]; // exit
extern const GraphicsModes ov112_021F3684;
extern const GraphicsBanks ov112_021F36DC;
extern const BgTemplate ov112_021F3704;
extern const BgTemplate ov112_021F3720;
extern const BgTemplate ov112_021F373C;
extern const BgTemplate ov112_021F3758;
extern const BgTemplate ov112_021F3774;
extern const BgTemplate ov112_021F3790;
BOOL PokewalkerApp_Init(OverlayManager *manager, int *state) {
    PokewalkerApp *app;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(volatile u32 *)0x04000000 &= 0xFFFFE0FF;
    *(volatile u32 *)0x04001000 &= 0xFFFFE0FF;
    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04001050 = 0;
    Sys_SetSoftResetDisableFlag(0x10);
    Heap_Create(HEAP_ID_3, HEAP_ID_154, 0x70000);
    app = OverlayManager_CreateAndGetData(manager, sizeof(PokewalkerApp), HEAP_ID_154);
    memset(app, 0, sizeof(PokewalkerApp));
    app->saveData = ((void **)OverlayManager_GetArgs(manager))[2];
    PokewalkerApp_GetSaveData(app);
    SetKeyRepeatTimers(3, 8);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    PokewalkerApp_Setup(app);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    return TRUE;
}

void PokewalkerApp_Setup(PokewalkerApp *app) {
    app->bgConfig = BgConfig_Alloc(HEAP_ID_154);
    FontID_Alloc(4, HEAP_ID_154);
    PokewalkerApp_InitMessages(app);
    PokewalkerApp_SetGxBanks();
    PokewalkerApp_InitBgs(app->bgConfig);
    PokewalkerApp_LoadBgGraphics(app);
    PokewalkerApp_AddWindowBank(app, 0);
    PokewalkerApp_AddWindowBank(app, 1);
    PokewalkerApp_InitSprites(app);
    PokewalkerApp_ResetCursor(app);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    PokewalkerApp_LoadStrings(app);
    Main_SetVBlankIntrCB(PokewalkerApp_VBlankCB, app);
    *(volatile u16 *)0x04000304 &= 0xFFFF7FFF;
    ToggleBgLayer(4, 1);
}

void PokewalkerApp_Teardown(PokewalkerApp *app) {
    PokewalkerApp_FreeSprites(app);
    PokewalkerApp_RemoveWindowBank(app, 1);
    PokewalkerApp_RemoveWindowBank(app, 0);
    PokewalkerApp_FreeBgs(app->bgConfig);
    GF_TouchpadStopSampling();
    GF_DestroyVramTransferManager();
    PokewalkerApp_FreeStrings(app);
    PokewalkerApp_FreeMessages(app);
    FontID_Release(4);
    Main_SetVBlankIntrCB(NULL, NULL);
    app->spriteManager = NULL;
}

#pragma force_active on

BOOL PokewalkerApp_Main(OverlayManager *manager, int *state) {
    PokewalkerApp *app = OverlayManager_GetData(manager);
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished() == TRUE) {
            *state = 1;
        }
        break;
    case 1:
        *state = ov112_021FF54C[app->mode].func(app);
        break;
    case 2:
        *state = ov112_021FF550[app->mode].func(app);
        BgSetPosTextAndCommit(app->bgConfig, 5, BG_POS_OP_SET_X, app->subBgScrollX);
        break;
    case 3:
        *state = ov112_021FF554[app->mode].func(app);
        if (app->nextMode == 11) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 6, 1, HEAP_ID_154);
            *state = 4;
        } else {
            app->mode = app->nextMode;
            ov112_021E9C94(app);
        }
        break;
    case 4:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    if (app->spriteManager) {
        SpriteSystem_DrawSprites(app->spriteManager);
    }
    return FALSE;
}

#pragma force_active reset

BOOL PokewalkerApp_Exit(OverlayManager *manager, int *state) {
    PokewalkerApp *app = OverlayManager_GetData(manager);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    PokewalkerApp_Teardown(app);
    OverlayManager_FreeData(manager);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_154);
    Sys_ClearSoftResetDisableFlag(0x10);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    return TRUE;
}

void PokewalkerApp_GetSaveData(PokewalkerApp *app) {
    app->bag = Save_Bag_Get(app->saveData);
    app->profile = Save_PlayerData_GetProfile(app->saveData);
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->pokewalker = Save_Pokewalker_Get(app->saveData);
    app->pokedex = Save_Pokedex_Get(app->saveData);
}

// OS_SetIrqCheckFlag(OS_IE_V_BLANK) with the DTCM base passed in (HW_INTR_CHECK_BUF = DTCM + 0x3FF8).
static inline void SetIrqCheckFlag(u32 *dtcm) {
    dtcm[0xFFE] |= OS_IE_V_BLANK;
}

#pragma force_active on

void PokewalkerApp_VBlankCB(void *arg) {
    PokewalkerApp *app = arg;
    DoScheduledBgGpuUpdates(app->bgConfig);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    SetIrqCheckFlag((u32 *)0x027E0000);
}

#pragma force_active reset

void PokewalkerApp_SetGxBanks(void) {
    GraphicsBanks banks = ov112_021F36DC;
    GfGfx_SetBanks(&banks);
}

void PokewalkerApp_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes = ov112_021F3684;
    SetBothScreensModesAndDisable(&modes);
    InitBgFromTemplate(bgConfig, 1, &ov112_021F3704, 0);
    InitBgFromTemplate(bgConfig, 2, &ov112_021F3720, 0);
    InitBgFromTemplate(bgConfig, 3, &ov112_021F373C, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 1);
    BgClearTilemapBufferAndCommit(bgConfig, 2);
    BgClearTilemapBufferAndCommit(bgConfig, 3);
    InitBgFromTemplate(bgConfig, 4, &ov112_021F3758, 0);
    InitBgFromTemplate(bgConfig, 5, &ov112_021F3774, 0);
    InitBgFromTemplate(bgConfig, 6, &ov112_021F3790, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 4);
    BgClearTilemapBufferAndCommit(bgConfig, 5);
    BgClearTilemapBufferAndCommit(bgConfig, 6);
    BG_ClearCharDataRange(4, 0x20, 0, HEAP_ID_154);
    BG_ClearCharDataRange(5, 0x20, 0, HEAP_ID_154);
    BG_ClearCharDataRange(6, 0x20, 0, HEAP_ID_154);
}

void PokewalkerApp_FreeBgs(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
    FreeBgTilemapBuffer(bgConfig, 6);
    FreeBgTilemapBuffer(bgConfig, 5);
    FreeBgTilemapBuffer(bgConfig, 4);
    FreeBgTilemapBuffer(bgConfig, 3);
    FreeBgTilemapBuffer(bgConfig, 2);
    FreeBgTilemapBuffer(bgConfig, 1);
    Heap_FreeExplicit(HEAP_ID_154, bgConfig);
}

#pragma force_active on

void PokewalkerApp_LoadBgGraphics(PokewalkerApp *app) {
    NARC *narc = NARC_New(NARC_a_2_4_7, HEAP_ID_154);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_154);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, app->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_154);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_154);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_154);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_MAIN_1, 994, 13, (u8)Options_GetFrame(app->options), HEAP_ID_154);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 1, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_154);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_154);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_154);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 9, app->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_154);
    LoadUserFrameGfx2(app->bgConfig, GF_BG_LYR_SUB_0, 994, 13, (u8)Options_GetFrame(app->options), HEAP_ID_154);
    NARC_Delete(narc);
}

#pragma force_active reset

void PokewalkerApp_InitMessages(PokewalkerApp *app) {
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x112, HEAP_ID_154);
    app->msgDataItemNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xDE, HEAP_ID_154);
    app->msgFormat = MessageFormat_New_Custom(16, 32, HEAP_ID_154);
    app->string = String_New(400, HEAP_ID_154);
}

void PokewalkerApp_FreeMessages(PokewalkerApp *app) {
    String_Delete(app->string);
    DestroyMsgData(app->msgDataItemNames);
    DestroyMsgData(app->msgData);
    MessageFormat_Delete(app->msgFormat);
}

void PokewalkerApp_LoadTilemap(PokewalkerApp *app, int bgId, int member) {
    GfGfxLoader_LoadScrnData(NARC_a_2_4_7, member, app->bgConfig, (GFBgLayer)bgId, 0, 0, TRUE, HEAP_ID_154);
}

#pragma force_active on

void PokewalkerApp_StartLink(PokewalkerApp *app) {
    u8 *p = (u8 *)app;
    String *name = PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(app->saveData), HEAP_ID_154);
    app->identity.unk_00 = 1;
    app->identity.unk_08 = GAME_VERSION;
    p[0x107F] &= ~1;
    p[0x107E] = 0;
    app->identity.unk_0C = PlayerProfile_GetTrainerID(app->profile);
    CopyStringToU16Array(name, (u16 *)(p + 0x106C), 8);
    app->identity.unk_04 = 1;
    app->identity.unk_0A = GAME_VERSION;
    MI_CpuCopyFast(Pokewalker_GetIrClock(app->pokewalker), &app->identity.clock, sizeof(PokewalkerIrClock));
    PokewalkerIr_Start(app->irThreadStack, sizeof(app->irThreadStack), 15);
    PokewalkerIr_SetIdentities(&app->identity, &app->peerIdentity);
    PokewalkerIr_SetCompressBuffer(app->irCompressBuffer);
    String_Delete(name);
}

#pragma force_active reset

void PokewalkerApp_BuildUploadData(PokewalkerApp *app) {
    PokewalkerApp_BuildUploadImage(app, app->uploadImage);
    PokewalkerApp_BuildStrollData(&app->strollData, app);
    ov112_021E7DC0(app);
}

void PokewalkerApp_BuildSendStrollData(PokewalkerApp *app) {
    PokewalkerApp_BuildStrollData(&app->strollData, app);
    ov112_021E7DC0(app);
}

void ov112_021E7DC0(PokewalkerApp *app) {
    void *p;
    PokewalkerApp_BuildTrainerHouseData(app->saveData, app->unk_F200);
    app->unk_F1D0 = 1;
    app->unk_F1D4 = GAME_VERSION;
    p = Pokewalker_GetIrClock(app->pokewalker);
    MIi_CpuCopyFast(p, (u32 *)&app->unk_F1D8, sizeof(PokewalkerIrClock));
}
