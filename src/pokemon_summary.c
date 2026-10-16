#include "global.h"

#include "constants/moves.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"

#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "pokemon_summary_internal.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite_system.h"
#include "sys_flags.h"
#include "system.h"
#include "touchpad.h"
#include "ui_util.h"
#include "pokemon_summary.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"

// Pokemon summary screen (ARM9 static 0x02088288-0x02088D48): the overlay manager's init, main and exit, the
// screen's graphics setup and the first input states. Tables: asm/pokemon_summary_data.s (before this TU).

extern const GraphicsBanks gPokemonSummaryGraphicsBanks;
extern const GraphicsModes gPokemonSummaryGraphicsModes;
extern const BgTemplate gPokemonSummaryBgTemplate1;
extern const BgTemplate gPokemonSummaryBgTemplate2;
extern const BgTemplate gPokemonSummaryBgTemplate3;
extern const BgTemplate gPokemonSummaryBgTemplate4;
extern const BgTemplate gPokemonSummaryBgTemplate5;
extern const BgTemplate gPokemonSummaryBgTemplate6;

// Elsewhere in the summary screen, declared with the types their call sites were matched against.
void *PokepicAnimManager_New(enum HeapID heapId, int count, int flag);
void PokemonSummary_Init3D(PokemonSummary *summary);
void PokemonSummary_CreatePokepic(PokemonSummary *summary);
void PokemonSummary_CreateCamera(PokemonSummary *summary);
void PokemonSummary_StartPokepicAnim(PokemonSummary *summary);
void PokemonSummary_DrawPokepic(PokemonSummary *summary);
void PokemonSummary_FreePokepicAndCamera(PokemonSummary *summary);

u32 PokemonSummary_GetDefaultUnk2C(SaveData *saveData) {
    return 1;
}

BOOL PokemonSummary_IsPokeathlonUnlocked(SaveData *saveData) {
    return CheckFlag982(Save_VarsFlags_Get(saveData));
}

BOOL PokemonSummary_Init(OverlayManager *manager, int *state) {
    NARC *ribbonNarc;
    NARC *graphicsNarc;
    PokemonSummary *summary;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    SetKeyRepeatTimers(4, 8);
    Heap_Create(HEAP_ID_3, HEAP_ID_19, 0x45000);
    ribbonNarc = NARC_New(NARC_a_0_3_9, HEAP_ID_19);
    graphicsNarc = NARC_New(NARC_a_1_6_2, HEAP_ID_19);
    summary = OverlayManager_CreateAndGetData(manager, sizeof(PokemonSummary), HEAP_ID_19);
    memset(summary, 0, sizeof(PokemonSummary));
    summary->args = OverlayManager_GetArgs(manager);
    summary->bgConfig = BgConfig_Alloc(HEAP_ID_19);
    *(u32 *)summary->args->monModified = 0;
    summary->pokepicAnimManager = PokepicAnimManager_New(HEAP_ID_19, 1, 1);
    summary->pokepicAnimNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_19);
    FontID_SetAccessDirect(0, HEAP_ID_19);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    PokemonSummary_SetGraphicsBanks();
    PokemonSummary_InitBgs(summary->bgConfig);
    PokemonSummary_LoadBgGraphics(summary, ribbonNarc, graphicsNarc);
    PokemonSummary_SetBlend();
    PokemonSummary_Init3D(summary);
    FontID_Alloc(4, HEAP_ID_19);
    PokemonSummary_InitText(summary);
    PokemonSummary_InitBgCells(summary, graphicsNarc);
    PokemonSummary_ReadShownMon(summary);
    PokemonSummary_InitSpriteSystem(summary);
    PokemonSummary_CreateSprites(summary);
    PokemonSummary_CreatePokepic(summary);
    PokemonSummary_LoadBallSprite(summary);
    PokemonSummary_UpdateStatusSprite(summary);
    PokemonSummary_LoadPartyIcons(summary);
    PokemonSummary_AddWindows(summary);
    PokemonSummary_ShowFirstPage(summary);
    PokemonSummary_CreateCamera(summary);
    Main_SetVBlankIntrCB(PokemonSummary_VBlank, summary);
    reg_GX_POWCNT |= 0x8000;
    GfGfx_BothDispOn();
    Sound_SetSceneAndPlayBGM(61, 0, 0);
    WirelessStatusIcon_CreateIfConnected();
    NARC_Delete(graphicsNarc);
    NARC_Delete(ribbonNarc);
    return TRUE;
}

BOOL PokemonSummary_Main(OverlayManager *manager, int *state) {
    PokemonSummary *summary = OverlayManager_GetData(manager);

    switch (*state) {
    case POKEMON_SUMMARY_STATE_START_FADE_IN:
        BeginAppScreenFade(0, HEAP_ID_19);
        Pokepic_SetAttr(summary->pokepic, POKEPIC_VANISHED, 0);
        *state = POKEMON_SUMMARY_STATE_WAIT_FADE_IN;
        break;
    case POKEMON_SUMMARY_STATE_WAIT_FADE_IN:
        *state = PokemonSummary_WaitFadeIn(summary);
        break;
    case POKEMON_SUMMARY_STATE_INPUT:
        *state = PokemonSummary_HandleInput(summary);
        break;
    case POKEMON_SUMMARY_STATE_SLIDE_IN_MOVE_INFO:
        *state = PokemonSummary_WaitSlideInMoveInfo(summary);
        break;
    case POKEMON_SUMMARY_STATE_SLIDE_OUT_MOVE_INFO:
        *state = PokemonSummary_WaitSlideOutMoveInfo(summary);
        break;
    case POKEMON_SUMMARY_STATE_MOVE_SELECT_INPUT:
        *state = PokemonSummary_HandleMoveSelectInput(summary);
        break;
    case POKEMON_SUMMARY_STATE_START_MOVE_SWAP:
        *state = PokemonSummary_StartMoveSwap(summary);
        break;
    case POKEMON_SUMMARY_STATE_MOVE_SWAP_INPUT:
        *state = PokemonSummary_HandleMoveSwapInput(summary);
        break;
    case POKEMON_SUMMARY_STATE_FORGET_MOVE_INPUT:
        *state = PokemonSummary_HandleForgetMoveInput(summary);
        break;
    case POKEMON_SUMMARY_STATE_FORGET_MOVE_CONFIRM_INPUT:
        *state = PokemonSummary_HandleForgetMoveConfirmInput(summary);
        break;
    case POKEMON_SUMMARY_STATE_OPEN_RIBBON_LIST:
        *state = PokemonSummary_OpenRibbonList(summary);
        break;
    case POKEMON_SUMMARY_STATE_CLOSE_RIBBON_LIST:
        *state = PokemonSummary_CloseRibbonList(summary);
        break;
    case POKEMON_SUMMARY_STATE_RIBBON_INPUT:
        *state = PokemonSummary_HandleRibbonInput(summary);
        break;
    case POKEMON_SUMMARY_STATE_RIBBON_PAGE_UP:
        *state = PokemonSummary_WaitRibbonPageUp(summary);
        break;
    case POKEMON_SUMMARY_STATE_RIBBON_PAGE_DOWN:
        *state = PokemonSummary_WaitRibbonPageDown(summary);
        break;
    case POKEMON_SUMMARY_STATE_FLASH_BUTTON:
        *state = PokemonSummary_FlashButton(summary);
        break;
    case POKEMON_SUMMARY_STATE_UPDATE_CONDITION:
        *state = PokemonSummary_UpdateCondition(summary);
        break;
    case POKEMON_SUMMARY_STATE_SHOW_CONDITION_CHANGES:
        *state = PokemonSummary_ShowConditionChanges(summary);
        break;
    case POKEMON_SUMMARY_STATE_WAIT_CONDITION_EXIT:
        *state = PokemonSummary_WaitConditionExit(summary);
        break;
    case POKEMON_SUMMARY_STATE_START_FADE_OUT:
        *state = PokemonSummary_StartFadeOut(summary);
        break;
    case POKEMON_SUMMARY_STATE_WAIT_FADE_OUT:
        if (PokemonSummary_WaitFadeOut(summary) == 1) {
            return TRUE;
        }
        break;
    case POKEMON_SUMMARY_STATE_SHOW_POKEPIC:
        Pokepic_SetAttr(summary->pokepic, POKEPIC_VANISHED, 0);
        *state = POKEMON_SUMMARY_STATE_INPUT;
        break;
    case POKEMON_SUMMARY_STATE_CHANGE_MON_BY_BUTTON:
        *state = PokemonSummary_ChangeMonByButton(summary);
        break;
    }
    PokemonSummary_UpdateSpriteAnims(summary);
    PokemonSummary_AnimateRibbonSprite66(summary);
    SpriteSystem_DrawSprites(summary->spriteManager);
    PokemonSummary_DrawPokepic(summary);
    return FALSE;
}

BOOL PokemonSummary_Exit(OverlayManager *manager, int *state) {
    PokemonSummary *summary = OverlayManager_GetData(manager);

    Main_SetVBlankIntrCB(NULL, NULL);
    PokemonSummary_FreePokepicAndCamera(summary);
    PokemonSummary_FreeSpriteSystem(summary);
    PokemonSummary_RemoveWindows(summary);
    PokemonSummary_FreeBgs(summary->bgConfig);
    GF_TouchpadStopSampling();
    GF_DestroyVramTransferManager();
    PokemonSummary_FreeBgCells(summary);
    PokemonSummary_FreeText(summary);
    FontID_Release(4);
    NARC_Delete(summary->pokepicAnimNarc);
    FontID_SetAccessLazy(0);
    reg_G2_BLDCNT = 0;
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_19);
    return TRUE;
}

void PokemonSummary_VBlank(void *data) {
    PokemonSummary *summary = data;
    DoScheduledBgGpuUpdates(summary->bgConfig);
    PokepicManager_HandleLoadImgAndOrPltt(summary->pokepicManager);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    *(u32 *)((u8 *)OS_IRQTable + 0x3FF8) |= OS_IE_VBLANK; // OS_SetIrqCheckFlag
}

void PokemonSummary_SetGraphicsBanks(void) {
    GraphicsBanks banks = gPokemonSummaryGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void PokemonSummary_InitBgs(BgConfig *bgConfig) {
    {
        GraphicsModes modes = gPokemonSummaryGraphicsModes;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        BgTemplate template = gPokemonSummaryBgTemplate1;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    }
    {
        BgTemplate template = gPokemonSummaryBgTemplate2;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
        ScheduleSetBgPosText(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
    }
    {
        BgTemplate template = gPokemonSummaryBgTemplate3;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &template, GF_BG_TYPE_TEXT);
    }
    {
        BgTemplate template = gPokemonSummaryBgTemplate4;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    {
        BgTemplate template = gPokemonSummaryBgTemplate5;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    }
    {
        BgTemplate template = gPokemonSummaryBgTemplate6;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &template, GF_BG_TYPE_TEXT);
    }
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_19);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_19);
}

void PokemonSummary_FreeBgs(BgConfig *bgConfig) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    Heap_FreeExplicit(HEAP_ID_19, bgConfig);
}

void PokemonSummary_LoadBgGraphics(PokemonSummary *summary, NARC *unused, NARC *narc) {
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x200, HEAP_ID_19);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, summary->bgConfig, GF_BG_LYR_MAIN_3, 0, 0x4400, FALSE, HEAP_ID_19);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 13, summary->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_19);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200, HEAP_ID_19);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, summary->bgConfig, GF_BG_LYR_SUB_2, 0, 0x4400, FALSE, HEAP_ID_19);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 20, summary->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_19);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 21, summary->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_19);
}

void PokemonSummary_SetBlend(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 23, 8);
}

void PokemonSummary_InitText(PokemonSummary *summary) {
    summary->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0302_bin, HEAP_ID_19);
    summary->ribbonMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0424_bin, HEAP_ID_19);
    summary->numberPrinter = MessagePrinter_New(1, 2, 0, HEAP_ID_19);
    summary->msgFormat = MessageFormat_New(HEAP_ID_19);
    summary->mon.speciesName = String_New(12, HEAP_ID_19);
    summary->mon.nickname = String_New(12, HEAP_ID_19);
    summary->mon.otName = String_New(8, HEAP_ID_19);
    summary->buffer = String_New(128, HEAP_ID_19);
    summary->moveNames = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0750_bin, HEAP_ID_19);
    summary->playerName = String_New(8, HEAP_ID_19);
    if (summary->args->playerName != NULL) {
        CopyU16ArrayToString(summary->playerName, summary->args->playerName);
    }
}

void PokemonSummary_FreeText(PokemonSummary *summary) {
    DestroyMsgData(summary->moveNames);
    DestroyMsgData(summary->ribbonMsgData);
    DestroyMsgData(summary->msgData);
    MessagePrinter_Delete(summary->numberPrinter);
    MessageFormat_Delete(summary->msgFormat);
    String_Delete(summary->mon.speciesName);
    String_Delete(summary->mon.nickname);
    String_Delete(summary->mon.otName);
    String_Delete(summary->buffer);
    String_Delete(summary->playerName);
}

void PokemonSummary_InitBgCells(PokemonSummary *summary, NARC *narc) {
    summary->cellManager = BgCellManager_New(summary->bgConfig, 2, 7, HEAP_ID_19);
    BgCellManager_InitCell(summary->cellManager, 0, 6, 17, 6);
    BgCellManager_LoadCellTilemapFromOpenNarc(summary->cellManager, 0, narc, 69, 0);
    BgCellManager_InitCell(summary->cellManager, 1, 6, 17, 6);
    BgCellManager_LoadCellTilemapFromOpenNarc(summary->cellManager, 1, narc, 70, 0);
    BgCellManager_InitCell(summary->cellManager, 2, 6, 10, 4);
    BgCellManager_LoadCellTilemapFromOpenNarc(summary->cellManager, 2, narc, 71, 0);
    BgCellManager_InitCell(summary->cellManager, 3, 6, 10, 4);
    BgCellManager_LoadCellTilemapFromOpenNarc(summary->cellManager, 3, narc, 72, 0);
    BgCellManager_InitCell(summary->cellManager, 4, 6, 11, 14);
    BgCellManager_LoadCellTilemapFromOpenNarc(summary->cellManager, 4, narc, 73, 0);
    BgCellManager_InitCell(summary->cellManager, 5, 6, 6, 3);
    BgCellManager_LoadCellTilemapFromOpenNarc(summary->cellManager, 5, narc, 74, 0);
    BgCellManager_InitCell(summary->cellManager, 6, 6, 6, 3);
    BgCellManager_LoadCellTilemapFromOpenNarc(summary->cellManager, 6, narc, 75, 0);
}

void PokemonSummary_FreeBgCells(PokemonSummary *summary) {
    BgCellManager_Delete(summary->cellManager);
}

// State 1: wait for the fade-in, then go to the screen's starting state.
int PokemonSummary_WaitFadeIn(PokemonSummary *summary) {
    u8 mode;

    if (IsPaletteFadeFinished() == TRUE) {
        PokemonSummary_StartPokepicAnim(summary);
        PokemonSummary_PlayCry(summary);
        mode = summary->args->mode;
        if (mode == 2) {
            return POKEMON_SUMMARY_STATE_FORGET_MOVE_INPUT;
        }
        if (mode == 4) {
            return POKEMON_SUMMARY_STATE_UPDATE_CONDITION;
        }
        return POKEMON_SUMMARY_STATE_INPUT;
    }
    return POKEMON_SUMMARY_STATE_WAIT_FADE_IN;
}

// State 2: the page input.
int PokemonSummary_HandleInput(PokemonSummary *summary) {
    int selection;

    if (summary->exitRequested == 1) {
        summary->args->exitedWithB = 1;
        return POKEMON_SUMMARY_STATE_START_FADE_OUT;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        PokemonSummary_ScrollPage(summary, -1);
        return POKEMON_SUMMARY_STATE_INPUT;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        PokemonSummary_ScrollPage(summary, 1);
        return POKEMON_SUMMARY_STATE_INPUT;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        PokemonSummary_ChangeMon(summary, -1);
        return POKEMON_SUMMARY_STATE_SHOW_POKEPIC;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        PokemonSummary_ChangeMon(summary, 1);
        return POKEMON_SUMMARY_STATE_SHOW_POKEPIC;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        summary->args->exitedWithB = 1;
        PokemonSummary_SetMenuInputState(summary, 0);
        return PokemonSummary_FlashCancelButton(summary, POKEMON_SUMMARY_STATE_START_FADE_OUT);
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        switch (summary->page) {
        case 1:
            PlaySE(SEQ_SE_DP_SYU01);
            summary->moveSlot = 0;
            return POKEMON_SUMMARY_STATE_SLIDE_IN_MOVE_INFO;
        case 2:
            if (summary->ribbonCount != 0) {
                PlaySE(SEQ_SE_DP_DECIDE);
                summary->ribbonCursor = 0;
                return POKEMON_SUMMARY_STATE_OPEN_RIBBON_LIST;
            }
            break;
        }
    }
    if (summary->page == 1) {
        selection = PokemonSummary_GetTouchedMove(summary);
        if (selection != -1 && summary->mon.moves[selection] != MOVE_NONE) {
            PlaySE(SEQ_SE_DP_SYU01);
            summary->moveSlot = selection;
            return POKEMON_SUMMARY_STATE_SLIDE_IN_MOVE_INFO;
        }
    }
    if (summary->page == 2) {
        selection = PokemonSummary_GetRibbonTouch(summary);
        if (selection != -1 && selection < 9 && selection < summary->ribbonCount) {
            PlaySE(SEQ_SE_DP_DECIDE);
            summary->ribbonCursor = selection;
            return POKEMON_SUMMARY_STATE_OPEN_RIBBON_LIST;
        }
    }
    if (summary->args->monDataType == 2) {
        selection = PokemonSummary_GetMonButtonTouch(summary);
        if (selection == 0) {
            if (PokemonSummary_FindNextMon(summary, -1) != -1) {
                PlaySE(SEQ_SE_DP_DECIDE);
                return PokemonSummary_FlashMonButton(summary, 0, POKEMON_SUMMARY_STATE_CHANGE_MON_BY_BUTTON);
            }
            return POKEMON_SUMMARY_STATE_INPUT;
        }
        if (selection == 1) {
            if (PokemonSummary_FindNextMon(summary, 1) != -1) {
                PlaySE(SEQ_SE_DP_DECIDE);
                return PokemonSummary_FlashMonButton(summary, 1, POKEMON_SUMMARY_STATE_CHANGE_MON_BY_BUTTON);
            }
            return POKEMON_SUMMARY_STATE_INPUT;
        }
    }
    return PokemonSummary_HandleTouchInput(summary);
}

int PokemonSummary_WaitSlideInMoveInfo(PokemonSummary *summary) {
    if (PokemonSummary_SlideInMoveInfo(summary) == TRUE) {
        PokemonSummary_DrawSwitchButton(summary, 1);
        return POKEMON_SUMMARY_STATE_MOVE_SELECT_INPUT;
    }
    return POKEMON_SUMMARY_STATE_SLIDE_IN_MOVE_INFO;
}

int PokemonSummary_WaitSlideOutMoveInfo(PokemonSummary *summary) {
    if (PokemonSummary_SlideOutMoveInfo(summary) == TRUE) {
        return POKEMON_SUMMARY_STATE_INPUT;
    }
    return POKEMON_SUMMARY_STATE_SLIDE_OUT_MOVE_INFO;
}
