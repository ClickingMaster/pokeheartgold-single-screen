#include "overlay_trainer_card_signature.h"

#include "global.h"

#include "constants/game_stats.h"
#include "constants/sndseq.h"

#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_trainer_card_signature_internal.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "render_window.h"
#include "save_trainer_card.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "touchpad_internal.h"
#include "touchscreen.h"
#include "sound_playback.h"
#include "oam_manager.h"
#include "unk_02013534.h"
#include "touchpad.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

typedef int (*TrainerCardSignatureStateFunc)(TrainerCardSignature *signature, int state);

extern const u16 sTrainerCardSignatureDoneButtonPos[];
extern const TouchscreenHitbox sTrainerCardSignatureCanvasHitbox[];
extern const TouchscreenHitbox sTrainerCardSignatureButtonHitboxes[];
extern const GraphicsModes sTrainerCardSignatureGraphicsModes;
extern const ObjCharTransferTemplate sTrainerCardSignatureObjCharTransferTemplate;
extern const BgTemplate sTrainerCardSignatureSubBg0Template;
extern const BgTemplate sTrainerCardSignatureSubBg1Template;
extern const BgTemplate sTrainerCardSignatureMainBg0Template;
extern const BgTemplate sTrainerCardSignatureMainBg1Template;
extern const BgTemplate sTrainerCardSignatureMainBg2Template;
extern const GraphicsBanks sTrainerCardSignatureGraphicsBanks;
extern const u8 sTrainerCardSignaturePenBitmaps[][16];
extern const TrainerCardSignatureStateFunc sTrainerCardSignatureStateFuncs[];

void TrainerCardSignature_VBlankCB(BgConfig *bgConfig);
void TrainerCardSignature_SetGfxBanks(void);
void TrainerCardSignature_InitBgLayers(BgConfig *bgConfig);
void TrainerCardSignature_InitStrings(TrainerCardSignature *signature);
void TrainerCardSignature_FreeStrings(TrainerCardSignature *signature);
void TrainerCardSignature_FreeBgLayers(BgConfig *bgConfig);
void TrainerCardSignature_LoadBgGraphics(TrainerCardSignature *signature, NARC *narc);
void TrainerCardSignature_InitObjTransfer(void);
void TrainerCardSignature_InitSpriteResources(TrainerCardSignature *signature, NARC *narc);
void TrainerCardSignature_CreateDoneButtonSprite(TrainerCardSignature *signature);
void *TrainerCardSignature_PrintCentered(Window *window, String *string, u32 y, u8 fontId, u32 color);
void TrainerCardSignature_CreateWindows(TrainerCardSignature *signature, OverlayManager *unused);
void TrainerCardSignature_RemoveWindows(TrainerCardSignature *signature);
void ov52_021E8BD8(void);
void TrainerCardSignature_HandleTouch(TrainerCardSignature *signature);
void ov52_021E8CBC(Sprite **sprite, int pressed);
void TrainerCardSignature_SetDoneButtonPressed(Sprite **sprite, int pressed);
int TrainerCardSignature_State_Draw(TrainerCardSignature *signature, int state);
void TrainerCardSignature_UpdateCanvas(TrainerCardSignature *signature);
void TrainerCardSignature_ShowYesNoPrompt(BgConfig *bgConfig, YesNoPrompt *yesNoPrompt);
int TrainerCardSignature_State_WaitConfirmMessage(TrainerCardSignature *signature, int state);
int TrainerCardSignature_State_ConfirmPrompt(TrainerCardSignature *signature, int state);
int TrainerCardSignature_State_ErasePrompt(TrainerCardSignature *signature, int state);
int TrainerCardSignature_State_PrintEraseMessage(TrainerCardSignature *signature, int state);
int TrainerCardSignature_State_WaitEraseMessage(TrainerCardSignature *signature, int state);
void TrainerCardSignature_BlitClipped(Window *window, const void *src, s32 srcX, s32 srcY, s32 srcWidth, s32 srcHeight, s32 destX, s32 destY, s32 destWidth, s32 destHeight);
void TrainerCardSignature_DrawLine(Window *window, const void *src, s32 x1, s32 y1, s32 *lastX, s32 *lastY, s32 sample, s32 hasLast);
void TrainerCardSignature_SavePenPositions(const TrainerCardSignatureStroke *strokes, TrainerCardSignaturePenPos *penPos);
void TrainerCardSignature_DrawStrokes(Window *window, TrainerCardSignatureStroke *strokes, TrainerCardSignaturePenPos *penPos, u32 copyToVram);
void ov52_021E9218(TrainerCardSignature *signature);
void TrainerCardSignature_AnimateButtonPalette(u16 *angle);
void ov52_021E925C(Window *windows, int unused1, u32 unused2, TrainerCardSignature *unused3);
void TrainerCardSignature_TakeNewStroke(TrainerCardSignature *signature);
void TrainerCardSignature_PrintMessage(TrainerCardSignature *signature, int msgId);
BOOL TrainerCardSignature_IsPrinterDone(u32 textPrinterId);
void TrainerCardSignature_PackCanvas(u8 *signature, const u8 *pixels);
void TrainerCardSignature_UpdatePenSound(TrainerCardSignature *signature);
void TrainerCardSignature_PlayPenSoundOnTurn(s8 *moves);
void TrainerCardSignature_SetDoneButtonHidden(TrainerCardSignature *signature, int hide);

int TrainerCardSignature_Init(OverlayManager *manager, int *state) {
    TrainerCardSignature *signature;
    NARC *narc;

    switch (*state) {
    case 0:
        SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= 0xffffe0ff;
        reg_GXS_DB_DISPCNT &= 0xffffe0ff;
        Heap_Create(HEAP_ID_3, HEAP_ID_39, 0x40000);
        signature = OverlayManager_CreateAndGetData(manager, sizeof(TrainerCardSignature), HEAP_ID_39);
        memset(signature, 0, sizeof(TrainerCardSignature));
        signature->bgConfig = BgConfig_Alloc(HEAP_ID_39);
        narc = NARC_New(NARC_a_0_9_1, HEAP_ID_39);
        signature->msgFormat = MessageFormat_New(HEAP_ID_39);
        signature->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xfc, HEAP_ID_39);
        TextFlags_SetCanTouchSpeedUpPrint(TRUE);
        TextFlags_SetCanABSpeedUpPrint(TRUE);
        SetKeyRepeatTimers(4, 8);
        TrainerCardSignature_SetGfxBanks();
        TrainerCardSignature_InitBgLayers(signature->bgConfig);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 0x10, 1, HEAP_ID_39);
        signature->signature = (u8 *)TrainerCard_GetSignature(Save_TrainerCard_Get(OverlayManager_GetArgs(manager)));
        signature->gameStats = Save_GameStats_Get(OverlayManager_GetArgs(manager));
        signature->options = Save_PlayerData_GetOptionsAddr(OverlayManager_GetArgs(manager));
        TrainerCardSignature_LoadBgGraphics(signature, narc);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(1);
        Main_SetVBlankIntrCB((GFIntrCB)TrainerCardSignature_VBlankCB, signature->bgConfig);
        FontID_Alloc(2, HEAP_ID_39);
        TrainerCardSignature_InitStrings(signature);
        TrainerCardSignature_InitObjTransfer();
        TrainerCardSignature_InitSpriteResources(signature, narc);
        TrainerCardSignature_CreateDoneButtonSprite(signature);
        TrainerCardSignature_CreateWindows(signature, manager);
        Sound_SetSceneAndPlayBGM(0x38, 0, 0);
        reg_GX_POWCNT &= 0xffff7fff;
        NARC_Delete(narc);
        (*state)++;
        break;
    case 1:
        OverlayManager_GetData(manager);
        *state = 0;
        return TRUE;
    }
    return FALSE;
}

int TrainerCardSignature_Main(OverlayManager *manager, int *state) {
    TrainerCardSignatureStateFunc func;
    TrainerCardSignature *signature = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *state = 1;
        }
        break;
    case 1: {
        func = sTrainerCardSignatureStateFuncs[signature->state];
        if (func != NULL) {
            *state = func(signature, *state);
        }
        TrainerCardSignature_AnimateButtonPalette(&signature->paletteAngle);
        break;
    }
    case 2:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    SpriteList_RenderAndAnimateSprites(signature->spriteList);
    return FALSE;
}

int TrainerCardSignature_Exit(OverlayManager *manager, int *state) {
    TrainerCardSignature *signature = OverlayManager_GetData(manager);
    int i;

    TrainerCardSignature_PackCanvas(signature->signature, signature->canvasWindow.pixelBuffer);
    Main_SetVBlankIntrCB(NULL, NULL);
    SpriteTransfer_DeleteCharTransferTask(signature->resourceObjects[0][0]);
    SpriteTransfer_DeleteCharTransferTask(signature->resourceObjects[1][0]);
    SpriteTransfer_DeletePlttTransferTask(signature->resourceObjects[0][1]);
    SpriteTransfer_DeletePlttTransferTask(signature->resourceObjects[1][1]);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(signature->resourceManagers[i]);
    }
    SpriteList_Delete(signature->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    TrainerCardSignature_RemoveWindows(signature);
    FontID_Release(2);
    TrainerCardSignature_FreeBgLayers(signature->bgConfig);
    GF_TouchpadStopSampling();
    TextFlags_SetCanTouchSpeedUpPrint(FALSE);
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    DestroyMsgData(signature->msgData);
    MessageFormat_Delete(signature->msgFormat);
    TrainerCardSignature_FreeStrings(signature);
    OverlayManager_FreeData(manager);
    reg_GX_POWCNT |= 0x8000;
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_39);
    return TRUE;
}

void TrainerCardSignature_VBlankCB(BgConfig *bgConfig) {
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(bgConfig);
    *(vu32 *)((u8 *)OS_IRQTable + 0x3ff8) |= OS_IE_VBLANK; // OS_SetIrqCheckFlag, addressed from OS_IRQTable
}

void TrainerCardSignature_SetGfxBanks(void) {
    GraphicsBanks banks = sTrainerCardSignatureGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

#pragma force_active on

void TrainerCardSignature_InitBgLayers(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg4;
    BgTemplate bg5;

    modes = sTrainerCardSignatureGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    bg0 = sTrainerCardSignatureMainBg0Template;
    InitBgFromTemplate(bgConfig, 0, &bg0, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 0);
    bg1 = sTrainerCardSignatureMainBg1Template;
    InitBgFromTemplate(bgConfig, 1, &bg1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 1);
    bg2 = sTrainerCardSignatureMainBg2Template;
    InitBgFromTemplate(bgConfig, 2, &bg2, 0);
    bg4 = sTrainerCardSignatureSubBg0Template;
    InitBgFromTemplate(bgConfig, 4, &bg4, 0);
    BgClearTilemapBufferAndCommit(bgConfig, 4);
    bg5 = sTrainerCardSignatureSubBg1Template;
    InitBgFromTemplate(bgConfig, 5, &bg5, 0);
    BG_ClearCharDataRange(0, 32, 0, HEAP_ID_39);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_39);
}

void TrainerCardSignature_InitStrings(TrainerCardSignature *signature) {
    u8 *pen;           // steps 4 bytes per pen: penNames[] and penPos[] have 4-byte elements
    u8 *styleAndCount; // the bit-field byte of each stroke
    int i;
    u32 countMask = 0xf0;

    for (i = 0, pen = (u8 *)signature, styleAndCount = signature->strokes[0].y + 8; i < TRAINER_CARD_SIGNATURE_PEN_COUNT; i++, pen += 4, styleAndCount += sizeof(TrainerCardSignatureStroke)) {
        ((TrainerCardSignature *)pen)->penNames[0] = String_New(8, HEAP_ID_39);
        *styleAndCount &= ~countMask;
        ((TrainerCardSignature *)pen)->penPos[0].count = 0;
    }
    signature->doneString = String_New(20, HEAP_ID_39);
    signature->titleString = String_New(40, HEAP_ID_39);
    signature->messageString = String_New(80, HEAP_ID_39);
    signature->penStyle = 0;
    signature->state = 1;
    ReadMsgDataIntoString(signature->msgData, 12, signature->doneString);
    ReadMsgDataIntoString(signature->msgData, 9, signature->titleString);
    signature->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_39);
    MIi_CpuClearFast(0, (u32 *)&signature->prevMoveX, 5);
}

#pragma force_active reset

void TrainerCardSignature_FreeStrings(TrainerCardSignature *signature) {
    int i;

    YesNoPrompt_Destroy(signature->yesNoPrompt);
    for (i = 0; i < TRAINER_CARD_SIGNATURE_PEN_COUNT; i++) {
        String_Delete(signature->penNames[i]);
    }
    String_Delete(signature->messageString);
    String_Delete(signature->titleString);
    String_Delete(signature->doneString);
}

void TrainerCardSignature_FreeBgLayers(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 5);
    FreeBgTilemapBuffer(bgConfig, 4);
    FreeBgTilemapBuffer(bgConfig, 3);
    FreeBgTilemapBuffer(bgConfig, 2);
    FreeBgTilemapBuffer(bgConfig, 1);
    FreeBgTilemapBuffer(bgConfig, 0);
    Heap_Free(bgConfig);
}

#pragma force_active on

void TrainerCardSignature_LoadBgGraphics(TrainerCardSignature *signature, NARC *narc) {
    BgConfig *bgConfig = signature->bgConfig;
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 96, HEAP_ID_39);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 64, HEAP_ID_39);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_39);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_39);
    BG_ClearCharDataRange(1, 32, 0, HEAP_ID_39);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 3, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x2000, TRUE, HEAP_ID_39);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_39);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, bgConfig, GF_BG_LYR_SUB_1, 0, 0x2000, TRUE, HEAP_ID_39);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, TRUE, HEAP_ID_39);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 10, (u8)Options_GetFrame(signature->options), HEAP_ID_39);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 31, 11, 0, HEAP_ID_39);
}

#pragma force_active reset

void TrainerCardSignature_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sTrainerCardSignatureObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_39);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

#pragma force_active on

void TrainerCardSignature_InitSpriteResources(TrainerCardSignature *signature, NARC *narc) {
    int i;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_39);
    signature->spriteList = G2dRenderer_Init(50, &signature->renderer, HEAP_ID_39);
    G2dRenderer_SetSubSurfaceCoords(&signature->renderer, 0, FX32_CONST(256));
    i = 0;
    do {
        signature->resourceManagers[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_39);
        i++;
    } while (i < 4);
    signature->resourceObjects[0][0] = AddCharResObjFromOpenNarc(signature->resourceManagers[0], narc, 7, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_39);
    signature->resourceObjects[0][1] = AddPlttResObjFromOpenNarc(signature->resourceManagers[1], narc, 1, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_39);
    signature->resourceObjects[0][2] = AddCellOrAnimResObjFromOpenNarc(signature->resourceManagers[2], narc, 8, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    signature->resourceObjects[0][3] = AddCellOrAnimResObjFromOpenNarc(signature->resourceManagers[3], narc, 9, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    signature->resourceObjects[1][0] = AddCharResObjFromOpenNarc(signature->resourceManagers[0], narc, 7, TRUE, 1, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_39);
    signature->resourceObjects[1][1] = AddPlttResObjFromOpenNarc(signature->resourceManagers[1], narc, 1, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 3, HEAP_ID_39);
    signature->resourceObjects[1][2] = AddCellOrAnimResObjFromOpenNarc(signature->resourceManagers[2], narc, 8, TRUE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    signature->resourceObjects[1][3] = AddCellOrAnimResObjFromOpenNarc(signature->resourceManagers[3], narc, 9, TRUE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    SpriteTransfer_CreateCharTransferTask(signature->resourceObjects[0][0]);
    SpriteTransfer_CreateCharTransferTask(signature->resourceObjects[1][0]);
    SpriteTransfer_CreateExtPlttTransferTask(signature->resourceObjects[0][1]);
    SpriteTransfer_CreateExtPlttTransferTask(signature->resourceObjects[1][1]);
}

#pragma force_active reset

void TrainerCardSignature_CreateDoneButtonSprite(TrainerCardSignature *signature) {
    SpriteTemplate template;

    CreateSpriteResourcesHeader(&signature->resourceHeaders[0], 0, 0, 0, 0, -1, -1, 0, 0, signature->resourceManagers[0], signature->resourceManagers[1], signature->resourceManagers[2], signature->resourceManagers[3], NULL, NULL);
    CreateSpriteResourcesHeader(&signature->resourceHeaders[1], 1, 1, 1, 1, -1, -1, 0, 0, signature->resourceManagers[0], signature->resourceManagers[1], signature->resourceManagers[2], signature->resourceManagers[3], NULL, NULL);
    template.spriteList = signature->spriteList;
    template.header = &signature->resourceHeaders[0];
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = 1;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_39;
    template.header = &signature->resourceHeaders[1];
    template.position.x = FX32_CONST(224);
    template.position.y = FX32_CONST(175);
    signature->doneButtonSprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(signature->doneButtonSprite, TRUE);
    Sprite_SetAnimCtrlSeq(signature->doneButtonSprite, 0);
    Sprite_SetPaletteOverride(signature->doneButtonSprite, 0);
    Sprite_SetPriority(signature->doneButtonSprite, 2);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void *TrainerCardSignature_PrintCentered(Window *window, String *string, u32 y, u8 fontId, u32 color) {
    int width = FontID_String_GetWidth(2, string, 0);
    int x = ((window->width << 3) - width) / 2;
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
    return window->pixelBuffer;
}

void TrainerCardSignature_CreateWindows(TrainerCardSignature *signature, OverlayManager *unused) {
    int i;
    int x;
    int width;
    u32 vramOffset;
    u8 *buffer;

    AddWindowParameterized(signature->bgConfig, &signature->messageWindow, 0, 2, 1, 0x1B, 4, 0xD, 0x28);
    FillWindowPixelBuffer(&signature->messageWindow, 0xF);
    AddWindowParameterized(signature->bgConfig, &signature->canvasWindow, 1, 4, 9, 0x18, 8, 1, 1);
    FillWindowPixelBuffer(&signature->canvasWindow, 2);
    AddWindowParameterized(signature->bgConfig, &signature->doneButtonWindow, 1, 0x1A, 0x15, 8, 2, 2, 0xC1);
    FillWindowPixelBuffer(&signature->doneButtonWindow, 0);
    DC_FlushRange(TrainerCardSignature_PrintCentered(&signature->doneButtonWindow, signature->doneString, 1, 2, MAKE_TEXT_COLOR(14, 7, 2)), 0x200);
    i = 0;
    x = 0;
    vramOffset = x;
    buffer = signature->objTextBuffer;
    for (; i < 2; i++) {
        sub_02013A50(&signature->doneButtonWindow, 4, 2, x, 0, buffer);
        DC_FlushRange(buffer, 0x100);
        GX_LoadOBJ(buffer, vramOffset, 0x100);
        vramOffset += 0x100;
        x += 4;
    }
    AddWindowParameterized(signature->bgConfig, &signature->titleWindow, 1, 2, 2, 0x1C, 2, 0xD, 0xD1);
    width = FontID_String_GetWidth(1, signature->titleString, 0);
    FillWindowPixelBuffer(&signature->titleWindow, 0);
    AddTextPrinterParameterizedWithColor(&signature->titleWindow, 1, signature->titleString, (0xE0 - width) / 2, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    {
        Window *window;
        int baseTile;
        int y;
        i = 0;
        baseTile = 1;
        y = 3;
        window = signature->penNameWindows;
        for (; i < TRAINER_CARD_SIGNATURE_PEN_COUNT; i++) {
            AddWindowParameterized(signature->bgConfig, window, 4, 5, y, 10, 2, 0xD, baseTile);
            FillWindowPixelBuffer(window, 0);
            baseTile += 0x14;
            y += 4;
            window++;
        }
    }
    ov52_021E925C(signature->penNameWindows, 0, MAKE_TEXT_COLOR(14, 13, 15), signature);
}

void TrainerCardSignature_RemoveWindows(TrainerCardSignature *signature) {
    int i;

    for (i = 0; i < TRAINER_CARD_SIGNATURE_PEN_COUNT; i++) {
        RemoveWindow(&signature->penNameWindows[i]);
    }
    RemoveWindow(&signature->titleWindow);
    RemoveWindow(&signature->doneButtonWindow);
    RemoveWindow(&signature->canvasWindow);
    RemoveWindow(&signature->messageWindow);
}

void ov52_021E8BD8(void) {
}

#pragma force_active on

void TrainerCardSignature_HandleTouch(TrainerCardSignature *signature) {
    TouchOutput touches;
    int i;
    int hit = TouchscreenHitbox_FindRectAtTouchNew(sTrainerCardSignatureButtonHitboxes);

    if (hit != -1) {
        if (hit == 0) {
            if (signature->state == 1) {
                TrainerCardSignature_PrintMessage(signature, 10);
                signature->state = 2;
                TrainerCardSignature_SetDoneButtonPressed(&signature->doneButtonSprite, 1);
                PlaySE(SEQ_SE_DP_DECIDE);
            }
        } else {
            signature->penStyle = hit;
            ov52_021E8CBC(&signature->doneButtonSprite, hit);
        }
    }
    if (TouchscreenHitbox_FindRectAtTouchHeld(sTrainerCardSignatureCanvasHitbox) != -1) {
        TrainerCardSignature_UpdatePenSound(signature);
    }
    if (GF_TouchpadUpdate(&touches, 4, 1) == 1) {
        for (i = 0; i < touches.count; i++) {
            signature->newStroke.x[i] = touches.samples[i].x;
            signature->newStroke.y[i] = touches.samples[i].y;
        }
        signature->newStroke.count = touches.count;
        signature->newStroke.style = signature->penStyle;
    }
}

#pragma force_active reset

void ov52_021E8CBC(Sprite **sprite, int pressed) {
    const volatile u16 *anims = sTrainerCardSignatureDoneButtonPos;
    if (pressed == 0) {
        Sprite_SetAnimCtrlSeq(*sprite, anims[2] + 1);
    } else {
        Sprite_SetAnimCtrlSeq(*sprite, anims[2]);
    }
}

void TrainerCardSignature_SetDoneButtonPressed(Sprite **sprite, int pressed) {
    if (pressed == 1) {
        Sprite_SetAnimCtrlSeq(*sprite, 1);
    } else {
        Sprite_SetAnimCtrlSeq(*sprite, 0);
    }
}

int TrainerCardSignature_State_Draw(TrainerCardSignature *signature, int state) {
    TrainerCardSignature_HandleTouch(signature);
    ov52_021E925C(signature->penNameWindows, 0, MAKE_TEXT_COLOR(14, 12, 15), signature);
    TrainerCardSignature_TakeNewStroke(signature);
    ov52_021E9218(signature);
    TrainerCardSignature_DrawStrokes(&signature->canvasWindow, signature->strokes, signature->penPos, TRUE);
    return state;
}

void TrainerCardSignature_UpdateCanvas(TrainerCardSignature *signature) {
    ov52_021E8BD8();
    ov52_021E9218(signature);
    TrainerCardSignature_DrawStrokes(&signature->canvasWindow, signature->strokes, signature->penPos, FALSE);
}

void TrainerCardSignature_ShowYesNoPrompt(BgConfig *bgConfig, YesNoPrompt *yesNoPrompt) {
    YesNoPromptTemplate template;
    template.bgConfig = bgConfig;
    template.bgId = 0;
    template.tileStart = 0xb4;
    template.plttSlot = 8;
    template.x = 0x19;
    template.y = 6;
    YesNoPrompt_InitFromTemplate(yesNoPrompt, &template);
}

int TrainerCardSignature_State_WaitConfirmMessage(TrainerCardSignature *signature, int state) {
    if (TrainerCardSignature_IsPrinterDone(signature->textPrinterId)) {
        TrainerCardSignature_ShowYesNoPrompt(signature->bgConfig, signature->yesNoPrompt);
        TrainerCardSignature_SetDoneButtonHidden(signature, 1);
        signature->state = 3;
    }
    TrainerCardSignature_UpdateCanvas(signature);
    return state;
}

#pragma force_active on

int TrainerCardSignature_State_ConfirmPrompt(TrainerCardSignature *signature, int state) {
    switch (YesNoPrompt_HandleInput(signature->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        GameStats_AddScore(signature->gameStats, SCORE_EVENT_TRAINER_CARD_SIGNED);
        GameStats_Inc(signature->gameStats, GAME_STAT_TRAINER_CARDS_SIGNED);
        ClearFrameAndWindow2(&signature->messageWindow, TRUE);
        YesNoPrompt_Reset(signature->yesNoPrompt);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 16, 1, HEAP_ID_39);
        return 2;
    case YESNORESPONSE_NO:
        signature->state = 4;
        TrainerCardSignature_SetDoneButtonPressed(&signature->doneButtonSprite, 0);
        ClearFrameAndWindow2(&signature->messageWindow, TRUE);
        YesNoPrompt_Reset(signature->yesNoPrompt);
        break;
    }
    signature->newStroke.count = 0;
    TrainerCardSignature_UpdateCanvas(signature);
    return state;
}

int TrainerCardSignature_State_ErasePrompt(TrainerCardSignature *signature, int state) {
    switch (YesNoPrompt_HandleInput(signature->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        signature->state = 1;
        ClearFrameAndWindow2(&signature->messageWindow, TRUE);
        YesNoPrompt_Reset(signature->yesNoPrompt);
        TrainerCardSignature_SetDoneButtonHidden(signature, 0);
        FillWindowPixelBuffer(&signature->canvasWindow, 2);
        CopyWindowToVram(&signature->canvasWindow);
        break;
    case YESNORESPONSE_NO:
        signature->state = 1;
        ClearFrameAndWindow2(&signature->messageWindow, TRUE);
        YesNoPrompt_Reset(signature->yesNoPrompt);
        TrainerCardSignature_SetDoneButtonHidden(signature, 0);
        break;
    }
    return state;
}

#pragma force_active reset

int TrainerCardSignature_State_PrintEraseMessage(TrainerCardSignature *signature, int state) {
    TrainerCardSignature_PrintMessage(signature, 11);
    signature->state = 5;
    TrainerCardSignature_UpdateCanvas(signature);
    return state;
}

int TrainerCardSignature_State_WaitEraseMessage(TrainerCardSignature *signature, int state) {
    if (TrainerCardSignature_IsPrinterDone(signature->textPrinterId)) {
        signature->state = 6;
        TrainerCardSignature_ShowYesNoPrompt(signature->bgConfig, signature->yesNoPrompt);
    }
    TrainerCardSignature_UpdateCanvas(signature);
    return state;
}

void TrainerCardSignature_BlitClipped(Window *window, const void *src, s32 srcX, s32 srcY, s32 srcWidth, s32 srcHeight, s32 destX, s32 destY, s32 destWidth, s32 destHeight) {
    if (destX < 0) {
        s32 clip = -destX;
        if (clip > destWidth) {
            clip = destWidth;
        }
        destX = 0;
        srcX += clip;
        srcWidth -= clip;
        destWidth -= clip;
    }
    if (destY < 0) {
        s32 clip = -destY;
        if (clip > destHeight) {
            clip = destHeight;
        }
        destY = 0;
        srcY += clip;
        srcHeight -= clip;
        destHeight -= clip;
    }
    BlitBitmapRectToWindow(window, src, srcX, srcY, srcWidth, srcHeight, destX, destY, destWidth, destHeight);
}

// Draws a line of 4x4 pen dots from (*lastX, *lastY) to (x1, y1) (Bresenham).
void TrainerCardSignature_DrawLine(Window *window, const void *src, s32 x1, s32 y1, s32 *lastX, s32 *lastY, s32 sample, s32 hasLast) {
    s32 endX;
    s32 dx;
    s32 endY;
    s32 stepY;
    s32 stepX;
    s32 x;
    s32 y;
    s32 err;
    s32 dy;

    x = *lastX;
    y = *lastY;
    endX = x1;
    endY = y1;
    if (sample == 0 && hasLast == 0) {
        *lastX = x1;
        *lastY = y1;
        return;
    }
    dx = x1 - x;
    if (dx < 0) {
        dx = -dx;
    }
    dy = y1 - y;
    if (dy < 0) {
        dy = -dy;
    }
    if (dx > dy) {
        if (x > x1) {
            if (y > y1) {
                stepY = 1;
            } else {
                stepY = -1;
            }
            endX = x;
            x = x1;
            y = y1;
        } else {
            if (y < y1) {
                stepY = 1;
            } else {
                stepY = -1;
            }
        }
        TrainerCardSignature_BlitClipped(window, src, 0, 0, 4, 4, x, y, 4, 4);
        err = dx >> 1;
        for (x++; x <= endX; x++) {
            err -= dy;
            if (err < 0) {
                err += dx;
                y += stepY;
            }
            TrainerCardSignature_BlitClipped(window, src, 0, 0, 4, 4, x, y, 4, 4);
        }
    } else {
        if (y > y1) {
            if (x > x1) {
                stepX = 1;
            } else {
                stepX = -1;
            }
            endY = y;
            y = y1;
            x = x1;
        } else {
            if (x < x1) {
                stepX = 1;
            } else {
                stepX = -1;
            }
        }
        TrainerCardSignature_BlitClipped(window, src, 0, 0, 4, 4, x, y, 4, 4);
        err = dy >> 1;
        for (y++; y <= endY; y++) {
            err -= dx;
            if (err < 0) {
                err += dy;
                x += stepX;
            }
            TrainerCardSignature_BlitClipped(window, src, 0, 0, 4, 4, x, y, 4, 4);
        }
    }
    *lastX = x1;
    *lastY = y1;
}

void TrainerCardSignature_SavePenPositions(const TrainerCardSignatureStroke *strokes, TrainerCardSignaturePenPos *penPos) {
    int i;
    for (i = 0; i < TRAINER_CARD_SIGNATURE_PEN_COUNT; i++, strokes++, penPos++) {
        penPos->count = strokes->count;
        if (strokes->count != 0) {
            penPos->x = strokes->x[strokes->count - 1];
            penPos->y = strokes->y[strokes->count - 1];
        }
    }
}

void TrainerCardSignature_DrawStrokes(Window *window, TrainerCardSignatureStroke *strokes, TrainerCardSignaturePenPos *penPos, u32 copyToVram) {
    s32 lastX;
    s32 lastY;
    int i;
    u32 drew;
    int j;
    TrainerCardSignatureStroke *stroke;
    TrainerCardSignaturePenPos *pos;

    drew = 0;
    for (i = 0, stroke = strokes, pos = penPos; i < TRAINER_CARD_SIGNATURE_PEN_COUNT; i++, stroke++, pos++) {
        if (stroke->count != 0) {
            if (pos->count != 0) {
                lastX = pos->x - 0x20;
                lastY = pos->y - 0x48;
            }
            for (j = 0; j < stroke->count; j++) {
                drew = 1;
                TrainerCardSignature_DrawLine(window, &sTrainerCardSignaturePenBitmaps[stroke->style], stroke->x[j] - 0x20, stroke->y[j] - 0x48, &lastX, &lastY, j, pos->count);
            }
        }
    }
    if (drew != 0 && copyToVram != 0) {
        CopyWindowToVram(window);
    }
    TrainerCardSignature_SavePenPositions(strokes, penPos);
    for (j = 0; j < TRAINER_CARD_SIGNATURE_PEN_COUNT; j++) {
        strokes->count = 0;
        strokes++;
    }
}

void ov52_021E9218(TrainerCardSignature *signature) {
#pragma unused(signature)
}

void TrainerCardSignature_AnimateButtonPalette(u16 *angle) {
    u16 color;
    s32 value;

    *angle += 20;
    if (*angle > 360) {
        *angle = 0;
    }
    value = GF_SinDeg(*angle) * 10 / FX32_ONE;
    color = ((value + 15) << 5) | 29;
    GX_LoadOBJPltt(&color, 0x18, 2);
}

void ov52_021E925C(Window *windows, int unused1, u32 unused2, TrainerCardSignature *unused3) {
#pragma unused(windows, unused1, unused2, unused3)
}

#pragma force_active on

void TrainerCardSignature_TakeNewStroke(TrainerCardSignature *signature) {
    signature->strokes[0] = signature->newStroke;
}

void TrainerCardSignature_PrintMessage(TrainerCardSignature *signature, int msgId) {
    String *template = String_New(80, HEAP_ID_39);
    ReadMsgDataIntoString(signature->msgData, msgId, template);
    StringExpandPlaceholders(signature->msgFormat, signature->messageString, template);
    String_Delete(template);
    FillWindowPixelBuffer(&signature->messageWindow, 15);
    DrawFrameAndWindow2(&signature->messageWindow, FALSE, 1, 10);
    signature->textPrinterId = AddTextPrinterParameterized(&signature->messageWindow, 1, signature->messageString, 0, 0, Options_GetTextFrameDelay(signature->options), NULL);
}

#pragma force_active reset

BOOL TrainerCardSignature_IsPrinterDone(u32 textPrinterId) {
    return TextPrinterCheckActive((u8)textPrinterId) ? FALSE : TRUE;
}

// Packs the 4bpp canvas (colour 1 = ink) into the save's 1bpp signature.
#pragma force_active on

void TrainerCardSignature_PackCanvas(u8 *signature, const u8 *pixels) {
    int n = 0;
    int bit = 0;
    int i = 0;

    *signature = 0;
    for (i = 0; i < 6144; i++) {
        if ((pixels[i] & 15) == 1) {
            *signature |= 1 << bit;
        }
        if ((pixels[i] >> 4) == 1) {
            *signature |= 1 << (bit + 1);
        }
        bit += 2;
        if (bit == 8) {
            signature++;
            n++;
            *signature = 0;
            bit = 0;
        }
        GF_ASSERT(n < 6144);
    }
}

void TrainerCardSignature_UpdatePenSound(TrainerCardSignature *signature) {
    int diff;

    if (gSystem.touchX != 0xFFFF && gSystem.touchY != 0xFFFF && signature->lastTouchX != 0xFFFF && signature->lastTouchY != 0xFFFF) {
        if (signature->lastTouchX > gSystem.touchX) {
            diff = signature->lastTouchX - gSystem.touchX;
            signature->moveX = -1;
        } else {
            diff = gSystem.touchX - signature->lastTouchX;
            signature->moveX = 1;
        }
        if (diff >= 3 && diff <= 40) {
            if (signature->lastTouchY > gSystem.touchY) {
                diff = signature->lastTouchY - gSystem.touchY;
                signature->moveY = -1;
            } else {
                diff = gSystem.touchY - signature->lastTouchY;
                signature->moveY = 1;
            }
            if (diff <= 40) {
                TrainerCardSignature_PlayPenSoundOnTurn(&signature->prevMoveX);
            }
        } else if (diff <= 40) {
            if (signature->lastTouchY > gSystem.touchY) {
                diff = signature->lastTouchY - gSystem.touchY;
                signature->moveY = -1;
            } else {
                diff = gSystem.touchY - signature->lastTouchY;
                signature->moveY = 1;
            }
            if (diff >= 3 && diff <= 40) {
                TrainerCardSignature_PlayPenSoundOnTurn(&signature->prevMoveX);
            }
        }
    }
    signature->lastTouchX = gSystem.touchX;
    signature->lastTouchY = gSystem.touchY;
}

#pragma force_active reset

// moves: previous x/y direction, current x/y direction. Plays the pen sound when the pen starts
// moving or changes direction.
void TrainerCardSignature_PlayPenSoundOnTurn(s8 *moves) {
    if (moves[0] == 0 && moves[1] == 0) {
        if (!IsSEPlaying(SEQ_SE_DP_KYU01)) {
            PlaySE(SEQ_SE_DP_KYU01);
        }
    }
    if (moves[0] * moves[2] < 0) {
        goto play;
    }
    if (moves[1] * moves[3] >= 0) {
        goto done;
    }
play:
    if (!IsSEPlaying(SEQ_SE_DP_KYU01)) {
        PlaySE(SEQ_SE_DP_KYU01);
    }
done:
    moves[0] = moves[2];
    moves[1] = moves[3];
    moves[2] = 0;
    moves[3] = 0;
}

void TrainerCardSignature_SetDoneButtonHidden(TrainerCardSignature *signature, int hide) {
    if (hide == 1) {
        Sprite_SetDrawFlag(signature->doneButtonSprite, FALSE);
        return;
    }
    Sprite_SetDrawFlag(signature->doneButtonSprite, TRUE);
}
