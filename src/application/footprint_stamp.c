#include "application/footprint_stamp.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0754.h"

#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "sound_playback.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchpad.h"
#include "unk_02009D48.h"
#include "unk_0200FA24.h"
#include "swap_3d_buffers.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"

// Declared only in other modules' private headers.
void *ov45_0222A2C8(void *lobby);
void *ov45_0222A2CC(void *lobby);
void WiFiPlazaWork_SetLowVolume(void *lobby, int a1);
void ov45_0222A4A8(void *lobby);
u32 WiFiPlazaWork_IsClosing(void *lobby);
BOOL WiFiPlaza_HasConnectionFailed(void);
void WiFiPlazaLobbyClient_ClearChannelCommands(void);
void HBlankInterruptDisable(void);
int GetUserFrameGfx2PaletteMember(int frame);

extern const OamManagerParam sFootprintStampOamManagerParam;
extern const OamCharTransferParam sFootprintStampOamCharTransfer;
extern SpriteResourceCountsListUnion sFootprintStampSpriteResourceCounts;
extern const GraphicsBanks sFootprintStampBanks;
extern const GraphicsModes sFootprintStampGraphicsModes;
extern const BgTemplate sFootprintStampMainBgTemplates[3];
extern const BgTemplate sFootprintStampSubBgTemplates[3];
extern const ManagedSpriteTemplate sFootprintStampInkSpriteTemplate;
extern const ManagedSpriteTemplate sFootprintStampSlotSpriteTemplate;
extern const ManagedSpriteTemplate sFootprintStampFootprintSpriteTemplate;

typedef struct FootprintStampBgTemplates {
    BgTemplate items[3];
} FootprintStampBgTemplates;

BOOL FootprintStamp_Init(OverlayManager *man, int *state) {
    FootprintStamp *work;

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
    Heap_Create(HEAP_ID_3, HEAP_ID_125, 0x50000);
    work = OverlayManager_CreateAndGetData(man, sizeof(FootprintStamp), HEAP_ID_125);
    MI_CpuFill8(work, 0, sizeof(FootprintStamp));
    work->args = OverlayManager_GetArgs(man);
    work->saveData = ov45_0222A2C8(work->args->lobby);
    work->unk19E0 = ov45_0222A2CC(work->args->lobby);
    FootprintStamp_InitLinkFilter(work);
    FootprintStamp_LoadParty(work->args->unk04, work->saveData, work->party);
    work->ink = FOOTPRINT_STAMP_INK_MAX;
    work->inkShown = work->ink;
    work->vramMan3d = FootprintStamp_Create3DVramMan(HEAP_ID_125);
    work->plttData = PaletteData_Init(HEAP_ID_125);
    PaletteData_SetAutoTransparent(work->plttData, TRUE);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_125);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_SUB_BG, 0x200, HEAP_ID_125);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_MAIN_OBJ, 0x1C0, HEAP_ID_125);
    PaletteData_AllocBuffers(work->plttData, PLTTBUF_SUB_OBJ, 0x200, HEAP_ID_125);
    PaletteData_SetAutoTransparent(work->plttData, TRUE);
    work->bgConfig = BgConfig_Alloc(HEAP_ID_125);
    GF_CreateVramTransferManager(0x40, HEAP_ID_125);
    SetKeyRepeatTimers(4, 8);
    FootprintStamp_InitBgs(work->bgConfig);
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    FontID_Alloc(2, HEAP_ID_125);
    work->msgFormat = MessageFormat_New(HEAP_ID_125);
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0754_bin, HEAP_ID_125);
    work->fontSystem = FontSystem_NewInit(4, HEAP_ID_125);
    work->narc = NARC_New(NARC_a_2_0_8, HEAP_ID_125);
    work->footprintNarc = NARC_New(NARC_poketool_pokefoot_pokefoot, HEAP_ID_125);
    FootprintStamp_LoadBgGfx(work, work->narc);
    FootprintStamp_LoadBoardModel(work, work->narc);
    FootprintStamp_CreateCamera(work);
    FootprintStamp_CreateWindows(work);
    work->string = String_New(0x100, HEAP_ID_125);
    work->spriteSystem = SpriteSystem_Alloc(HEAP_ID_125);
    SpriteSystem_Init(work->spriteSystem, &sFootprintStampOamManagerParam, &sFootprintStampOamCharTransfer, 0x20);
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    work->spriteManager = SpriteManager_New(work->spriteSystem);
    SpriteSystem_InitSprites(work->spriteSystem, work->spriteManager, 0x80);
    SpriteSystem_InitManagerWithCapacities(work->spriteSystem, work->spriteManager, &sFootprintStampSpriteResourceCounts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, 0x110000);
    WirelessStatusIcon_Create();
    FootprintStampBoard_Init(&work->board, work->unk19E0);
    FootprintStamp_LoadPartySpriteGfx(work, work->narc);
    FootprintStamp_LoadPlayerSpriteGfx(work, work->narc);
    FootprintStamp_CreatePartySprites(work);
    FootprintStamp_CreatePlayerSprites(work);
    FootprintStamp_ShowParty(work);
    work->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_125);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 1, HEAP_ID_125);
    if (work->args->lobby != NULL) {
        WiFiPlazaWork_SetLowVolume(work->args->lobby, 1);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    GfGfx_BothDispOn();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    work->task = SysTask_CreateOnMainQueue(FootprintStamp_MainTask, work, 60000);
    Main_SetVBlankIntrCB(FootprintStamp_VBlank, work);
    return TRUE;
}

#pragma force_active on

BOOL FootprintStamp_Main(OverlayManager *man, int *state) {
    FootprintStamp *work = OverlayManager_GetData(man);
    FootprintStampMon dummy;
    YesNoPromptTemplate template;

    FootprintStampBoard_Update(&work->board, work->camera, work->phase, work->args->unk04);
    switch (*state) {
    case 0:
        work->linkFilter.active = TRUE;
        FootprintStamp_RegisterLinkCommands(work);
        FootprintStampMon_InitDummy(work, &dummy);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            work->phase = 1;
            (*state)++;
        }
        break;
    case 2: {
        int slot;
        if (gSystem.touchNew && gSystem.touchY < 0xA0 && FootprintStamp_UseInk(work, FOOTPRINT_STAMP_INK_PER_STAMP) == TRUE) {
            work->party[work->selectedSlot].x = gSystem.touchX;
            work->party[work->selectedSlot].y = gSystem.touchY;
            FootprintStamp_PlaceStamp(work, &work->party[work->selectedSlot], work->linkFilter.myId);
            FootprintStamp_SendStamp(&work->party[work->selectedSlot]);
        }
        slot = (int)FootprintStamp_GetTouchedPartySlot(work->party, work->selectedSlot);
        if (slot < FOOTPRINT_STAMP_PARTY_SLOTS) {
            work->selectedSlot = slot;
            FootprintStamp_StartSelectEffect(work, slot);
        } else if (*state == 2 && slot == FOOTPRINT_STAMP_TOUCH_EXIT) {
            PlaySE(SEQ_SE_DP_BUTTON3);
            PaletteData_BlendPalette(work->plttData, PLTTBUF_MAIN_BG, 9, 1, 8, RGB_BLACK);
            PaletteData_BlendPalette(work->plttData, PLTTBUF_MAIN_OBJ, (u16)(work->cursorPlttSlot << 4), 16, 8, RGB_BLACK);
            *state = 3;
        }
        break;
    }
    case 3:
        FillWindowPixelBuffer(&work->msgWindow, 15);
        DrawFrameAndWindow2(&work->msgWindow, FALSE, 1, 14);
        ReadMsgDataIntoString(work->msgData, msg_0754_00002, work->string);
        work->printerId = AddTextPrinterParameterized(&work->msgWindow, 1, work->string, 0, 0, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(work->saveData)), NULL);
        (*state)++;
        break;
    case 4:
        if (!TextPrinterCheckActive(work->printerId)) {
            template.bgConfig = work->bgConfig;
            template.bgId = GF_BG_LYR_MAIN_1;
            template.tileStart = 31;
            template.plttSlot = 11;
            template.x = 25;
            template.y = 6;
            template.shapeParam = 0;
            YesNoPrompt_InitFromTemplate(work->yesNoPrompt, &template);
            work->yesNoActive = TRUE;
            (*state)++;
        }
        break;
    case 5:
        PaletteData_LoadPaletteSlotFromHardware(work->plttData, PLTTBUF_MAIN_BG, 0xB0, 0x40);
        switch (YesNoPrompt_HandleInput(work->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(work->yesNoPrompt);
            work->yesNoActive = FALSE;
            ClearFrameAndWindow2(&work->msgWindow, FALSE);
            work->linkFilter.active = FALSE;
            work->phase = 3;
            *state = 9;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(work->yesNoPrompt);
            work->yesNoActive = FALSE;
            ClearFrameAndWindow2(&work->msgWindow, FALSE);
            PaletteData_BlendPalette(work->plttData, PLTTBUF_MAIN_BG, 9, 1, 0, RGB_BLACK);
            PaletteData_BlendPalette(work->plttData, PLTTBUF_MAIN_OBJ, (u16)(work->cursorPlttSlot << 4), 16, 0, RGB_BLACK);
            *state = 2;
            break;
        }
        break;
    case 6:
        FillWindowPixelBuffer(&work->msgWindow, 15);
        DrawFrameAndWindow2(&work->msgWindow, FALSE, 1, 14);
        ReadMsgDataIntoString(work->msgData, msg_0754_00003, work->string);
        work->printerId = AddTextPrinterParameterized(&work->msgWindow, 1, work->string, 0, 0, Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(work->saveData)), NULL);
        (*state)++;
        break;
    case 7:
        if (!TextPrinterCheckActive(work->printerId)) {
            (*state)++;
        }
        break;
    case 8:
        work->closeTimer++;
        if (work->closeTimer > 90) {
            *state = 9;
        }
        break;
    case 9:
        if (!IsPaletteFadeFinished()) {
            StopPaletteFade();
        }
        BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_125);
        (*state)++;
        break;
    case 10:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    default:
        WiFiPlazaLobbyClient_ClearChannelCommands();
        return TRUE;
    }
    FootprintStamp_UpdateSelectEffects(work);
    FootprintStamp_UpdateInkGauge(work);
    if (work->phase == 1) {
        work->nameTimer = (s32)((u32)work->nameTimer + 1);
        if (work->nameTimer > 30) {
            work->nameTimer = 0;
            FootprintStamp_UpdatePlayers(work);
        }
        // the lobby closes the attraction, or this player has to leave
        if ((WiFiPlaza_HasConnectionFailed() == TRUE || WiFiPlazaWork_IsClosing(work->args->lobby) == TRUE) && *state != 4) {
            if (work->yesNoActive == TRUE) {
                YesNoPrompt_Reset(work->yesNoPrompt);
            }
            work->linkFilter.active = FALSE;
            work->phase = 3;
            if (WiFiPlaza_HasConnectionFailed() == TRUE) {
                *state = 9;
            } else {
                PlaySE(SEQ_SE_DP_PINPON);
                ov45_0222A4A8(work->args->lobby);
                *state = 6;
            }
        }
    }
    ov89_0225A468(work);
    return FALSE;
}

#pragma force_active reset

BOOL FootprintStamp_Exit(OverlayManager *man, int *state) {
    FootprintStamp *work = OverlayManager_GetData(man);

    SysTask_Destroy(work->task);
    FootprintStamp_FreePartySprites(work);
    FootprintStamp_FreePlayerSprites(work);
    YesNoPrompt_Destroy(work->yesNoPrompt);
    FootprintStampBoard_Free(&work->board);
    String_Delete(work->string);
    FontID_Release(2);
    FontSystem_Delete(work->fontSystem);
    DestroyMsgData(work->msgData);
    MessageFormat_Delete(work->msgFormat);
    FootprintStamp_FreeWindows(work);
    FootprintStamp_FreeBgs(work->bgConfig);
    Heap_Free(work->bgConfig);
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(work->plttData, PLTTBUF_SUB_OBJ);
    PaletteData_Free(work->plttData);
    FootprintStamp_FreeBoardModel(work);
    FootprintStamp_FreeCamera(work);
    FootprintStamp_Free3DVramMan(work->vramMan3d);
    NARC_Delete(work->narc);
    NARC_Delete(work->footprintNarc);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GF_DestroyVramTransferManager();
    GF_TouchpadStopSampling();
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    WirelessStatusIcon_Destroy();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_125);
    return TRUE;
}

void FootprintStamp_MainTask(SysTask *task, void *data) {
    FootprintStamp *work = data;

    FootprintStampBoard_ResolveChains(&work->board);
    FootprintStamp_Draw3D(work);
    SpriteSystem_DrawSprites(work->spriteManager);
    SpriteSystem_UpdateTransfer();
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

void FootprintStamp_VBlank(void *data) {
    FootprintStamp *work = data;
    vu32 *base;

    FootprintStampBoard_VBlank(&work->board, work->phase);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    PaletteData_PushTransparentBuffers(work->plttData);
    DoScheduledBgGpuUpdates(work->bgConfig);
    base = (vu32 *)0x027E0000;
    base[0x3FF8 / 4] |= 1; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

void FootprintStamp_InitBgs(BgConfig *bgConfig) {
    FootprintStampBgTemplates mainTemplates;
    FootprintStampBgTemplates subTemplates;
    GraphicsBanks banks;
    GraphicsModes modes;

    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    banks = sFootprintStampBanks;
    GfGfx_SetBanks(&banks);
    MIi_CpuClear32(0, (u32 *)0x06000000, 0x80000);
    MIi_CpuClear32(0, (u32 *)0x06200000, 0x20000);
    MIi_CpuClear32(0, (u32 *)0x06400000, 0x40000);
    MIi_CpuClear32(0, (u32 *)0x06600000, 0x20000);
    modes = sFootprintStampGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    mainTemplates = *(const FootprintStampBgTemplates *)sFootprintStampMainBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &mainTemplates.items[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &mainTemplates.items[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &mainTemplates.items[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    reg_G2_BG0CNT = (u16)((reg_G2_BG0CNT & ~3) | 2); // 3D layer priority 2
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    subTemplates = *(const FootprintStampBgTemplates *)sFootprintStampSubBgTemplates;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &subTemplates.items[0], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &subTemplates.items[1], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 0);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &subTemplates.items[2], GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_125);
}

void FootprintStamp_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
}

void FootprintStamp_LoadBgGfx(FootprintStamp *work, NARC *narc) {
    void *tilemap;
    int frame;

    PaletteData_LoadNarc(work->plttData, NARC_a_2_0_8, 19, HEAP_ID_125, PLTTBUF_MAIN_BG, 0x1C0, 0);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 18, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_125);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 17, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, HEAP_ID_125);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 20, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_125);
    PaletteData_LoadNarc(work->plttData, NARC_a_2_0_8, 23, HEAP_ID_125, PLTTBUF_SUB_BG, 0, 0);
    if (work->args->unk04 == 0) {
        PaletteData_CopyPalette(work->plttData, PLTTBUF_SUB_BG, 0x10, PLTTBUF_SUB_BG, 0, 0x20);
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 22, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_125);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 21, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_125);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 24, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_125);
    // the player list is drawn into BG 6 one row at a time as players join
    tilemap = GetBgTilemapBuffer(work->bgConfig, GF_BG_LYR_SUB_2);
    MI_CpuCopy16(tilemap, work->subTilemap, sizeof(work->subTilemap));
    MIi_CpuClear16(0, tilemap, sizeof(work->subTilemap));
    frame = Options_GetFrame(Save_PlayerData_GetOptionsAddr(work->saveData));
    PaletteData_LoadNarc(work->plttData, NARC_a_0_3_8, GetUserFrameGfx2PaletteMember(frame), HEAP_ID_125, PLTTBUF_MAIN_BG, 0x20, 0xE0);
    LoadUserFrameGfx2(work->bgConfig, GF_BG_LYR_MAIN_1, 1, 14, frame, HEAP_ID_125);
    PaletteData_LoadNarc(work->plttData, NARC_graphic_font, 7, HEAP_ID_125, PLTTBUF_MAIN_BG, 0x20, 0xD0);
    if (work->args->unk04 == 0) {
        PaletteData_LoadNarc(work->plttData, NARC_graphic_font, 7, HEAP_ID_125, PLTTBUF_SUB_BG, 0x20, 0xD0);
    } else {
        PaletteData_LoadNarc(work->plttData, NARC_a_2_0_8, 25, HEAP_ID_125, PLTTBUF_SUB_BG, 0x20, 0xD0);
    }
    ScheduleBgTilemapBufferTransfer(work->bgConfig, GF_BG_LYR_SUB_2);
}

void FootprintStamp_LoadPartySpriteGfx(FootprintStamp *work, NARC *narc) {
    int i;
    u16 color;
    u8 plttNum;

    plttNum = SpriteSystem_LoadPaletteBufferFromOpenNarc(work->plttData, PLTTBUF_MAIN_OBJ, work->spriteSystem, work->spriteManager, narc, 3, FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 10001);
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 4, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 10007);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 5, FALSE, 10002);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 6, FALSE, 10002);
    for (i = 0; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 16, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 10001 + i);
    }
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 15, FALSE, 10001);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 14, FALSE, 10001);
    plttNum = SpriteSystem_LoadPaletteBufferFromOpenNarc(work->plttData, PLTTBUF_MAIN_OBJ, work->spriteSystem, work->spriteManager, narc, 3, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 10002);
    if (work->args->unk04 == 0) {
        color = RGB_BLACK;
    } else {
        color = RGB_WHITE;
    }
    PaletteData_FillPaletteInBuffer(work->plttData, PLTTBUF_MAIN_OBJ, PLTTSEL_BOTH, color, (u16)(plttNum << 4), (u16)((plttNum << 4) + 16));
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 2, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 10008);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 1, FALSE, 10003);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 0, FALSE, 10003);
    work->cursorPlttSlot = SpriteSystem_LoadPaletteBuffer(work->plttData, PLTTBUF_MAIN_OBJ, work->spriteSystem, work->spriteManager, NARC_a_2_0_8, 26, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 10003);
}

#pragma force_active on

void FootprintStamp_CreatePartySprites(FootprintStamp *work) {
    ManagedSpriteTemplate template;
    int i;
    String *string;

    template = sFootprintStampInkSpriteTemplate;
    for (i = 0; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++) {
        template.x = i * 32 + 16;
        template.y = 176;
        work->inkSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_SetAnim(work->inkSprites[i], i);
        Sprite_TickFrame(work->inkSprites[i]->sprite);
    }
    template = sFootprintStampSlotSpriteTemplate;
    for (i = 0; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++) {
        template.x = i * 32 + 16;
        template.y = 176;
        work->slotSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_SetAnim(work->slotSprites[i], i);
        Sprite_TickFrame(work->slotSprites[i]->sprite);
    }
    template = sFootprintStampFootprintSpriteTemplate;
    for (i = 0; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++) {
        template.x = i * 32 + 16;
        template.y = 176;
        template.resIdList[0] = 10001 + i;
        work->footprintSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        Sprite_TickFrame(work->footprintSprites[i]->sprite);
    }
    string = NewString_ReadMsgData(work->msgData, msg_0754_00001);
    FootprintStamp_CreateTextSprite(work, &work->exitText, string, 2, MAKE_TEXT_COLOR(1, 2, 3), 0, 10003, 224, 176, TRUE);
    TextOBJ_SetSpritesDrawFlag(work->exitText.textObj, TRUE);
    String_Delete(string);
}

#pragma force_active reset

void FootprintStamp_FreePartySprites(FootprintStamp *work) {
    {
        int i = 0;
        FootprintStamp *slot = work;
        for (; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++, slot = (FootprintStamp *)((u8 *)slot + 4)) {
            Sprite_DeleteAndFreeResources(slot->inkSprites[0]);
        }
    }
    {
        FootprintStamp *slot;
        int i;
        i = 0;
        slot = work;
        for (; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++, slot = (FootprintStamp *)((u8 *)slot + 4)) {
            Sprite_DeleteAndFreeResources(slot->slotSprites[0]);
        }
    }
    {
        FootprintStamp *slot;
        int i;
        i = 0;
        slot = work;
        for (; i < FOOTPRINT_STAMP_PARTY_SLOTS; i++, slot = (FootprintStamp *)((u8 *)slot + 4)) {
            Sprite_DeleteAndFreeResources(slot->footprintSprites[0]);
        }
    }
    FootprintStamp_FreeTextSprite(&work->exitText);
}

void FootprintStamp_LoadPlayerSpriteGfx(FootprintStamp *work, NARC *narc) {
    int i;

    SpriteSystem_LoadPaletteBufferFromOpenNarc(work->plttData, PLTTBUF_SUB_OBJ, work->spriteSystem, work->spriteManager, narc, 10, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 10004);
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 9, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 10009);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 8, FALSE, 10004);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 7, FALSE, 10004);
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 13, FALSE, NNS_G2D_VRAM_TYPE_2DSUB, 10010 + i);
    }
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 12, FALSE, 10005);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 11, FALSE, 10005);
}
