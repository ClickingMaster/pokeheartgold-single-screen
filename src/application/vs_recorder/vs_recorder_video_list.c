#include "global.h"

#include "constants/sndseq.h"

#include "application/vs_recorder/vs_recorder_screens.h"
#include "msgdata/msg/msg_0013.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "pm_string.h"
#include "render_text.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

enum VsRecorderVideoListDownloadVideoStep {
    VIDEO_LIST_DOWNLOAD_VIDEO_STEP_HIDE_INFO,
    VIDEO_LIST_DOWNLOAD_VIDEO_STEP_FADE_OUT,
    VIDEO_LIST_DOWNLOAD_VIDEO_STEP_SHOW_RECEIVING,
    VIDEO_LIST_DOWNLOAD_VIDEO_STEP_REQUEST,
    VIDEO_LIST_DOWNLOAD_VIDEO_STEP_CHECK_RESULT,
    VIDEO_LIST_DOWNLOAD_VIDEO_STEP_DONE,
};

enum VsRecorderVideoListOverwriteNoStep {
    VIDEO_LIST_OVERWRITE_NO_STEP_HIDE,
    VIDEO_LIST_OVERWRITE_NO_STEP_BLEND_OUT,
    VIDEO_LIST_OVERWRITE_NO_STEP_FADE_IN,
    VIDEO_LIST_OVERWRITE_NO_STEP_DONE,
};

enum VsRecorderVideoListSaveStep {
    VIDEO_LIST_SAVE_STEP_HIDE,
    VIDEO_LIST_SAVE_STEP_FADE_OUT,
    VIDEO_LIST_SAVE_STEP_WRITE_RECORD,
    VIDEO_LIST_SAVE_STEP_SHOW_CURSOR,
    VIDEO_LIST_SAVE_STEP_REPORT_SAVED,
    VIDEO_LIST_SAVE_STEP_CHECK_REPORT,
    VIDEO_LIST_SAVE_STEP_WAIT_TOUCH,
    VIDEO_LIST_SAVE_STEP_FREE,
    VIDEO_LIST_SAVE_STEP_FADE_IN_MENU,
};

enum VsRecorderVideoListConfirmOverwriteStep {
    VIDEO_LIST_CONFIRM_OVERWRITE_STEP_FADE_OUT,
    VIDEO_LIST_CONFIRM_OVERWRITE_STEP_BLEND_IN,
    VIDEO_LIST_CONFIRM_OVERWRITE_STEP_DONE,
};

enum VsRecorderVideoListReturnToDetailsStep {
    VIDEO_LIST_RETURN_TO_DETAILS_STEP_HIDE,
    VIDEO_LIST_RETURN_TO_DETAILS_STEP_FADE_OUT,
    VIDEO_LIST_RETURN_TO_DETAILS_STEP_CREATE_BUTTONS,
    VIDEO_LIST_RETURN_TO_DETAILS_STEP_FADE_IN,
    VIDEO_LIST_RETURN_TO_DETAILS_STEP_DONE,
};

enum VsRecorderVideoListSlotsCancelStep {
    VIDEO_LIST_SLOTS_CANCEL_STEP_HIDE,
    VIDEO_LIST_SLOTS_CANCEL_STEP_FADE_OUT,
    VIDEO_LIST_SLOTS_CANCEL_STEP_CREATE_BUTTONS,
    VIDEO_LIST_SLOTS_CANCEL_STEP_FADE_IN,
    VIDEO_LIST_SLOTS_CANCEL_STEP_DONE,
};

enum VsRecorderVideoListBackToListStep {
    VIDEO_LIST_BACK_TO_LIST_STEP_FREE_DETAILS,
    VIDEO_LIST_BACK_TO_LIST_STEP_FADE_OUT,
    VIDEO_LIST_BACK_TO_LIST_STEP_DONE,
};

enum VsRecorderVideoListEnterStep {
    VIDEO_LIST_ENTER_STEP_FADE_OUT_MENU,
    VIDEO_LIST_ENTER_STEP_FLIP_SCREENS,
    VIDEO_LIST_ENTER_STEP_CREATE_BUTTONS,
    VIDEO_LIST_ENTER_STEP_DONE,
};

enum VsRecorderVideoListDownloadListStep {
    VIDEO_LIST_DOWNLOAD_LIST_STEP_SHOW_RECEIVING,
    VIDEO_LIST_DOWNLOAD_LIST_STEP_REQUEST,
    VIDEO_LIST_DOWNLOAD_LIST_STEP_CHECK_RESULT,
    VIDEO_LIST_DOWNLOAD_LIST_STEP_HIDE_CURSOR,
    VIDEO_LIST_DOWNLOAD_LIST_STEP_FADE_OUT,
    VIDEO_LIST_DOWNLOAD_LIST_STEP_REBUILD_MENU,
    VIDEO_LIST_DOWNLOAD_LIST_STEP_FADE_IN_MENU,
};

enum VsRecorderVideoListShowListStep {
    VIDEO_LIST_SHOW_LIST_STEP_CREATE_LIST,
    VIDEO_LIST_SHOW_LIST_STEP_FADE_IN,
    VIDEO_LIST_SHOW_LIST_STEP_DONE,
};

enum VsRecorderVideoListSelectStep {
    VIDEO_LIST_SELECT_STEP_INPUT,
    VIDEO_LIST_SELECT_STEP_FREE_LIST,
    VIDEO_LIST_SELECT_STEP_FADE_OUT,
    VIDEO_LIST_SELECT_STEP_DONE,
};

enum VsRecorderVideoListExitStep {
    VIDEO_LIST_EXIT_STEP_REMOVE_WINDOWS,
    VIDEO_LIST_EXIT_STEP_FADE_OUT,
    VIDEO_LIST_EXIT_STEP_REBUILD_MENU,
    VIDEO_LIST_EXIT_STEP_FADE_IN_MENU,
};

enum VsRecorderVideoListShowDetailsStep {
    VIDEO_LIST_SHOW_DETAILS_STEP_LOAD_SCREEN,
    VIDEO_LIST_SHOW_DETAILS_STEP_CREATE_HITBOXES,
    VIDEO_LIST_SHOW_DETAILS_STEP_CREATE_INFO,
    VIDEO_LIST_SHOW_DETAILS_STEP_BLEND_IN,
    VIDEO_LIST_SHOW_DETAILS_STEP_DONE,
};

enum VsRecorderVideoListToggleProfileStep {
    VIDEO_LIST_TOGGLE_PROFILE_STEP_FREE,
    VIDEO_LIST_TOGGLE_PROFILE_STEP_BLEND_OUT,
    VIDEO_LIST_TOGGLE_PROFILE_STEP_CREATE,
    VIDEO_LIST_TOGGLE_PROFILE_STEP_BLEND_IN,
    VIDEO_LIST_TOGGLE_PROFILE_STEP_DONE,
};

enum VsRecorderVideoListPlayStep {
    VIDEO_LIST_PLAY_STEP_FADE_OUT,
    VIDEO_LIST_PLAY_STEP_WAIT_FADE,
    VIDEO_LIST_PLAY_STEP_FREE,
    VIDEO_LIST_PLAY_STEP_DONE,
};

enum VsRecorderVideoListResumeStep {
    VIDEO_LIST_RESUME_STEP_LOAD_SCREEN,
    VIDEO_LIST_RESUME_STEP_CREATE_HITBOXES,
    VIDEO_LIST_RESUME_STEP_CREATE_INFO,
    VIDEO_LIST_RESUME_STEP_BLEND_IN,
    VIDEO_LIST_RESUME_STEP_FADE_IN,
    VIDEO_LIST_RESUME_STEP_WAIT_FADE,
    VIDEO_LIST_RESUME_STEP_DONE,
};

typedef struct VsRecorderTitleMsgIds {
    u32 msgIds[3];
} VsRecorderTitleMsgIds;

extern const TouchscreenHitbox gVsRecorderVideoListOpenHitbox;
extern const TouchscreenHitbox gVsRecorderVideoListExitHitbox;
extern const TouchscreenHitbox gVsRecorderOverwriteHitboxes[2];
extern const VsRecorderTitleMsgIds gVsRecorderVideoListTitleMsgIds;
extern const TouchscreenHitbox gVsRecorderSaveSlotHitboxes[4];
extern const VsRecorderSlotButtonTable gVsRecorderSaveSlotMsgIds;
extern const VsRecorderSlotButtonTable gVsRecorderSaveSlotAnims;
extern const TouchscreenHitbox gVsRecorderVideoDetailHitboxes[4];
extern const VsRecorderListConfig gVsRecorderVideoListConfig;
extern const VsRecorderPartyIconOrder gVsRecorderScrollIconOrder;
extern const VsRecorderPartyIconOrder gVsRecorderListIconOrder;
extern const VsRecorderIconPositions gVsRecorderVideoListIconPositions;

// the list entries are video headers; the icons read their party part
#define VIDEO_PARTY(work, i) ((VsRecorderVideoParty *)(work)->net.videoList.headers[i])

// VsRecorderVideoListScreen substates (gVsRecorderVideoListSubStates)
#define VIDEO_LIST_ENTER             1
#define VIDEO_LIST_DOWNLOAD_LIST     2
#define VIDEO_LIST_SHOW_LIST         3
#define VIDEO_LIST_SELECT            4
#define VIDEO_LIST_EXIT              5
#define VIDEO_LIST_SHOW_DETAILS      6
#define VIDEO_LIST_DETAILS_MAIN      7
#define VIDEO_LIST_TOGGLE_PROFILE    8
#define VIDEO_LIST_PLAY              9
#define VIDEO_LIST_BACK_TO_LIST      10
#define VIDEO_LIST_SHOW_SLOTS        11
#define VIDEO_LIST_SLOTS_MAIN        12
#define VIDEO_LIST_SLOTS_CANCEL      13
#define VIDEO_LIST_CONFIRM_OVERWRITE 14
#define VIDEO_LIST_OVERWRITE_MAIN    15
#define VIDEO_LIST_OVERWRITE_NO      16
#define VIDEO_LIST_SAVE              17
#define VIDEO_LIST_DOWNLOAD_VIDEO    18
#define VIDEO_LIST_RESUME            19
#define VIDEO_LIST_RETURN_TO_DETAILS 20

static void VsRecorder_PlaceVideoListIcons(VsRecorderWork *work);
static void VsRecorder_CreateVideoListButtons(VsRecorderWork *work);
static void VsRecorder_PrintVideoListMessage(VsRecorderWork *work, int msgId, int slot);
static void VsRecorder_FreeVideoDetails(VsRecorderWork *work);
static void VsRecorder_SetSlotButtonName(VsRecorderTextSprite *button, VsRecorderWork *work, int slot);
static void VsRecorder_CreateSlotButtons(VsRecorderWork *work);
static void VsRecorder_FreeSlotButtons(VsRecorderWork *work);
static void VsRecorder_FreeVideoListButtons(VsRecorderWork *work);
static void VsRecorder_SetVideoListButtonsMode(VsRecorderWork *work, int backOnly);
static void VsRecorder_RemoveVideoListWindows(VsRecorderWork *work);
static void VsRecorder_RemoveVideoListButtonWindows(VsRecorderWork *work);
static void VsRecorder_CreateVideoListWindows(VsRecorderWork *work);
static void VsRecorder_ShowVideoListMessage(VsRecorderWork *work, int msgId, int unused);
static void VsRecorder_RemoveDetailsButtonWindow(VsRecorderWork *work);
static void VsRecorder_PrintDetailsButtonLabel(VsRecorderWork *work, int msgId);
static void VsRecorder_CreateDetailsButtonWindow(VsRecorderWork *work);
static void VsRecorder_CreateOverwriteWindows(VsRecorderWork *work);
static void VsRecorder_RemoveOverwriteWindows(VsRecorderWork *work);
static void VsRecorder_OnVideoDetailsTouch(u32 index, u32 event, void *arg);
static void VsRecorder_OnSaveSlotTouch(u32 index, u32 event, void *arg);
static void VsRecorder_OnOverwriteTouch(u32 index, u32 event, void *arg);
static void VsRecorder_LoadVideoDetails(VsRecorderWork *work);
static void VsRecorder_CreateVideoListIcons(VsRecorderWork *work);
static void VsRecorder_SetVideoListIconsDrawFlag(VsRecorderWork *work, BOOL draw);
static void VsRecorder_FreeVideoListIcons(VsRecorderWork *work);
static void VsRecorder_ScrollVideoListIcons(VsRecorderWork *work);
static void VsRecorder_FreeDetailsButtons(VsRecorderWork *work);

void VsRecorder_OnRankingsHeaderReceived(void *arg, GlobalTerminalNetError *error) {
    VsRecorderWork *work = arg;
    VsRecorderRankings *rankings = work->screenWork;

    if (error->isError == TRUE) {
        rankings->recordIds = rankings->recordIdsBuf;
    } else {
        GlobalTerminalNet_GetRankingsHeader(VsRecorder_GetNet(work), (void **)&rankings->recordIds);
        MI_CpuCopy8(rankings->recordIds, rankings->recordIdsBuf, sizeof(rankings->recordIdsBuf));
    }
}

void VsRecorder_OnRankingsReceived(void *arg, GlobalTerminalNetError *error) {
    VsRecorderWork *work = arg;
    VsRecorderRankings *rankings = work->screenWork;

    if (error->isError == TRUE) {
        rankings->results = rankings->resultsBuf;
        rankings->standings = rankings->standingsBuf;
    } else {
        GlobalTerminalNet_GetRankings(VsRecorder_GetNet(work), (void **)&rankings->results, (void **)&rankings->standings);
        MI_CpuCopy8(rankings->results, rankings->resultsBuf, sizeof(rankings->resultsBuf));
        MI_CpuCopy8(rankings->standings, rankings->standingsBuf, sizeof(rankings->standingsBuf));
    }
}

static void VsRecorder_PlaceVideoListIcons(VsRecorderWork *work) {
    VsRecorderIconPositions positions = gVsRecorderVideoListIconPositions;
    int i, j;
    VsRecorderVideoListScreen *listWork = work->screenWork;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            if (listWork->icons[i][j].sprite != NULL) {
                ManagedSprite_SetPositionXY(listWork->icons[i][j].sprite, positions.pos[i * 6 + j].x, positions.pos[i * 6 + j].y);
            }
        }
    }
}

// exit and back buttons of the list and the details
static void VsRecorder_CreateVideoListButtons(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    VsRecorder_LoadButtonGfx31(work, NNS_G2D_VRAM_TYPE_2DSUB);
    listWork->buttons[0].sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    listWork->buttons[1].sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&listWork->buttons[0], (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&listWork->buttons[1], (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_SetTextSpriteMsg(&listWork->buttons[0], (VsRecorderGfx *)&work->narc, 3);
    VsRecorder_SetTextSpriteMsg(&listWork->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
    ManagedSprite_SetAnim(listWork->buttons[0].sprite, 0);
    ManagedSprite_SetAnim(listWork->buttons[1].sprite, 3);
    ManagedSprite_SetPositionXY(listWork->buttons[0].sprite, 32, 232);
    ManagedSprite_SetPositionXY(listWork->buttons[1].sprite, 128, 232);
    TextOBJ_SetPosition(listWork->buttons[0].text, 36, -8);
    TextOBJ_SetPosition(listWork->buttons[1].text, 36, -8);
    TextOBJ_SetSpritesDrawFlag(listWork->buttons[0].text, TRUE);
    TextOBJ_SetSpritesDrawFlag(listWork->buttons[1].text, TRUE);
    VsRecorder_SetVideoListButtonsMode(work, FALSE);
}

// message window; msg 0x64 names the video's owner, msg 0x66 the owner of the video in slot
static void VsRecorder_PrintVideoListMessage(VsRecorderWork *work, int msgId, int slot) {
    Window *window = &work->messageWindow;
    VsRecorderVideoListScreen *listWork = work->screenWork;
    String *str;

    if (msgId == msg_0013_00100) {
        FrontierCard *card = listWork->profile.card;
        MessageFormat *msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
        String *formatted = String_New(255, HEAP_ID_109);
        String *name = FrontierCard_NewNameString((const u16 *)card, HEAP_ID_109);
        String *template;

        VsRecorder_SanitizeName(work, name);
        template = NewString_ReadMsgData(work->msgData, msgId);
        BufferString(msgFormat, 0, name, 0, 1, 2);
        StringExpandPlaceholders(msgFormat, formatted, template);
        String_Delete(name);
        String_Delete(template);
        MessageFormat_Delete(msgFormat);
        str = formatted;
    } else if (msgId == msg_0013_00102) {
        FrontierCard *card = work->videoOwners[slot];
        MessageFormat *msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
        String *formatted = String_New(255, HEAP_ID_109);
        String *name = FrontierCard_NewNameString((const u16 *)card, HEAP_ID_109);
        String *template;

        VsRecorder_SanitizeName(work, name);
        template = NewString_ReadMsgData(work->msgData, msgId);
        BufferString(msgFormat, 0, name, 0, 1, 2);
        StringExpandPlaceholders(msgFormat, formatted, template);
        String_Delete(name);
        String_Delete(template);
        MessageFormat_Delete(msgFormat);
        str = formatted;
    } else {
        str = NewString_ReadMsgData(work->msgData, msgId);
    }
    FillWindowPixelBuffer(window, 0xCC);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 12), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

// download the chosen video, then go on to nextSubState
BOOL VsRecorder_VideoListDownloadVideo(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    if (!VsRecorder_IsNetIdle(work)) {
        return FALSE;
    }
    switch (work->step) {
    case VIDEO_LIST_DOWNLOAD_VIDEO_STEP_HIDE_INFO:
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        if (listWork->showProfile == 0) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, FALSE);
        } else {
            VsRecorder_SetProfileDrawFlag(&listWork->profile, FALSE);
        }
        work->step++;
        // fallthrough
    case VIDEO_LIST_DOWNLOAD_VIDEO_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_DOWNLOAD_VIDEO_STEP_SHOW_RECEIVING:
        VsRecorder_ShowTopMessage(work, msg_0013_00117);
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        listWork->downloaded = FALSE;
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case VIDEO_LIST_DOWNLOAD_VIDEO_STEP_REQUEST: {
        GlobalTerminalNetVideoHeader *header;
        GlobalTerminalNet *net = VsRecorder_GetNet(work);

        header = work->net.videoList.headers[work->listWindow.cursor];
        if (GlobalTerminalNet_RequestVideoDownload(net, (u32)header->number, (u32)(header->number >> 32)) == TRUE) {
            work->step++;
        }
        break;
    }
    case VIDEO_LIST_DOWNLOAD_VIDEO_STEP_CHECK_RESULT: {
        GlobalTerminalNetError *error;
        GlobalTerminalNet *net;

        VsRecorder_HideTopMessage(work);
        net = VsRecorder_GetNet(work);
        if (GlobalTerminalNet_GetError(net, &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_ShowMessageScreen(work, VS_RECORDER_REQUEST_VIDEO_DOWNLOAD, error->code, error->type);
            work->step++;
            listWork->downloaded = FALSE;
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            work->step = VS_RECORDER_STEP_FINISH;
            listWork->downloaded = TRUE;
            work->videoFlagPtr = (u32)&work->net.videoList.flags[work->listWindow.cursor];
            listWork->downloadedVideo = work->net.downloadedVideo;
            MI_CpuCopy8(listWork->downloadedVideo, work->net.videoBuffer, 0x1D4C);
        }
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        break;
    }
    default:
        if (listWork->downloaded) {
            PlaySE(SEQ_SE_PL_BREC12);
            VsRecorder_SetSubState(work, listWork->nextSubState);
            VsRecorder_SetBattleResult(work, 1);
        } else {
            VsRecorder_SetSubState(work, VIDEO_LIST_RETURN_TO_DETAILS);
        }
        break;
    }
    return FALSE;
}

static void VsRecorder_FreeVideoDetails(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    BattleRecordHeader_Free(listWork->info.header);
    FrontierCard_Delete(listWork->profile.card);
}

BOOL VsRecorder_VideoListOverwriteMain(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    TouchHitboxController_IsTriggered(listWork->overwriteHitboxes);
    return FALSE;
}

// NO to the overwrite prompt: back to the slot choice
BOOL VsRecorder_VideoListOverwriteNo(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_OVERWRITE_NO_STEP_HIDE:
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case VIDEO_LIST_OVERWRITE_NO_STEP_BLEND_OUT:
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 2)) {
            VsRecorder_RemoveOverwriteWindows(work);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
            work->step++;
        }
        break;
    case VIDEO_LIST_OVERWRITE_NO_STEP_FADE_IN:
        VsRecorder_StepFade(&listWork->fade, 0);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 2)) {
            VsRecorder_PrintVideoListMessage(work, msg_0013_00100, 0);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, VIDEO_LIST_SLOTS_MAIN);
        break;
    }
    return FALSE;
}

// save the downloaded video to listWork->slot, then back to the menu
BOOL VsRecorder_VideoListSave(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_SAVE_STEP_HIDE:
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case VIDEO_LIST_SAVE_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 2)) {
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            VsRecorder_PrintVideoListMessage(work, msg_0013_00278, 0);
            VsRecorder_ResetRecordSave(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_SAVE_STEP_WRITE_RECORD: {
        u32 cursor = work->listWindow.cursor;

        if (VsRecorder_StepRecordSave(work, listWork->slot, work->net.videoList.flags[cursor])) {
            work->step++;
        }
        break;
    }
    case VIDEO_LIST_SAVE_STEP_SHOW_CURSOR:
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        work->step++;
        PlaySE(SEQ_SE_PL_BREC80);
        break;
    case VIDEO_LIST_SAVE_STEP_REPORT_SAVED: {
        u64 number;

        if (!VsRecorder_IsNetIdle(work)) {
            return FALSE;
        }
        VsRecorder_PrintVideoListMessage(work, msg_0013_00280, 0);
        number = BattleRecordHeader_GetField(BattleRecord_GetHeader(), 4, 0);
        if (GlobalTerminalNet_ReportVideoSaved(VsRecorder_GetNet(work), (u32)number, (u32)(number >> 32)) == TRUE) {
            work->step++;
        }
        break;
    }
    case VIDEO_LIST_SAVE_STEP_CHECK_REPORT: {
        GlobalTerminalNetError *error;

        if (!VsRecorder_IsNetIdle(work)) {
            return FALSE;
        }
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            PlaySE(SEQ_SE_PL_BREC12);
        }
        work->step++;
        VsRecorder_PrintVideoListMessage(work, msg_0013_00281, 0);
        break;
    }
    case VIDEO_LIST_SAVE_STEP_WAIT_TOUCH:
        if (System_GetTouchNew()) {
            VsRecorder_RemoveTopMessage(work);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeSlotButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
            VsRecorder_FreeSavedVideos(work);
            VsRecorder_LoadSavedVideos(work);
            VsRecorder_ClearListWindow(&work->listWindow);
            work->step++;
        }
        break;
    case VIDEO_LIST_SAVE_STEP_FREE:
        TouchHitboxController_Destroy(listWork->detailHitboxes);
        TouchHitboxController_Destroy(listWork->slotHitboxes);
        TouchHitboxController_Destroy(listWork->overwriteHitboxes);
        VsRecorder_FreeVideoDetails(work);
        VsRecorder_ResetFade(&listWork->fade);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_109);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&listWork->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&listWork->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, VS_RECORDER_MENU_SLIDE_IN);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                Heap_Free(listWork);
                if (BattleRecord_Exists() == TRUE) {
                    BattleRecord_Delete();
                }
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)listWork->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

// the chosen slot holds a video: YES / NO to overwrite it
BOOL VsRecorder_VideoListConfirmOverwrite(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_CONFIRM_OVERWRITE_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 2)) {
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 84, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            VsRecorder_PrintVideoListMessage(work, msg_0013_00102, listWork->slot);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_CONFIRM_OVERWRITE_STEP_BLEND_IN:
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 2)) {
            VsRecorder_CreateOverwriteWindows(work);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        break;
    default:
        VsRecorder_SetSubState(work, VIDEO_LIST_OVERWRITE_MAIN);
        break;
    }
    return FALSE;
}

// back from the message screen (download error) to the details
BOOL VsRecorder_VideoListReturnToDetails(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;
    int i;

    switch (work->step) {
    case VIDEO_LIST_RETURN_TO_DETAILS_STEP_HIDE:
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case VIDEO_LIST_RETURN_TO_DETAILS_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_RETURN_TO_DETAILS_STEP_CREATE_BUTTONS:
        if (work->selection == VS_RECORDER_SELECTION_SEARCH) {
            VsRecorder_SetTextSpriteMsg(&listWork->buttons[0], (VsRecorderGfx *)&work->narc, 3);
            VsRecorder_SetTextSpriteMsg(&listWork->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
            ManagedSprite_SetAnim(listWork->buttons[0].sprite, 0);
            ManagedSprite_SetAnim(listWork->buttons[1].sprite, 3);
        }
        VsRecorder_RemoveDetailsButtonWindow(work);
        VsRecorder_CreateDetailsButtonWindow(work);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (listWork->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&listWork->info, work);
            VsRecorder_CreateVideoInfo(&listWork->info, work);
            VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, FALSE);
            VsRecorder_LoadScreen(work, VsRecorderVideoInfo_GetScreenId(&listWork->info), GF_BG_LYR_MAIN_3);
        } else {
            VsRecorder_FreeProfile(&listWork->profile, work);
            VsRecorder_CreateProfile(&listWork->profile, work);
            VsRecorder_SetProfileDrawFlag(&listWork->profile, FALSE);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 80, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 6, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    case VIDEO_LIST_RETURN_TO_DETAILS_STEP_FADE_IN:
        VsRecorder_StepFade(&listWork->fade, 0);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 0)) {
            if (listWork->showProfile == 0) {
                VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, TRUE);
            } else {
                VsRecorder_SetProfileDrawFlag(&listWork->profile, TRUE);
            }
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    default:
        for (i = 0; i < work->net.videoList.count; i++) {
            work->net.videoList.headers[i] = &work->net.videoList.videos[i];
        }
        VsRecorder_SetSubState(work, VIDEO_LIST_DETAILS_MAIN);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoListSlotsMain(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    TouchHitboxController_IsTriggered(listWork->slotHitboxes);
    return FALSE;
}

// cancel in the slot menu: back to the details
BOOL VsRecorder_VideoListSlotsCancel(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_SLOTS_CANCEL_STEP_HIDE:
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case VIDEO_LIST_SLOTS_CANCEL_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeSlotButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_SLOTS_CANCEL_STEP_CREATE_BUTTONS:
        VsRecorder_CreateVideoListButtons(work);
        if (work->selection == VS_RECORDER_SELECTION_SEARCH) {
            VsRecorder_SetTextSpriteMsg(&listWork->buttons[0], (VsRecorderGfx *)&work->narc, 3);
            VsRecorder_SetTextSpriteMsg(&listWork->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
            ManagedSprite_SetAnim(listWork->buttons[0].sprite, 0);
            ManagedSprite_SetAnim(listWork->buttons[1].sprite, 3);
        }
        VsRecorder_CreateDetailsButtonWindow(work);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (listWork->showProfile == 0) {
            VsRecorder_CreateVideoInfo(&listWork->info, work);
            VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, FALSE);
            VsRecorder_LoadScreen(work, VsRecorderVideoInfo_GetScreenId(&listWork->info), GF_BG_LYR_MAIN_3);
        } else {
            VsRecorder_CreateProfile(&listWork->profile, work);
            VsRecorder_SetProfileDrawFlag(&listWork->profile, FALSE);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 80, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 6, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    case VIDEO_LIST_SLOTS_CANCEL_STEP_FADE_IN:
        VsRecorder_StepFade(&listWork->fade, 0);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 0)) {
            if (listWork->showProfile == 0) {
                VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, TRUE);
            } else {
                VsRecorder_SetProfileDrawFlag(&listWork->profile, TRUE);
            }
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    default: {
        int i;

        for (i = 0; i < work->net.videoList.count; i++) {
            work->net.videoList.headers[i] = &work->net.videoList.videos[i];
        }
        VsRecorder_SetSubState(work, VIDEO_LIST_DETAILS_MAIN);
        break;
    }
    }
    return FALSE;
}

// back from the details to the list
BOOL VsRecorder_VideoListBackToList(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_BACK_TO_LIST_STEP_FREE_DETAILS:
        TouchHitboxController_Destroy(listWork->detailHitboxes);
        TouchHitboxController_Destroy(listWork->slotHitboxes);
        TouchHitboxController_Destroy(listWork->overwriteHitboxes);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        VsRecorder_FreeVideoDetails(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (listWork->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&listWork->info, work);
        } else {
            VsRecorder_FreeProfile(&listWork->profile, work);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_RemoveDetailsButtonWindow(work);
        work->step++;
        break;
    case VIDEO_LIST_BACK_TO_LIST_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
            VsRecorder_SetTextSpriteMsg(&listWork->buttons[0], (VsRecorderGfx *)&work->narc, 3);
            VsRecorder_SetTextSpriteMsg(&listWork->buttons[1], (VsRecorderGfx *)&work->narc, 0x6F);
            ManagedSprite_SetAnim(listWork->buttons[0].sprite, 0);
            ManagedSprite_SetAnim(listWork->buttons[1].sprite, 1);
            VsRecorder_SetVideoListButtonsMode(work, TRUE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, VIDEO_LIST_SHOW_LIST);
        break;
    }
    return FALSE;
}

// slot button label: the owner of the video saved there, or empty
static void VsRecorder_SetSlotButtonName(VsRecorderTextSprite *button, VsRecorderWork *work, int slot) {
    Window window;
    String *text;
    FrontierCard *owner = work->videoOwners[slot];
    MsgData *msgData = work->msgData;

    if (owner == NULL) {
        text = NewString_ReadMsgData(msgData, msg_0013_00008);
    } else {
        MessageFormat *msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
        String *template = NewString_ReadMsgData(work->msgData, msg_0013_00007);
        String *name = FrontierCard_NewNameString((const u16 *)work->videoOwners[slot], HEAP_ID_109);

        VsRecorder_SanitizeName(work, name);
        text = String_New(255, HEAP_ID_109);
        BufferString(msgFormat, 0, name, 0, 1, 2);
        StringExpandPlaceholders(msgFormat, text, template);
        String_Delete(template);
        String_Delete(name);
        MessageFormat_Delete(msgFormat);
    }
    InitWindow(&window);
    AddTextWindowTopLeftCorner(work->bgConfig, &window, 20, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(&window, 0, text, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 13, 0), NULL);
    TextOBJ_CopyFromBGWindow(button->text, button->textLayout, &window, HEAP_ID_109);
    String_Delete(text);
    RemoveWindow(&window);
}

// the three slots and cancel
static void VsRecorder_CreateSlotButtons(VsRecorderWork *work) {
    u8 *listWork = work->screenWork;
    VsRecorderTextSprite *button;
    VsRecorderSlotButtonTable msgIds = gVsRecorderSaveSlotMsgIds;
    VsRecorderSlotButtonTable anims = gVsRecorderSaveSlotAnims;

    struct {
        int y;
        int *msgId;
        VsRecorderGfx *volatile gfx;
    } vars;

    int *anim;
    int i;

    VsRecorder_LoadSlotButtonGfx(work, NNS_G2D_VRAM_TYPE_2DSUB);
    button = ((VsRecorderVideoListScreen *)listWork)->buttons;
    vars.msgId = msgIds.values;
    vars.y = 105;
    i = 0;
    vars.gfx = (VsRecorderGfx *)work;
    anim = anims.values;
    vars.gfx = (VsRecorderGfx *)&work->narc;
    for (; i < 4; i++) {
        ((VsRecorderVideoListScreen *)listWork)->buttons[0].sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
        VsRecorder_CreateTextSprite(button, vars.gfx, NNS_G2D_VRAM_TYPE_2DSUB);
        VsRecorder_SetTextSpriteMsg(button, vars.gfx, *vars.msgId);
        if (i != 3) {
            VsRecorder_SetSlotButtonName(button, work, i + 1);
        }
        ManagedSprite_SetPositionXY(((VsRecorderVideoListScreen *)listWork)->buttons[0].sprite, 50, vars.y);
        ManagedSprite_SetAnim(((VsRecorderVideoListScreen *)listWork)->buttons[0].sprite, *anim);
        TextOBJ_SetPosition(((VsRecorderVideoListScreen *)listWork)->buttons[0].text, 36, -8);
        TextOBJ_SetSpritesDrawFlag(((VsRecorderVideoListScreen *)listWork)->buttons[0].text, TRUE);
        vars.msgId++;
        {
            int nextY = vars.y + 36;

            listWork += sizeof(VsRecorderTextSprite);
            button++;
            anim++;
            vars.y = nextY;
        }
    }
}

static void VsRecorder_FreeSlotButtons(VsRecorderWork *work) {
    int i;
    VsRecorderVideoListScreen *listWork = work->screenWork;

    for (i = 0; i < 4; i++) {
        VsRecorder_FreeTextSprite(&listWork->buttons[i]);
        Sprite_DeleteAndFreeResources(listWork->buttons[i].sprite);
    }
    VsRecorder_FreeButtonGfx(work);
}

static void VsRecorder_FreeVideoListButtons(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    VsRecorder_FreeTextSprite(&listWork->buttons[0]);
    VsRecorder_FreeTextSprite(&listWork->buttons[1]);
    Sprite_DeleteAndFreeResources(listWork->buttons[0].sprite);
    Sprite_DeleteAndFreeResources(listWork->buttons[1].sprite);
    VsRecorder_FreeButtonGfx(work);
}

// backOnly: hide the second button and move the first one right
static void VsRecorder_SetVideoListButtonsMode(VsRecorderWork *work, int backOnly) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    if (backOnly == FALSE) {
        ManagedSprite_SetDrawFlag(listWork->buttons[1].sprite, TRUE);
        TextOBJ_SetSpritesDrawFlag(listWork->buttons[1].text, TRUE);
        ManagedSprite_SetPositionXY(listWork->buttons[0].sprite, 32, 232);
    } else {
        ManagedSprite_SetDrawFlag(listWork->buttons[1].sprite, FALSE);
        TextOBJ_SetSpritesDrawFlag(listWork->buttons[1].text, FALSE);
        ManagedSprite_SetPositionXY(listWork->buttons[0].sprite, 80, 232);
    }
    TextOBJ_SetPosition(listWork->buttons[0].text, 36, -8);
    TextOBJ_SetPosition(listWork->buttons[1].text, 36, -8);
}

static void VsRecorder_RemoveVideoListWindows(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;
    int i;
    Window *window;

    for (i = 0; i < listWork->windowCount; i++) {
        window = &listWork->windows[i];
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
    listWork->windowCount = 0;
}

static void VsRecorder_RemoveVideoListButtonWindows(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;
    int i;
    Window *window;

    for (i = 0; i < listWork->buttonWindowCount; i++) {
        window = &listWork->buttonWindows[i];
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
    listWork->buttonWindowCount = 0;
}

BOOL VsRecorder_VideoListInit(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderVideoListScreen));

    memset(listWork, 0, sizeof(VsRecorderVideoListScreen));
    work->screenWork = listWork;
    if (work->selection == VS_RECORDER_SELECTION_COLOSSEUM) {
        listWork->listType = VS_RECORDER_VIDEO_LIST_COLOSSEUM;
    } else if (work->selection == VS_RECORDER_SELECTION_BATTLE_FRONTIER) {
        listWork->listType = VS_RECORDER_VIDEO_LIST_BATTLE_FRONTIER;
    } else if (work->selection == VS_RECORDER_SELECTION_NEWEST_30) {
        listWork->listType = VS_RECORDER_VIDEO_LIST_NEWEST_30;
    }
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
    VsRecorder_InitBgBlend(&listWork->eva, &listWork->evb, 0);
    VsRecorder_SetSubState(work, VIDEO_LIST_ENTER);
    return FALSE;
}

BOOL VsRecorder_VideoListEnter(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_ENTER_STEP_FADE_OUT_MENU:
        if (VsRecorder_StepFade(&listWork->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_ENTER_STEP_FLIP_SCREENS:
        SetBgPriority(GF_BG_LYR_MAIN_0, 1);
        SetBgPriority(GF_BG_LYR_MAIN_1, 3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_SUB_0, 1);
        SetBgPriority(GF_BG_LYR_SUB_1, 3);
        SetBgPriority(GF_BG_LYR_SUB_2, 0);
        SetBgPriority(GF_BG_LYR_SUB_3, 2);
        VsRecorder_StartFlipScreens(work, 0);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        work->step++;
        break;
    case VIDEO_LIST_ENTER_STEP_CREATE_BUTTONS:
        if (VsRecorder_IsFlipScreensDone(work)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeMenuRows(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateVideoListButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step++;
        }
        break;
    default:
        if (VsRecorder_HasResumeScreen(work) == TRUE) {
            VsRecorder_SetSubState(work, VIDEO_LIST_RESUME);
        } else {
            VsRecorder_SetSubState(work, VIDEO_LIST_DOWNLOAD_LIST);
        }
        break;
    }
    return FALSE;
}

// request the list (latest / popular / search result) from the server
BOOL VsRecorder_VideoListDownloadList(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;
    GlobalTerminalNetError *error;

    if (!VsRecorder_IsNetIdle(work)) {
        return FALSE;
    }
    switch (work->step) {
    case VIDEO_LIST_DOWNLOAD_LIST_STEP_SHOW_RECEIVING:
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        if (work->selection == VS_RECORDER_SELECTION_COLOSSEUM || work->selection == VS_RECORDER_SELECTION_BATTLE_FRONTIER) {
            VsRecorder_ShowTopMessage(work, msg_0013_00279);
        } else {
            VsRecorder_ShowTopMessage(work, msg_0013_00292);
        }
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case VIDEO_LIST_DOWNLOAD_LIST_STEP_REQUEST:
        if (work->selection == VS_RECORDER_SELECTION_COLOSSEUM) {
            listWork->listType = VS_RECORDER_VIDEO_LIST_COLOSSEUM;
            if (GlobalTerminalNet_RequestTopColosseumVideos(VsRecorder_GetNet(work)) == TRUE) {
                work->step++;
            }
        } else if (work->selection == VS_RECORDER_SELECTION_BATTLE_FRONTIER) {
            listWork->listType = VS_RECORDER_VIDEO_LIST_BATTLE_FRONTIER;
            if (GlobalTerminalNet_RequestTopFrontierVideos(VsRecorder_GetNet(work)) == TRUE) {
                work->step++;
            }
        } else if (work->selection == VS_RECORDER_SELECTION_NEWEST_30) {
            listWork->listType = VS_RECORDER_VIDEO_LIST_NEWEST_30;
            if (VsRecorderApp_HasVisitedBattleFrontier(work) == TRUE) {
                if (GlobalTerminalNet_RequestDefaultVideoSearch(VsRecorder_GetNet(work)) == TRUE) {
                    work->step++;
                }
            } else if (GlobalTerminalNet_RequestDefaultVideoSearchNoFrontier(VsRecorder_GetNet(work)) == TRUE) {
                work->step++;
            }
        } else {
            GF_ASSERT(FALSE);
        }
        break;
    case VIDEO_LIST_DOWNLOAD_LIST_STEP_CHECK_RESULT:
        VsRecorder_HideTopMessage(work);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_ShowMessageScreen(work, VS_RECORDER_REQUEST_VIDEO_SEARCH, error->code, error->type);
            work->step++;
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            PlaySE(SEQ_SE_PL_BREC12);
            if (work->net.videoList.count == 0) {
                VsRecorder_ShowMessageScreen(work, 5, 0, 0);
                work->messageScreen.msgId = listWork->listType == VS_RECORDER_VIDEO_LIST_NEWEST_30 ? msg_0013_00118 : msg_0013_00300;
                OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
                work->step++;
            } else {
                work->step = VS_RECORDER_STEP_FINISH;
            }
        }
        break;
    case VIDEO_LIST_DOWNLOAD_LIST_STEP_HIDE_CURSOR:
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        work->step++;
        break;
    case VIDEO_LIST_DOWNLOAD_LIST_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_DOWNLOAD_LIST_STEP_REBUILD_MENU:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeVideoListButtons(work);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_109);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    case VIDEO_LIST_DOWNLOAD_LIST_STEP_FADE_IN_MENU:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&listWork->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&listWork->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, VS_RECORDER_MENU_SLIDE_IN);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                Heap_Free(listWork);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)listWork->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
            }
        }
        break;
    default:
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        VsRecorder_SetSubState(work, VIDEO_LIST_SHOW_LIST);
        break;
    }
    return FALSE;
}

// list title (by list type) and the label under the touch area
static void VsRecorder_CreateVideoListWindows(VsRecorderWork *work) {
    VsRecorderTitleMsgIds titles;
    VsRecorderVideoListScreen *listWork = work->screenWork;
    Window *window = &listWork->windows[0];
    String *str;
    u32 x;

    titles = gVsRecorderVideoListTitleMsgIds;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 3, 3, 20, 4, 14, 0x20);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, titles.msgIds[listWork->listType]);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
    window = &listWork->buttonWindows[0];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 11, 6, 10, 4, 14, 0x100);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msg_0013_00095);
    x = (80 - FontID_String_GetWidthMultiline(0, str, 0)) / 2;
    AddTextPrinterParameterizedWithColor(window, 0, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
    listWork->buttonWindowCount = 1;
    listWork->windowCount = 1;
}

BOOL VsRecorder_VideoListShowList(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_SHOW_LIST_STEP_CREATE_LIST:
        if (BattleRecord_Exists()) {
            BattleRecord_Delete();
        }
        listWork->listConfig = gVsRecorderVideoListConfig;
        listWork->listConfig.entries = NULL;
        listWork->listConfig.count = work->net.videoList.count;
        VsRecorder_LoadMonIconGfx(work);
        listWork->monIconNarc = VsRecorder_OpenMonIconNarc(HEAP_ID_109);
        listWork->iconResId = 500;
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_LoadScreen(work, 4, GF_BG_LYR_MAIN_3);
        VsRecorder_LoadScreen(work, 7, GF_BG_LYR_SUB_3);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        VsRecorder_SetVideoListButtonsMode(work, TRUE);
        VsRecorder_CreateVideoListWindows(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateTouchScrollSprites(&work->touchScroll, work);
        VsRecorder_InitList(&work->listWindow, work, NULL, &listWork->listConfig);
        work->listWindow.isPairList = 1;
        VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
        VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DMAIN);
        VsRecorder_SetListArrowsY(&work->listWindow, 64, 184);
        VsRecorder_RedrawNameList(&work->listWindow, work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateVideoListIcons(work);
        VsRecorder_SetVideoListIconsDrawFlag(work, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        break;
    case VIDEO_LIST_SHOW_LIST_STEP_FADE_IN:
        VsRecorder_StepFade(&listWork->fade, 0);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 0)) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            VsRecorder_SetVideoListIconsDrawFlag(work, TRUE);
            VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 16, work->listWindow.cursorRow * 24 + 76);
            OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 12, 12);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, VIDEO_LIST_SELECT);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoListSelect(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_SELECT_STEP_INPUT:
        VsRecorder_UpdateTouchScroll(&work->touchScroll);
        VsRecorder_ListMoveCursor(&work->listWindow, work->touchScroll.direction);
        VsRecorder_RedrawNameList(&work->listWindow, work);
        OrbitCursor_MoveTarget(work->orbitCursors[0], 16, (s16)(work->listWindow.cursorRow * 24 + 76));
        VsRecorder_ScrollVideoListIcons(work);
        if (TouchscreenHitbox_TouchNewIsIn(&gVsRecorderVideoListOpenHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            if (work->net.videoList.headers[work->listWindow.cursor] != NULL) {
                listWork->nextSubState = VIDEO_LIST_SHOW_DETAILS;
                listWork->showProfile = 0;
                work->step++;
            }
        }
        if (TouchscreenHitbox_TouchNewIsIn(&gVsRecorderVideoListExitHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            listWork->nextSubState = VIDEO_LIST_EXIT;
            work->step++;
        }
        break;
    case VIDEO_LIST_SELECT_STEP_FREE_LIST:
        VsRecorder_FreeTouchScroll(&work->touchScroll);
        VsRecorder_RemoveListWindow(&work->listWindow);
        VsRecorder_FreeListArrows(&work->listWindow, work);
        VsRecorder_CloseMonIconNarc(listWork->monIconNarc);
        VsRecorder_FreeVideoListIcons(work);
        VsRecorder_UnloadMonIconGfx(work);
        VsRecorder_RemoveVideoListWindows(work);
        VsRecorder_RemoveVideoListButtonWindows(work);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        if (listWork->nextSubState == VIDEO_LIST_EXIT) {
            VsRecorder_InitTouchScroll(&work->touchScroll);
            VsRecorder_ClearListWindow(&work->listWindow);
            listWork->iconScroll = 0;
        }
        work->step++;
        // fallthrough
    case VIDEO_LIST_SELECT_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
            VsRecorder_SetVideoListButtonsMode(work, FALSE);
            if (listWork->nextSubState == VIDEO_LIST_SHOW_DETAILS) {
                VsRecorder_SetTextSpriteMsg(&listWork->buttons[0], (VsRecorderGfx *)&work->narc, 3);
                VsRecorder_SetTextSpriteMsg(&listWork->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
                ManagedSprite_SetAnim(listWork->buttons[0].sprite, 0);
                ManagedSprite_SetAnim(listWork->buttons[1].sprite, 3);
            }
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, listWork->nextSubState);
        break;
    }
    return FALSE;
}

// leave the list for the menu
BOOL VsRecorder_VideoListExit(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_EXIT_STEP_REMOVE_WINDOWS:
        VsRecorder_RemoveVideoListWindows(work);
        VsRecorder_RemoveVideoListButtonWindows(work);
        VsRecorder_HideTopMessage(work);
        work->step++;
        break;
    case VIDEO_LIST_EXIT_STEP_FADE_OUT:
        VsRecorder_StepFade(&listWork->fade, 1);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeVideoListButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_EXIT_STEP_REBUILD_MENU:
        VsRecorder_ResetFade(&listWork->fade);
        VsRecorder_RebuildMenuRowsParent(work);
        WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_109);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&listWork->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&listWork->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, VS_RECORDER_MENU_SLIDE_IN);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                Heap_Free(listWork);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)listWork->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoListShowDetails(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_SHOW_DETAILS_STEP_LOAD_SCREEN:
        VsRecorder_LoadScreen(work, 6, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case VIDEO_LIST_SHOW_DETAILS_STEP_CREATE_HITBOXES:
        VsRecorder_LoadVideoDetails(work);
        listWork->detailHitboxes = TouchHitboxController_Create(gVsRecorderVideoDetailHitboxes, 4, VsRecorder_OnVideoDetailsTouch, work, HEAP_ID_109);
        listWork->slotHitboxes = TouchHitboxController_Create(gVsRecorderSaveSlotHitboxes, 4, VsRecorder_OnSaveSlotTouch, work, HEAP_ID_109);
        listWork->overwriteHitboxes = TouchHitboxController_Create(gVsRecorderOverwriteHitboxes, 2, VsRecorder_OnOverwriteTouch, work, HEAP_ID_109);
        work->step++;
        break;
    case VIDEO_LIST_SHOW_DETAILS_STEP_CREATE_INFO:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateVideoInfo(&listWork->info, work);
        VsRecorder_LoadScreen(work, VsRecorderVideoInfo_GetScreenId(&listWork->info), GF_BG_LYR_MAIN_3);
        VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case VIDEO_LIST_SHOW_DETAILS_STEP_BLEND_IN:
        VsRecorder_StepFade(&listWork->fade, 0);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 0)) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, TRUE);
            VsRecorder_CreateDetailsButtonWindow(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, VIDEO_LIST_DETAILS_MAIN);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoListDetailsMain(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    TouchHitboxController_IsTriggered(listWork->detailHitboxes);
    return FALSE;
}

// top screen: video info <-> owner's profile
BOOL VsRecorder_VideoListToggleProfile(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_TOGGLE_PROFILE_STEP_FREE:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (listWork->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&listWork->info, work);
        } else {
            VsRecorder_FreeProfile(&listWork->profile, work);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        break;
    case VIDEO_LIST_TOGGLE_PROFILE_STEP_BLEND_OUT:
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 1)) {
            work->step++;
        }
        break;
    case VIDEO_LIST_TOGGLE_PROFILE_STEP_CREATE:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (listWork->showProfile == 0) {
            VsRecorder_CreateProfile(&listWork->profile, work);
            VsRecorder_SetProfileDrawFlag(&listWork->profile, FALSE);
            VsRecorder_LoadScreen(work, 80, GF_BG_LYR_MAIN_3);
        } else {
            VsRecorder_CreateVideoInfo(&listWork->info, work);
            VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, FALSE);
            VsRecorder_LoadScreen(work, VsRecorderVideoInfo_GetScreenId(&listWork->info), GF_BG_LYR_MAIN_3);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        break;
    case VIDEO_LIST_TOGGLE_PROFILE_STEP_BLEND_IN:
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 1)) {
            if (listWork->showProfile == 0) {
                VsRecorder_SetProfileDrawFlag(&listWork->profile, TRUE);
            } else {
                VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, TRUE);
            }
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        break;
    default:
        listWork->showProfile = 1 ^ listWork->showProfile;
        VsRecorder_PrintDetailsButtonLabel(work, listWork->showProfile + 0x79);
        VsRecorder_SetSubState(work, VIDEO_LIST_DETAILS_MAIN);
        break;
    }
    return FALSE;
}

// leave the app to play the downloaded video (the launcher comes back with VsRecorder_VideoListResume)
BOOL VsRecorder_VideoListPlay(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_PLAY_STEP_FADE_OUT:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_109);
        work->step++;
        break;
    case VIDEO_LIST_PLAY_STEP_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            work->step++;
        }
        break;
    case VIDEO_LIST_PLAY_STEP_FREE:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (listWork->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&listWork->info, work);
        } else {
            VsRecorder_FreeProfile(&listWork->profile, work);
        }
        TouchHitboxController_Destroy(listWork->detailHitboxes);
        TouchHitboxController_Destroy(listWork->slotHitboxes);
        TouchHitboxController_Destroy(listWork->overwriteHitboxes);
        VsRecorder_FreeVideoDetails(work);
        VsRecorder_RemoveDetailsButtonWindow(work);
        VsRecorder_FreeDetailsButtons(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_SetResumeScreen(work, *work->screenPtr);
        VsRecorderApp_SetResult(work->result, 1, 0);
        work->step++;
        break;
    default:
        Heap_Free(listWork);
        return TRUE;
    }
    return FALSE;
}

// back from the video playback: the details again
BOOL VsRecorder_VideoListResume(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    switch (work->step) {
    case VIDEO_LIST_RESUME_STEP_LOAD_SCREEN: {
        int i;

        VsRecorder_ResetBgPriorities();
        VsRecorder_InitBgBlend(&listWork->eva, &listWork->evb, 0);
        for (i = 0; i < work->net.videoList.count; i++) {
            work->net.videoList.headers[i] = &work->net.videoList.videos[i];
        }
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_LoadScreen(work, 6, GF_BG_LYR_SUB_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        VsRecorder_SetTextSpriteMsg(&listWork->buttons[0], (VsRecorderGfx *)&work->narc, 3);
        VsRecorder_SetTextSpriteMsg(&listWork->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
        ManagedSprite_SetAnim(listWork->buttons[0].sprite, 0);
        ManagedSprite_SetAnim(listWork->buttons[1].sprite, 3);
        listWork->iconResId = 500;
        listWork->iconScroll = work->listWindow.scroll;
        work->step++;
        break;
    }
    case VIDEO_LIST_RESUME_STEP_CREATE_HITBOXES:
        VsRecorder_LoadVideoDetails(work);
        listWork->detailHitboxes = TouchHitboxController_Create(gVsRecorderVideoDetailHitboxes, 4, VsRecorder_OnVideoDetailsTouch, work, HEAP_ID_109);
        listWork->slotHitboxes = TouchHitboxController_Create(gVsRecorderSaveSlotHitboxes, 4, VsRecorder_OnSaveSlotTouch, work, HEAP_ID_109);
        listWork->overwriteHitboxes = TouchHitboxController_Create(gVsRecorderOverwriteHitboxes, 2, VsRecorder_OnOverwriteTouch, work, HEAP_ID_109);
        work->step++;
        break;
    case VIDEO_LIST_RESUME_STEP_CREATE_INFO:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateVideoInfo(&listWork->info, work);
        VsRecorder_LoadScreen(work, VsRecorderVideoInfo_GetScreenId(&listWork->info), GF_BG_LYR_MAIN_3);
        VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case VIDEO_LIST_RESUME_STEP_BLEND_IN:
        VsRecorder_StepFade(&listWork->fade, 0);
        if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 0)) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, TRUE);
            VsRecorder_CreateDetailsButtonWindow(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
        break;
    case VIDEO_LIST_RESUME_STEP_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_109);
        work->step++;
        break;
    case VIDEO_LIST_RESUME_STEP_WAIT_FADE:
        if (IsPaletteFadeFinished() == TRUE) {
            work->step++;
        }
        break;
    default:
        work->menuDepth++;
        VsRecorder_ClearResumeState(work);
        VsRecorder_SetSubState(work, VIDEO_LIST_DETAILS_MAIN);
        break;
    }
    return FALSE;
}

// message window over the details (main screen BG 2)
static void VsRecorder_ShowVideoListMessage(VsRecorderWork *work, int msgId, int unused) {
    Window *window = &work->messageWindow;
    VsRecorderVideoListScreen *listWork = work->screenWork;
    String *str;

    VsRecorder_LoadMessageBg(work, GF_BG_LYR_MAIN_2, unused);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 1, 19, 30, 4, 14, 0x12C);
    if (msgId == msg_0013_00100) {
        FrontierCard *card = listWork->profile.card;
        MessageFormat *msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
        String *template;
        String *name;

        str = String_New(255, HEAP_ID_109);
        name = FrontierCard_NewNameString((const u16 *)card, HEAP_ID_109);
        VsRecorder_SanitizeName(work, name);
        template = NewString_ReadMsgData(work->msgData, msgId);
        BufferString(msgFormat, 0, name, 0, 1, 2);
        StringExpandPlaceholders(msgFormat, str, template);
        String_Delete(name);
        String_Delete(template);
        MessageFormat_Delete(msgFormat);
    } else {
        str = NewString_ReadMsgData(work->msgData, msgId);
    }
    FillWindowPixelBuffer(window, 0xCC);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 12), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

// save: the slot menu, or a notice when the video is saved already
BOOL VsRecorder_VideoListShowSlots(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    if (VsRecorder_IsVideoSaved(work, BattleRecordHeader_GetField(BattleRecord_GetHeader(), 4, 0))) {
        switch (work->step) {
        case 0:
            VsRecorder_SetVBlankPaused(work, TRUE);
            if (listWork->showProfile == 0) {
                VsRecorder_FreeVideoInfo(&listWork->info, work);
            } else {
                VsRecorder_FreeProfile(&listWork->profile, work);
            }
            VsRecorder_RemoveDetailsButtonWindow(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeDetailsButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            work->step++;
            break;
        case 1:
            VsRecorder_StepFade(&listWork->fade, 1);
            if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
                VsRecorder_SetVBlankPaused(work, TRUE);
                VsRecorder_CreateSlotButtons(work);
                VsRecorder_SetVBlankPaused(work, FALSE);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
            break;
        case 2:
            VsRecorder_ShowTopMessage(work, msg_0013_00277);
            work->step++;
            break;
        case 3:
            if (System_GetTouchNew()) {
                VsRecorder_HideTopMessage(work);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
                work->step++;
            }
            break;
        case 4:
            VsRecorder_StepFade(&listWork->fade, 1);
            if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
                VsRecorder_SetVBlankPaused(work, TRUE);
                VsRecorder_FreeSlotButtons(work);
                VsRecorder_SetVBlankPaused(work, FALSE);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
            break;
        case 5:
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateVideoListButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            if (work->selection == VS_RECORDER_SELECTION_SEARCH) {
                VsRecorder_SetTextSpriteMsg(&listWork->buttons[0], (VsRecorderGfx *)&work->narc, 3);
                VsRecorder_SetTextSpriteMsg(&listWork->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
                ManagedSprite_SetAnim(listWork->buttons[0].sprite, 0);
                ManagedSprite_SetAnim(listWork->buttons[1].sprite, 3);
            }
            VsRecorder_CreateDetailsButtonWindow(work);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            VsRecorder_SetVBlankPaused(work, TRUE);
            if (listWork->showProfile == 0) {
                VsRecorder_CreateVideoInfo(&listWork->info, work);
                VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, FALSE);
                VsRecorder_LoadScreen(work, VsRecorderVideoInfo_GetScreenId(&listWork->info), GF_BG_LYR_MAIN_3);
            } else {
                VsRecorder_CreateProfile(&listWork->profile, work);
                VsRecorder_SetProfileDrawFlag(&listWork->profile, FALSE);
                GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 80, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            }
            VsRecorder_SetVBlankPaused(work, FALSE);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 6, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            work->step++;
            break;
        case 6:
            VsRecorder_StepFade(&listWork->fade, 0);
            if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 0)) {
                if (listWork->showProfile == 0) {
                    VsRecorder_SetVideoPartyIconsDrawFlag(&listWork->info, TRUE);
                } else {
                    VsRecorder_SetProfileDrawFlag(&listWork->profile, TRUE);
                }
                GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
            break;
        default: {
            int i;

            for (i = 0; i < work->net.videoList.count; i++) {
                work->net.videoList.headers[i] = &work->net.videoList.videos[i];
            }
            VsRecorder_SetSubState(work, VIDEO_LIST_DETAILS_MAIN);
            break;
        }
        }
    } else {
        switch (work->step) {
        case 0:
            VsRecorder_SetVBlankPaused(work, TRUE);
            if (listWork->showProfile == 0) {
                VsRecorder_FreeVideoInfo(&listWork->info, work);
            } else {
                VsRecorder_FreeProfile(&listWork->profile, work);
            }
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeDetailsButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_RemoveDetailsButtonWindow(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            work->step++;
            break;
        case 1:
            VsRecorder_StepFade(&listWork->fade, 1);
            if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, TRUE, 0)) {
                VsRecorder_SetVBlankPaused(work, TRUE);
                VsRecorder_CreateSlotButtons(work);
                VsRecorder_SetVBlankPaused(work, FALSE);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
            break;
        case 2:
            VsRecorder_StepFade(&listWork->fade, 0);
            if (VsRecorder_StepBlend(&listWork->eva, &listWork->evb, FALSE, 1)) {
                VsRecorder_ShowVideoListMessage(work, msg_0013_00100, 0);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)listWork->fade, (u16)work->blendColor);
            break;
        default:
            VsRecorder_SetSubState(work, VIDEO_LIST_SLOTS_MAIN);
            break;
        }
    }
    return FALSE;
}

static void VsRecorder_RemoveDetailsButtonWindow(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    ClearWindowTilemapAndCopyToVram(&listWork->buttonWindows[0]);
    RemoveWindow(&listWork->buttonWindows[0]);
}

static void VsRecorder_PrintDetailsButtonLabel(VsRecorderWork *work, int msgId) {
    Window *window = &((VsRecorderVideoListScreen *)work->screenWork)->buttonWindows[0];
    String *str;
    int x;

    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msgId);
    x = VsRecorder_GetCenteredTextX(window, str);
    AddTextPrinterParameterizedWithColor(window, 0, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

// label of the video info / profile toggle
static void VsRecorder_CreateDetailsButtonWindow(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;
    Window *window = &listWork->buttonWindows[0];

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 8, 3, 16, 2, 14, 1);
    VsRecorder_PrintDetailsButtonLabel(work, listWork->showProfile + 0x79);
}

static void VsRecorder_CreateOverwriteWindows(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;
    Window *window = &listWork->buttonWindows[0];
    String *str;

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 4, 6, 10, 2, 14, 1);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msg_0013_00130);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
    window = &listWork->buttonWindows[1];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 18, 6, 10, 2, 14, 21);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msg_0013_00131);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

static void VsRecorder_RemoveOverwriteWindows(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    ClearWindowTilemapAndCopyToVram(&listWork->buttonWindows[0]);
    RemoveWindow(&listWork->buttonWindows[0]);
    ClearWindowTilemapAndCopyToVram(&listWork->buttonWindows[1]);
    RemoveWindow(&listWork->buttonWindows[1]);
}

// details: video info / profile toggle, play, save, back
static void VsRecorder_OnVideoDetailsTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderVideoListScreen *listWork = work->screenWork;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, VIDEO_LIST_TOGGLE_PROFILE);
        break;
    case 1:
        VsRecorder_OnTouchConfirm(work);
        if (BattleRecord_Exists()) {
            VsRecorder_SetSubState(work, VIDEO_LIST_PLAY);
        } else {
            listWork->nextSubState = VIDEO_LIST_PLAY;
            VsRecorder_SetSubState(work, VIDEO_LIST_DOWNLOAD_VIDEO);
        }
        break;
    case 2:
        VsRecorder_OnTouchConfirm(work);
        if (BattleRecord_Exists()) {
            VsRecorder_SetSubState(work, VIDEO_LIST_SHOW_SLOTS);
        } else {
            listWork->nextSubState = VIDEO_LIST_SHOW_SLOTS;
            VsRecorder_SetSubState(work, VIDEO_LIST_DOWNLOAD_VIDEO);
        }
        break;
    case 3:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, VIDEO_LIST_BACK_TO_LIST);
        break;
    }
}

// slot menu: slots 1-3 (a used slot asks first), cancel
static void VsRecorder_OnSaveSlotTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderVideoListScreen *listWork = work->screenWork;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
    case 1:
    case 2:
        listWork->slot = index + 1;
        if (work->videoOwners[listWork->slot] != NULL) {
            VsRecorder_SetSubState(work, VIDEO_LIST_CONFIRM_OVERWRITE);
        } else {
            VsRecorder_SetSubState(work, VIDEO_LIST_SAVE);
        }
        VsRecorder_OnTouchConfirm(work);
        break;
    case 3:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_RemoveTopMessage(work);
        VsRecorder_SetSubState(work, VIDEO_LIST_SLOTS_CANCEL);
        break;
    }
}

static void VsRecorder_OnOverwriteTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_RemoveOverwriteWindows(work);
        VsRecorder_SetSubState(work, VIDEO_LIST_SAVE);
        break;
    case 1:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, VIDEO_LIST_OVERWRITE_NO);
        break;
    }
}

// owner's profile and battle header of the chosen list entry
static void VsRecorder_LoadVideoDetails(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    listWork->profile.card = FrontierCard_New(HEAP_ID_109);
    GlobalTerminalNet_CopyRequestHeader(work->net.videoList.headers[work->listWindow.cursor], listWork->profile.card);
    listWork->info.ownerName = (const u16 *)listWork->profile.card;
    listWork->info.header = BattleRecordHeader_New(HEAP_ID_109);
    MI_CpuCopy8(work->net.videoList.headers[work->listWindow.cursor]->data, listWork->info.header, 0x64);
}

// party icons of the visible list rows
static void VsRecorder_CreateVideoListIcons(VsRecorderWork *work) {
    int i;
    VsRecorderVideoListScreen *listWork = work->screenWork;
    int scroll = work->listWindow.scroll;
    VsRecorderPartyIconOrder order = gVsRecorderListIconOrder;
    int species[6] = { 0 };
    int forms[6] = { 0 };
    int j;
    int count;
    int layout;
    int k;

    for (i = 0; i < 5; i++) {
        if (VIDEO_PARTY(work, scroll + i) == NULL) {
            continue;
        }
        if (VsRecorder_HeaderUsesThreeMonLayout(VIDEO_PARTY(work, scroll + i)->species)) {
            layout = 1;
        } else {
            layout = 0;
        }
        count = 0;
        for (j = 0; j < 6; j++) {
            species[j] = 0;
            forms[j] = 0;
        }
        for (j = 0; j < 3; j++) {
            if (VIDEO_PARTY(work, scroll + i)->species[order.slots[layout][j]] != SPECIES_NONE) {
                species[count] = VIDEO_PARTY(work, scroll + i)->species[order.slots[layout][j]];
                forms[count] = VIDEO_PARTY(work, scroll + i)->forms[order.slots[layout][j]];
                count++;
            }
        }
        if (VsRecorder_HeaderUsesThreeMonLayout(VIDEO_PARTY(work, scroll + i)->species)) {
            count = 3;
        }
        for (j = 3; j < 6; j++) {
            if (VIDEO_PARTY(work, scroll + i)->species[order.slots[layout][j]] != SPECIES_NONE) {
                species[count] = VIDEO_PARTY(work, scroll + i)->species[order.slots[layout][j]];
                forms[count] = VIDEO_PARTY(work, scroll + i)->forms[order.slots[layout][j]];
                count++;
            }
        }
        for (k = 0; k < 6; k++) {
            int iconSpecies = species[k];
            int iconForm = forms[k];

            listWork->icons[i][k].resId = listWork->iconResId;
            listWork->icons[i][k].sprite = VsRecorder_NewMonIcon(work, listWork->monIconNarc, listWork->icons[i][k].resId, iconSpecies, iconForm, FALSE);
            if (listWork->icons[i][k].sprite != NULL) {
                ((void (*)(ManagedSprite *, int))ManagedSprite_SetDrawPriority)(listWork->icons[i][k].sprite, 6 - k);
            }
            listWork->iconResId++;
        }
    }
    VsRecorder_PlaceVideoListIcons(work);
}

static void VsRecorder_SetVideoListIconsDrawFlag(VsRecorderWork *work, BOOL draw) {
    int i;
    int j;
    VsRecorderVideoListScreen *listWork = work->screenWork;

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 6; i++) {
            if (listWork->icons[j][i].sprite != NULL) {
                ManagedSprite_SetDrawFlag(listWork->icons[j][i].sprite, draw);
            }
        }
    }
}

static void VsRecorder_FreeVideoListIcons(VsRecorderWork *work) {
    int i;
    int j;
    VsRecorderVideoListScreen *listWork = work->screenWork;

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 6; i++) {
            if (listWork->icons[j][i].sprite != NULL) {
                VsRecorder_DeleteMonIcon(work, listWork->icons[j][i].resId, listWork->icons[j][i].sprite);
                listWork->icons[j][i].sprite = NULL;
            }
        }
    }
}

// keep the icons in step with the list scroll: drop the row that left, shift, make the row that came in
static void VsRecorder_ScrollVideoListIcons(VsRecorderWork *work) {
    int row = 0;
    int freed[6] = { 0 };
    VsRecorderVideoListScreen *listWork = work->screenWork;
    int i;
    int j;
    int index;
    int count;
    int layout;

    if (listWork->iconScroll == work->listWindow.scroll) {
        return;
    }
    VsRecorder_SetVBlankPaused(work, TRUE);
    if (listWork->iconScroll > work->listWindow.scroll) {
        row = 4;
    }
    for (j = 0; j < 6; j++) {
        if (listWork->icons[row][j].sprite != NULL) {
            VsRecorder_DeleteSprite(work, (void *)listWork->icons[row][j].resId, listWork->icons[row][j].sprite);
            freed[j] = listWork->icons[row][j].resId;
            listWork->icons[row][j].sprite = NULL;
        }
    }
    if (row != 0) {
        for (j = 4; j >= 1; j--) {
            for (layout = 0; layout < 6; layout++) {
                listWork->icons[j][layout].sprite = listWork->icons[j - 1][layout].sprite;
                listWork->icons[j][layout].resId = listWork->icons[j - 1][layout].resId;
            }
        }
        row = 0;
    } else {
        for (i = 1; i < 5; i++) {
            for (j = 0; j < 6; j++) {
                listWork->icons[i - 1][j].sprite = listWork->icons[i][j].sprite;
                listWork->icons[i - 1][j].resId = listWork->icons[i][j].resId;
            }
        }
        row = 4;
    }
    index = work->listWindow.scroll + row;
    {
        VsRecorderPartyIconOrder order = gVsRecorderScrollIconOrder;
        int species[6] = { 0 };
        int forms[6] = { 0 };
        layout = 0;
        if (VsRecorder_HeaderUsesThreeMonLayout(VIDEO_PARTY(work, index)->species)) {
            layout = 1;
        }
        count = 0;
        {
            int n;

            for (n = 0; n < 6; n++) {
                forms[n] = species[n] = 0;
            }
        }
        for (j = 0; j < 3; j++) {
            if (VIDEO_PARTY(work, index)->species[order.slots[layout][j]] != SPECIES_NONE) {
                species[count] = VIDEO_PARTY(work, index)->species[order.slots[layout][j]];
                forms[count] = VIDEO_PARTY(work, index)->forms[order.slots[layout][j]];
                count++;
            }
        }
        if (VsRecorder_HeaderUsesThreeMonLayout(VIDEO_PARTY(work, index)->species)) {
            count = 3;
        }
        for (j = 3; j < 6; j++) {
            if (VIDEO_PARTY(work, index)->species[order.slots[layout][j]] != SPECIES_NONE) {
                species[count] = VIDEO_PARTY(work, index)->species[order.slots[layout][j]];
                forms[count] = VIDEO_PARTY(work, index)->forms[order.slots[layout][j]];
                count++;
            }
        }
        for (j = 0; j < 6; j++) {
            int iconSpecies = species[j];
            int iconForm = forms[j];

            listWork->icons[row][j].resId = listWork->iconResId;
            listWork->icons[row][j].sprite = VsRecorder_NewMonIcon(work, listWork->monIconNarc, listWork->icons[row][j].resId, iconSpecies, iconForm, FALSE);
            if (listWork->icons[row][j].sprite != NULL) {
                ((void (*)(ManagedSprite *, int))ManagedSprite_SetDrawPriority)(listWork->icons[row][j].sprite, 6 - j);
            }
            listWork->iconResId++;
        }
    }
    VsRecorder_PlaceVideoListIcons(work);
    listWork->iconScroll = work->listWindow.scroll;
    for (j = 0; j < 6; j++) {
        if (freed[j] != 0) {
            SpriteManager_UnloadCharObjById(work->spriteManager, freed[j] + 100000);
        }
    }
    VsRecorder_SetVBlankPaused(work, FALSE);
}

static void VsRecorder_FreeDetailsButtons(VsRecorderWork *work) {
    VsRecorderVideoListScreen *listWork = work->screenWork;

    VsRecorder_FreeTextSprite(&listWork->buttons[0]);
    VsRecorder_FreeTextSprite(&listWork->buttons[1]);
    Sprite_DeleteAndFreeResources(listWork->buttons[0].sprite);
    Sprite_DeleteAndFreeResources(listWork->buttons[1].sprite);
    VsRecorder_FreeButtonGfx(work);
}
