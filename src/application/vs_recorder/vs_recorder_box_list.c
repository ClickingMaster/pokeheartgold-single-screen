#include "global.h"

#include "constants/sndseq.h"

#include "application/vs_recorder/vs_recorder_internal.h"
#include "msgdata/msg/msg_0013.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "global_terminal_net.h"
#include "heap.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "pm_string.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"

// VS Recorder box data view (VS_RECORDER_SCREEN_BOX_DATA_VIEW), after the category is chosen: receive the latest boxes
// of the category, the box list (box name and sender), the sender's profile, the chosen box, and the way back to the menu.

extern const VsRecorderListConfig sVsRecorderBoxListConfig; // box list

// view screen, substate 4: receive the boxes of the chosen category
BOOL VsRecorder_BoxDataViewDownload(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    GlobalTerminalNetError *error;

    if (!VsRecorder_IsNetIdle(work)) {
        return FALSE;
    }
    switch (work->step) {
    case 0:
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        VsRecorder_RemoveBoxDataWindows(work);
        VsRecorder_RemoveBoxDataCategoryButtons(work);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        VsRecorder_ApplyBlend(&boxData->eva, &boxData->evb, 8, 18, 8, 18, 1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        // fallthrough
    case 1:
        VsRecorder_StepFade(&boxData->fade, 1);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 0)) {
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 3, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 78, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        VsRecorder_ShowTopMessage(work, msg_0013_00289); // Receiving Box Data...
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 3:
        if (((BOOL(*)(GlobalTerminalNet *, int))GlobalTerminalNet_RequestBoxDownload)(VsRecorder_GetNet(work), boxData->category) == TRUE) {
            work->step++;
        }
        break;
    case 4:
        VsRecorder_HideTopMessage(work);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_ShowMessageScreen(work, 3, error->code, error->type);
            boxData->downloadCount = 0;
            boxData->showProfile = FALSE;
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateBoxDataWindows(work);
            OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
            OrbitCursor_SetActive(work->orbitCursors[0], FALSE);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step = 7;
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            PlaySE(SEQ_SE_PL_BREC12);
            work->step++;
        }
        break;
    case 5:
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        if (boxData->downloadCount == 0) {
            work->step++;
            work->counter = 0;
            VsRecorder_ShowTopMessage(work, msg_0013_00294); // No matching Box Data was found...
        } else {
            work->step = 0xFF;
        }
        break;
    case 6:
        work->counter++;
        if (work->counter >= 60 || System_GetTouchNew() == TRUE) {
            work->counter = 0;
            VsRecorder_HideTopMessage(work);
            boxData->showProfile = FALSE;
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateBoxDataWindows(work);
            OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
            OrbitCursor_SetActive(work->orbitCursors[0], FALSE);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step++;
        }
        break;
    case 7:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 0)) {
            break;
        }
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 79, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    case 8:
        VsRecorder_StepFade(&boxData->fade, 0);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 2)) {
            VsRecorder_LoadBoxDataMessageBg(work, GF_BG_LYR_MAIN_2);
            VsRecorder_PrintBoxDataMessage(work, msg_0013_00275); // Which category would you like to see?
            VsRecorder_CreateBoxDataCategoryButtons(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            VsRecorder_SetSubState(work, 3);
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_StepFade(&boxData->fade, 0);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 0)) {
            VsRecorder_CreateBoxListTitle(work);
            VsRecorder_CreateBoxListItems(work);
            VsRecorder_SetVBlankPaused(work, TRUE);
            boxData->listConfig = sVsRecorderBoxListConfig;
            boxData->listConfig.count = boxData->downloadCount;
            VsRecorder_CreateTouchScrollSpritesWithAnim(&work->touchScroll, work, 2);
            VsRecorder_ClearListWindow(&work->listWindow);
            VsRecorder_InitPairList(&work->listWindow, work, &boxData->listConfig, boxData->listItems[0]);
            VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
            VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DMAIN);
            VsRecorder_SetListArrowsY(&work->listWindow, 56, 176);
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_CreateBoxListButtons(work);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 16, (s16)(work->listWindow.cursorRow * 24 + 68));
            OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 12, 12);
            VsRecorder_SetSubState(work, 5);
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    }
    return FALSE;
}

// box list rows: box name and sender's name
void VsRecorder_CreateBoxListItems(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    int i;

    for (i = 0; i < boxData->downloadCount; i++) {
        boxData->listItems[i][1] = String_New(20, HEAP_ID_109);
        CopyU16ArrayToString(boxData->listItems[i][1], (u16 *)boxData->downloads[i]->header);
        VsRecorder_SanitizeName(work, boxData->listItems[i][1]);
        boxData->listItems[i][0] = String_New(30, HEAP_ID_109);
        CopyU16ArrayToString(boxData->listItems[i][0], boxData->downloads[i]->form.name);
    }
}

void VsRecorder_FreeBoxListItems(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    int i;
    String *str;

    for (i = 0; i < boxData->downloadCount; i++) {
        str = boxData->listItems[i][0];
        if (str != NULL) {
            String_Delete(str);
            boxData->listItems[i][0] = NULL;
        }
        str = boxData->listItems[i][1];
        if (str != NULL) {
            String_Delete(str);
            boxData->listItems[i][1] = NULL;
        }
    }
}

// view screen, substate 5: the box list
BOOL VsRecorder_BoxListMain(VsRecorderWork *work) {
    VsRecorderBoxData *boxData;

    boxData = work->screenWork;
    if (boxData->showProfile == FALSE) {
        VsRecorder_UpdateTouchScroll(&work->touchScroll);
        VsRecorder_ListMoveCursor(&work->listWindow, work->touchScroll.direction);
        VsRecorder_RedrawPairList(&work->listWindow, work, boxData->listItems[0]);
        OrbitCursor_MoveTarget(work->orbitCursors[0], 16, (s16)(work->listWindow.cursorRow * 24 + 68));
    }
    TouchHitboxController_IsTriggered(boxData->hitboxes[3]);
    return FALSE;
}

// view screen, substate 6: back to the categories
BOOL VsRecorder_BoxListBack(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_FreeBoxListItems(work);
        VsRecorder_RemoveBoxListButtons(work);
        if (boxData->showProfile == FALSE) {
            VsRecorder_RemoveBoxListTitle(work);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeTouchScroll(&work->touchScroll);
            VsRecorder_RemoveListWindow(&work->listWindow);
            VsRecorder_FreeListArrows(&work->listWindow, work);
            VsRecorder_SetVBlankPaused(work, FALSE);
        } else {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeProfile(&boxData->profile, work);
            VsRecorder_FreeBoxDataOwner(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
        }
        boxData->showProfile = FALSE;
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateBoxDataWindows(work);
        VsRecorder_PrintBoxDataOwnerMessage(work, 255);
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        OrbitCursor_SetActive(work->orbitCursors[0], FALSE);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        // fallthrough
    case 1:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 0)) {
            break;
        }
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 79, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    case 2:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 2)) {
            break;
        }
        VsRecorder_LoadBoxDataMessageBg(work, GF_BG_LYR_MAIN_2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        VsRecorder_PrintBoxDataMessage(work, msg_0013_00275); // Which category would you like to see?
        VsRecorder_CreateBoxDataCategoryButtons(work);
        VsRecorder_SetSubState(work, 3);
        break;
    }
    return FALSE;
}

#pragma force_active on

BOOL VsRecorder_BoxDataViewOpenBox(VsRecorderWork *work);

// view screen, substate 9: show the chosen box
BOOL VsRecorder_BoxDataViewOpenBox(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_RemoveBoxListButtons(work);
        if (boxData->showProfile == FALSE) {
            VsRecorder_RemoveBoxListTitle(work);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeTouchScroll(&work->touchScroll);
            VsRecorder_RemoveListWindow(&work->listWindow);
            VsRecorder_FreeListArrows(&work->listWindow, work);
            VsRecorder_SetVBlankPaused(work, FALSE);
        } else {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeProfile(&boxData->profile, work);
            VsRecorder_FreeBoxDataOwner(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
        }
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        OrbitCursor_SetActive(work->orbitCursors[0], FALSE);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        // fallthrough
    case 1:
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 0)) {
            work->step++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        boxData->form = boxData->downloads[work->listWindow.cursor]->form;
        VsRecorder_SelectFirstBoxMon(boxData);
        VsRecorder_LoadBoxDataMessageBg(work, GF_BG_LYR_MAIN_2);
        VsRecorder_CreateBoxDataWindows(work);
        VsRecorder_ShowBox(work);
        VsRecorder_PrintBoxDataOwnerMessage(work, 255);
        VsRecorder_SetBoxMonPicVanished(work, TRUE);
        VsRecorder_SetBoxMonIconsDrawFlag(work, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 75, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        work->step++;
        break;
    case 3:
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 0)) {
            VsRecorder_SetBoxMonPicVanished(work, FALSE);
            VsRecorder_SetBoxMonIconsDrawFlag(work, TRUE);
            VsRecorder_CreateNextMonButton(work);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            reg_G2_BLDCNT = 0;
            work->step++;
        }
        break;
    default:
        if (VsRecorder_StepFade(&boxData->fade, 0)) {
            boxData->activeHitbox = boxData->hitboxes[0];
            VsRecorder_SelectFirstBoxMon(boxData);
            boxData->cursorX = boxData->cursor % 6;
            boxData->cursorY = boxData->cursor / 6;
            VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], (s16)(boxData->cursorX * 24 + 110), (s16)(boxData->cursorY * 22 + 52));
            OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 12, 12);
            if (boxData->form.species[boxData->cursor] != 0 && VsRecorder_HasAllBits(boxData->form.eggFlags, 1 << boxData->cursor) != TRUE) {
                PlayCry(boxData->form.species[boxData->cursor], boxData->form.forms[boxData->cursor]);
            }
            VsRecorder_SetSubState(work, 10);
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    }
    return FALSE;
}

#pragma force_active reset

// view screen, substate 10: the box
BOOL VsRecorder_BoxDataViewBoxMain(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    TouchHitboxController_IsTriggered(boxData->hitboxes[4]);
    return FALSE;
}

// view screen, substate 11: back to the box list
BOOL VsRecorder_BoxDataViewCloseBox(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_ApplyBlend(&boxData->eva, &boxData->evb, 8, 18, 8, 18, 1);
        VsRecorder_FreeBoxMonPic(work);
        VsRecorder_FreeBoxMonIcons(work);
        VsRecorder_ClearBoxDataInfoWindows(work);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        VsRecorder_RemoveBoxDataWindows(work);
        VsRecorder_RemoveNextMonButton(work);
        work->step++;
        // fallthrough
    case 1:
        boxData->showProfile = FALSE;
        work->step++;
        // fallthrough
    case 2:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 0)) {
            break;
        }
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 3, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 78, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    default:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 0)) {
            break;
        }
        VsRecorder_SetVBlankPaused(work, TRUE);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        VsRecorder_CreateBoxListTitle(work);
        VsRecorder_CreateBoxListButtons(work);
        VsRecorder_CreateTouchScrollSpritesWithAnim(&work->touchScroll, work, 2);
        VsRecorder_InitPairList(&work->listWindow, work, &boxData->listConfig, boxData->listItems[0]);
        VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
        VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DMAIN);
        VsRecorder_SetListArrowsY(&work->listWindow, 56, 176);
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 16, (s16)(work->listWindow.cursorRow * 24 + 68));
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 12, 12);
        if (boxData->showProfile == FALSE) {
            VsRecorder_UpdateTouchScroll(&work->touchScroll);
            VsRecorder_ListMoveCursor(&work->listWindow, work->touchScroll.direction);
            VsRecorder_RedrawPairList(&work->listWindow, work, boxData->listItems[0]);
            OrbitCursor_MoveTarget(work->orbitCursors[0], 16, (s16)(work->listWindow.cursorRow * 24 + 68));
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        VsRecorder_SetSubState(work, 5);
        break;
    }
    return FALSE;
}

// view screen, substate 7: show the sender's profile instead of the list
BOOL VsRecorder_BoxListShowProfile(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        if (boxData->showProfile == FALSE) {
            VsRecorder_RemoveBoxListTitle(work);
        }
        VsRecorder_FreeTouchScroll(&work->touchScroll);
        VsRecorder_RemoveListWindow(&work->listWindow);
        VsRecorder_FreeListArrows(&work->listWindow, work);
        boxData->showProfile = TRUE;
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        VsRecorder_ApplyBlend(&boxData->eva, &boxData->evb, 8, 18, 8, 18, 1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        // fallthrough
    case 1:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 1)) {
            break;
        }
        VsRecorder_LoadBoxDataOwner(work, work->listWindow.cursor);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateProfile(&boxData->profile, work);
        VsRecorder_SetProfileDrawFlag(&boxData->profile, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 80, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    case 2:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 1)) {
            break;
        }
        VsRecorder_SetProfileDrawFlag(&boxData->profile, TRUE);
        VsRecorder_PrintBoxListProfileButton(work, boxData->showProfile + 121); // VIEW PROFILE / CLOSE PROFILE
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        VsRecorder_SetSubState(work, 5);
        break;
    }
    return FALSE;
}

// view screen, substate 8: back from the profile to the list
BOOL VsRecorder_BoxListHideProfile(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        boxData->showProfile = FALSE;
        work->step++;
        break;
    case 1:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeProfile(&boxData->profile, work);
        VsRecorder_FreeBoxDataOwner(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        // fallthrough
    case 2:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 1)) {
            break;
        }
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 3, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        work->step++;
        break;
    default:
        if (!VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 1)) {
            break;
        }
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_PrintBoxListProfileButton(work, boxData->showProfile + 121); // VIEW PROFILE / CLOSE PROFILE
        VsRecorder_CreateBoxListTitle(work);
        VsRecorder_CreateTouchScrollSpritesWithAnim(&work->touchScroll, work, 2);
        VsRecorder_InitPairList(&work->listWindow, work, &boxData->listConfig, boxData->listItems[0]);
        VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
        VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DMAIN);
        VsRecorder_SetListArrowsY(&work->listWindow, 56, 176);
        if (boxData->showProfile == FALSE) {
            VsRecorder_UpdateTouchScroll(&work->touchScroll);
            VsRecorder_ListMoveCursor(&work->listWindow, work->touchScroll.direction);
            VsRecorder_RedrawPairList(&work->listWindow, work, boxData->listItems[0]);
            OrbitCursor_MoveTarget(work->orbitCursors[0], 16, (s16)(work->listWindow.cursorRow * 24 + 68));
        }
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_SetSubState(work, 5);
        break;
    }
    return FALSE;
}

// view screen, substate 13: back to the menu
BOOL VsRecorder_BoxDataViewExit(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RemoveBoxDataWindows(work);
        VsRecorder_FreeBoxDataButtons(work);
        OrbitCursor_SetActive(work->orbitCursors[1], FALSE);
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        OrbitCursor_SetActive(work->orbitCursors[0], TRUE);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    case 1:
        // called without the work (the fragment's prototype had no parameter)
        if (((u32(*)(void))VsRecorder_IsFlipScreensDone)() != 0) {
            int i;
            for (i = 0; i < 5; i++) {
                TouchHitboxController_Destroy(boxData->hitboxes[i]);
            }
            VsRecorder_ResetFade(&boxData->fade);
            VsRecorder_RebuildMenuRowsParent(work);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
            work->step++;
        }
        break;
    default:
        if (VsRecorder_StepFade(&boxData->fade, 0) != FALSE) {
            VsRecorder_ClearPaletteBlend(work);
            VsRecorder_ResetFade(&boxData->fade);
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, 16, (u16)work->blendColor);
            VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
            VsRecorder_SetSubState(work, 5);
            FrontierCard_Delete(boxData->card);
            Heap_Free(boxData);
        } else {
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)boxData->fade, (u16)work->blendColor);
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        }
        break;
    }
    return FALSE;
}
