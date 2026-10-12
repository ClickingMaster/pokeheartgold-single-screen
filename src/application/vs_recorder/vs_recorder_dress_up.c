#include <string.h>

#include "global.h"

#include "constants/sndseq.h"

#include "application/dress_up/accessory_portrait.h"
#include "application/vs_recorder/vs_recorder_internal.h"
#include "msgdata/msg.naix"

#include "bg_window.h"
#include "fashion_case.h"
#include "font.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "global_terminal_net.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "pokedex.h"
#include "render_text.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_02026E30.h"
#include "vram_transfer_manager.h"

// VS Recorder dress-up photo screens: send the player's own photo (VS_RECORDER_SCREEN_DRESS_UP_SEND) and view photos
// of a chosen species from the server (VS_RECORDER_SCREEN_DRESS_UP_VIEW). The photos are drawn by the OVY_41 portrait
// renderer. Starts with the Frontier record screen's exit substate.

FS_EXTERN_OVERLAY(OVY_41);

extern const TouchscreenHitbox ov40_022451C4;    // centre bottom button (exit)
extern const TouchscreenHitbox ov40_022451C8;    // left bottom button (exit)
extern const TouchscreenHitbox ov40_022451CC;    // right bottom button (send)
extern const TouchscreenHitbox ov40_022451D0;    // top bar (photo / profile)
extern const TouchscreenHitbox ov40_022451D4;    // previous photo
extern const TouchscreenHitbox ov40_022451D8;    // next photo
extern const VsRecorderListConfig ov40_022451F4; // species list
extern const TouchscreenHitbox ov40_02245708[];  // species groups (9)
extern const u16 ov40_02245CD4[];                // first species list index of each group

BOOL VsRecorder_FrontierExit(VsRecorderWork *work) {
    VsRecorderFrontierRecord *record = work->screenWork;

    switch (work->step) {
    case 0:
        if (record->fade == 0) {
            PlaySE(SEQ_SE_PL_BREC21);
        }
        if (record->fade != 16) {
            record->fade += 2;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 28, (u8)record->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0x4018, (u8)record->fade, (u16)work->blendColor);
        if (!VsRecorder_SpinOutFrontierSprites(work)) {
            record->fade = 0;
            work->step++;
        }
        break;
    case 1:
        work->step++;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeFrontierSprites(work);
        VsRecorder_FreeFrontierButtons(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        record->fade = 0;
        work->step++;
        break;
    case 3:
        VsRecorder_SetScreensFlipped(TRUE);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 4:
        if (record->fade != 0) {
            record->fade -= 2;
            break;
        }
        work->step++;
        VsRecorder_ResetFade(&record->fade);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, (u8)record->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 28, (u8)record->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)record->fade, (u16)work->blendColor);
        break;
    default:
        if (VsRecorder_StepFade(&record->fade, 0)) {
            G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 7, 8);
            G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 7, 8);
            Heap_Free(record);
            VsRecorder_ClearPaletteBlend(work);
            VsRecorder_ResetFade(&record->fade);
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, 16, (u16)work->blendColor);
            VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
            VsRecorder_SetSubState(work, 5);
        } else {
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)record->fade, (u16)work->blendColor);
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)record->fade, (u16)work->blendColor);
        }
        break;
    }
    return FALSE;
}

// both dress-up screens, substate 0
BOOL VsRecorder_DressUpInit(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderDressUp));
    memset(dressUp, 0, sizeof(VsRecorderDressUp));
    work->screenWork = dressUp;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_41), OVY_LOAD_ASYNC);
    dressUp->portraitConfig.bgConfig = work->bgConfig;
    dressUp->portraitConfig.x = 72;
    dressUp->portraitConfig.y = 16;
    dressUp->portraitConfig.heapID = HEAP_ID_109;
    dressUp->photo = FashionEntry_New(HEAP_ID_109);
    dressUp->profile.card = FrontierCard_New(HEAP_ID_109);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
    VsRecorder_InitBgBlend(&dressUp->eva, &dressUp->evb, 0);
    VsRecorder_SetSubState(work, 1);
    return FALSE;
}

// both dress-up screens, substate 1: fade out the menu and flip the screens
BOOL VsRecorder_DressUpEnter(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;

    switch (work->step) {
    case 0:
        if (VsRecorder_StepFade(&dressUp->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    case 1:
        SetBgPriority(GF_BG_LYR_MAIN_0, 0);
        SetBgPriority(GF_BG_LYR_MAIN_2, 0);
        SetBgPriority(GF_BG_LYR_MAIN_1, 3);
        SetBgPriority(GF_BG_LYR_MAIN_3, 2);
        SetBgPriority(GF_BG_LYR_SUB_0, 0);
        SetBgPriority(GF_BG_LYR_SUB_2, 1);
        SetBgPriority(GF_BG_LYR_SUB_1, 3);
        SetBgPriority(GF_BG_LYR_SUB_3, 2);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        work->step++;
        break;
    case 2:
        VsRecorder_StartFlipScreens(work, 0);
        VsRecorder_SetVBlankPaused(work, TRUE);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeMenuRows(work);
            VsRecorder_CreateDressUpButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            VsRecorder_SetSubState(work, 2);
        }
        break;
    }
    return FALSE;
}

// send screen, substate 2: show the player's own photo
BOOL VsRecorder_DressUpSendShow(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetPhotoWindow();
        VsRecorder_LoadScreen(work, 35, GF_BG_LYR_MAIN_3);
        VsRecorder_LoadScreen(work, 37, GF_BG_LYR_SUB_3);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        VsRecorder_SetVBlankPaused(work, TRUE);
        dressUp->ownPhoto = Save_FashionData_GetEntry(Save_FashionData_Get(work->saveData), 0);
        if (dressUp->portrait == NULL) {
            dressUp->portrait = AccessoryPortrait_Create(&dressUp->portraitConfig, dressUp->ownPhoto);
            AccessoryPortrait_SetMonVisible(dressUp->portrait, TRUE);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        Main_SetVBlankIntrCB(VsRecorder_DressUpVBlankCB, work);
        work->step++;
        break;
    case 1:
        PaletteData_LoadPaletteSlotFromHardware(work->plttData, PLTTBUF_MAIN_BG, 0, 0x200);
        work->step++;
        break;
    case 2:
        VsRecorder_StepFade(&dressUp->fade, 0);
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, FALSE, 0)) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            VsRecorder_PrintOwnPhotoCaption(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, 3);
        break;
    }
    if (dressUp->portrait != NULL) {
        Thunk_G3X_Reset();
        AccessoryPortrait_Draw(dressUp->portrait);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    return FALSE;
}

// send screen, substate 3: wait for EXIT or SEND
BOOL VsRecorder_DressUpSendMain(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451C8)) {
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 5);
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451CC)) {
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 4);
    }
    if (dressUp->portrait != NULL) {
        Thunk_G3X_Reset();
        AccessoryPortrait_Draw(dressUp->portrait);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    return FALSE;
}

#pragma force_active on

// send screen, substate 4: upload the photo, then return to the menu
BOOL VsRecorder_DressUpSendUpload(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    GlobalTerminalNetError *error;

    if (VsRecorder_IsNetIdle(work) == FALSE) {
        return FALSE;
    }
    switch (work->step) {
    case 0:
        SetBgPriority(GF_BG_LYR_MAIN_0, 1);
        SetBgPriority(GF_BG_LYR_MAIN_1, 3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 2);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_SUB_0, 1);
        SetBgPriority(GF_BG_LYR_SUB_1, 3);
        SetBgPriority(GF_BG_LYR_SUB_2, 2);
        SetBgPriority(GF_BG_LYR_SUB_3, 1);
        Heap_Free(dressUp->photo);
        FrontierCard_Delete(dressUp->profile.card);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        VsRecorder_RemovePhotoCaption(work);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&dressUp->fade, 1);
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 0)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeDressUpButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
            BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
            SetBgPriority(GF_BG_LYR_MAIN_2, 0);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        VsRecorder_ShowTopMessage(work, 286);
        dressUp->uploadCard = FrontierCard_New(HEAP_ID_109);
        FrontierCard_InitFromSave(dressUp->uploadCard, work->saveData);
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 3:
        if (GlobalTerminalNet_RequestDressUpUpload(VsRecorder_GetNet(work), dressUp->uploadCard, (GlobalTerminalNetDressUpRecord *)dressUp->ownPhoto) == TRUE) {
            FrontierCard_Delete(dressUp->uploadCard);
            work->step++;
        }
        break;
    case 4:
        VsRecorder_HideTopMessage(work);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_ShowMessageScreen(work, 0, error->code, error->type);
            work->step++;
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_AddGameStatsScore(work, 36);
            PlaySE(SEQ_SE_PL_BREC12);
            work->step++;
        }
        break;
    case 5:
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        work->step++;
        break;
    case 6:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_ResetFade(&dressUp->fade);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&dressUp->fade, 0)) {
                if (dressUp->portrait) {
                    AccessoryPortrait_Destroy(dressUp->portrait);
                    VsRecorder_ResetBg2(work);
                    SetBgPriority(GF_BG_LYR_MAIN_2, 0);
                    VsRecorder_DisablePhotoWindow();
                }
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&dressUp->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(dressUp);
                UnloadOverlayByID(FS_OVERLAY_ID(OVY_41));
                Main_SetVBlankIntrCB(VsRecorder_VBlankCB, work);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)dressUp->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

// send screen, substate 5: back to the menu without sending
BOOL VsRecorder_DressUpSendExit(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;

    switch (work->step) {
    case 0:
        SetBgPriority(GF_BG_LYR_MAIN_0, 1);
        SetBgPriority(GF_BG_LYR_MAIN_1, 3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 2);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_SUB_0, 1);
        SetBgPriority(GF_BG_LYR_SUB_1, 3);
        SetBgPriority(GF_BG_LYR_SUB_2, 2);
        SetBgPriority(GF_BG_LYR_SUB_3, 1);
        Heap_Free(dressUp->photo);
        FrontierCard_Delete(dressUp->profile.card);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        VsRecorder_RemovePhotoCaption(work);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&dressUp->fade, 1);
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 0) != FALSE) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeDressUpButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_ResetFade(&dressUp->fade);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work) != FALSE) {
            if (VsRecorder_StepFade(&dressUp->fade, 0) != FALSE) {
                VsRecorder_SetVBlankPaused(work, TRUE);
                if (dressUp->portrait != NULL) {
                    AccessoryPortrait_Destroy(dressUp->portrait);
                    VsRecorder_ResetBg2(work);
                    SetBgPriority(GF_BG_LYR_MAIN_2, 0);
                    VsRecorder_DisablePhotoWindow();
                }
                VsRecorder_SetVBlankPaused(work, FALSE);
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&dressUp->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(dressUp);
                UnloadOverlayByID(FS_OVERLAY_ID(OVY_41));
                Main_SetVBlankIntrCB(VsRecorder_VBlankCB, work);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)dressUp->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

// view screen, substate 2: EXIT only
BOOL VsRecorder_DressUpViewInit(VsRecorderWork *work) {
    if (work->step == 0) {
        VsRecorder_SetDressUpSendButtonHidden(work, TRUE);
        work->step = work->step + 1;
    } else {
        VsRecorder_SetSubState(work, 3);
    }
    return FALSE;
}

// view screen, substate 3: choose a species group
BOOL VsRecorder_DressUpViewSelectCategory(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    switch (work->step) {
    case 0:
        VsRecorder_LoadScreen(work, 60, GF_BG_LYR_SUB_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        dressUp->hitboxes = TouchHitboxController_Create(ov40_02245708, 9, VsRecorder_OnDressUpCategoryTouch, work, HEAP_ID_109);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&dressUp->fade, 0);
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, FALSE, 0)) {
            VsRecorder_ShowTopMessage(work, 114);
            VsRecorder_CreateDressUpCategoryWindows(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    case 2:
        TouchHitboxController_IsTriggered(dressUp->hitboxes);
        if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451C4) || dressUp->categoryChosen == TRUE) {
            VsRecorder_OnTouchConfirm(work);
            work->step++;
        }
        break;
    case 3:
        ((void (*)(void))VsRecorder_RemoveDressUpCategoryWindows)();
        TouchHitboxController_Destroy(dressUp->hitboxes);
        VsRecorder_HideTopMessage(work);
        work->step++;
        // fallthrough
    case 4:
        if (dressUp->categoryChosen == TRUE) {
            if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 2)) {
                work->step++;
            }
        } else {
            VsRecorder_StepFade(&dressUp->fade, 1);
            if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 2)) {
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        }
        break;
    default:
        if (dressUp->categoryChosen == TRUE) {
            VsRecorder_SetSubState(work, 4);
        } else {
            VsRecorder_SetSubState(work, 5);
        }
        break;
    }
    return FALSE;
}

// view screen, substate 4: choose a species of the group
BOOL VsRecorder_DressUpViewSelectSpecies(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    switch (work->step) {
    case 0:
        VsRecorder_SetDressUpSendButtonHidden(work, TRUE);
        VsRecorder_LoadScreen(work, 58, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, FALSE, 2)) {
            VsRecorder_ShowTopMessage(work, 114);
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateTouchScrollSprites(&work->touchScroll, work);
            dressUp->listConfig = ov40_022451F4;
            dressUp->listConfig.entries = dressUp->entries;
            dressUp->listConfig.count = dressUp->speciesCount;
            VsRecorder_ClearListWindow(&work->listWindow);
            VsRecorder_InitList(&work->listWindow, work, dressUp->speciesNames, &dressUp->listConfig);
            VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
            VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DSUB);
            VsRecorder_SetVBlankPaused(work, FALSE);
            dressUp->species = 0;
            work->step++;
        }
        break;
    case 2: {
        int species;
        VsRecorder_UpdateTouchScroll(&work->touchScroll);
        VsRecorder_ListScroll(&work->listWindow, work->touchScroll.direction);
        species = VsRecorder_UpdateMessageList(&work->listWindow, work);
        if (species != 0) {
            dressUp->species = species;
            VsRecorder_OnTouchConfirm(work);
            work->step++;
        }
        if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451C4)) {
            VsRecorder_OnTouchConfirm(work);
            work->step++;
        }
        break;
    }
    case 3:
        VsRecorder_HideTopMessage(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeTouchScroll(&work->touchScroll);
        VsRecorder_RemoveListWindow(&work->listWindow);
        VsRecorder_FreeListArrows(&work->listWindow, work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        // fallthrough
    case 4:
        VsRecorder_StepFade(&dressUp->fade, 1);
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_FreeDressUpSpeciesList(work);
        if (dressUp->species != 0) {
            VsRecorder_SetSubState(work, 6);
        } else {
            VsRecorder_SetSubState(work, 3);
        }
        break;
    }
    return FALSE;
}

// view screen, substate 5: back to the menu
BOOL VsRecorder_DressUpViewExit(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    switch (work->step) {
    case 0:
        SetBgPriority(GF_BG_LYR_MAIN_0, 1);
        SetBgPriority(GF_BG_LYR_MAIN_1, 3);
        SetBgPriority(GF_BG_LYR_MAIN_2, 2);
        SetBgPriority(GF_BG_LYR_MAIN_3, 1);
        SetBgPriority(GF_BG_LYR_SUB_0, 1);
        SetBgPriority(GF_BG_LYR_SUB_1, 3);
        SetBgPriority(GF_BG_LYR_SUB_2, 2);
        SetBgPriority(GF_BG_LYR_SUB_3, 1);
        Heap_Free(dressUp->photo);
        FrontierCard_Delete(dressUp->profile.card);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&dressUp->fade, 1);
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 0)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeDressUpButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_ResetFade(&dressUp->fade);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&dressUp->fade, 0)) {
                VsRecorder_SetVBlankPaused(work, TRUE);
                if (dressUp->portrait != NULL) {
                    AccessoryPortrait_Destroy(dressUp->portrait);
                    VsRecorder_ResetBg2(work);
                    SetBgPriority(GF_BG_LYR_MAIN_2, 0);
                    VsRecorder_DisablePhotoWindow();
                }
                VsRecorder_SetVBlankPaused(work, FALSE);
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&dressUp->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                Heap_Free(dressUp);
                UnloadOverlayByID(FS_OVERLAY_ID(OVY_41));
                Main_SetVBlankIntrCB(VsRecorder_VBlankCB, work);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)dressUp->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

// view screen, substate 6: download the photos of the chosen species
BOOL VsRecorder_DressUpViewDownload(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    GlobalTerminalNetError *error;
    if (!VsRecorder_IsNetIdle(work)) {
        return FALSE;
    }
    switch (work->step) {
    case 0:
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        VsRecorder_ShowTopMessage(work, 287);
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 1:
        // called with the int species (no u16 truncation)
        if (((BOOL(*)(GlobalTerminalNet *, int))GlobalTerminalNet_RequestDressUpDownload)(VsRecorder_GetNet(work), dressUp->species) == TRUE) {
            work->step++;
        }
        break;
    case 2:
        VsRecorder_HideTopMessage(work);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_ShowMessageScreen(work, 1, error->code, error->type);
            dressUp->downloadCount = 0;
            work->counter = 0;
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            VsRecorder_SetSubState(work, 3);
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            PlaySE(SEQ_SE_PL_BREC12);
            work->step++;
        }
        break;
    case 3:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        work->step++;
        break;
    case 4:
        if (dressUp->downloadCount != 0) {
            dressUp->photoIndex = 0;
            dressUp->unk_230 = 0;
            VsRecorder_SetSubState(work, 7);
        } else {
            VsRecorder_ShowTopMessage(work, 293); // no photos
            work->step++;
            work->counter = 0;
        }
        break;
    default:
        work->counter++;
        if (work->counter >= 60 || System_GetTouchNew() == TRUE) {
            work->counter = 0;
            VsRecorder_HideTopMessage(work);
            VsRecorder_SetSubState(work, 3);
        }
        break;
    }
    return FALSE;
}

// view screen, substate 7: show the current downloaded photo
BOOL VsRecorder_DressUpViewShowPhoto(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;

    switch (work->step) {
    case 0:
        dressUp->showProfile = FALSE;
        {
            VsRecorderDressUpDownload *download = dressUp->downloads[dressUp->photoIndex];
            GlobalTerminalNet_UnpackDressUpPhoto(&download->form, (GlobalTerminalNetDressUpRecord *)dressUp->photo);
            dressUp->portrait = AccessoryPortrait_Create(&dressUp->portraitConfig, dressUp->photo);
        }
        AccessoryPortrait_SetMonVisible(dressUp->portrait, FALSE);
        VsRecorder_SetPhotoWindow();
        VsRecorder_LoadScreen(work, 35, GF_BG_LYR_MAIN_3);
        VsRecorder_LoadScreen(work, 36, GF_BG_LYR_SUB_3);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        Main_SetVBlankIntrCB(VsRecorder_DressUpVBlankCB, work);
        work->step++;
        break;
    case 1:
        PaletteData_LoadPaletteSlotFromHardware(work->plttData, PLTTBUF_MAIN_BG, 0, 0x200);
        work->step++;
        break;
    case 2:
        VsRecorder_StepFade(&dressUp->fade, 0);
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, FALSE, 0)) {
            AccessoryPortrait_SetMonVisible(dressUp->portrait, TRUE);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            VsRecorder_PrintPhotoCaption(work, dressUp->photoIndex);
            VsRecorder_CreateDressUpToggleWindow(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, 8);
        break;
    }
    if (dressUp->portrait != NULL) {
        Thunk_G3X_Reset();
        AccessoryPortrait_Draw(dressUp->portrait);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    return FALSE;
}

// view screen, substate 8: EXIT, photo / profile toggle, previous / next photo
BOOL VsRecorder_DressUpViewMain(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;

    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451C4)) {
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 9);
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451D0)) {
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 10);
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451D4)) {
        if (dressUp->photoIndex != 0) {
            dressUp->photoIndex--;
        } else {
            dressUp->photoIndex = dressUp->downloadCount - 1;
        }
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 11);
    }
    if (TouchscreenHitbox_TouchNewIsIn(&ov40_022451D8)) {
        dressUp->photoIndex++;
        dressUp->photoIndex %= dressUp->downloadCount;
        VsRecorder_OnTouchConfirm(work);
        VsRecorder_SetSubState(work, 11);
    }
    if (dressUp->portrait != NULL) {
        Thunk_G3X_Reset();
        AccessoryPortrait_Draw(dressUp->portrait);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    return FALSE;
}

// view screen, substate 9: close the photo, back to the species groups
BOOL VsRecorder_DressUpViewClosePhoto(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;

    switch (work->step) {
    case 0:
        if (dressUp->showProfile == FALSE) {
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            VsRecorder_RemovePhotoCaption(work);
        } else {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeProfile(&dressUp->profile, work);
            VsRecorder_SetVBlankPaused(work, FALSE);
        }
        VsRecorder_RemoveDressUpToggleWindow(work);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&dressUp->fade, 1);
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 0)) {
            if (dressUp->portrait != NULL) {
                AccessoryPortrait_Destroy(dressUp->portrait);
                VsRecorder_ResetBg2(work);
                SetBgPriority(GF_BG_LYR_MAIN_2, 0);
                dressUp->portrait = NULL;
                VsRecorder_DisablePhotoWindow();
            }
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)dressUp->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, 3);
        break;
    }
    return FALSE;
}

// view screen, substate 10: switch between the photo and the sender's profile
BOOL VsRecorder_DressUpViewToggleProfile(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;

    switch (work->step) {
    case 0:
        if (dressUp->showProfile == FALSE) {
            VsRecorder_RemovePhotoCaption(work);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        } else {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeProfile(&dressUp->profile, work);
            VsRecorder_SetVBlankPaused(work, FALSE);
        }
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 1)) {
            if (dressUp->showProfile == FALSE) {
                if (dressUp->portrait != NULL) {
                    AccessoryPortrait_Destroy(dressUp->portrait);
                    VsRecorder_ResetBg2(work);
                    SetBgPriority(GF_BG_LYR_MAIN_2, 0);
                    dressUp->portrait = NULL;
                    VsRecorder_DisablePhotoWindow();
                }
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            }
            work->step++;
        }
        break;
    case 2:
        if (dressUp->showProfile == FALSE) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            GlobalTerminalNet_CopyRequestHeader(dressUp->downloads[dressUp->photoIndex], dressUp->profile.card);
            VsRecorder_CreateProfile(&dressUp->profile, work);
            VsRecorder_SetProfileDrawFlag(&dressUp->profile, FALSE);
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_LoadScreen(work, 80, GF_BG_LYR_MAIN_3);
        } else {
            GlobalTerminalNet_UnpackDressUpPhoto(&dressUp->downloads[dressUp->photoIndex]->form, (GlobalTerminalNetDressUpRecord *)dressUp->photo);
            dressUp->portrait = AccessoryPortrait_Create(&dressUp->portraitConfig, dressUp->photo);
            AccessoryPortrait_SetMonVisible(dressUp->portrait, FALSE);
            VsRecorder_SetPhotoWindow();
            VsRecorder_LoadScreen(work, 35, GF_BG_LYR_MAIN_3);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        }
        work->step++;
        break;
    case 3:
        if (dressUp->showProfile != FALSE) {
            PaletteData_LoadPaletteSlotFromHardware(work->plttData, PLTTBUF_MAIN_BG, 0, 0x200);
        }
        work->step++;
        break;
    case 4:
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, FALSE, 1)) {
            if (dressUp->showProfile == FALSE) {
                VsRecorder_SetProfileDrawFlag(&dressUp->profile, TRUE);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            } else {
                VsRecorder_PrintPhotoCaption(work, dressUp->photoIndex);
                AccessoryPortrait_SetMonVisible(dressUp->portrait, TRUE);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            }
            work->step++;
        }
        break;
    default:
        dressUp->showProfile ^= TRUE;
        VsRecorder_PrintDressUpToggleLabel(work, dressUp->showProfile + 121);
        VsRecorder_SetSubState(work, 8);
        break;
    }
    return FALSE;
}

// view screen, substate 11: switch to the previous / next photo (photoIndex already moved)
BOOL VsRecorder_DressUpViewChangePhoto(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;

    switch (work->step) {
    case 0:
        if (dressUp->showProfile == FALSE) {
            VsRecorder_RemovePhotoCaption(work);
            BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        } else {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeProfile(&dressUp->profile, work);
            VsRecorder_SetVBlankPaused(work, FALSE);
        }
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, TRUE, 1)) {
            if (dressUp->showProfile == FALSE) {
                if (dressUp->portrait != NULL) {
                    AccessoryPortrait_Destroy(dressUp->portrait);
                    VsRecorder_ResetBg2(work);
                    SetBgPriority(GF_BG_LYR_MAIN_2, 0);
                    dressUp->portrait = NULL;
                    VsRecorder_DisablePhotoWindow();
                }
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            }
            work->step++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (dressUp->showProfile == FALSE) {
            GlobalTerminalNet_UnpackDressUpPhoto(&dressUp->downloads[dressUp->photoIndex]->form, (GlobalTerminalNetDressUpRecord *)dressUp->photo);
            dressUp->portrait = AccessoryPortrait_Create(&dressUp->portraitConfig, dressUp->photo);
            AccessoryPortrait_SetMonVisible(dressUp->portrait, FALSE);
            VsRecorder_SetPhotoWindow();
            VsRecorder_LoadScreen(work, 35, GF_BG_LYR_MAIN_3);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_OFF);
        } else {
            GlobalTerminalNet_CopyRequestHeader(dressUp->downloads[dressUp->photoIndex], dressUp->profile.card);
            VsRecorder_CreateProfile(&dressUp->profile, work);
            VsRecorder_SetProfileDrawFlag(&dressUp->profile, FALSE);
            VsRecorder_LoadScreen(work, 80, GF_BG_LYR_MAIN_3);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        break;
    case 3:
        if (dressUp->showProfile == FALSE) {
            PaletteData_LoadPaletteSlotFromHardware(work->plttData, PLTTBUF_MAIN_BG, 0, 0x200);
        }
        work->step++;
        break;
    case 4:
        if (VsRecorder_StepBlend(&dressUp->eva, &dressUp->evb, FALSE, 1)) {
            if (dressUp->showProfile == FALSE) {
                VsRecorder_PrintPhotoCaption(work, dressUp->photoIndex);
                AccessoryPortrait_SetMonVisible(dressUp->portrait, TRUE);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            } else {
                VsRecorder_SetProfileDrawFlag(&dressUp->profile, TRUE);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            }
            work->step++;
        }
        break;
    default:
        VsRecorder_SetSubState(work, 8);
        break;
    }
    return FALSE;
}

void VsRecorder_RemoveDressUpToggleWindow(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    ClearWindowTilemapAndCopyToVram(&dressUp->windows[0]);
    RemoveWindow(&dressUp->windows[0]);
}

void VsRecorder_PrintDressUpToggleLabel(VsRecorderWork *work, u32 msgId) {
    Window *window = &((VsRecorderDressUp *)work->screenWork)->windows[0];
    String *str;
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msgId);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

void VsRecorder_CreateDressUpToggleWindow(VsRecorderWork *work) {
    Window *window = &((VsRecorderDressUp *)work->screenWork)->windows[0];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 8, 3, 16, 2, 14, 1);
    VsRecorder_PrintDressUpToggleLabel(work, 121); // the photo is shown
}

void VsRecorder_DressUpVBlankCB(void *data) {
    vu32 *dtcm = (vu32 *)0x027E0000; // DTCM as a constant, for OS_SetIrqCheckFlag below
    VsRecorderWork *work = data;
    if (((VsRecorderDressUp *)work->screenWork)->portrait != NULL) {
        AccessoryPortrait_VBlank(((VsRecorderDressUp *)work->screenWork)->portrait);
    }
    GF_RunVramTransferTasks();
    PaletteData_PushTransparentBuffers(work->plttData);
    DoScheduledBgGpuUpdates(work->bgConfig);
    dtcm[(HW_INTR_CHECK_BUF - HW_DTCM) / 4] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
}

// window 0 around the photo: everything inside, BG0 (the frame) hidden outside
void VsRecorder_SetPhotoWindow(void) {
    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, TRUE);
    G2_SetWnd0Position(72, 16, 184, 144);
}

void VsRecorder_DisablePhotoWindow(void) {
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
}

// send screen: EXIT on the left and SEND on the right; view screens: SEND hidden, EXIT centred
void VsRecorder_SetDressUpSendButtonHidden(VsRecorderWork *work, BOOL hidden) {
    VsRecorderDressUp *dressUp = work->screenWork;
    if (hidden == FALSE) {
        ManagedSprite_SetDrawFlag(dressUp->sendButton.sprite, TRUE);
        TextOBJ_SetSpritesDrawFlag(dressUp->sendButton.text, TRUE);
        ManagedSprite_SetPositionXY(dressUp->exitButton.sprite, 32, 232);
    } else {
        ManagedSprite_SetDrawFlag(dressUp->sendButton.sprite, FALSE);
        TextOBJ_SetSpritesDrawFlag(dressUp->sendButton.text, FALSE);
        ManagedSprite_SetPositionXY(dressUp->exitButton.sprite, 80, 232);
    }
    TextOBJ_SetPosition(dressUp->exitButton.text, 36, -8);
    TextOBJ_SetPosition(dressUp->sendButton.text, 36, -8);
}

#pragma force_active on

void VsRecorder_CreateDressUpButtons(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    ov40_0222D6EC(work, NNS_G2D_VRAM_TYPE_2DSUB);
    dressUp->exitButton.sprite = ov40_0222D800(work, NNS_G2D_VRAM_TYPE_2DSUB);
    dressUp->sendButton.sprite = ov40_0222D800(work, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&dressUp->exitButton, (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateTextSprite(&dressUp->sendButton, (VsRecorderGfx *)&work->narc, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_SetTextSpriteMsg(&dressUp->exitButton, (VsRecorderGfx *)&work->narc, 3);  // EXIT
    VsRecorder_SetTextSpriteMsg(&dressUp->sendButton, (VsRecorderGfx *)&work->narc, 53); // SEND
    ManagedSprite_SetAnim(dressUp->exitButton.sprite, 0);
    ManagedSprite_SetAnim(dressUp->sendButton.sprite, 4);
    ManagedSprite_SetPositionXY(dressUp->exitButton.sprite, 32, 232);
    ManagedSprite_SetPositionXY(dressUp->sendButton.sprite, 128, 232);
    TextOBJ_SetPosition(dressUp->exitButton.text, 36, -8);
    TextOBJ_SetPosition(dressUp->sendButton.text, 36, -8);
    TextOBJ_SetSpritesDrawFlag(dressUp->exitButton.text, TRUE);
    TextOBJ_SetSpritesDrawFlag(dressUp->sendButton.text, TRUE);
    VsRecorder_SetDressUpSendButtonHidden(work, FALSE);
}

#pragma force_active reset

void VsRecorder_FreeDressUpButtons(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    VsRecorder_FreeTextSprite(&dressUp->exitButton);
    VsRecorder_FreeTextSprite(&dressUp->sendButton);
    Sprite_DeleteAndFreeResources(dressUp->exitButton.sprite);
    Sprite_DeleteAndFreeResources(dressUp->sendButton.sprite);
    ov40_0222D7DC(work);
}

// caption under the player's own photo: the Pokemon's name and the photo's phrase
void VsRecorder_PrintOwnPhotoCaption(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    Window *window;
    String *formatted;
    String *template;
    MessageFormat *msgFormat;
    String *line;
    u16 word;
    u32 lineCount;
    u32 i;
    u32 y;

    if (WindowIsInUse(&dressUp->caption) == TRUE) {
        return;
    }
    window = &dressUp->caption;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 16, 19, 16, 4, 14, 300);
    FillWindowPixelBuffer(window, 0);
    msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
    formatted = String_New(255, HEAP_ID_109);
    template = NewString_ReadMsgData(work->msgData, 56);
    line = String_New(255, HEAP_ID_109);
    FashionEntry_GetMonSecondText(dressUp->ownPhoto, line);
    VsRecorder_SanitizeName(work, line);
    word = FashionEntry_GetMailMessageWord(dressUp->ownPhoto);
    BufferString(msgFormat, 0, line, 0, 1, 2);
    BufferECWord(msgFormat, 1, word);
    StringExpandPlaceholders(msgFormat, formatted, template);
    lineCount = String_CountLines(formatted);
    for (i = 0; i < lineCount; i++) {
        y = i * 16;
        u32 x;
        String_GetLineN(line, formatted, i);
        x = FontID_String_GetWidth(0, line, 0);
        x = (128 - x) >> 1;
        AddTextPrinterParameterizedWithColor(window, 0, line, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    }
    ScheduleWindowCopyToVram(window);
    String_Delete(line);
    String_Delete(template);
    String_Delete(formatted);
    MessageFormat_Delete(msgFormat);
}

// the same for the downloaded photo (index: unused, the photo is already unpacked)
void VsRecorder_PrintPhotoCaption(VsRecorderWork *work, int index) {
    VsRecorderDressUp *dressUp = work->screenWork;
    Window *window;
    String *formatted;
    String *template;
    MessageFormat *msgFormat;
    String *line;
    u16 word;
    u32 lineCount;
    u32 i;
    u32 y;

    if (WindowIsInUse(&dressUp->caption) == TRUE) {
        return;
    }
    window = &dressUp->caption;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 16, 19, 16, 4, 14, 300);
    FillWindowPixelBuffer(window, 0);
    msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
    formatted = String_New(255, HEAP_ID_109);
    template = NewString_ReadMsgData(work->msgData, 56);
    line = String_New(255, HEAP_ID_109);
    FashionEntry_GetMonSecondText(dressUp->photo, line);
    VsRecorder_SanitizeName(work, line);
    word = FashionEntry_GetMailMessageWord(dressUp->photo);
    BufferString(msgFormat, 0, line, 0, 1, 2);
    BufferECWord(msgFormat, 1, word);
    StringExpandPlaceholders(msgFormat, formatted, template);
    lineCount = String_CountLines(formatted);
    for (i = 0; i < lineCount; i++) {
        y = i * 16;
        u32 x;
        String_GetLineN(line, formatted, i);
        x = FontID_String_GetWidth(0, line, 0);
        x = (128 - x) >> 1;
        AddTextPrinterParameterizedWithColor(window, 0, line, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    }
    ScheduleWindowCopyToVram(window);
    String_Delete(line);
    String_Delete(template);
    String_Delete(formatted);
    MessageFormat_Delete(msgFormat);
}

#pragma force_active on

// TRUE if the player has seen a species of the group
BOOL VsRecorder_DressUpCategoryHasSeen(VsRecorderWork *work, int category) {
    BOOL found = FALSE;
    int end = ov40_02245CD4[category + 1];
    int start = ov40_02245CD4[category];
    int count, i;
    u16 *list = ov40_0222DD68(HEAP_ID_109, found, &count);
    Pokedex *pokedex = Save_Pokedex_Get(work->saveData);
    for (i = start; i < end; i++) {
        if (!Pokedex_CheckMonSeenFlag(pokedex, list[i])) {
            list[i] = 0xFFFF;
        }
    }
    for (; start < end; start++) {
        if (list[start] != 0xFFFF) {
            found = TRUE;
            break;
        }
    }
    Heap_Free(list);
    return found;
}

// the seen species of the group first, then the rest of the list; entries for the list window
void VsRecorder_BuildDressUpSpeciesList(VsRecorderWork *work, int category) {
    VsRecorderDressUp *dressUp = work->screenWork;
    int end = ov40_02245CD4[category + 1];
    u16 *tmp;
    Pokedex *pokedex;
    int start = ov40_02245CD4[category];
    int i;

    dressUp->speciesList = ov40_0222DD68(HEAP_ID_109, 0, &dressUp->listSize);
    tmp = ov40_0222DD68(HEAP_ID_109, 0, &dressUp->listSize);
    pokedex = Save_Pokedex_Get(work->saveData);
    dressUp->speciesCount = 0;
    for (i = start; i < end; i++) {
        if (!Pokedex_CheckMonSeenFlag(pokedex, tmp[i])) {
            tmp[i] = 0xFFFF;
        } else {
            dressUp->speciesCount++;
        }
    }
    i = 0;
    for (; start < end; start++) {
        if (tmp[start] != 0xFFFF) {
            dressUp->speciesList[i] = tmp[start];
            i++;
        }
    }
    for (; i < dressUp->listSize; i++) {
        dressUp->speciesList[i] = tmp[i];
    }
    Heap_Free(tmp);
    dressUp->entries = Heap_Alloc(HEAP_ID_109, dressUp->speciesCount * sizeof(VsRecorderListEntry));
    MI_CpuFill8(dressUp->entries, 0, dressUp->speciesCount * sizeof(VsRecorderListEntry));
    for (i = 0; i < dressUp->speciesCount; i++) {
        dressUp->entries[i].msgId = dressUp->speciesList[i];
        dressUp->entries[i].value = dressUp->speciesList[i];
    }
    dressUp->speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_109);
    dressUp->categoryChosen = TRUE;
}

#pragma force_active reset

void VsRecorder_FreeDressUpSpeciesList(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    DestroyMsgData(dressUp->speciesNames);
    Heap_Free(dressUp->entries);
    Heap_Free(dressUp->speciesList);
    dressUp->categoryChosen = FALSE;
}

void VsRecorder_OnDressUpCategoryTouch(u32 index, u32 event, void *arg) {
    if (event != 0) {
        return;
    }
    if (!VsRecorder_DressUpCategoryHasSeen(arg, index)) {
        PlaySE(SEQ_SE_PL_BREC58);
        return;
    }
    VsRecorder_BuildDressUpSpeciesList(arg, index);
}

#pragma force_active on

// one window per species group, grayed out if no species of it was seen
void VsRecorder_CreateDressUpCategoryWindows(VsRecorderWork *work) {
    u32 textX;
    String *string;
    int baseTile;
    u8 x, y, width;
    int textY;
    u8 height;
    u32 i;
    Window *window;
    const TouchscreenHitbox *hitbox;

    baseTile = 1;
    hitbox = ov40_02245708;
    for (i = 0, window = ((VsRecorderDressUp *)work->screenWork)->windows; i < 9; i++) {
        x = hitbox->rect.left >> 3;
        y = hitbox->rect.top >> 3;
        width = ((u32)hitbox->rect.right >> 3) - x;
        height = ((u32)hitbox->rect.bottom >> 3) - y;
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, x, y, width, height, 14, baseTile);
        FillWindowPixelBuffer(window, 0);
        string = NewString_ReadMsgData(work->msgData, i + 68);
        textX = VsRecorder_GetCenteredTextX(window, string);
        textY = (height * 8 - 16) / 2;
        if (VsRecorder_DressUpCategoryHasSeen(work, i) == TRUE) {
            AddTextPrinterParameterizedWithColor(window, 0, string, textX, textY, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        } else {
            AddTextPrinterParameterizedWithColor(window, 0, string, textX, textY, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(12, 11, 0), NULL);
        }
        ScheduleWindowCopyToVram(window);
        String_Delete(string);
        baseTile += width * height;
        window++;
        hitbox++;
    }
}

#pragma force_active reset

void VsRecorder_RemoveDressUpCategoryWindows(VsRecorderWork *work) {
    u32 i;
    Window *window = ((VsRecorderDressUp *)work->screenWork)->windows;
    for (i = 0; i < 9; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
}

void VsRecorder_RemovePhotoCaption(VsRecorderWork *work) {
    VsRecorderDressUp *dressUp = work->screenWork;
    if (WindowIsInUse(&dressUp->caption) == TRUE) {
        ClearWindowTilemapAndCopyToVram(&dressUp->caption);
        RemoveWindow(&dressUp->caption);
    }
}
