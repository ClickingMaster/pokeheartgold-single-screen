#include "global.h"

#include "constants/sndseq.h"

#include "application/vs_recorder/vs_recorder_screens.h"

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

// VS Recorder video transfers: the end of the video search (download a found video, back from playing it), the
// video number search (VS_RECORDER_SCREEN_SEARCH_NUMBER), sending the own video (VS_RECORDER_SCREEN_SEND_VIDEO),
// and what these screens share: the found video's details, the save slots, the buttons and the touch callbacks,
// and the server response handlers of the video requests. All of them use VsRecorderVideoSearch.

typedef struct VsRecorderSlotButtonTable {
    int values[4];
} VsRecorderSlotButtonTable;

// asm/vs_recorder_data.s
extern const VsRecorderSlotButtonTable sVsRecorderSearchSlotMsgIds;
extern const VsRecorderSlotButtonTable sVsRecorderSearchSlotAnims;
extern const TouchscreenHitbox sVsRecorderSearchSaveSlotHitboxes[4];
extern const TouchscreenHitbox sVsRecorderSearchOverwriteHitboxes[2];
extern const TouchscreenHitbox sVsRecorderSearchDetailHitboxes[4];

// flag byte of the video found by number (VsRecorderVideoList.flags of a listed video)
#define NUMBER_SEARCH_VIDEO_FLAG(work) ((work)->net.videoBuffer[0x1D4C])

// VsRecorderVideoSearch substates of the video search (sVsRecorderVideoSearchSubStates)
#define SEARCH_SHOW_CONDITIONS   2
#define SEARCH_EXIT              4
#define SEARCH_OPEN_CONDITION    5
#define SEARCH_REQUEST           12
#define SEARCH_DETAILS_MAIN      18
#define SEARCH_TOGGLE_PROFILE    19
#define SEARCH_PLAY              20
#define SEARCH_BACK_TO_RESULTS   21
#define SEARCH_SHOW_SLOTS        22
#define SEARCH_SLOTS_CANCEL      24
#define SEARCH_CONFIRM_OVERWRITE 25
#define SEARCH_OVERWRITE_NO      27
#define SEARCH_SAVE              28
#define SEARCH_DOWNLOAD_VIDEO    29
#define SEARCH_RESUME            30
#define SEARCH_RETURN_TO_DETAILS 31

// substates of the video number search (sVsRecorderNumberSearchSubStates)
#define NUMBER_SEARCH_SHOW_PAD          2
#define NUMBER_SEARCH_PAD_MAIN          3
#define NUMBER_SEARCH_CLOSE_PAD         4
#define NUMBER_SEARCH_EXIT              5
#define NUMBER_SEARCH_DOWNLOAD_VIDEO    6
#define NUMBER_SEARCH_SHOW_DETAILS      7
#define NUMBER_SEARCH_DETAILS_MAIN      8
#define NUMBER_SEARCH_TOGGLE_PROFILE    9
#define NUMBER_SEARCH_PLAY              10
#define NUMBER_SEARCH_EXIT_DETAILS      11
#define NUMBER_SEARCH_SHOW_SLOTS        12
#define NUMBER_SEARCH_SLOTS_CANCEL      14
#define NUMBER_SEARCH_CONFIRM_OVERWRITE 15
#define NUMBER_SEARCH_OVERWRITE_NO      17
#define NUMBER_SEARCH_SAVE              18
#define NUMBER_SEARCH_RESUME            19

// substates of sending the own video (sVsRecorderSendVideoSubStates)
#define SEND_VIDEO_UPLOAD 3
#define SEND_VIDEO_EXIT   4

// download the chosen result's video, then go on to search->nextSubState
BOOL VsRecorder_VideoSearchDownloadVideo(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    GlobalTerminalNetError *error;

    if (!VsRecorder_IsNetIdle(work)) {
        return FALSE;
    }
    switch (work->step) {
    case 0:
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        if (search->showProfile == 0) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
        } else {
            VsRecorder_SetProfileDrawFlag(&search->profile, FALSE);
        }
        work->step++;
        // fallthrough
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_ShowTopMessage(work, 0x75);
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        search->downloaded = FALSE;
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 3: {
        GlobalTerminalNet *net = VsRecorder_GetNet(work);

        if (GlobalTerminalNet_RequestVideoDownload(net, (u32)work->net.videoList.headers[work->listWindow.cursor]->number, (u32)(work->net.videoList.headers[work->listWindow.cursor]->number >> 32)) == TRUE) {
            work->step++;
        }
        break;
    }
    case 4:
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_HideTopMessage(work);
            VsRecorder_ShowMessageScreen(work, 8, error->code, error->type);
            work->step++;
            search->downloaded = FALSE;
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            work->step = 0xFF;
            search->downloaded = TRUE;
            work->videoFlagPtr = (u32)&work->net.videoList.flags[work->listWindow.cursor];
            search->foundVideo = work->net.downloadedVideo;
            MI_CpuCopy8(search->foundVideo, work->net.videoBuffer, 0x1D4C);
        }
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        break;
    default:
        if (search->downloaded) {
            PlaySE(SEQ_SE_PL_BREC12);
            VsRecorder_HideTopMessage(work);
            VsRecorder_SetSubState(work, search->nextSubState);
            VsRecorder_SetBattleResult(work, 1);
        } else {
            VsRecorder_HideTopMessage(work);
            VsRecorder_SetSubState(work, SEARCH_RETURN_TO_DETAILS);
        }
        break;
    }
    return FALSE;
}

// back from the video playback: the found video's details again
BOOL VsRecorder_VideoSearchResume(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    int i;

    switch (work->step) {
    case 0:
        VsRecorder_ResetBgPriorities();
        VsRecorder_InitBgBlend(&search->eva, &search->evb, 0);
        for (i = 0; i < work->net.videoList.count; i++) {
            work->net.videoList.headers[i] = &work->net.videoList.videos[i];
        }
        search->videoFlag = work->net.videoList.flags[work->listWindow.cursor];
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_LoadScreen(work, 6, GF_BG_LYR_SUB_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        VsRecorder_CreateSearchButtons(work);
        VsRecorder_SetSearchButtonsMode(work, FALSE);
        VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
        VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
        ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
        ManagedSprite_SetAnim(search->buttons[1].sprite, 3);
        search->species = 0xFFFF; // any
        search->facility = 0xFF;
        search->country = 0xFF;
        search->region = 0xFF;
        search->iconResId = 500;
        search->iconScroll = work->listWindow.scroll;
        work->step++;
        break;
    case 1:
        VsRecorder_LoadSearchResultDetails(work);
        search->hitboxes = TouchHitboxController_Create(sVsRecorderSearchDetailHitboxes, 4, VsRecorder_OnSearchDetailsTouch, work, HEAP_ID_109);
        search->slotHitboxes = TouchHitboxController_Create(sVsRecorderSearchSaveSlotHitboxes, 4, VsRecorder_OnSearchSlotTouch, work, HEAP_ID_109);
        search->overwriteHitboxes = TouchHitboxController_Create(sVsRecorderSearchOverwriteHitboxes, 2, VsRecorder_OnSearchOverwriteTouch, work, HEAP_ID_109);
        work->step++;
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateVideoInfo(&search->info, work);
        VsRecorder_LoadScreen(work, ov40_02230410(&search->info), GF_BG_LYR_MAIN_3);
        VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 3:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, TRUE);
            VsRecorder_CreateSearchDetailsButtonWindow(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_109);
        work->step++;
        break;
    case 5:
        if (IsPaletteFadeFinished() == TRUE) {
            work->step++;
        }
        break;
    default:
        work->menuDepth++;
        VsRecorder_ClearResumeState(work);
        VsRecorder_SetSubState(work, SEARCH_DETAILS_MAIN);
        break;
    }
    return FALSE;
}

// substate 0 of the video search, the number search and sending a video
BOOL VsRecorder_VideoSearchInit(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderVideoSearch));

    memset(search, 0, sizeof(VsRecorderVideoSearch));
    work->screenWork = search;
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
    VsRecorder_InitBgBlend(&search->eva, &search->evb, 0);
    VsRecorder_SetSubState(work, 1);
    return FALSE;
}

// substate 1 of the same screens: leave the menu; a screen left to play a video goes on at its resume substate
BOOL VsRecorder_VideoSearchEnter(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        if (VsRecorder_StepFade(&search->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)search->fade, (u16)work->blendColor);
        break;
    case 1:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeMenuRows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        SetBgPriority(GF_BG_LYR_MAIN_0, 0);
        SetBgPriority(GF_BG_LYR_MAIN_1, 3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_3, 2);
        SetBgPriority(GF_BG_LYR_SUB_0, 0);
        SetBgPriority(GF_BG_LYR_SUB_1, 3);
        SetBgPriority(GF_BG_LYR_SUB_2, 1);
        SetBgPriority(GF_BG_LYR_SUB_3, 2);
        work->step++;
        break;
    default:
        switch (work->selection) {
        case 200:
            VsRecorder_StartFlipScreens(work, 0);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            if (VsRecorder_HasResumeScreen(work) == TRUE) {
                VsRecorder_SetSubState(work, 6);
            } else {
                VsRecorder_SetSubState(work, 2);
            }
            break;
        case 210: // video search
            VsRecorder_StartFlipScreens(work, 0);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            if (VsRecorder_HasResumeScreen(work) == TRUE) {
                VsRecorder_SetSubState(work, SEARCH_RESUME);
            } else {
                VsRecorder_SetSubState(work, SEARCH_SHOW_CONDITIONS);
            }
            break;
        case 220: // number search
            if (VsRecorder_HasResumeScreen(work) == TRUE) {
                VsRecorder_SetSubState(work, NUMBER_SEARCH_RESUME);
            } else {
                VsRecorder_SetSubState(work, NUMBER_SEARCH_SHOW_PAD);
            }
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            break;
        default:
            VsRecorder_StartFlipScreens(work, 0);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            VsRecorder_SetSubState(work, 2);
            break;
        }
        break;
    }
    return FALSE;
}

// open the video number pad (12 digits in groups of 2, 5 and 5)
BOOL VsRecorder_NumberSearchShowPad(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    VsRecorderNumberPad *pad = &search->numberPad;
    int groupLengths[3];

    switch (work->step) {
    case 0:
        groupLengths[0] = 2;
        groupLengths[1] = 5;
        groupLengths[2] = 5;
        search->numberPadConfig = VsRecorder_NewNumberPadConfig(HEAP_ID_109, 12, groupLengths, Save_PlayerData_GetOptionsAddr(work->saveData));
        pad->narc = work->narc;
        pad->spriteSystem = work->spriteSystem;
        pad->spriteManager = work->spriteManager;
        pad->bgConfig = work->bgConfig;
        pad->plttData = work->plttData;
        pad->config = *search->numberPadConfig;
        pad->owner = work;
        VsRecorder_CreateNumberPadButtons(work);
        work->step++;
        // fallthrough
    case 1:
        VsRecorder_InitBlend(&search->eva, &search->evb, GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        work->step++;
        break;
    case 2:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            VsRecorder_ShowNotice(work, 0x70);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, NUMBER_SEARCH_PAD_MAIN);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_NumberSearchPadMain(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (VsRecorder_NumberPadMain(&search->numberPad) == TRUE) {
        VsRecorder_SetSubState(work, NUMBER_SEARCH_CLOSE_PAD);
    }
    return FALSE;
}

// number entered: download the video; cancelled: back to the menu
BOOL VsRecorder_NumberSearchClosePad(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    VsRecorderNumberPad *pad = &search->numberPad;

    switch (work->step) {
    case 0:
        VsRecorder_HideNotice(work);
        VsRecorder_ApplyBlend(&search->eva, &search->evb, GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 0);
        VsRecorder_FreeNumberPadConfig(search->numberPadConfig);
        work->step++;
        // fallthrough
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
            VsRecorder_FreeNumberPadButtons(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        if (pad->confirmed) {
            VsRecorder_SetSubState(work, NUMBER_SEARCH_DOWNLOAD_VIDEO);
        } else {
            VsRecorder_SetSubState(work, NUMBER_SEARCH_EXIT);
        }
        break;
    }
    return FALSE;
}

// back to the menu
BOOL VsRecorder_NumberSearchExit(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (work->step == 0) {
        VsRecorder_ResetFade(&search->fade);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
    } else if (VsRecorder_StepFade(&search->fade, 0)) {
        VsRecorder_ClearPaletteBlend(work);
        VsRecorder_ResetFade(&search->fade);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
        VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
        VsRecorder_SetSubState(work, 5);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        Heap_Free(search);
    } else {
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)search->fade, (u16)work->blendColor);
    }
    return FALSE;
}

// download the video with the entered number
BOOL VsRecorder_NumberSearchDownloadVideo(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    VsRecorderNumberPad *pad = &search->numberPad;
    GlobalTerminalNetError *error;

    switch (work->step) {
    case 0:
        VsRecorder_ShowNotice(work, 0x75);
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[1], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[1], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[1], 24, 24);
        search->downloaded = FALSE;
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 1: {
        GlobalTerminalNet *net;

        if (!VsRecorder_IsNetIdle(work)) {
            return FALSE;
        }
        net = VsRecorder_GetNet(work);
        if (GlobalTerminalNet_RequestVideoDownload(net, (u32)pad->number, (u32)(pad->number >> 32)) == TRUE) {
            work->step++;
        }
        break;
    }
    case 2:
        if (!VsRecorder_IsNetIdle(work)) {
            return FALSE;
        }
        VsRecorder_HideNotice(work);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            reg_G2_BLDCNT = 0;
            VsRecorder_ShowMessageScreen(work, 8, error->code, error->type);
            VsRecorder_StartFlipScreens(work, 0);
            work->step++;
            search->downloaded = FALSE;
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            work->step = 0xFF;
            search->downloaded = TRUE;
            PlaySE(SEQ_SE_PL_BREC12);
            work->videoFlagPtr = (u32)&NUMBER_SEARCH_VIDEO_FLAG(work);
            search->foundVideo = work->net.downloadedVideo;
            MI_CpuCopy8(search->foundVideo, work->net.videoBuffer, 0x1D4C);
        }
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[1], 0, 0);
        break;
    case 3:
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    case 4:
        work->step++;
        break;
    default:
        if (search->downloaded) {
            VsRecorder_SetSubState(work, NUMBER_SEARCH_SHOW_DETAILS);
        } else {
            VsRecorder_SetSubState(work, NUMBER_SEARCH_EXIT);
        }
        break;
    }
    return FALSE;
}

// owner's profile and battle header of the video found by number
void VsRecorder_LoadFoundVideoDetails(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    search->profile.card = FrontierCard_New(HEAP_ID_109);
    GlobalTerminalNet_CopyRequestHeader(search->foundVideo, search->profile.card);
    search->info.ownerName = (const u16 *)search->profile.card;
    search->info.header = BattleRecordHeader_New(HEAP_ID_109);
    MI_CpuCopy8(search->foundVideo + 0x80, search->info.header, 0x64);
}

// owner's profile and battle header of the chosen search result
void VsRecorder_LoadSearchResultDetails(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    search->profile.card = FrontierCard_New(HEAP_ID_109);
    GlobalTerminalNet_CopyRequestHeader(work->net.videoList.headers[work->listWindow.cursor], search->profile.card);
    search->info.ownerName = (const u16 *)search->profile.card;
    search->info.header = BattleRecordHeader_New(HEAP_ID_109);
    MI_CpuCopy8(work->net.videoList.headers[work->listWindow.cursor]->data, search->info.header, 0x64);
}

void VsRecorder_FreeFoundVideoDetails(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    BattleRecordHeader_Free(search->info.header);
    FrontierCard_Delete(search->profile.card);
}

// the found video's details (video info, play, save, back)
BOOL VsRecorder_NumberSearchShowDetails(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_StartFlipScreens(work, 0);
        work->step++;
        break;
    case 1:
        if (VsRecorder_IsFlipScreensDone(work)) {
            VsRecorder_ApplyBlend(&search->eva, &search->evb, GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 0);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            VsRecorder_LoadScreen(work, 6, GF_BG_LYR_SUB_3);
            SetBgPriority(GF_BG_LYR_MAIN_0, 1);
            SetBgPriority(GF_BG_LYR_MAIN_1, 3);
            SetBgPriority(GF_BG_LYR_MAIN_2, 0);
            SetBgPriority(GF_BG_LYR_MAIN_3, 1);
            SetBgPriority(GF_BG_LYR_SUB_0, 1);
            SetBgPriority(GF_BG_LYR_SUB_1, 3);
            SetBgPriority(GF_BG_LYR_SUB_2, 0);
            SetBgPriority(GF_BG_LYR_SUB_3, 2);
            VsRecorder_LoadFoundVideoDetails(work);
            VsRecorder_CreateSearchButtons(work);
            search->hitboxes = TouchHitboxController_Create(sVsRecorderSearchDetailHitboxes, 4, VsRecorder_OnNumberSearchDetailsTouch, work, HEAP_ID_109);
            search->slotHitboxes = TouchHitboxController_Create(sVsRecorderSearchSaveSlotHitboxes, 4, VsRecorder_OnNumberSearchSlotTouch, work, HEAP_ID_109);
            search->overwriteHitboxes = TouchHitboxController_Create(sVsRecorderSearchOverwriteHitboxes, 2, VsRecorder_OnNumberSearchOverwriteTouch, work, HEAP_ID_109);
            work->step++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        search->showProfile = 0;
        VsRecorder_CreateVideoInfo(&search->info, work);
        VsRecorder_LoadScreen(work, ov40_02230410(&search->info), GF_BG_LYR_MAIN_3);
        VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 3:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, TRUE);
            VsRecorder_CreateSearchDetailsButtonWindow(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, NUMBER_SEARCH_DETAILS_MAIN);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_NumberSearchDetailsMain(VsRecorderWork *work) {
    TouchHitboxController_IsTriggered(((VsRecorderVideoSearch *)work->screenWork)->hitboxes);
    return FALSE;
}

BOOL VsRecorder_NumberSearchToggleProfile(VsRecorderWork *work) {
    if (VsRecorder_FoundVideoToggleProfile(work)) {
        VsRecorder_SetSubState(work, NUMBER_SEARCH_DETAILS_MAIN);
    }
    return FALSE;
}

// ends the screen (the battle is played by the launcher)
BOOL VsRecorder_NumberSearchPlay(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (VsRecorder_FoundVideoPlay(work)) {
        Heap_Free(search);
        return TRUE;
    }
    return FALSE;
}

// back from the details to the menu
BOOL VsRecorder_NumberSearchExitDetails(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        TouchHitboxController_Destroy(search->hitboxes);
        TouchHitboxController_Destroy(search->slotHitboxes);
        TouchHitboxController_Destroy(search->overwriteHitboxes);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        VsRecorder_FreeFoundVideoDetails(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&search->info, work);
        } else {
            VsRecorder_FreeProfile(&search->profile, work);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_RemoveSearchDetailsButtonWindow(work);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            VsRecorder_FreeSearchButtons(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_ResetFade(&search->fade);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    case 3:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&search->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&search->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(search);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                if (BattleRecord_Exists() == TRUE) {
                    BattleRecord_Delete();
                }
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)search->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

// slot button label: the owner of the video saved there, or empty
void VsRecorder_SetSearchSlotButtonName(VsRecorderTextSprite *button, VsRecorderWork *work, int slot) {
    Window window;
    String *text;
    MsgData *msgData = work->msgData;

    if (work->videoOwners[slot] == NULL) {
        text = NewString_ReadMsgData(msgData, 8);
    } else {
        MessageFormat *msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
        String *template = NewString_ReadMsgData(work->msgData, 7);
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

#pragma force_active reset

// message window over the details (main screen BG 2); msg 0x64 names the video's owner
void VsRecorder_ShowSearchMessage(VsRecorderWork *work, int msgId, int unused) {
    Window *window = &work->messageWindow;
    VsRecorderVideoSearch *search = work->screenWork;
    String *str;

    ((void (*)(VsRecorderWork *, GFBgLayer))ov40_0222C6C8)(work, GF_BG_LYR_MAIN_2); // the third argument is left unset
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 1, 19, 30, 4, 14, 0x12C);
    if (msgId == 0x64) {
        FrontierCard *card = search->profile.card;
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

// message window; msg 0x64 names the video's owner, msg 0x66 the owner of the video in slot
void VsRecorder_PrintSearchMessage(VsRecorderWork *work, int msgId, int slot) {
    Window *window = &work->messageWindow;
    VsRecorderVideoSearch *search = work->screenWork;
    String *str;

    if (msgId == 0x64) {
        FrontierCard *card = search->profile.card;
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
    } else if (msgId == 0x66) {
        FrontierCard *card = work->videoOwners[slot];
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

// the three slots and cancel
void VsRecorder_CreateSearchSlotButtons(VsRecorderWork *work) {
    u8 *search = work->screenWork; // walks by one button
    VsRecorderSlotButtonTable msgIds = sVsRecorderSearchSlotMsgIds;
    VsRecorderSlotButtonTable anims = sVsRecorderSearchSlotAnims;
    VsRecorderGfx *gfx;
    const int *msgId;
    int y;
    VsRecorderTextSprite *button;
    const int *anim;
    int i;

    ov40_0222D78C(work, NNS_G2D_VRAM_TYPE_2DSUB);
    msgId = msgIds.values;
    y = 105;
    gfx = (VsRecorderGfx *)work;
    i = 0;
    button = ((VsRecorderVideoSearch *)search)->buttons;
    anim = anims.values;
    gfx = (VsRecorderGfx *)&work->narc;
    for (; i < 4; i++) {
        ((VsRecorderVideoSearch *)search)->buttons[0].sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
        VsRecorder_CreateTextSprite(button, gfx, NNS_G2D_VRAM_TYPE_2DSUB);
        VsRecorder_SetTextSpriteMsg(button, gfx, *msgId);
        if (i != 3) {
            VsRecorder_SetSearchSlotButtonName(button, work, i + 1);
        }
        ManagedSprite_SetPositionXY(((VsRecorderVideoSearch *)search)->buttons[0].sprite, 50, y);
        ManagedSprite_SetAnim(((VsRecorderVideoSearch *)search)->buttons[0].sprite, *anim);
        TextOBJ_SetPosition(((VsRecorderVideoSearch *)search)->buttons[0].text, 36, -8);
        TextOBJ_SetSpritesDrawFlag(((VsRecorderVideoSearch *)search)->buttons[0].text, TRUE);
        msgId++;
        search += sizeof(VsRecorderTextSprite);
        button++;
        anim++;
        y += 36;
    }
}

void VsRecorder_FreeSearchSlotButtons(VsRecorderWork *work) {
    VsRecorderTextSprite *button;
    VsRecorderVideoSearch *search = work->screenWork; // walks by one button
    int i;

    button = search->buttons;
    for (i = 0; i < 4; i++) {
        VsRecorder_FreeTextSprite(button);
        Sprite_DeleteAndFreeResources(search->buttons[0].sprite);
        button++;
        search = (VsRecorderVideoSearch *)((u8 *)search + sizeof(VsRecorderTextSprite));
    }
    VsRecorder_FreeButtonGfx(work);
}

// save: the slot menu, or a notice (and back to the menu) when the video is saved already
BOOL VsRecorder_NumberSearchShowSlots(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (VsRecorder_IsVideoSaved(work, BattleRecordHeader_GetField(BattleRecord_GetHeader(), 4, 0))) {
        switch (work->step) {
        case 0:
            VsRecorder_SetVBlankPaused(work, TRUE);
            if (search->showProfile == 0) {
                VsRecorder_FreeVideoInfo(&search->info, work);
            } else {
                VsRecorder_FreeProfile(&search->profile, work);
            }
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_RemoveSearchDetailsButtonWindow(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            work->step++;
            break;
        case 1:
            VsRecorder_StepFade(&search->fade, 1);
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
                VsRecorder_SetVBlankPaused(work, TRUE);
                VsRecorder_FreeSearchButtons(work);
                VsRecorder_CreateSearchSlotButtons(work);
                VsRecorder_SetVBlankPaused(work, FALSE);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
            break;
        case 2:
            VsRecorder_ShowTopMessage(work, 0x115);
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
            VsRecorder_StepFade(&search->fade, 1);
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
                VsRecorder_FreeSearchSlotButtons(work);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
            break;
        case 5:
            TouchHitboxController_Destroy(search->hitboxes);
            TouchHitboxController_Destroy(search->slotHitboxes);
            TouchHitboxController_Destroy(search->overwriteHitboxes);
            VsRecorder_ResetFade(&search->fade);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeFoundVideoDetails(work);
            VsRecorder_RebuildMenuRowsParent(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_StartFlipScreens(work, 1);
            work->step++;
            break;
        default:
            if (VsRecorder_IsFlipScreensDone(work)) {
                if (VsRecorder_StepFade(&search->fade, 0)) {
                    VsRecorder_ClearPaletteBlend(work);
                    VsRecorder_ResetFade(&search->fade);
                    PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
                    VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                    VsRecorder_SetSubState(work, 5);
                    Heap_Free(search);
                    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                    if (BattleRecord_Exists() == TRUE) {
                        BattleRecord_Delete();
                    }
                } else {
                    PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)search->fade, (u16)work->blendColor);
                    PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
                }
            }
            break;
        }
    } else if (VsRecorder_FoundVideoShowSlots(work)) {
        VsRecorder_SetSubState(work, 13);
    }
    return FALSE;
}

BOOL VsRecorder_NumberSearchSlotsMain(VsRecorderWork *work) {
    TouchHitboxController_IsTriggered(((VsRecorderVideoSearch *)work->screenWork)->slotHitboxes);
    return FALSE;
}

BOOL VsRecorder_NumberSearchSlotsCancel(VsRecorderWork *work) {
    if (VsRecorder_FoundVideoSlotsCancel(work)) {
        VsRecorder_SetSubState(work, NUMBER_SEARCH_DETAILS_MAIN);
    }
    return FALSE;
}

BOOL VsRecorder_NumberSearchReturnToDetails(VsRecorderWork *work) {
    if (VsRecorder_FoundVideoReturnToDetails(work)) {
        VsRecorder_SetSubState(work, NUMBER_SEARCH_DETAILS_MAIN);
    }
    return FALSE;
}

BOOL VsRecorder_NumberSearchConfirmOverwrite(VsRecorderWork *work) {
    if (VsRecorder_FoundVideoConfirmOverwrite(work)) {
        VsRecorder_SetSubState(work, 16);
    }
    return FALSE;
}

BOOL VsRecorder_NumberSearchOverwriteMain(VsRecorderWork *work) {
    TouchHitboxController_IsTriggered(((VsRecorderVideoSearch *)work->screenWork)->overwriteHitboxes);
    return FALSE;
}

BOOL VsRecorder_NumberSearchOverwriteNo(VsRecorderWork *work) {
    if (VsRecorder_FoundVideoOverwriteNo(work)) {
        VsRecorder_SetSubState(work, 13);
    }
    return FALSE;
}

BOOL VsRecorder_NumberSearchSave(VsRecorderWork *work) {
    VsRecorder_FoundVideoSave(work);
    return FALSE;
}

// back from the video playback: the found video's details again
BOOL VsRecorder_NumberSearchResume(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        if (VsRecorder_IsFlipScreensDone(work)) {
            VsRecorder_InitBgBlend(&search->eva, &search->evb, 0);
            search->videoFlag = NUMBER_SEARCH_VIDEO_FLAG(work);
            search->foundVideo = work->net.videoBuffer;
            work->net.downloadedVideo = search->foundVideo;
            VsRecorder_ImportBattleRecord(work);
            VsRecorder_LoadFoundVideoDetails(work);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            VsRecorder_StartFlipScreens(work, 0);
            work->step++;
        }
        break;
    case 1:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 1)) {
                SetBgPriority(GF_BG_LYR_MAIN_0, 1);
                SetBgPriority(GF_BG_LYR_MAIN_1, 3);
                SetBgPriority(GF_BG_LYR_MAIN_2, 0);
                SetBgPriority(GF_BG_LYR_MAIN_3, 1);
                SetBgPriority(GF_BG_LYR_SUB_0, 1);
                SetBgPriority(GF_BG_LYR_SUB_1, 3);
                SetBgPriority(GF_BG_LYR_SUB_2, 0);
                SetBgPriority(GF_BG_LYR_SUB_3, 2);
                search->hitboxes = TouchHitboxController_Create(sVsRecorderSearchDetailHitboxes, 4, VsRecorder_OnNumberSearchDetailsTouch, work, HEAP_ID_109);
                search->slotHitboxes = TouchHitboxController_Create(sVsRecorderSearchSaveSlotHitboxes, 4, VsRecorder_OnNumberSearchSlotTouch, work, HEAP_ID_109);
                search->overwriteHitboxes = TouchHitboxController_Create(sVsRecorderSearchOverwriteHitboxes, 2, VsRecorder_OnNumberSearchOverwriteTouch, work, HEAP_ID_109);
                work->step++;
            }
        }
        break;
    case 2:
        VsRecorder_CreateSearchButtons(work);
        VsRecorder_CreateSearchDetailsButtonWindow(work);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_CreateVideoInfo(&search->info, work);
            VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
            VsRecorder_LoadScreen(work, ov40_02230410(&search->info), GF_BG_LYR_MAIN_3);
        } else {
            VsRecorder_CreateProfile(&search->profile, work);
            VsRecorder_SetProfileDrawFlag(&search->profile, FALSE);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 80, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 6, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    case 3:
        search->fade = 0;
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            if (search->showProfile == 0) {
                VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, TRUE);
            } else {
                VsRecorder_SetProfileDrawFlag(&search->profile, TRUE);
            }
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_109);
        work->step++;
        break;
    case 5:
        if (IsPaletteFadeFinished() == TRUE) {
            work->step++;
        }
        break;
    default:
        work->menuDepth++;
        VsRecorder_ClearResumeState(work);
        VsRecorder_SetSubState(work, NUMBER_SEARCH_DETAILS_MAIN);
        break;
    }
    return FALSE;
}

// send video: the busy cursor
BOOL VsRecorder_SendVideoShowCursor(VsRecorderWork *work) {
    VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
    OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
    OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
    VsRecorder_SetSubState(work, SEND_VIDEO_UPLOAD);
    return FALSE;
}

// upload the saved video with the own profile, then show its video number
BOOL VsRecorder_SendVideoUpload(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    GlobalTerminalNetError *error;
    int result;

    switch (work->step) {
    case 0:
        VsRecorder_ShowTopMessage(work, 0x77);
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 1:
        if (!VsRecorder_IsNetIdle(work)) {
            return FALSE;
        }
        BattleRecord_Load(work->saveData, HEAP_ID_109, &result, NULL, 0);
        search->uploadCard = FrontierCard_New(HEAP_ID_109);
        FrontierCard_InitFromSave(search->uploadCard, work->saveData);
        if (GlobalTerminalNet_RequestVideoUpload(VsRecorder_GetNet(work), search->uploadCard) == TRUE) {
            FrontierCard_Delete(search->uploadCard);
            work->step++;
        }
        break;
    case 2:
        if (!VsRecorder_IsNetIdle(work)) {
            return FALSE;
        }
        BattleRecord_Delete();
        VsRecorder_HideTopMessage(work);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_ShowMessageScreen(work, 6, error->code, error->type);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
            work->step = 0xFF;
        } else {
            VsRecorder_ShowTopMessage(work, 0x77);
            VsRecorder_PrintVideoNumber(search, work);
            StopSE(SEQ_SE_PL_BREC80, 0);
            PlaySE(SEQ_SE_PL_BREC12);
            work->step++;
        }
        break;
    case 3:
        if (System_GetTouchNew()) {
            VsRecorder_HideTopMessage(work);
            work->step++;
        }
        break;
    default:
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        VsRecorder_AddGameStatsScore(work, 0x23);
        VsRecorder_SetSubState(work, SEND_VIDEO_EXIT);
        break;
    }
    return FALSE;
}

// back to the menu
BOOL VsRecorder_SendVideoExit(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (work->step == 0) {
        VsRecorder_ResetFade(&search->fade);
        VsRecorder_MenuPop(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
    } else if (VsRecorder_IsFlipScreensDone(work)) {
        if (VsRecorder_StepFade(&search->fade, 0)) {
            VsRecorder_ClearPaletteBlend(work);
            VsRecorder_ResetFade(&search->fade);
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
            VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
            VsRecorder_SetSubState(work, 5);
            Heap_Free(search);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        } else {
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)search->fade, (u16)work->blendColor);
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        }
    }
    return FALSE;
}

void VsRecorder_RemoveSearchDetailsButtonWindow(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    ClearWindowTilemapAndCopyToVram(&search->buttonWindows[0]);
    RemoveWindow(&search->buttonWindows[0]);
}

void VsRecorder_PrintSearchDetailsButtonLabel(VsRecorderWork *work, int msgId) {
    VsRecorderVideoSearch *search = work->screenWork;
    Window *window = &search->buttonWindows[0];
    String *str;

    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msgId);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

// label of the video info / profile toggle
void VsRecorder_CreateSearchDetailsButtonWindow(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    Window *window = &search->buttonWindows[0];

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 8, 3, 16, 2, 14, 1);
    VsRecorder_PrintSearchDetailsButtonLabel(work, search->showProfile + 0x79);
}

// YES / NO to overwrite a saved video
void VsRecorder_CreateSearchOverwriteWindows(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    Window *window = &search->buttonWindows[0];
    String *str;

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 4, 6, 10, 2, 14, 1);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, 130);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
    window = &search->buttonWindows[1];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 18, 6, 10, 2, 14, 21);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, 131);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

void VsRecorder_RemoveSearchOverwriteWindows(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    ClearWindowTilemapAndCopyToVram(&search->buttonWindows[0]);
    RemoveWindow(&search->buttonWindows[0]);
    ClearWindowTilemapAndCopyToVram(&search->buttonWindows[1]);
    RemoveWindow(&search->buttonWindows[1]);
}

// number search details: video info / profile toggle, play, save, back
void VsRecorder_OnNumberSearchDetailsTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, NUMBER_SEARCH_TOGGLE_PROFILE);
        break;
    case 1:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, NUMBER_SEARCH_PLAY);
        break;
    case 2:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, NUMBER_SEARCH_SHOW_SLOTS);
        break;
    case 3:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, NUMBER_SEARCH_EXIT_DETAILS);
        break;
    }
}

// number search slot menu: slots 1-3 (a used slot asks first), cancel
void VsRecorder_OnNumberSearchSlotTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderVideoSearch *search = work->screenWork;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
    case 1:
    case 2:
        search->slot = index + 1;
        if (work->videoOwners[search->slot] != NULL) {
            VsRecorder_SetSubState(work, NUMBER_SEARCH_CONFIRM_OVERWRITE);
        } else {
            VsRecorder_SetSubState(work, NUMBER_SEARCH_SAVE);
        }
        VsRecorder_OnTouchConfirm(work);
        break;
    case 3:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, NUMBER_SEARCH_SLOTS_CANCEL);
        break;
    }
}

void VsRecorder_OnNumberSearchOverwriteTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_RemoveSearchOverwriteWindows(work);
        VsRecorder_SetSubState(work, NUMBER_SEARCH_SAVE);
        break;
    case 1:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, NUMBER_SEARCH_OVERWRITE_NO);
        break;
    }
}

// search conditions: facility, species, own location (toggle), exit, search
void VsRecorder_OnSearchConditionTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderVideoSearch *search = work->screenWork;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
        VsRecorder_OnTouchConfirm(work);
        search->condition = index;
        VsRecorder_SetSubState(work, SEARCH_OPEN_CONDITION);
        break;
    case 1:
        VsRecorder_OnTouchConfirm(work);
        search->condition = index;
        VsRecorder_SetSubState(work, SEARCH_OPEN_CONDITION);
        break;
    case 2: {
        u32 country = FrontierCard_GetCountry(work->videoOwners[0]);

        VsRecorder_OnTouchConfirm(work);
        if (country == 0) {
            PlaySE(SEQ_SE_PL_BREC58);
            VsRecorder_SetMessageText(work, 0x80);
        } else {
            if (search->country == 0xFF) {
                search->country = FrontierCard_GetCountry(work->videoOwners[0]);
                search->region = FrontierCard_GetRegion(work->videoOwners[0]);
            } else {
                search->country = 0xFF;
                search->region = 0xFF;
            }
            VsRecorder_PrintSearchLocation(work, search->country, search->region);
            VsRecorder_PrintLocationButtonLabel(work);
        }
        break;
    }
    case 3:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, SEARCH_EXIT);
        break;
    case 4:
        VsRecorder_OnTouchConfirm(work);
        if (VsRecorder_HasSearchConditions(work) == FALSE) {
            PlaySE(SEQ_SE_PL_BREC58);
            VsRecorder_SetMessageText(work, 0x74);
        } else {
            VsRecorder_SetSubState(work, SEARCH_REQUEST);
        }
        break;
    }
}

// species group buttons: the seen species of the group
void VsRecorder_OnSpeciesGroupTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;

    if (event != 0) {
        return;
    }
    if (VsRecorder_SearchCategoryHasSeen(work, index) == FALSE) {
        PlaySE(SEQ_SE_PL_BREC58);
    } else {
        VsRecorder_BuildSearchSpeciesList(work, index);
    }
}

// search result details: video info / profile toggle, play, save, back (play and save download the video first)
void VsRecorder_OnSearchDetailsTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderVideoSearch *search = work->screenWork;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, SEARCH_TOGGLE_PROFILE);
        break;
    case 1:
        VsRecorder_OnTouchConfirm(work);
        if (BattleRecord_Exists()) {
            VsRecorder_SetSubState(work, SEARCH_PLAY);
        } else {
            search->nextSubState = SEARCH_PLAY;
            VsRecorder_SetSubState(work, SEARCH_DOWNLOAD_VIDEO);
        }
        break;
    case 2:
        VsRecorder_OnTouchConfirm(work);
        if (BattleRecord_Exists()) {
            VsRecorder_SetSubState(work, SEARCH_SHOW_SLOTS);
        } else {
            search->nextSubState = SEARCH_SHOW_SLOTS;
            VsRecorder_SetSubState(work, SEARCH_DOWNLOAD_VIDEO);
        }
        break;
    case 3:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, SEARCH_BACK_TO_RESULTS);
        break;
    }
}

// search result slot menu: slots 1-3 (a used slot asks first), cancel
void VsRecorder_OnSearchSlotTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderVideoSearch *search = work->screenWork;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
    case 1:
    case 2:
        search->slot = index + 1;
        if (work->videoOwners[search->slot] != NULL) {
            VsRecorder_SetSubState(work, SEARCH_CONFIRM_OVERWRITE);
        } else {
            VsRecorder_SetSubState(work, SEARCH_SAVE);
        }
        VsRecorder_OnTouchConfirm(work);
        break;
    case 3:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, SEARCH_SLOTS_CANCEL);
        break;
    }
}

void VsRecorder_OnSearchOverwriteTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_RemoveSearchOverwriteWindows(work);
        VsRecorder_SetSubState(work, SEARCH_SAVE);
        break;
    case 1:
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, SEARCH_OVERWRITE_NO);
        break;
    }
}

// backOnly: hide the first button and move the second one left
void VsRecorder_SetNumberPadButtonsMode(VsRecorderWork *work, int backOnly) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (backOnly == FALSE) {
        ManagedSprite_SetDrawFlag(search->buttons[0].sprite, TRUE);
        TextOBJ_SetSpritesDrawFlag(search->buttons[0].text, TRUE);
        ManagedSprite_SetPositionXY(search->buttons[1].sprite, 128, 168);
    } else {
        ManagedSprite_SetDrawFlag(search->buttons[0].sprite, FALSE);
        TextOBJ_SetSpritesDrawFlag(search->buttons[0].text, FALSE);
        ManagedSprite_SetPositionXY(search->buttons[1].sprite, 80, 168);
    }
    TextOBJ_SetPosition(search->buttons[0].text, 36, -8);
    TextOBJ_SetPosition(search->buttons[1].text, 36, -8);
}

// exit and back buttons under the number pad (main screen)
void VsRecorder_CreateNumberPadButtons(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    ov40_0222D6EC(work, NNS_G2D_VRAM_TYPE_2DMAIN);
    search->buttons[0].sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DMAIN);
    search->buttons[1].sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DMAIN);
    VsRecorder_CreateTextSprite(&search->buttons[0], (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DMAIN);
    VsRecorder_CreateTextSprite(&search->buttons[1], (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DMAIN);
    VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
    VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x6F);
    ManagedSprite_SetPositionXY(search->buttons[0].sprite, 32, 168);
    ManagedSprite_SetPositionXY(search->buttons[1].sprite, 128, 168);
    TextOBJ_SetPosition(search->buttons[0].text, 36, -8);
    TextOBJ_SetPosition(search->buttons[1].text, 36, -8);
    TextOBJ_SetSpritesDrawFlag(search->buttons[0].text, TRUE);
    TextOBJ_SetSpritesDrawFlag(search->buttons[1].text, TRUE);
    ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
    ManagedSprite_SetAnim(search->buttons[1].sprite, 1);
    VsRecorder_SetNumberPadButtonsMode(work, FALSE);
}

void VsRecorder_FreeNumberPadButtons(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    VsRecorder_FreeTextSprite(&search->buttons[0]);
    VsRecorder_FreeTextSprite(&search->buttons[1]);
    Sprite_DeleteAndFreeResources(search->buttons[0].sprite);
    Sprite_DeleteAndFreeResources(search->buttons[1].sprite);
    VsRecorder_FreeButtonGfx(work);
}

// backOnly: hide the second button and move the first one right
void VsRecorder_SetSearchButtonsMode(VsRecorderWork *work, int backOnly) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (backOnly == FALSE) {
        ManagedSprite_SetDrawFlag(search->buttons[1].sprite, TRUE);
        TextOBJ_SetSpritesDrawFlag(search->buttons[1].text, TRUE);
        ManagedSprite_SetPositionXY(search->buttons[0].sprite, 32, 232);
    } else {
        ManagedSprite_SetDrawFlag(search->buttons[1].sprite, FALSE);
        TextOBJ_SetSpritesDrawFlag(search->buttons[1].text, FALSE);
        ManagedSprite_SetPositionXY(search->buttons[0].sprite, 80, 232);
    }
    TextOBJ_SetPosition(search->buttons[0].text, 36, -8);
    TextOBJ_SetPosition(search->buttons[1].text, 36, -8);
}

// exit and back buttons of the details (sub screen)
void VsRecorder_CreateSearchButtons(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    ov40_0222D6EC(work, NNS_G2D_VRAM_TYPE_2DSUB);
    search->buttons[0].sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    search->buttons[1].sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&search->buttons[0], (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&search->buttons[1], (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    if (work->selection == 210) {
        VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
        VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x6F);
        ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
        ManagedSprite_SetAnim(search->buttons[1].sprite, 1);
    } else {
        VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
        VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
        ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
        ManagedSprite_SetAnim(search->buttons[1].sprite, 3);
    }
    ManagedSprite_SetPositionXY(search->buttons[0].sprite, 32, 232);
    ManagedSprite_SetPositionXY(search->buttons[1].sprite, 128, 232);
    TextOBJ_SetPosition(search->buttons[0].text, 36, -8);
    TextOBJ_SetPosition(search->buttons[1].text, 36, -8);
    TextOBJ_SetSpritesDrawFlag(search->buttons[0].text, TRUE);
    TextOBJ_SetSpritesDrawFlag(search->buttons[1].text, TRUE);
    VsRecorder_SetSearchButtonsMode(work, FALSE);
}

void VsRecorder_FreeSearchButtons(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    VsRecorder_FreeTextSprite(&search->buttons[0]);
    VsRecorder_FreeTextSprite(&search->buttons[1]);
    Sprite_DeleteAndFreeResources(search->buttons[0].sprite);
    Sprite_DeleteAndFreeResources(search->buttons[1].sprite);
    VsRecorder_FreeButtonGfx(work);
}

// details: switch between the video info and the owner's profile; TRUE when done
BOOL VsRecorder_FoundVideoToggleProfile(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&search->info, work);
        } else {
            VsRecorder_FreeProfile(&search->profile, work);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 1)) {
            work->step++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_CreateProfile(&search->profile, work);
            VsRecorder_SetProfileDrawFlag(&search->profile, FALSE);
            VsRecorder_LoadScreen(work, 80, GF_BG_LYR_MAIN_3);
        } else {
            VsRecorder_CreateVideoInfo(&search->info, work);
            VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
            VsRecorder_LoadScreen(work, ov40_02230410(&search->info), GF_BG_LYR_MAIN_3);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        break;
    case 3:
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 1)) {
            if (search->showProfile == 0) {
                VsRecorder_SetProfileDrawFlag(&search->profile, TRUE);
            } else {
                VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, TRUE);
            }
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        break;
    default:
        search->showProfile = 1 ^ search->showProfile;
        VsRecorder_PrintSearchDetailsButtonLabel(work, search->showProfile + 0x79);
        return TRUE;
    }
    return FALSE;
}

// save: the slot menu with the owner's name in the message window; TRUE when done
BOOL VsRecorder_FoundVideoShowSlots(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&search->info, work);
        } else {
            VsRecorder_FreeProfile(&search->profile, work);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_RemoveSearchDetailsButtonWindow(work);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeSearchButtons(work);
            VsRecorder_CreateSearchSlotButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 1)) {
            VsRecorder_ShowSearchMessage(work, 0x64, 0);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

// the chosen slot holds a video: YES / NO to overwrite it; TRUE when shown
BOOL VsRecorder_FoundVideoConfirmOverwrite(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 84, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            VsRecorder_PrintSearchMessage(work, 0x66, search->slot);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 1:
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 2)) {
            VsRecorder_CreateSearchOverwriteWindows(work);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

// NO to the overwrite prompt: back to the slot choice; TRUE when done
BOOL VsRecorder_FoundVideoOverwriteNo(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
            VsRecorder_RemoveSearchOverwriteWindows(work);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
            work->step++;
        }
        break;
    case 2:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 2)) {
            VsRecorder_PrintSearchMessage(work, 0x64, 0);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

// save the downloaded video to search->slot, then back to the menu
BOOL VsRecorder_FoundVideoSave(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            VsRecorder_PrintSearchMessage(work, 0x116, 0);
            VsRecorder_ResetRecordSave(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        if (VsRecorder_StepRecordSave(work, search->slot, search->videoFlag)) {
            search->videoFlag = 0;
            work->step++;
        }
        break;
    case 3:
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 4: {
        u64 number;

        if (!VsRecorder_IsNetIdle(work)) {
            return FALSE;
        }
        VsRecorder_PrintSearchMessage(work, 0x118, 0);
        number = BattleRecordHeader_GetField(BattleRecord_GetHeader(), 4, 0);
        if (GlobalTerminalNet_ReportVideoSaved(VsRecorder_GetNet(work), (u32)number, (u32)(number >> 32)) == TRUE) {
            work->step++;
        }
        break;
    }
    case 5: {
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
        VsRecorder_PrintSearchMessage(work, 0x119, 0);
        break;
    }
    case 6:
        if (System_GetTouchNew()) {
            VsRecorder_RemoveTopMessage(work);
            VsRecorder_FreeSearchSlotButtons(work);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
            VsRecorder_FreeSavedVideos(work);
            VsRecorder_LoadSavedVideos(work);
            VsRecorder_ClearListWindow(&work->listWindow);
            work->step++;
        }
        break;
    case 7:
        TouchHitboxController_Destroy(search->hitboxes);
        TouchHitboxController_Destroy(search->slotHitboxes);
        TouchHitboxController_Destroy(search->overwriteHitboxes);
        VsRecorder_ResetFade(&search->fade);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeFoundVideoDetails(work);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_109);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&search->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&search->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(search);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                if (BattleRecord_Exists() == TRUE) {
                    BattleRecord_Delete();
                }
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)search->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

// back from the message screen (download error) to the details; TRUE when done
BOOL VsRecorder_FoundVideoReturnToDetails(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        if (work->selection == 210) {
            VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
            VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
            ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
            ManagedSprite_SetAnim(search->buttons[1].sprite, 3);
        }
        VsRecorder_RemoveSearchDetailsButtonWindow(work);
        VsRecorder_CreateSearchDetailsButtonWindow(work);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&search->info, work);
            VsRecorder_CreateVideoInfo(&search->info, work);
            VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
            VsRecorder_LoadScreen(work, ov40_02230410(&search->info), GF_BG_LYR_MAIN_3);
        } else {
            VsRecorder_FreeProfile(&search->profile, work);
            VsRecorder_CreateProfile(&search->profile, work);
            VsRecorder_SetProfileDrawFlag(&search->profile, FALSE);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 80, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 6, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    case 3:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            if (search->showProfile == 0) {
                VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, TRUE);
            } else {
                VsRecorder_SetProfileDrawFlag(&search->profile, TRUE);
            }
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

// cancel in the slot menu: back to the details; TRUE when done
BOOL VsRecorder_FoundVideoSlotsCancel(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_RemoveTopMessage(work);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            VsRecorder_FreeSearchSlotButtons(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_CreateSearchButtons(work);
        if (work->selection == 210) {
            VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
            VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
            ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
            ManagedSprite_SetAnim(search->buttons[1].sprite, 3);
        }
        VsRecorder_CreateSearchDetailsButtonWindow(work);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_CreateVideoInfo(&search->info, work);
            VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
            VsRecorder_LoadScreen(work, ov40_02230410(&search->info), GF_BG_LYR_MAIN_3);
        } else {
            VsRecorder_CreateProfile(&search->profile, work);
            VsRecorder_SetProfileDrawFlag(&search->profile, FALSE);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 80, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 6, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    case 3:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            if (search->showProfile == 0) {
                VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, TRUE);
            } else {
                VsRecorder_SetProfileDrawFlag(&search->profile, TRUE);
            }
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

// leave the app to play the found video (the launcher comes back with the screen's resume substate); TRUE when done
BOOL VsRecorder_FoundVideoPlay(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_109);
        work->step++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            work->step++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&search->info, work);
        } else {
            VsRecorder_FreeProfile(&search->profile, work);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        TouchHitboxController_Destroy(search->hitboxes);
        TouchHitboxController_Destroy(search->slotHitboxes);
        TouchHitboxController_Destroy(search->overwriteHitboxes);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeFoundVideoDetails(work);
        VsRecorder_RemoveSearchDetailsButtonWindow(work);
        VsRecorder_FreeSearchButtons(work);
        VsRecorder_RebuildMenuRows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_SetResumeScreen(work, *work->screenPtr);
        VsRecorderApp_SetResult(work->result, 1, 0);
        work->step++;
        break;
    default:
        return TRUE;
    }
    return FALSE;
}

// onResponse[6]: the video number the server gave the sent video
void VsRecorder_OnVideoUploaded(void *arg, GlobalTerminalNetError *error) {
    VsRecorderWork *work = arg;
    VsRecorderVideoSearch *search = work->screenWork;

    if (error->isError != TRUE) {
        search->videoNumber = GlobalTerminalNet_GetUploadedVideoNumber(VsRecorder_GetNet(work));
    }
}

// onResponse[8]: the downloaded video becomes the battle record
void VsRecorder_OnVideoDownloaded(void *arg, GlobalTerminalNetError *error) {
    VsRecorderWork *work = arg;

    if (error->isError != TRUE) {
        GlobalTerminalNet_GetDownloadedVideo(VsRecorder_GetNet(work), &work->net.downloadedVideo);
        VsRecorder_ImportBattleRecord(work);
        NUMBER_SEARCH_VIDEO_FLAG(work) = work->net.downloadedVideo[0xA7];
    }
}

// onResponse[9] (GlobalTerminalNet_ReportVideoSaved, sent after saving a downloaded video): nothing to do
void VsRecorder_OnVideoSaveReported(void *arg, GlobalTerminalNetError *error) {
}
