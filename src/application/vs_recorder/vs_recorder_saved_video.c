#include <string.h>

#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "render_text.h"
#include "save.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"

// VS Recorder saved video screen (VS_RECORDER_SCREEN_SAVED_VIDEO): the battle video in the chosen save slot with its
// owner's profile, play (after saving the game if needed) and exit. Also the menu's next-colour-scheme substate.

extern const TouchscreenHitbox ov40_0224512C;    // YES
extern const TouchscreenHitbox ov40_02245130;    // NO
extern const TouchscreenHitbox ov40_02245134[3]; // profile, play, exit

// right colour scheme arrow (VS_RECORDER_SCREEN_MENU substate 8): like VsRecorder_MenuPrevScheme the other way round
BOOL VsRecorder_MenuNextScheme(VsRecorderWork *work) {
    switch (work->step) {
    case 0:
        work->fade = 0;
        work->colorScheme = work->colorScheme + 1;
        work->colorScheme = work->colorScheme % 7;
        work->blendColor = VsRecorder_GetBlendColor(work);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepFade(&work->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, work->fade, work->blendColor);
        break;
    case 2:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_OFF);
        VsRecorder_ReloadSchemePalettes(work, work->colorScheme);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, work->fade, work->blendColor);
        work->step++;
        break;
    case 3:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        work->step++;
        break;
    case 4:
        if (VsRecorder_StepFade(&work->fade, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xFFFF, work->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, work->fade, work->blendColor);
        break;
    default:
        work->fade = 0;
        VsRecorder_SetSubState(work, 0);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_SavedVideoInit(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderSavedVideo));
    memset(savedVideo, 0, sizeof(VsRecorderSavedVideo));
    work->screenWork = savedVideo;
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
    VsRecorder_InitBgBlend(&savedVideo->eva, &savedVideo->evb, 0);
    VsRecorder_SetSubState(work, 1);
    return FALSE;
}

BOOL VsRecorder_SavedVideoEnter(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    int ok;

    switch (work->step) {
    case 0:
        if (VsRecorder_StepFade(&savedVideo->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, savedVideo->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, savedVideo->fade, work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, savedVideo->fade, work->blendColor);
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
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    default:
        VsRecorder_StartFlipScreens(work, 0);
        if (VsRecorder_HasResumeScreen(work) == TRUE) {
            // back from playback: the video still needs saving unless it is the player's own (slot 0), already saved
            // (header field 5) or the battle was not won
            ok = FALSE;
            if (work->selection == 0) {
                ok = TRUE;
            } else if (BattleRecordHeader_GetField(work->savedVideos[work->selection], 5, ok) == 1) {
                ok = TRUE;
            } else if (work->battleResult == 1) {
                ok = TRUE;
            }
            if (ok) {
                VsRecorder_CreateExitButton(work);
                savedVideo->exitMode = 0;
                VsRecorder_SetSubState(work, 9);
            } else {
                savedVideo->exitMode = 1;
                VsRecorder_SetSubState(work, 6);
            }
        } else {
            VsRecorder_SetSubState(work, 2);
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorder_SavedVideoShow(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_CreateExitButton(work);
        VsRecorder_LoadScreen(work, 6, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 1:
        VsRecorder_SetSavedVideoSlot(work);
        savedVideo->hitboxes = TouchHitboxController_Create(ov40_02245134, 3, VsRecorder_OnSavedVideoTouch, work, HEAP_ID_109);
        work->step++;
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateVideoInfo(&savedVideo->info, work);
        VsRecorder_LoadScreen(work, ov40_02230410(&savedVideo->info), GF_BG_LYR_MAIN_3);
        VsRecorder_SetVideoPartyIconsDrawFlag(&savedVideo->info, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 3:
        VsRecorder_StepFade(&savedVideo->fade, 0);
        if (VsRecorder_StepBlend(&savedVideo->eva, &savedVideo->evb, FALSE, 0)) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&savedVideo->info, TRUE);
            VsRecorder_ShowProfileButton(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, savedVideo->fade, work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, 3);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_SavedVideoMain(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    TouchHitboxController_IsTriggered(savedVideo->hitboxes);
    return FALSE;
}

// swaps the top screen between the video info and its owner's profile
BOOL VsRecorder_SavedVideoToggleProfile(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    switch (work->step) {
    case 0:
        if (savedVideo->showProfile == FALSE) {
            VsRecorder_FreeVideoInfo(&savedVideo->info, work);
        } else {
            VsRecorder_FreeProfile(&savedVideo->profile, work);
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&savedVideo->eva, &savedVideo->evb, TRUE, 1)) {
            work->step++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (savedVideo->showProfile == FALSE) {
            VsRecorder_CreateProfile(&savedVideo->profile, work);
            VsRecorder_SetProfileDrawFlag(&savedVideo->profile, FALSE);
            VsRecorder_LoadScreen(work, 0x50, GF_BG_LYR_MAIN_3);
        } else {
            VsRecorder_CreateVideoInfo(&savedVideo->info, work);
            VsRecorder_SetVideoPartyIconsDrawFlag(&savedVideo->info, FALSE);
            VsRecorder_LoadScreen(work, ov40_02230410(&savedVideo->info), GF_BG_LYR_MAIN_3);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        break;
    case 3:
        if (VsRecorder_StepBlend(&savedVideo->eva, &savedVideo->evb, FALSE, 1)) {
            if (savedVideo->showProfile == FALSE) {
                VsRecorder_SetProfileDrawFlag(&savedVideo->profile, TRUE);
            } else {
                VsRecorder_SetVideoPartyIconsDrawFlag(&savedVideo->info, TRUE);
            }
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        break;
    default:
        savedVideo->showProfile ^= 1;
        VsRecorder_PrintProfileButton(work, savedVideo->showProfile + 0x79); // VIEW PROFILE / CLOSE PROFILE
        VsRecorder_SetSubState(work, 3);
        break;
    }
    return FALSE;
}

void VsRecorder_ShowYesNoWindows(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    Window *window = &savedVideo->windows[0];
    String *str;

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 4, 6, 10, 2, 0xE, 1);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, 0x82); // YES
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
    window = &savedVideo->windows[1];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 18, 6, 10, 2, 0xE, 21);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, 0x83); // NO
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

void VsRecorder_HideYesNoWindows(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    ClearWindowTilemapAndCopyToVram(&savedVideo->windows[0]);
    RemoveWindow(&savedVideo->windows[0]);
    ClearWindowTilemapAndCopyToVram(&savedVideo->windows[1]);
    RemoveWindow(&savedVideo->windows[1]);
}

#pragma force_active on

// "The game will be saved before you can view Battle Videos." YES saves, NO goes back
BOOL VsRecorder_SavedVideoSavePrompt(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    switch (work->step) {
    case 0:
        TouchHitboxController_Destroy(savedVideo->hitboxes);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (savedVideo->showProfile == FALSE) {
            VsRecorder_FreeVideoInfo(&savedVideo->info, work);
        } else {
            VsRecorder_FreeProfile(&savedVideo->profile, work);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_RemoveProfileButton(work);
        savedVideo->exitMode = 0;
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&savedVideo->fade, 1);
        if (VsRecorder_StepBlend(&savedVideo->eva, &savedVideo->evb, TRUE, 0)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeExitButton(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)savedVideo->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_ShowYesNoWindows(work);
        VsRecorder_ShowTopMessage(work, 0x128);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 84, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        work->step++;
        break;
    case 3:
        VsRecorder_StepFade(&savedVideo->fade, 0);
        if (VsRecorder_StepBlend(&savedVideo->eva, &savedVideo->evb, FALSE, 2)) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)savedVideo->fade, (u16)work->blendColor);
        break;
    case 4:
        if (TouchscreenHitbox_TouchNewIsIn(&ov40_0224512C) != FALSE) {
            savedVideo->exitMode = 1;
            VsRecorder_OnTouchConfirm(work);
            if (Save_NumModifiedPCBoxesIsMany(work->saveData)) {
                VsRecorder_SetMessageText(work, 0x12A);
            } else {
                VsRecorder_SetMessageText(work, 0x12B);
            }
            work->step++;
        } else if (TouchscreenHitbox_TouchNewIsIn(&ov40_02245130) != FALSE) {
            savedVideo->exitMode = 2;
            VsRecorder_OnTouchConfirm(work);
            work->step = 6;
        }
        break;
    case 5:
        SaveGameNormal(work->saveData);
        work->step++;
        break;
    case 6:
        VsRecorder_HideTopMessage(work);
        VsRecorder_HideYesNoWindows(work);
        work->step++;
        break;
    case 7:
        VsRecorder_StepFade(&savedVideo->fade, 1);
        if (VsRecorder_StepBlend(&savedVideo->eva, &savedVideo->evb, TRUE, 2)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)savedVideo->fade, (u16)work->blendColor);
        break;
    default:
        if (savedVideo->exitMode == 1) {
            VsRecorder_SetSubState(work, 7);
        } else {
            savedVideo->exitMode = 0;
            savedVideo->showProfile = FALSE;
            VsRecorder_SetSubState(work, 2);
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

// back from playback of a video that was not saved yet: save it into its slot and reload the slots
BOOL VsRecorder_SavedVideoSave(VsRecorderWork *work) {
    switch (work->step) {
    case 0:
        VsRecorder_ShowTopMessage(work, 0x12B);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_109);
        work->step++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            work->step++;
        }
        break;
    case 2:
        if (work->battleResult == 0) {
            int result;
            BattleRecord_Load(work->saveData, HEAP_ID_109, &result, NULL, work->selection);
            VsRecorder_WriteRecord(work->saveData, work->selection);
            BattleRecord_Delete();
            VsRecorder_FreeSavedVideos(work);
            VsRecorder_LoadSavedVideos(work);
        }
        work->step++;
        break;
    default:
        ((void (*)(void))VsRecorder_HideTopMessage)(); // called without the work
        VsRecorder_CreateExitButton(work);
        VsRecorder_SetSubState(work, 9);
        break;
    }
    return FALSE;
}

// play: leave the app with the resume screen set (sub_02087A84 starts the playback)
BOOL VsRecorder_SavedVideoPlay(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;

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
        if (savedVideo->exitMode == 0) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            if (savedVideo->showProfile == FALSE) {
                VsRecorder_FreeVideoInfo(&savedVideo->info, work);
            } else {
                VsRecorder_FreeProfile(&savedVideo->profile, work);
            }
            VsRecorder_SetVBlankPaused(work, FALSE);
            TouchHitboxController_Destroy(savedVideo->hitboxes);
            VsRecorder_RemoveProfileButton(work);
            VsRecorder_FreeExitButton(work);
        }
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_SetResumeScreen(work, *work->screenPtr);
        sub_02087A84(work->result, 1, 0);
        work->step++;
        break;
    default:
        Heap_Free(savedVideo);
        return TRUE;
    }
    return FALSE;
}

BOOL VsRecorder_SavedVideoExit(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;

    switch (work->step) {
    case 0:
        TouchHitboxController_Destroy(savedVideo->hitboxes);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        if (savedVideo->showProfile == FALSE) {
            VsRecorder_FreeVideoInfo(&savedVideo->info, work);
        } else {
            VsRecorder_FreeProfile(&savedVideo->profile, work);
        }
        VsRecorder_RemoveProfileButton(work);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&savedVideo->fade, 1);
        if (VsRecorder_StepBlend(&savedVideo->eva, &savedVideo->evb, TRUE, 0)) {
            VsRecorder_FreeExitButton(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, savedVideo->fade, work->blendColor);
        break;
    case 2:
        VsRecorder_StartFlipScreens(work, 1);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    default:
        if (((u32(*)(void))VsRecorder_IsFlipScreensDone)()) { // called without the work
            if (VsRecorder_StepFade(&savedVideo->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&savedVideo->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(savedVideo);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, savedVideo->fade, work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, savedVideo->fade, work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

// back from playback: show the slot again, then return to the menu's select substate
BOOL VsRecorder_SavedVideoResume(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_LoadScreen(work, 6, GF_BG_LYR_SUB_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        work->step++;
        break;
    case 1:
        VsRecorder_SetSavedVideoSlot(work);
        savedVideo->hitboxes = TouchHitboxController_Create(ov40_02245134, 3, VsRecorder_OnSavedVideoTouch, work, HEAP_ID_109);
        work->step++;
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateVideoInfo(&savedVideo->info, work);
        VsRecorder_LoadScreen(work, ov40_02230410(&savedVideo->info), GF_BG_LYR_MAIN_3);
        VsRecorder_SetVideoPartyIconsDrawFlag(&savedVideo->info, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 3:
        VsRecorder_StepFade(&savedVideo->fade, 0);
        if (VsRecorder_StepBlend(&savedVideo->eva, &savedVideo->evb, FALSE, 0)) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&savedVideo->info, TRUE);
            VsRecorder_ShowProfileButton(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            if (savedVideo->exitMode == 1) {
                work->step = 0xFF;
            } else {
                work->step++;
            }
            savedVideo->exitMode = 0;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, savedVideo->fade, work->blendColor);
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
        VsRecorder_SetSubState(work, 3);
        break;
    }
    return FALSE;
}

void VsRecorder_OnSavedVideoTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderSavedVideo *savedVideo = work->screenWork;

    if (event != 0) {
        return;
    }
    switch (index) {
    case 0: // profile
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 4);
        break;
    case 1: // play
        VsRecorder_OnTouchConfirm(work);
        if (work->selection == 0 || BattleRecordHeader_GetField(savedVideo->info.header, 5, 0) == 1) {
            VsRecorder_SetSubState(work, 7);
        } else {
            VsRecorder_SetSubState(work, 5);
        }
        break;
    case 2: // exit
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 8);
        break;
    }
}

void VsRecorder_SetSavedVideoSlot(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    FrontierCard *owner = work->videoOwners[work->selection];
    savedVideo->profile.card = owner;
    savedVideo->info.ownerName = (const u16 *)owner; // the name is the card's first field
    savedVideo->info.header = work->savedVideos[work->selection];
}

void VsRecorder_ShowProfileButton(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    Window *window = &savedVideo->windows[0];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 8, 3, 16, 2, 0xE, 1);
    VsRecorder_PrintProfileButton(work, 0x79); // VIEW PROFILE
}

void VsRecorder_RemoveProfileButton(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    ClearWindowTilemapAndCopyToVram(&savedVideo->windows[0]);
    RemoveWindow(&savedVideo->windows[0]);
}

void VsRecorder_PrintProfileButton(VsRecorderWork *work, u32 msgId) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    Window *window = &savedVideo->windows[0];
    String *str;
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msgId);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

void VsRecorder_CreateExitButton(VsRecorderWork *work) {
    VsRecorderSavedVideo *savedVideo = work->screenWork;
    ov40_0222D6EC(work, NNS_G2D_VRAM_TYPE_2DSUB);
    savedVideo->exitButton.sprite = VsRecorder_NewButtonSprite(work, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&savedVideo->exitButton, (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_SetTextSpriteMsg(&savedVideo->exitButton, (VsRecorderGfx *)&work->narc, 3); // EXIT
    ManagedSprite_SetAnim(savedVideo->exitButton.sprite, 0);
    ManagedSprite_SetPositionXY(savedVideo->exitButton.sprite, 80, 232);
    TextOBJ_SetPosition(savedVideo->exitButton.text, 36, -8);
    TextOBJ_SetSpritesDrawFlag(savedVideo->exitButton.text, TRUE);
}
