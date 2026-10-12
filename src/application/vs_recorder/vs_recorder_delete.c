#include "global.h"

#include "application/vs_recorder/vs_recorder_screens.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "render_text.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

// YES / NO of the delete prompt (asm/to91_w41503_battle_names_anchor.s)
extern const TouchscreenHitbox sVsRecorderDeleteYesHitbox;
extern const TouchscreenHitbox sVsRecorderDeleteNoHitbox;

// the message window of the delete screen (main screen BG 2)
void VsRecorder_ShowDeleteWindow(VsRecorderWork *work, BOOL alternate) {
    Window *window = &work->messageWindow;
    ov40_0222C6C8(work, GF_BG_LYR_MAIN_2, alternate);
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 1, 0x13, 0x1E, 4, 0xE, 0x12C);
}

void VsRecorder_HideDeleteWindow(VsRecorderWork *work) {
    ClearWindowTilemapAndCopyToVram(&work->messageWindow);
    RemoveWindow(&work->messageWindow);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

#pragma force_active on

// msg 282 ("delete the video of {owner}?") names the owner of saved video `slot`
void VsRecorder_PrintDeleteMessage(VsRecorderWork *work, int msgId, int slot) {
    Window *window = &work->messageWindow;
    String *text;
    if (msgId == 282) {
        const u16 *ownerName = (const u16 *)work->videoOwners[slot];
        MessageFormat *format = VsRecorder_NewMessageFormat(HEAP_ID_109);
        String *name, *message;
        text = String_New(255, HEAP_ID_109);
        name = FrontierCard_NewNameString(ownerName, HEAP_ID_109);
        VsRecorder_SanitizeName(work, name);
        message = NewString_ReadMsgData(work->msgData, msgId);
        BufferString(format, 0, name, 0, 1, 2);
        StringExpandPlaceholders(format, text, message);
        String_Delete(name);
        String_Delete(message);
        MessageFormat_Delete(format);
    } else {
        text = NewString_ReadMsgData(work->msgData, msgId);
    }
    FillWindowPixelBuffer(window, 0xCC);
    AddTextPrinterParameterizedWithColor(window, 0, text, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 12), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(text);
}

#pragma force_active reset

// YES / NO buttons (sub screen BG 2)
void VsRecorder_ShowDeleteYesNo(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    Window *window = &deleteWork->yesNoWindows[0];
    String *str;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 4, 6, 0xA, 2, 0xE, 1);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, 0x82);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
    window = &deleteWork->yesNoWindows[1];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 0x12, 6, 0xA, 2, 0xE, 0x15);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, 0x83);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

void VsRecorder_HideDeleteYesNo(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    ClearWindowTilemapAndCopyToVram(&deleteWork->yesNoWindows[0]);
    RemoveWindow(&deleteWork->yesNoWindows[0]);
    ClearWindowTilemapAndCopyToVram(&deleteWork->yesNoWindows[1]);
    RemoveWindow(&deleteWork->yesNoWindows[1]);
}

BOOL VsRecorder_DeleteInit(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderDeleteVideo));
    memset(deleteWork, 0, sizeof(VsRecorderDeleteVideo));
    work->screenWork = deleteWork;
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
    VsRecorder_InitBgBlend(&deleteWork->eva, &deleteWork->evb, 0);
    VsRecorder_SetSubState(work, 1);
    return FALSE;
}

BOOL VsRecorder_DeleteEnter(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    switch (work->step) {
    case 0:
        if (VsRecorder_StepFade(&deleteWork->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)deleteWork->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)deleteWork->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)deleteWork->fade, (u16)work->blendColor);
        break;
    case 1:
        SetBgPriority(GF_BG_LYR_MAIN_0, 0);
        SetBgPriority(GF_BG_LYR_MAIN_1, 3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 2);
        SetBgPriority(GF_BG_LYR_SUB_0, 0);
        SetBgPriority(GF_BG_LYR_SUB_1, 3);
        SetBgPriority(GF_BG_LYR_SUB_2, 1);
        SetBgPriority(GF_BG_LYR_SUB_3, 2);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_SetVBlankPaused(work, 1);
        VsRecorder_FreeMenuRows(work);
        VsRecorder_SetVBlankPaused(work, 0);
        work->step++;
        break;
    default:
        VsRecorder_StartFlipScreens(work, 0);
        VsRecorder_SetSubState(work, 2);
        break;
    }
    return FALSE;
}

#pragma force_active on

// asks twice, then deletes the saved video in slot work->selection and says so
BOOL VsRecorder_DeleteMain(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    switch (work->step) {
    case 0:
        VsRecorder_ShowDeleteYesNo(work);
        VsRecorder_ShowDeleteWindow(work, FALSE);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 84, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&deleteWork->fade, 0);
        if (VsRecorder_StepBlend(&deleteWork->eva, &deleteWork->evb, FALSE, 2)) {
            VsRecorder_PrintDeleteMessage(work, 282, work->selection);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)deleteWork->fade, (u16)work->blendColor);
        break;
    case 2:
        if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderDeleteYesHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_PrintDeleteMessage(work, 283, 0);
            work->step++;
        } else if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderDeleteNoHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            deleteWork->cancelled = TRUE;
            VsRecorder_HideDeleteWindow(work);
            work->step = 4;
        }
        break;
    case 3:
        if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderDeleteYesHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            ov40_022306E0(work);
            deleteWork->cancelled = FALSE;
            work->step++;
        } else if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderDeleteNoHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_HideDeleteWindow(work);
            deleteWork->cancelled = TRUE;
            work->step++;
        }
        break;
    case 4:
        VsRecorder_HideDeleteYesNo(work);
        work->step++;
        break;
    case 5:
        VsRecorder_StepFade(&deleteWork->fade, 1);
        if (VsRecorder_StepBlend(&deleteWork->eva, &deleteWork->evb, TRUE, 0)) {
            if (deleteWork->cancelled == TRUE) {
                VsRecorder_SetSubState(work, 3);
            } else {
                work->step++;
            }
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)deleteWork->fade, (u16)work->blendColor);
        break;
    case 6:
        if (ov40_022306F0(work)) {
            VsRecorder_FreeSavedVideos(work);
            VsRecorder_LoadSavedVideos(work);
            VsRecorder_PrintDeleteMessage(work, 285, 0);
            work->step++;
        } else {
            VsRecorder_SetSubState(work, 3);
        }
        break;
    default:
        if (System_GetTouchNew()) {
            VsRecorder_HideDeleteWindow(work);
            VsRecorder_SetSubState(work, 3);
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL VsRecorder_DeleteExit(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    switch (work->step) {
    case 0:
        work->step++;
    case 1:
        work->step++;
    case 2:
        VsRecorder_RebuildMenuRowsParent(work);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work) != 0) {
            if (VsRecorder_StepFade(&deleteWork->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&deleteWork->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 0x10, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(deleteWork);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)deleteWork->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)deleteWork->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}
