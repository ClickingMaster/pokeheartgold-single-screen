#include "global.h"

#include "application/vs_recorder/vs_recorder_screens.h"
#include "msgdata/msg/msg_0013.h"

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

enum VsRecorderDeleteEnterStep {
    DELETE_ENTER_STEP_FADE_OUT_MENU,
    DELETE_ENTER_STEP_FREE_MENU,
    DELETE_ENTER_STEP_DONE,
};

enum VsRecorderDeleteMainStep {
    DELETE_MAIN_STEP_SHOW,
    DELETE_MAIN_STEP_FADE_IN,
    DELETE_MAIN_STEP_CONFIRM,
    DELETE_MAIN_STEP_CONFIRM_AGAIN,
    DELETE_MAIN_STEP_HIDE_YES_NO,
    DELETE_MAIN_STEP_FADE_OUT,
    DELETE_MAIN_STEP_DELETE,
    DELETE_MAIN_STEP_WAIT_TOUCH,
};

enum VsRecorderDeleteExitStep {
    DELETE_EXIT_STEP_WAIT,
    DELETE_EXIT_STEP_WAIT_AGAIN,
    DELETE_EXIT_STEP_REBUILD_MENU,
    DELETE_EXIT_STEP_FADE_IN_MENU,
};

// YES / NO of the delete prompt
extern const TouchscreenHitbox gVsRecorderDeleteYesHitbox;
extern const TouchscreenHitbox gVsRecorderDeleteNoHitbox;

// substates of the delete screen (gVsRecorderDeleteSubStates)
#define DELETE_ENTER 1
#define DELETE_MAIN  2
#define DELETE_EXIT  3

static void VsRecorder_ShowDeleteWindow(VsRecorderWork *work, BOOL alternate);
static void VsRecorder_HideDeleteWindow(VsRecorderWork *work);
static void VsRecorder_PrintDeleteMessage(VsRecorderWork *work, int msgId, int slot);
static void VsRecorder_ShowDeleteYesNo(VsRecorderWork *work);
static void VsRecorder_HideDeleteYesNo(VsRecorderWork *work);

// the message window of the delete screen (main screen BG 2)
static void VsRecorder_ShowDeleteWindow(VsRecorderWork *work, BOOL alternate) {
    Window *window = &work->messageWindow;
    VsRecorder_LoadMessageBg(work, GF_BG_LYR_MAIN_2, alternate);
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 1, 0x13, 0x1E, 4, 0xE, 0x12C);
}

static void VsRecorder_HideDeleteWindow(VsRecorderWork *work) {
    ClearWindowTilemapAndCopyToVram(&work->messageWindow);
    RemoveWindow(&work->messageWindow);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

// msg 282 ("delete the video of {owner}?") names the owner of saved video `slot`
static void VsRecorder_PrintDeleteMessage(VsRecorderWork *work, int msgId, int slot) {
    Window *window = &work->messageWindow;
    String *text;
    if (msgId == msg_0013_00282) {
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

// YES / NO buttons (sub screen BG 2)
static void VsRecorder_ShowDeleteYesNo(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    Window *window = &deleteWork->yesNoWindows[0];
    String *str;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 4, 6, 0xA, 2, 0xE, 1);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msg_0013_00130);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
    window = &deleteWork->yesNoWindows[1];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 0x12, 6, 0xA, 2, 0xE, 0x15);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msg_0013_00131);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

static void VsRecorder_HideDeleteYesNo(VsRecorderWork *work) {
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
    VsRecorder_SetSubState(work, DELETE_ENTER);
    return FALSE;
}

BOOL VsRecorder_DeleteEnter(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    switch (work->step) {
    case DELETE_ENTER_STEP_FADE_OUT_MENU:
        if (VsRecorder_StepFade(&deleteWork->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)deleteWork->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)deleteWork->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)deleteWork->fade, (u16)work->blendColor);
        break;
    case DELETE_ENTER_STEP_FREE_MENU:
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
        VsRecorder_SetSubState(work, DELETE_MAIN);
        break;
    }
    return FALSE;
}

// asks twice, then deletes the saved video in slot work->selection and says so
BOOL VsRecorder_DeleteMain(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    switch (work->step) {
    case DELETE_MAIN_STEP_SHOW:
        VsRecorder_ShowDeleteYesNo(work);
        VsRecorder_ShowDeleteWindow(work, FALSE);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 84, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case DELETE_MAIN_STEP_FADE_IN:
        VsRecorder_StepFade(&deleteWork->fade, 0);
        if (VsRecorder_StepBlend(&deleteWork->eva, &deleteWork->evb, FALSE, 2)) {
            VsRecorder_PrintDeleteMessage(work, msg_0013_00282, work->selection);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)deleteWork->fade, (u16)work->blendColor);
        break;
    case DELETE_MAIN_STEP_CONFIRM:
        if (TouchscreenHitbox_TouchNewIsIn(&gVsRecorderDeleteYesHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_PrintDeleteMessage(work, msg_0013_00283, 0);
            work->step++;
        } else if (TouchscreenHitbox_TouchNewIsIn(&gVsRecorderDeleteNoHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            deleteWork->cancelled = TRUE;
            VsRecorder_HideDeleteWindow(work);
            work->step = DELETE_MAIN_STEP_HIDE_YES_NO;
        }
        break;
    case DELETE_MAIN_STEP_CONFIRM_AGAIN:
        if (TouchscreenHitbox_TouchNewIsIn(&gVsRecorderDeleteYesHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_ResetRecordSave(work);
            deleteWork->cancelled = FALSE;
            work->step++;
        } else if (TouchscreenHitbox_TouchNewIsIn(&gVsRecorderDeleteNoHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_HideDeleteWindow(work);
            deleteWork->cancelled = TRUE;
            work->step++;
        }
        break;
    case DELETE_MAIN_STEP_HIDE_YES_NO:
        VsRecorder_HideDeleteYesNo(work);
        work->step++;
        break;
    case DELETE_MAIN_STEP_FADE_OUT:
        VsRecorder_StepFade(&deleteWork->fade, 1);
        if (VsRecorder_StepBlend(&deleteWork->eva, &deleteWork->evb, TRUE, 0)) {
            if (deleteWork->cancelled == TRUE) {
                VsRecorder_SetSubState(work, DELETE_EXIT);
            } else {
                work->step++;
            }
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)deleteWork->fade, (u16)work->blendColor);
        break;
    case DELETE_MAIN_STEP_DELETE:
        if (VsRecorder_ClearSavedVideoKeys(work)) {
            VsRecorder_FreeSavedVideos(work);
            VsRecorder_LoadSavedVideos(work);
            VsRecorder_PrintDeleteMessage(work, msg_0013_00285, 0);
            work->step++;
        } else {
            VsRecorder_SetSubState(work, DELETE_EXIT);
        }
        break;
    default:
        if (System_GetTouchNew()) {
            VsRecorder_HideDeleteWindow(work);
            VsRecorder_SetSubState(work, DELETE_EXIT);
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorder_DeleteExit(VsRecorderWork *work) {
    VsRecorderDeleteVideo *deleteWork = work->screenWork;
    switch (work->step) {
    case DELETE_EXIT_STEP_WAIT:
        work->step++;
    case DELETE_EXIT_STEP_WAIT_AGAIN:
        work->step++;
    case DELETE_EXIT_STEP_REBUILD_MENU:
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
                VsRecorder_SetSubState(work, VS_RECORDER_MENU_SLIDE_IN);
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
