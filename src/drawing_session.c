#include "drawing_session.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0252.h"

#include "assert.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_37.h"
#include "overlay_manager.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "oam_manager.h"
#include "touchpad.h"
#include "wireless_manager.h"
#include "communication_player_state.h"
#include "unk_02035900.h"
#include "link_synchronization.h"
#include "comm_state.h"
#include "wireless_status_icon.h"
#include "union_room_talk.h"
#include "vram_transfer_manager.h"

typedef struct DrawingSessionButtonSprite {
    u16 x;
    u16 y;
    u16 anim;
} DrawingSessionButtonSprite;

// Link layer functions without a public header (the headers of their units declare them with
// placeholder types).
extern BOOL sub_02037108(int command, const void *data, int size);
extern int sub_0203783C(void);
extern int sub_02037854(void);
extern void CommState_SetMaxEntries(int count);
extern void CommWireless_SetDetectDrops(BOOL value);
extern void UnionRoomObjects_ResetSlots(struct UnionRoomObjects *unionRoom);
extern void DrawingSessionLink_RegisterCommands(DrawingSessionWork *work); // wireless_room_packets.c: registers the command table

extern const TouchscreenHitbox sDrawingSessionYesNoHitbox[];
extern const TouchscreenHitbox sDrawingSessionCanvasHitbox[];
extern const GraphicsModes sDrawingSessionGraphicsModes;
extern const ObjCharTransferTemplate sDrawingSessionObjCharTransferTemplate;
extern const BgTemplate sDrawingSessionBgTemplateMain0;
extern const BgTemplate sDrawingSessionBgTemplateSub1;
extern const BgTemplate sDrawingSessionBgTemplateMain1;
extern const BgTemplate sDrawingSessionBgTemplateMain2;
extern const BgTemplate sDrawingSessionBgTemplateSub0;
extern const GraphicsBanks sDrawingSessionGraphicsBanks;
extern const TouchscreenHitbox sDrawingSessionButtonHitboxes[];
extern const DrawingSessionButtonSprite sDrawingSessionButtonSprites[];
extern const u8 sDrawingSessionBrushBitmaps[3][8][0x18]; // [pen size][pen colour], 6x6 dots, 4bpp
extern const DrawingSessionState sDrawingSessionSubStates[];

void DrawingSession_VBlankCB(void *bgConfig);
void DrawingSession_SetVramBanks(void);
void DrawingSession_InitBgs(BgConfig *bgConfig);
void DrawingSession_InitWork(DrawingSessionWork *work);
void DrawingSession_FreeWork(DrawingSessionWork *work);
void DrawingSession_FreeBgs(BgConfig *bgConfig);
void DrawingSession_LoadBgGraphics(DrawingSessionWork *work, NARC *narc);
void DrawingSession_InitObjTransfer(void);
void DrawingSession_InitSpriteResources(DrawingSessionWork *work, NARC *narc);
void DrawingSession_CreateSprites(DrawingSessionWork *work);
void DrawingSession_AddWindows(DrawingSessionWork *work, OverlayManager *man);
void DrawingSession_RemoveWindows(DrawingSessionWork *work);
void DrawingSession_SetPenCursorPos(Sprite *sprite, s32 x, s32 y);
void DrawingSession_HandleTouch(DrawingSessionWork *work);
void DrawingSession_SetColorButtonSelected(Sprite **buttons, int selected);
void DrawingSession_SetQuitButtonPressed(Sprite **buttons, BOOL pressed);
int DrawingSession_StateDraw(DrawingSessionWork *work, int state);
void DrawingSession_UpdateCanvas(DrawingSessionWork *work);
int DrawingSession_StateMemberJoined(DrawingSessionWork *work, int state);
int DrawingSession_StateHidePenCursors(DrawingSessionWork *work, int state);
int DrawingSession_StateResumeDrawing(DrawingSessionWork *work, int state);
int DrawingSession_StateShowQuitPrompt(DrawingSessionWork *work, int state);
int DrawingSession_StateHandleQuitPrompt(DrawingSessionWork *work, int state);
int DrawingSession_StateWaitLeaveReply(DrawingSessionWork *work, int state);
int DrawingSession_StateLeaveAccepted(DrawingSessionWork *work, int state);
int DrawingSession_StateLeaveRefused(DrawingSessionWork *work, int state);
int DrawingSession_StatePrintSessionEnded(DrawingSessionWork *work, int state);
int DrawingSession_StateWaitSessionEndedMsg(DrawingSessionWork *work, int state);
int DrawingSession_StateWaitAndFadeOut(DrawingSessionWork *work, int state);
int DrawingSession_StateShowDisbandPrompt(DrawingSessionWork *work, int state);
int DrawingSession_StateHandleDisbandPrompt(DrawingSessionWork *work, int state);
int DrawingSession_StatePrintLeaderLeft(DrawingSessionWork *work, int state);
int DrawingSession_StateWaitLeaderLeftMsg(DrawingSessionWork *work, int state);
int DrawingSession_StateWaitSyncAndFadeOut(DrawingSessionWork *work, int state);
int ov37_021E6E6C(DrawingSessionWork *work, int state);
int DrawingSession_StatePrintNoMembers(DrawingSessionWork *work, int state);
int DrawingSession_StateWaitNoMembersMsg(DrawingSessionWork *work, int state);
int DrawingSession_StatePrintMemberLeft(DrawingSessionWork *work, int state);
int DrawingSession_StateWaitMemberLeftMsg(DrawingSessionWork *work, int state);
int DrawingSession_StateWaitAfterMemberLeft(DrawingSessionWork *work, int state);
void DrawingSession_BlitClipped(Window *window, const void *src, s32 srcX, s32 srcY, s32 srcWidth, s32 srcHeight, s32 destX, s32 destY, s32 destWidth, s32 destHeight);
void DrawingSession_DrawLine(Window *window, const void *brush, s32 x1, s32 y1, s32 *lastX, s32 *lastY, s32 pointIndex, s32 lastCount);
void DrawingSession_SaveLastPoints(const DrawingSessionStroke *strokes, DrawingSessionPoint *points);
void DrawingSession_DrawStrokes(Window *window, DrawingSessionStroke *strokes, DrawingSessionPoint *points, BOOL copyToVram);
void DrawingSession_UpdatePenCursors(DrawingSessionWork *work);
void DrawingSession_AnimateCursorPalette(u16 *angle);
u32 ov37_021E745C(void);
void DrawingSession_PrintMemberNames(Window *windows, int unused, u32 color, DrawingSessionWork *work);
void DrawingSession_SendStrokes(DrawingSessionWork *work);
int DrawingSession_CountMembers(void);
BOOL DrawingSession_UpdateMembers(DrawingSessionWork *work);
void DrawingSession_PrintMessage(DrawingSessionWork *work, u32 msgId, u32 speed);
BOOL DrawingSession_IsPrinterDone(u32 printerId);
void DrawingSession_ClearMessageWindow(DrawingSessionWork *work);
void ov37_021E76D0(DrawingSessionWork *work, int extra);
int ov37_021E76F0(DrawingSessionWork *work);
void DrawingSession_SetNextSubState(DrawingSessionWork *work, int state);
void DrawingSession_ApplyNextSubState(DrawingSessionWork *work, int mainState);
BOOL DrawingSession_ShowYesNo(DrawingSessionWork *work, const YesNoPromptTemplate *template);
void DrawingSession_HideYesNo(DrawingSessionWork *work);
BOOL DrawingSession_IsYesNoAreaTouched(DrawingSessionWork *work);

BOOL DrawingSession_Init(OverlayManager *man, int *state) {
    DrawingSessionWork *work;
    NARC *narc;
    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        GX_SetVisiblePlane(0);
        GXS_SetVisiblePlane(0);
        Heap_Create(HEAP_ID_3, HEAP_ID_39, 0x40000);
        work = OverlayManager_CreateAndGetData(man, sizeof(DrawingSessionWork), HEAP_ID_39);
        memset(work, 0, sizeof(DrawingSessionWork));
        work->bgConfig = BgConfig_Alloc(HEAP_ID_39);
        work->msgFormat = MessageFormat_New(HEAP_ID_39);
        work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0252_bin, HEAP_ID_39);
        SetKeyRepeatTimers(4, 8);
        DrawingSession_SetVramBanks();
        DrawingSession_InitBgs(work->bgConfig);
        SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)0x11, (enum FadeType)0x11, RGB_BLACK, 0x10, 1, HEAP_ID_39);
        work->args = OverlayManager_GetArgs(man);
        narc = NARC_New(NARC_a_0_7_8, HEAP_ID_39);
        DrawingSession_LoadBgGraphics(work, narc);
        GF_TouchpadResetSampling();
        GF_TouchpadStartSampling(2);
        Main_SetVBlankIntrCB(DrawingSession_VBlankCB, work->bgConfig);
        DrawingSession_InitWork(work);
        DrawingSession_InitObjTransfer();
        DrawingSession_InitSpriteResources(work, narc);
        DrawingSession_CreateSprites(work);
        DrawingSession_AddWindows(work, man);
        Sound_SetSceneAndPlayBGM(0x34, 0, 0);
        GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
        DrawingSessionLink_RegisterCommands(work);
        CommState_SetErrorWatch(0, 1);
        if (Comm_GetLocalNetId() == 0) {
            CommState_SetMaxEntries(3);
        }
        WirelessStatusIcon_Create();
        if (Comm_GetLocalNetId() == 0) {
            UnionRoom_SetBeaconActivity(1);
            CommWireless_SetDetectDrops(TRUE);
        }
        NARC_Delete(narc);
        (*state)++;
        break;
    case 1:
        OverlayManager_GetData(man);
        *state = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL DrawingSession_Main(OverlayManager *man, int *state) {
    DrawingSessionWork *work = OverlayManager_GetData(man);
    if (!Comm_GetLocalNetId() && work->unk_93B4) {
        work->unk_93B4 &= Wireless_GetConnectedBitmap();
    }
    work->mainState = *state;
    DrawingSession_ApplyNextSubState(work, *state);
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            if (Comm_GetLocalNetId()) {
                if (DrawingSession_CountMembers() >= 2) {
                    Comm_SendCommand(128, NULL, 0);
                    *state = 1;
                }
            } else {
                *state = 1;
            }
        }
        break;
    case 1:
        if (sDrawingSessionSubStates[work->subState].func) {
            *state = sDrawingSessionSubStates[work->subState].func(work, *state);
        }
        DrawingSession_PrintMemberNames(work->nameWindows, 0, MAKE_TEXT_COLOR(14, 13, 15), work);
        if (!Comm_GetLocalNetId()) {
            int next = ov37_021E76F0(work);
            if (*state == 1) {
                *state = next;
            }
        }
        DrawingSession_AnimateCursorPalette(&work->cursorPaletteAngle);
        break;
    case 2:
        if (sDrawingSessionSubStates[work->subState].func && sDrawingSessionSubStates[work->subState].unk_04) {
            *state = sDrawingSessionSubStates[work->subState].func(work, *state);
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    SpriteList_RenderAndAnimateSprites(work->spriteList);
    return FALSE;
}

#pragma force_active reset

BOOL DrawingSession_Exit(OverlayManager *man, int *state) {
    DrawingSessionWork *work;
    UnkStruct_0203F4C8 *args;
    s32 i;
    work = OverlayManager_GetData(man);
    args = OverlayManager_GetArgs(man);
    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        SpriteTransfer_DeleteCharTransferTask(work->resources[0]);
        SpriteTransfer_DeleteCharTransferTask(work->resources[4]);
        SpriteTransfer_DeletePlttTransferTask(work->resources[1]);
        SpriteTransfer_DeletePlttTransferTask(work->resources[5]);
        for (i = 0; i < 4; i++) {
            Destroy2DGfxResObjMan(work->resMans[i]);
        }
        SpriteList_Delete(work->spriteList);
        OamManager_Free();
        ObjCharTransfer_Destroy();
        ObjPlttTransfer_Destroy();
        DrawingSession_RemoveWindows(work);
        DrawingSession_FreeBgs(work->bgConfig);
        GF_TouchpadStopSampling();
        DestroyMsgData(work->msgData);
        MessageFormat_Delete(work->msgFormat);
        (*state)++;
        break;
    case 1:
        CommState_SetMaxEntries(1);
        CommState_ResumeAlternateSearch();
        UnionRoomObjects_ResetSlots(args->unk0);
        GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
        UnionRoom_SetBeaconActivity(0);
        CommWireless_SetDetectDrops(FALSE);
        (*state)++;
        break;
    case 2:
        if (work->unk_9400 != 0) {
            if (Wireless_GetConnectedBitmap() == 1) {
                (*state)++;
            }
        } else {
            (*state)++;
        }
        break;
    case 3:
        CommState_SetMaxEntries(2);
        DrawingSession_FreeWork(work);
        Heap_Free(work->args);
        OverlayManager_FreeData(man);
        Main_SetVBlankIntrCB(NULL, NULL);
        Heap_Destroy(HEAP_ID_39);
        CommState_SetMaxEntries(2);
        return TRUE;
    }
    return FALSE;
}

void DrawingSession_VBlankCB(void *bgConfig) {
    // OS_SetIrqCheckFlag(OS_IE_VBLANK), with the DTCM base as a constant address
    volatile u32 *dtcm = (volatile u32 *)0x027E0000;
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(bgConfig);
    dtcm[0x3FF8 / 4] |= OS_IE_VBLANK;
}

void DrawingSession_SetVramBanks(void) {
    GraphicsBanks banks = sDrawingSessionGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void DrawingSession_InitBgs(BgConfig *bgConfig) {
    GraphicsModes modes;
    BgTemplate main0;
    BgTemplate main1;
    BgTemplate main2;
    BgTemplate sub0;
    BgTemplate sub1;
    modes = sDrawingSessionGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    main0 = sDrawingSessionBgTemplateMain0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &main0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    main1 = sDrawingSessionBgTemplateMain1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &main1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    main2 = sDrawingSessionBgTemplateMain2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &main2, GF_BG_TYPE_TEXT);
    sub0 = sDrawingSessionBgTemplateSub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &sub0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    sub1 = sDrawingSessionBgTemplateSub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &sub1, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_39);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_39);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_39);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_39);
}

void DrawingSession_InitWork(DrawingSessionWork *work) {
    s32 i;
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        work->memberNames[i] = String_New(8, HEAP_ID_39);
        work->strokes[i].count = 0;
        work->lastPoints[i].count = 0;
        work->members[i].cur = NULL;
        work->members[i].prev = NULL;
    }
    work->quitText = String_New(10, HEAP_ID_39);
    work->messageText = String_New(0x50, HEAP_ID_39);
    work->penColor = 0;
    work->penSize = 1;
    work->unk_93F4 = 1;
    work->yesNoActive = FALSE;
    work->unk_318 = 2;
    work->unk_31C = Wireless_GetConnectedBitmap();
    work->unk_93FC = 0;
    work->unk_9400 = 0;
    work->subState = 0;
    work->unk_9404 = 0;
    work->unk_9408 = 0;
    Comm_GetLocalNetId();
    DrawingSession_SetNextSubState(work, 4);
    ReadMsgDataIntoString(work->msgData, msg_0252_00007, work->quitText);
    work->unk_43C8 = Heap_Alloc(HEAP_ID_39, 0x3C00);
    work->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_39);
    work->memberCount = 0;
}

void DrawingSession_FreeWork(DrawingSessionWork *work) {
    int i;
    Heap_Free(work->unk_43C8);
    YesNoPrompt_Destroy(work->yesNoPrompt);
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        String_Delete(work->memberNames[i]);
    }
    String_Delete(work->messageText);
    String_Delete(work->quitText);
}

void DrawingSession_FreeBgs(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(bgConfig);
}

void DrawingSession_LoadBgGraphics(DrawingSessionWork *work, NARC *narc) {
    BgConfig *bgConfig = work->bgConfig;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40, HEAP_ID_39);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 1, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, HEAP_ID_39);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_39);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_39);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x2000, TRUE, HEAP_ID_39);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, HEAP_ID_39);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 3, bgConfig, GF_BG_LYR_SUB_1, 0, 0x2000, TRUE, HEAP_ID_39);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, bgConfig, GF_BG_LYR_SUB_1, 0, 0x600, TRUE, HEAP_ID_39);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_0, 1, 10, Options_GetFrame(work->args->options), HEAP_ID_39);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_0, 0x1F, 11, 0, HEAP_ID_39);
}

void DrawingSession_InitObjTransfer(void) {
    ObjCharTransferTemplate template = sDrawingSessionObjCharTransferTemplate;

    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, HEAP_ID_39);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void DrawingSession_InitSpriteResources(DrawingSessionWork *work, NARC *narc) {
    int i;
    int zero = 0; // a variable, not a literal: the three palette arguments share one register

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x20, 0, 0x7E, 0, 0x20, HEAP_ID_39);
    work->spriteList = G2dRenderer_Init(0x35, &work->renderer, HEAP_ID_39);
    G2dRenderer_SetSubSurfaceCoords(&work->renderer, 0, 256 * FX32_ONE);
    for (i = 0; i < 4; i++) {
        work->resMans[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_39);
    }

    work->resources[0] = AddCharResObjFromOpenNarc(work->resMans[0], narc, 6, TRUE, 0, 1, HEAP_ID_39);
    work->resources[1] = AddPlttResObjFromOpenNarc(work->resMans[1], narc, zero, zero, zero, 1, 7, HEAP_ID_39);
    work->resources[2] = AddCellOrAnimResObjFromOpenNarc(work->resMans[2], narc, 7, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    work->resources[3] = AddCellOrAnimResObjFromOpenNarc(work->resMans[3], narc, 8, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    work->resources[4] = AddCharResObjFromOpenNarc(work->resMans[0], narc, 6, TRUE, 1, 2, HEAP_ID_39);
    work->resources[5] = AddPlttResObjFromOpenNarc(work->resMans[1], narc, zero, zero, 1, 2, 3, HEAP_ID_39);
    work->resources[6] = AddCellOrAnimResObjFromOpenNarc(work->resMans[2], narc, 7, TRUE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    work->resources[7] = AddCellOrAnimResObjFromOpenNarc(work->resMans[3], narc, 8, TRUE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    SpriteTransfer_CreateCharTransferTask(work->resources[0]);
    SpriteTransfer_CreateCharTransferTask(work->resources[4]);
    SpriteTransfer_CreateExtPlttTransferTask(work->resources[1]);
    SpriteTransfer_CreateExtPlttTransferTask(work->resources[5]);
}

#pragma force_active on

void DrawingSession_CreateSprites(DrawingSessionWork *work) {
    SpriteTemplate spriteTemplate;
    int i;

    CreateSpriteResourcesHeader(&work->headers[0], 0, 0, 0, 0, -1, -1, 0, 0, work->resMans[0], work->resMans[1], work->resMans[2], work->resMans[3], NULL, NULL);
    CreateSpriteResourcesHeader(&work->headers[1], 1, 1, 1, 1, -1, -1, 0, 0, work->resMans[0], work->resMans[1], work->resMans[2], work->resMans[3], NULL, NULL);
    spriteTemplate.spriteList = work->spriteList;
    spriteTemplate.header = &work->headers[0];
    spriteTemplate.position.z = 0;
    spriteTemplate.scale.x = FX32_ONE;
    spriteTemplate.scale.y = FX32_ONE;
    spriteTemplate.scale.z = FX32_ONE;
    spriteTemplate.rotation = 0;
    spriteTemplate.drawPriority = 1;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_39;
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        spriteTemplate.header = &work->headers[1];
        spriteTemplate.position.x = (24 + i * 40) * FX32_ONE;
        spriteTemplate.position.y = 64 * FX32_ONE;
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
        work->penCursors[i] = Sprite_CreateAffine(&spriteTemplate);
        Sprite_SetAnimActiveFlag(work->penCursors[i], TRUE);
        Sprite_SetAnimCtrlSeq(work->penCursors[i], i);
        Sprite_SetDrawFlag(work->penCursors[i], FALSE);
    }
    for (i = 0; i < 12; i++) {
        spriteTemplate.header = &work->headers[1];
        spriteTemplate.position.x = sDrawingSessionButtonSprites[i].x * FX32_ONE;
        spriteTemplate.position.y = sDrawingSessionButtonSprites[i].y * FX32_ONE;
        work->buttons[i] = Sprite_CreateAffine(&spriteTemplate);
        Sprite_SetAnimActiveFlag(work->buttons[i], TRUE);
        Sprite_SetAnimCtrlSeq(work->buttons[i], sDrawingSessionButtonSprites[i].anim);
        if (i >= 8) {
            Sprite_SetPriority(work->buttons[i], 2);
        }
    }
    Sprite_SetAnimCtrlSeq(work->buttons[0], 6);
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        spriteTemplate.position.x = 24 * FX32_ONE;
        spriteTemplate.position.y = (32 + i * 32) * FX32_ONE + 256 * FX32_ONE;
        work->memberIcons[i] = Sprite_CreateAffine(&spriteTemplate);
        Sprite_SetAnimActiveFlag(work->memberIcons[i], TRUE);
        Sprite_SetAnimCtrlSeq(work->memberIcons[i], i);
        Sprite_SetDrawPriority(work->memberIcons[i], 1);
        Sprite_SetDrawFlag(work->memberIcons[i], FALSE);
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
}

#pragma force_active reset

// man is not used.
void DrawingSession_AddWindows(DrawingSessionWork *work, OverlayManager *man) {
    int i;
    int baseTile;
    int y;

    AddWindowParameterized(work->bgConfig, &work->messageWindow, GF_BG_LYR_MAIN_0, 2, 1, 27, 4, 13, 0x28);
    FillWindowPixelBuffer(&work->messageWindow, 15);
    AddWindowParameterized(work->bgConfig, &work->canvasWindow, GF_BG_LYR_MAIN_1, 1, 2, 30, 15, 0, 1);
    FillWindowPixelBuffer(&work->canvasWindow, 2);
    AddWindowParameterized(work->bgConfig, &work->quitWindow, GF_BG_LYR_MAIN_1, 25, 21, 7, 2, 13, 0x1C3);
    FillWindowPixelBuffer(&work->quitWindow, 0);
    AddTextPrinterParameterizedWithColor(&work->quitWindow, 1, work->quitText, FontID_String_GetCenterAlignmentX(1, work->quitText, 0, 0x30) + 2, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(7, 1, 0), NULL);
    baseTile = 1;
    y = 3;
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        AddWindowParameterized(work->bgConfig, &work->nameWindows[i], GF_BG_LYR_SUB_0, 5, y, 10, 2, 13, baseTile);
        FillWindowPixelBuffer(&work->nameWindows[i], 0);
        baseTile += 20;
        y += 4;
    }
    DrawingSession_PrintMemberNames(work->nameWindows, 0, MAKE_TEXT_COLOR(14, 13, 15), work);
}

void DrawingSession_RemoveWindows(DrawingSessionWork *work) {
    int i;
    Window *window;
    for (i = 0, window = work->nameWindows; i < DRAWING_SESSION_MAX_MEMBERS; i++, window++) {
        RemoveWindow(window);
    }
    RemoveWindow(&work->quitWindow);
    RemoveWindow(&work->canvasWindow);
    RemoveWindow(&work->messageWindow);
}

void DrawingSession_SetPenCursorPos(Sprite *sprite, s32 x, s32 y) {
    VecFx32 vec;
    float value;

    // rounded through float
    if (x > 0) {
        value = (float)(x << FX32_SHIFT) + 0.5f;
    } else {
        value = (float)(x << FX32_SHIFT) - 0.5f;
    }
    vec.x = (s32)value;
    y -= 8;
    if (y > 0) {
        value = (float)(y << FX32_SHIFT) + 0.5f;
    } else {
        value = (float)(y << FX32_SHIFT) - 0.5f;
    }
    vec.y = (s32)value;
    vec.z = 0;
    Sprite_SetMatrix(sprite, &vec);
}

#pragma force_active on

void DrawingSession_HandleTouch(DrawingSessionWork *work) {
    int selected;
    int hit;
    int changed;
    TouchOutput touch;

    changed = 0;
    hit = TouchscreenHitbox_FindRectAtTouchNew(sDrawingSessionButtonHitboxes);
    if (hit != -1) {
        switch (hit) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if (work->penColor != hit) {
                work->penColor = hit;
                DrawingSession_SetColorButtonSelected(work->buttons, hit);
                PlaySE(SEQ_SE_DP_SELECT);
            }
            break;
        case 8:
            if (work->subState == 4) {
                if (Comm_GetLocalNetId() == 0) {
                    if (work->unk_31C != Wireless_GetConnectedBitmap()) {
                        PlaySE(SEQ_SE_DP_CUSTOM06);
                        break;
                    }
                    CommState_SetMaxEntries(Comm_CountConnected());
                    work->unk_93F4 = 2;
                    DrawingSession_PrintMessage(work, msg_0252_00001, 1);
                    DrawingSession_SetNextSubState(work, 5);
                    DrawingSession_SetQuitButtonPressed(work->buttons, TRUE);
                    changed = 1;
                    PlaySE(SEQ_SE_DP_SELECT);
                } else {
                    if (work->strokes[0].unk_09 == 2) {
                        PlaySE(SEQ_SE_DP_CUSTOM06);
                        break;
                    }
                    DrawingSession_PrintMessage(work, msg_0252_00001, 1);
                    DrawingSession_SetNextSubState(work, 5);
                    DrawingSession_SetQuitButtonPressed(work->buttons, TRUE);
                    changed = 1;
                    PlaySE(SEQ_SE_DP_SELECT);
                }
            }
            break;
        case 9:
        case 10:
        case 11: {
            selected = hit;
            int size = selected - 9;
            int i = 0;
            int button = 0;
            int onAnim = 30;
            int offAnim = 29;
            selected = size;
            for (; i < 3; i++, onAnim += 2, button++, offAnim += 2) {
                if (selected == i) {
                    Sprite_SetAnimCtrlSeq(work->buttons[9 + button], onAnim);
                } else {
                    Sprite_SetAnimCtrlSeq(work->buttons[9 + button], offAnim);
                }
            }
            hit -= 9;
            if (work->penSize != hit) {
                work->penSize = hit;
                PlaySE(SEQ_SE_DP_BUTTON3);
            }
            break;
        }
        }
    }
    hit = TouchscreenHitbox_FindRectAtTouchHeld(sDrawingSessionCanvasHitbox);
    Sprite_SetDrawFlag(work->penCursors[Comm_GetLocalNetId()], FALSE);
    if (hit != -1) {
        DrawingSession_SetPenCursorPos(work->penCursors[Comm_GetLocalNetId()], gSystem.touchX, gSystem.touchY);
        Sprite_SetDrawFlag(work->penCursors[Comm_GetLocalNetId()], TRUE);
    }
    if (GF_TouchpadUpdate(&touch, 4, 64) == 1) {
        DrawingSession_BuildStroke(&work->ownStroke, &touch, work->penColor, work->penSize);
        if (changed == 1) {
            work->ownStroke.count = 0;
        }
    }
}

#pragma force_active reset

void DrawingSession_SetColorButtonSelected(Sprite **buttons, int selected) {
    int i;
    for (i = 0; i < 8; i++) {
        if (i == selected) {
            Sprite_SetAnimCtrlSeq(buttons[i], sDrawingSessionButtonSprites[i].anim + 1);
        } else {
            Sprite_SetAnimCtrlSeq(buttons[i], sDrawingSessionButtonSprites[i].anim);
        }
    }
}

void DrawingSession_SetQuitButtonPressed(Sprite **buttons, BOOL pressed) {
    if (pressed == TRUE) {
        Sprite_SetAnimCtrlSeq(buttons[8], 0x16);
    } else {
        Sprite_SetAnimCtrlSeq(buttons[8], 0x15);
    }
}

int DrawingSession_StateDraw(DrawingSessionWork *work, int state) {
    DrawingSession_HandleTouch(work);
    if (Comm_GetLocalNetId() == 0) {
        if (ov37_021E745C() != 1) {
            DrawingSession_SendStrokes(work);
        }
    } else {
        DrawingSession_SendStrokes(work);
    }
    DrawingSession_UpdatePenCursors(work);
    DrawingSession_DrawStrokes(&work->canvasWindow, work->strokes, work->lastPoints, TRUE);
    return state;
}

void DrawingSession_UpdateCanvas(DrawingSessionWork *work) {
    DrawingSession_UpdatePenCursors(work);
    DrawingSession_DrawStrokes(&work->canvasWindow, work->strokes, work->lastPoints, FALSE);
}

int DrawingSession_StateMemberJoined(DrawingSessionWork *work, int state) {
    if (work->joinedMember == Comm_GetLocalNetId()) {
        DrawingSession_PrintMessage(work, msg_0252_00013, 0);
    } else {
        DrawingSession_PrintMessage(work, msg_0252_00000, 0);
    }
    DrawingSession_SetNextSubState(work, 2);
    PlaySE(SEQ_SE_DP_BUTTON9);
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, -6);
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateHidePenCursors(DrawingSessionWork *work, int state) {
    int i;
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        Sprite_SetDrawFlag(work->penCursors[i], FALSE);
    }
    return state;
}

int DrawingSession_StateResumeDrawing(DrawingSessionWork *work, int state) {
    G2_BlendNone();
    ov37_021E76D0(work, 1);
    DrawingSession_SetNextSubState(work, 4);
    DrawingSession_UpdateCanvas(work);
    work->unk_9404 = 0;
    return state;
}

int DrawingSession_StateShowQuitPrompt(DrawingSessionWork *work, int state) {
    YesNoPromptTemplate template;
    if (DrawingSession_IsPrinterDone(work->printerId)) {
        MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
        template.bgConfig = work->bgConfig;
        template.bgId = GF_BG_LYR_MAIN_0;
        template.tileStart = 0xB4;
        template.plttSlot = 8;
        template.x = 25;
        template.y = 6;
        template.shapeParam = 0;
        if (!DrawingSession_ShowYesNo(work, &template)) {
            GF_AssertFail();
        }
        DrawingSession_SetNextSubState(work, 6);
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateHandleQuitPrompt(DrawingSessionWork *work, int state) {
    u8 packet[4];
    u32 memberCount;

    work->ownStroke.count = 0;
    if (work->strokes[0].unk_09 == 2 && Comm_GetLocalNetId() != 0) {
        DrawingSession_UpdateCanvas(work);
        if (DrawingSession_IsYesNoAreaTouched(work)) {
            PlaySE(SEQ_SE_DP_CUSTOM06);
        }
        return state;
    }
    if (Comm_GetLocalNetId() == 0 && work->unk_93B4 != 0) {
        DrawingSession_UpdateCanvas(work);
        return state;
    }
    memberCount = DrawingSession_CountMembers();
    if (memberCount != Comm_CountConnected()) {
        DrawingSession_UpdateCanvas(work);
        return state;
    }
    switch (YesNoPrompt_HandleInput(work->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        if (Comm_GetLocalNetId() == 0) {
            DrawingSession_SetNextSubState(work, 13);
            DrawingSession_PrintMessage(work, msg_0252_00004, 1);
        } else {
            MI_CpuFill8(packet, 0, 4);
            packet[2] = 0;
            packet[0] = Comm_GetLocalNetId();
            work->leaveRequested = 1;
            work->networkCounter = 0;
            DrawingSession_SetNextSubState(work, 7);
            Comm_SendCommand(0x7E, packet, 4);
            ClearFrameAndWindow2(&work->messageWindow, TRUE);
        }
        DrawingSession_HideYesNo(work);
        CopyWindowToVram(&work->canvasWindow);
        break;
    case YESNORESPONSE_NO:
        DrawingSession_SetNextSubState(work, 4);
        DrawingSession_SetQuitButtonPressed(work->buttons, FALSE);
        ClearFrameAndWindow2(&work->messageWindow, TRUE);
        DrawingSession_HideYesNo(work);
        CopyWindowToVram(&work->canvasWindow);
        if (Comm_GetLocalNetId() == 0) {
            CommState_SetMaxEntries(Comm_CountConnected() + 1);
            work->unk_93F4 = 1;
        }
        break;
    default:
        break;
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateWaitLeaveReply(DrawingSessionWork *work, int state) {
    work->ownStroke.count = 0;
    work->networkCounter = 0;
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateLeaveAccepted(DrawingSessionWork *work, int state) {
    u8 packet[4];

    work->ownStroke.count = 0;
    if (work->unk_93B8 != Comm_CountConnected() || work->unk_93B8 != DrawingSession_CountMembers()) {
        work->networkCounter = 0;
        DrawingSession_SetNextSubState(work, 9);
        DrawingSession_UpdateCanvas(work);
        return state;
    }
    work->networkCounter++;
    if (work->networkCounter > 30) {
        MI_CpuFill8(packet, 0, 4);
        packet[2] = 1;
        packet[0] = Comm_GetLocalNetId();
        Comm_SendCommand(0x7E, packet, 4);
        work->networkCounter = 0;
        DrawingSession_SetNextSubState(work, 10);
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateLeaveRefused(DrawingSessionWork *work, int state) {
    work->ownStroke.count = 0;
    work->leaveRequested = 0;
    DrawingSession_SetNextSubState(work, 4);
    DrawingSession_SetQuitButtonPressed(work->buttons, FALSE);
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StatePrintSessionEnded(DrawingSessionWork *work, int state) {
    DrawingSession_PrintMessage(work, msg_0252_00006, 1);
    DrawingSession_SetNextSubState(work, 11);
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateWaitSessionEndedMsg(DrawingSessionWork *work, int state) {
    if (DrawingSession_IsPrinterDone(work->printerId)) {
        work->counter = 0;
        DrawingSession_SetNextSubState(work, 12);
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateWaitAndFadeOut(DrawingSessionWork *work, int state) {
    work->counter++;
    if (work->counter > 60) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)0x10, (enum FadeType)0x10, RGB_BLACK, 0x10, 1, HEAP_ID_39);
        state = 3;
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateShowDisbandPrompt(DrawingSessionWork *work, int state) {
    YesNoPromptTemplate template;
    if (DrawingSession_IsPrinterDone(work->printerId)) {
        template.bgConfig = work->bgConfig;
        template.bgId = GF_BG_LYR_MAIN_0;
        template.tileStart = 0xB4;
        template.plttSlot = 8;
        template.x = 25;
        template.y = 6;
        template.shapeParam = 0;
        if (!DrawingSession_ShowYesNo(work, &template)) {
            GF_AssertFail();
        }
        DrawingSession_SetNextSubState(work, 14);
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateHandleDisbandPrompt(DrawingSessionWork *work, int state) {
    YesNoResponse input;

    input = YesNoPrompt_HandleInput(work->yesNoPrompt);
    if (work->unk_318 != DrawingSession_CountMembers() || work->unk_93B4 != 0) {
        DrawingSession_UpdateCanvas(work);
        return state;
    }
    switch (input) {
    case YESNORESPONSE_YES:
        DrawingSession_SetNextSubState(work, 15);
        sub_02037108(0x7F, NULL, 0);
        BufferPlayersName(work->msgFormat, 0, CommPlayerInfo_GetProfile(0));
        state = 2;
        DrawingSession_HideYesNo(work);
        break;
    case YESNORESPONSE_NO:
        DrawingSession_SetNextSubState(work, 4);
        DrawingSession_SetQuitButtonPressed(work->buttons, FALSE);
        ClearFrameAndWindow2(&work->messageWindow, TRUE);
        DrawingSession_HideYesNo(work);
        if (Comm_GetLocalNetId() == 0) {
            CommState_SetMaxEntries(Comm_CountConnected() + 1);
            work->unk_93F4 = 1;
        }
        break;
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StatePrintLeaderLeft(DrawingSessionWork *work, int state) {
    BufferPlayersName(work->msgFormat, 0, CommPlayerInfo_GetProfile(0));
    DrawingSession_PrintMessage(work, msg_0252_00003, 1);
    DrawingSession_SetNextSubState(work, 16);
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateWaitLeaderLeftMsg(DrawingSessionWork *work, int state) {
    if (DrawingSession_IsPrinterDone(work->printerId)) {
        DrawingSession_SetNextSubState(work, 17);
        LinkSync_StartSync(200);
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateWaitSyncAndFadeOut(DrawingSessionWork *work, int state) {
    if (LinkSync_IsSynced(200) || Comm_CountConnected() == 1) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)0x10, (enum FadeType)0x10, RGB_BLACK, 0x10, 1, HEAP_ID_39);
        state = 3;
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int ov37_021E6E6C(DrawingSessionWork *work, int state) {
#pragma unused(work)
    return state;
}

int DrawingSession_StatePrintNoMembers(DrawingSessionWork *work, int state) {
    DrawingSession_PrintMessage(work, msg_0252_00005, 1);
    DrawingSession_SetNextSubState(work, 20);
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateWaitNoMembersMsg(DrawingSessionWork *work, int state) {
    if (DrawingSession_IsPrinterDone(work->printerId)) {
        DrawingSession_SetNextSubState(work, 12);
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StatePrintMemberLeft(DrawingSessionWork *work, int state) {
    if (work->printerId != 0xFF && !DrawingSession_IsPrinterDone(work->printerId)) {
        RemoveTextPrinter(work->printerId);
    }
    DrawingSession_PrintMessage(work, msg_0252_00002, 1);
    DrawingSession_SetNextSubState(work, 22);
    PlaySE(SEQ_SE_DP_BUTTON9);
    if (!Comm_GetLocalNetId()) {
        ov37_021E76D0(work, 0);
    }
    work->unk_93B0 = Comm_CountConnected();
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateWaitMemberLeftMsg(DrawingSessionWork *work, int state) {
    if (work->unk_93B0 != 0) {
        if (work->unk_93B0 != Comm_CountConnected()) {
            work->unk_93B0 = 0;
        }
    }
    if (DrawingSession_IsPrinterDone(work->printerId)) {
        DrawingSession_SetNextSubState(work, 23);
        work->counter = 0;
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

int DrawingSession_StateWaitAfterMemberLeft(DrawingSessionWork *work, int state) {
    if (work->unk_93B0 != 0) {
        if (work->unk_93B0 != Comm_CountConnected()) {
            work->unk_93B0 = 0;
        }
    }
    work->counter++;
    if (work->counter > 60 && work->unk_93B0 == 0) {
        DrawingSession_ClearMessageWindow(work);
        DrawingSession_SetNextSubState(work, 4);
        if (!Comm_GetLocalNetId()) {
            work->unk_93F4 = 1;
            ov37_021E76D0(work, 1);
        }
    }
    DrawingSession_UpdateCanvas(work);
    return state;
}

void DrawingSession_HandleNetEvent(DrawingSessionWork *work, int event, u32 member) {
    switch (event) {
    case 1:
        if (work->subState == 6 || work->subState == 14) {
            DrawingSession_HideYesNo(work);
        }
        DrawingSession_SetQuitButtonPressed(work->buttons, FALSE);
        BufferPlayersName(work->msgFormat, 0, CommPlayerInfo_GetProfile(member));
        work->joinedMember = member;
        work->unk_93B4 = 0;
        break;
    case 3:
        DrawingSession_ClearMessageWindow(work);
        break;
    case 8:
    case 9:
    case 15:
        break;
    case 0x15:
        if (work->leaveRequested == 1) {
            return;
        }
        BufferPlayersName(work->msgFormat, 0, CommPlayerInfo_GetProfile(member));
        if (member == Comm_GetLocalNetId()) {
            return;
        }
        if (Comm_GetLocalNetId() == 0) {
            work->unk_93B4 &= 0xFFFF ^ member;
        }
        if (work->subState == 6 || work->subState == 14) {
            DrawingSession_HideYesNo(work);
            CopyWindowToVram(&work->canvasWindow);
        }
        DrawingSession_SetQuitButtonPressed(work->buttons, FALSE);
        break;
    default:
        return;
    }
    DrawingSession_SetNextSubState(work, event);
}

void DrawingSession_BlitClipped(Window *window, const void *src, s32 srcX, s32 srcY, s32 srcWidth, s32 srcHeight, s32 destX, s32 destY, s32 destWidth, s32 destHeight) {
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
    BlitBitmapRectToWindow(window, (void *)src, srcX, srcY, srcWidth, srcHeight, destX, destY, destWidth, destHeight);
}

// Bresenham line from (*lastX, *lastY) to (x1, y1), stamping the 6x6 brush at every step. The first
// point of a stroke without a previous one only moves the pen.
void DrawingSession_DrawLine(Window *window, const void *brush, s32 x1, s32 y1, s32 *lastX, s32 *lastY, s32 pointIndex, s32 lastCount) {
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
    if (pointIndex == 0 && lastCount == 0) {
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
        DrawingSession_BlitClipped(window, brush, 0, 0, 6, 6, x, y, 6, 6);
        err = dx >> 1;
        for (x++; x <= endX; x++) {
            err -= dy;
            if (err < 0) {
                err += dx;
                y += stepY;
            }
            DrawingSession_BlitClipped(window, brush, 0, 0, 6, 6, x, y, 6, 6);
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
        DrawingSession_BlitClipped(window, brush, 0, 0, 6, 6, x, y, 6, 6);
        err = dy >> 1;
        for (y++; y <= endY; y++) {
            err -= dx;
            if (err < 0) {
                err += dy;
                x += stepX;
            }
            DrawingSession_BlitClipped(window, brush, 0, 0, 6, 6, x, y, 6, 6);
        }
    }
    *lastX = x1;
    *lastY = y1;
}

void DrawingSession_SaveLastPoints(const DrawingSessionStroke *strokes, DrawingSessionPoint *points) {
    int i;

    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++, strokes++, points++) {
        points->count = strokes->count;
        if (strokes->count != 0) {
            points->x = strokes->xs[strokes->count - 1];
            points->y = strokes->ys[strokes->count - 1];
        }
    }
}

void DrawingSession_DrawStrokes(Window *window, DrawingSessionStroke *strokes, DrawingSessionPoint *points, BOOL copyToVram) {
    s32 lastX;
    s32 lastY;
    int i;
    BOOL drew;
    int j;
    DrawingSessionStroke *stroke;
    DrawingSessionPoint *point;

    drew = FALSE;
    for (i = 0, stroke = strokes, point = points; i < DRAWING_SESSION_MAX_MEMBERS; i++, stroke++, point++) {
        if (stroke->count != 0) {
            if (point->count != 0) {
                lastX = point->x - 9;
                lastY = point->y - 17;
            }
            for (j = 0; j < stroke->count; j++) {
                drew = TRUE;
                DrawingSession_DrawLine(window, sDrawingSessionBrushBitmaps[stroke->size][stroke->color], stroke->xs[j] - 9, stroke->ys[j] - 17, &lastX, &lastY, j, point->count);
            }
        }
    }
    if (drew != FALSE && copyToVram != FALSE) {
        CopyWindowToVram(window);
    }
    DrawingSession_SaveLastPoints(strokes, points);
    for (j = 0; j < DRAWING_SESSION_MAX_MEMBERS; j++) {
        strokes->count = 0;
        strokes++;
    }
}

void DrawingSession_UpdatePenCursors(DrawingSessionWork *work) {
    int i;
    DrawingSessionStroke *stroke;

    for (i = 0, stroke = work->strokes; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        if (i != Comm_GetLocalNetId()) {
            if (stroke->count != 0 && CommPlayerInfo_GetProfile(i)) {
                Sprite_SetDrawFlag(work->penCursors[i], TRUE);
                DrawingSession_SetPenCursorPos(work->penCursors[i], stroke->xs[stroke->count - 1], stroke->ys[stroke->count - 1]);
            } else {
                Sprite_SetDrawFlag(work->penCursors[i], FALSE);
            }
        }
        stroke++;
    }
}

void DrawingSession_AnimateCursorPalette(u16 *angle) {
    u16 color;

    *angle += 20;
    if (*angle > 360) {
        *angle = 0;
    }
    color = (u16)(((GF_SinDeg(*angle) * 10 / FX32_ONE + 15) << 5) | 0x1D);
    GX_LoadOBJPltt(&color, 0x18, 2);
}

// Same body as DrawingSession_CountMembers.
u32 ov37_021E745C(void) {
    int i;
    u32 count = 0;
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        if (CommPlayerInfo_GetProfile(i)) {
            count++;
        }
    }
    return count;
}

#pragma force_active on

void DrawingSession_PrintMemberNames(Window *windows, int unused, u32 color, DrawingSessionWork *work) {
    int ownId = Comm_GetLocalNetId();
    int i;
    int member;
    int icon;
    Window *window;

    if (DrawingSession_UpdateMembers(work)) {
        volatile int counter = 0; // volatile keeps the original loop shape
        member = 0;
        icon = 0;
        do {
            if (!work->members[member].cur) {
                Sprite_SetDrawFlag(work->memberIcons[icon], FALSE);
            } else {
                Sprite_SetDrawFlag(work->memberIcons[icon], TRUE);
            }
            int next = counter + 1;
            member++;
            icon++;
            counter = next;
            if (next >= DRAWING_SESSION_MAX_MEMBERS) {
                break;
            }
        } while (1);
        i = 0;
        window = windows;
        for (; i < DRAWING_SESSION_MAX_MEMBERS; i++, window++) {
            FillWindowPixelRect(window, 0, 0, 0, 80, 16);
        }
        for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++, windows++) {
            if (work->members[i].cur) {
                PlayerName_FlatToString(work->members[i].cur, work->memberNames[i]);
                if (ownId == i) {
                    AddTextPrinterParameterizedWithColor(windows, 1, work->memberNames[i], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 15), NULL);
                } else {
                    AddTextPrinterParameterizedWithColor(windows, 1, work->memberNames[i], 0, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
                }
            }
            CopyWindowToVram(windows);
        }
    }
}

void DrawingSession_SendStrokes(DrawingSessionWork *work) {
    if (Comm_GetLocalNetId() == 0) {
        if (sub_0203783C()) {
            work->ownStroke.unk_09 = work->unk_93F4;
            {
                // byte copy of the own stroke into the first slot of the leader's packet
                u32 remaining;
                u8 *dest;
                const u8 *src;
                src = (const u8 *)&work->ownStroke;
                dest = work->strokeSendBuffer;
                for (remaining = sizeof(DrawingSessionStroke); remaining != 0; remaining--) {
                    *dest = *src;
                    src++;
                    dest++;
                }
            }
            sub_02037108(120, work->strokeSendBuffer, sizeof(work->strokeSendBuffer));
        }
    } else {
        if (sub_02037854()) {
            Comm_SendCommand(119, &work->ownStroke, sizeof(DrawingSessionStroke));
        }
    }
}

#pragma force_active reset

int DrawingSession_CountMembers(void) {
    int i;
    u32 count = 0;
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        if (CommPlayerInfo_GetProfile(i)) {
            count++;
        }
    }
    return count;
}

u32 DrawingSession_GetMemberCount(void) {
    return DrawingSession_CountMembers();
}

BOOL DrawingSession_UpdateMembers(DrawingSessionWork *work) {
    int i;
    BOOL changed = FALSE;
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        work->members[i].prev = work->members[i].cur;
        work->members[i].cur = CommPlayerInfo_GetProfile(i);
    }
    for (i = 0; i < DRAWING_SESSION_MAX_MEMBERS; i++) {
        if (work->members[i].prev != work->members[i].cur) {
            changed = TRUE;
        }
    }
    return changed;
}

void DrawingSession_PrintMessage(DrawingSessionWork *work, u32 msgId, u32 speed) {
    String *string;

    string = String_New(0x50, HEAP_ID_39);
    ReadMsgDataIntoString(work->msgData, msgId, string);
    StringExpandPlaceholders(work->msgFormat, work->messageText, string);
    String_Delete(string);
    FillWindowPixelBuffer(&work->messageWindow, 15);
    DrawFrameAndWindow2(&work->messageWindow, FALSE, 1, 10);
    if (speed == 0) {
        speed = TEXT_SPEED_INSTANT;
    }
    work->printerId = AddTextPrinterParameterized(&work->messageWindow, 1, work->messageText, 0, 0, speed, NULL);
    if (speed == 0) {
        work->printerId = 0xFF;
    }
}

BOOL DrawingSession_IsPrinterDone(u32 printerId) {
    if (printerId == 0xFF) {
        return TRUE;
    }
    if (!TextPrinterCheckActive(printerId)) {
        return TRUE;
    }
    return FALSE;
}

void DrawingSession_ClearMessageWindow(DrawingSessionWork *work) {
    ClearFrameAndWindow2(&work->messageWindow, FALSE);
}

void ov37_021E76D0(DrawingSessionWork *work, int extra) {
#pragma unused(work)
    s32 count;
    if (!Comm_GetLocalNetId()) {
        count = Comm_CountConnected() + extra;
        if (count > DRAWING_SESSION_MAX_MEMBERS) {
            count = DRAWING_SESSION_MAX_MEMBERS;
        }
        CommState_SetMaxEntries(count);
    }
}

#pragma force_active on
#pragma opt_common_subs off

// TRUE for 2, 4, 8 and 16: the bit of exactly one member other than the leader.
static inline BOOL DrawingSession_IsOneMemberBit(u32 bits) {
    BOOL result = FALSE;
    u32 key = bits - 2;
    if (key <= 14) {
        if (0x4045u & (1u << key)) {
            result = TRUE;
        }
    }
    return result;
}

int ov37_021E76F0(DrawingSessionWork *work) {
    int count = DrawingSession_CountMembers();
    switch (count) {
    case 1:
        if (work->subState <= 14) {
            if (Wireless_GetConnectedBitmap() != 1) {
                work->unk_9400 = 1;
            }
            work->nextSubState = 19;
            CommState_SetMaxEntries(1);
            if (work->yesNoActive) {
                DrawingSession_HideYesNo(work);
                Sprite_SetAnimCtrlSeq(work->buttons[8], 21);
            }
            return 2;
        }
        break;
    case 2:
    case 3:
    case 4:
        UnionRoom_SetBeaconActivity(1);
        if (count < work->memberCount) {
            if (work->unk_93F4 == 2) {
                ov37_021E76D0(work, 0);
            } else {
                ov37_021E76D0(work, 1);
            }
        }
        break;
    case 5:
        UnionRoom_SetBeaconActivity(8);
        break;
    default:
        break;
    }
    if (count < work->memberCount) {
        work->unk_318 = Comm_CountConnected();
        work->unk_31C = Wireless_GetConnectedBitmap();
        if (work->unk_9404 && !(work->unk_31C & work->unk_9408)) {
            ov37_021E76D0(work, 1);
            work->unk_93F4 = 1;
            work->unk_9404 = 0;
            work->unk_9408 = 0;
        }
    }
    work->memberCount = DrawingSession_CountMembers();
    if (work->unk_318 < Comm_CountConnected()) {
        work->unk_93F4 = 2;
        work->unk_9404 = 1;
        work->unk_9408 = Wireless_GetConnectedBitmap() ^ work->unk_31C;
        GF_ASSERT(DrawingSession_IsOneMemberBit(work->unk_9408));
    }
    return 1;
}

#pragma opt_common_subs reset
#pragma force_active reset

void DrawingSession_SetNextSubState(DrawingSessionWork *work, int state) {
    work->nextSubState = state;
}

void DrawingSession_ApplyNextSubState(DrawingSessionWork *work, int mainState) {
    int state = work->nextSubState;
    if (work->subState != state) {
        if (mainState != 2) {
            work->subState = state;
        } else if (sDrawingSessionSubStates[state].unk_04 != 0) {
            work->subState = state;
        }
    }
}

BOOL DrawingSession_ShowYesNo(DrawingSessionWork *work, const YesNoPromptTemplate *template) {
    if (work->yesNoActive == FALSE) {
        YesNoPrompt_InitFromTemplate(work->yesNoPrompt, template);
        work->yesNoActive = TRUE;
        return TRUE;
    }
    return FALSE;
}

void DrawingSession_HideYesNo(DrawingSessionWork *work) {
    if (work->yesNoActive != FALSE) {
        YesNoPrompt_Reset(work->yesNoPrompt);
        work->yesNoActive = FALSE;
    }
}

// work is not used.
BOOL DrawingSession_IsYesNoAreaTouched(DrawingSessionWork *work) {
    if (TouchscreenHitbox_FindRectAtTouchNew(sDrawingSessionYesNoHitbox) != -1) {
        return TRUE;
    }
    return FALSE;
}
