#include <string.h>

#include "global.h"

#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "bg_window.h"
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
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokemon_storage_system.h"
#include "pokepic.h"
#include "save_arrays.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "text.h"
#include "touch_hitbox_controller.h"

// VS Recorder box data screens: send a box of the player's PC to the server (VS_RECORDER_SCREEN_BOX_DATA_SEND) and view
// the latest boxes of a category from the server (VS_RECORDER_SCREEN_BOX_DATA_VIEW). Starts with the dress-up photo
// screens' server response handlers.

// window position and size in tiles
typedef struct VsRecorderWindowRect {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} VsRecorderWindowRect;

typedef struct VsRecorderMsgIdPair {
    u32 msgIds[2];
} VsRecorderMsgIdPair;

extern const VsRecorderWindowRect ov40_02245268[]; // box list title; the box list buttons' message ids follow at +0x14
extern const TouchscreenHitbox sVsRecorderBoxDataHitboxes[];    // box: next Pokemon, back
extern const VsRecorderWindowRect sVsRecorderBoxListButtonWindows[]; // box list buttons (2)
extern const TouchscreenHitbox sVsRecorderBoxListHitboxes[];    // box list: profile button, list, back
extern const u32 sVsRecorderBoxDataWindowFillColors[];                  // fill colours of VsRecorderBoxData.windows
extern const VsRecorderWindowRect sVsRecorderBoxDataWindows[]; // VsRecorderBoxData.windows
extern const TouchscreenHitbox sVsRecorderBoxDataSendHitboxes[];    // send: previous box, next box, exit, send
extern const VsRecorderWindowRect sVsRecorderBoxDataCategoryButtonWindows[]; // category buttons (6)
extern const TouchscreenHitbox sVsRecorderBoxDataCategoryHitboxes[];    // categories (6) and cancel

// ---- dress-up photo screens: server responses

void VsRecorder_OnDressUpUploaded(void *arg, GlobalTerminalNetError *error) {
}

void VsRecorder_OnDressUpListReceived(void *arg, GlobalTerminalNetError *error) {
    VsRecorderWork *work = arg;
    VsRecorderDressUp *dressUp = work->screenWork;

    if (error->isError != TRUE) {
        dressUp->downloadCount = GlobalTerminalNet_GetDressUpList(VsRecorder_GetNet(work), (void **)dressUp->downloads, 5);
    }
}

// ---- box data screens

void VsRecorder_LoadBoxDataOwner(VsRecorderWork *work, int index) {
    VsRecorderBoxData *boxData = work->screenWork;

    boxData->profile.card = FrontierCard_New(HEAP_ID_109);
    GlobalTerminalNet_CopyRequestHeader(boxData->downloads[index], boxData->profile.card);
}

void VsRecorder_FreeBoxDataOwner(VsRecorderWork *work) {
    FrontierCard_Delete(((VsRecorderBoxData *)work->screenWork)->profile.card);
}

BOOL VsRecorder_BoxFormHasMon(GlobalTerminalNetBoxForm *form) {
    int i;

    for (i = 0; i < 30; i++) {
        if (form->species[i] != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active on

void VsRecorder_SkipToOccupiedSlot(VsRecorderBoxData *boxData);

// from the cursor on, then from the first slot: put the cursor on the next slot with a Pokemon
void VsRecorder_SkipToOccupiedSlot(VsRecorderBoxData *boxData) {
    // the slots are walked through a view of the work shifted by one u16 per slot (codegen)
    int i;
    VsRecorderBoxData *slot;
    int start = boxData->cursor;

    i = start;
    if (start < 30) {
        slot = (VsRecorderBoxData *)((u16 *)boxData + start);
        do {
            if (slot->form.species[0] != 0) {
                boxData->cursor = i;
                return;
            }
            i++;
            slot = (VsRecorderBoxData *)((u16 *)slot + 1);
        } while (i < 30);
    }
    {
        VsRecorderBoxData *wrapped;
        int j = 0;
        if (start > 0) {
            wrapped = boxData;
            do {
                if (wrapped->form.species[0] != 0) {
                    boxData->cursor = j;
                    return;
                }
                j++;
                wrapped = (VsRecorderBoxData *)((u16 *)wrapped + 1);
            } while (j < start);
        }
    }
}

#pragma force_active reset

void VsRecorder_OnBoxDataSendTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderBoxData *boxData = work->screenWork;

    switch (index) {
    case 0:
        if (event == 2) {
            VsRecorder_OnTouchConfirm(work);
            if (boxData->box > 0) {
                boxData->box = boxData->box - 1;
            } else {
                boxData->box = 17;
            }
            VsRecorder_SetSubState(work, 4);
        }
        break;
    case 1:
        if (event == 2) {
            VsRecorder_OnTouchConfirm(work);
            boxData->box++;
            boxData->box = boxData->box % 18;
            VsRecorder_SetSubState(work, 4);
        }
        break;
    case 2:
        if (event == 0) {
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_SetSubState(work, 9);
        }
        break;
    case 3:
        if (event == 0) {
            VsRecorder_OnTouchConfirm(work);
            if (VsRecorder_BoxFormHasMon(&boxData->form)) {
                VsRecorder_SetSubState(work, 5);
            } else {
                VsRecorder_PrintBoxDataMessage(work, 271); // An empty Box's Data can't be sent.
            }
        }
        break;
    }
}

void VsRecorder_OnBoxDataSendCategoryTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderBoxData *boxData = work->screenWork;

    if (event == 0) {
        if (index != 6) {
            VsRecorder_OnTouchConfirm(work);
            boxData->category = index;
            VsRecorder_SetSubState(work, 8);
        } else {
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_SetSubState(work, 7);
        }
    }
}

void VsRecorder_OnBoxDataViewCategoryTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderBoxData *boxData = work->screenWork;

    if (event == 0) {
        if (index != 6) {
            VsRecorder_OnTouchConfirm(work);
            boxData->category = index;
            VsRecorder_SetSubState(work, 4);
        } else {
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_SetSubState(work, 12);
        }
    }
}

void VsRecorder_OnBoxListTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderBoxData *boxData = work->screenWork;

    if (event == 0) {
        switch (index) {
        case 0:
        default:
            VsRecorder_OnTouchConfirm(work);
            if (boxData->showProfile == FALSE) {
                VsRecorder_SetSubState(work, 7);
            } else {
                VsRecorder_SetSubState(work, 8);
            }
            break;
        case 1:
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_SetSubState(work, 9);
            break;
        case 2:
            VsRecorder_OnTouchConfirm(work);
            VsRecorder_SetSubState(work, 6);
            break;
        }
    }
}

void VsRecorder_OnBoxDataBoxTouch(u32 index, u32 event, void *arg) {
    VsRecorderWork *work = arg;
    VsRecorderBoxData *boxData = work->screenWork;

    if (event == 0) {
        switch (index) {
        case 0: {
            int cursor;
            VsRecorder_OnTouchConfirm(work);
            boxData->cursor++;
            boxData->cursor = boxData->cursor % 30;
            VsRecorder_SkipToOccupiedSlot(boxData);
            boxData->cursorX = boxData->cursor % 6;
            boxData->cursorY = boxData->cursor / 6;
            OrbitCursor_SetTarget(work->orbitCursors[0], (s16)(boxData->cursorX * 24 + 110), (s16)(boxData->cursorY * 22 + 52));
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_FreeBoxMonPic(work);
            VsRecorder_CreateBoxMonPic(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_PrintBoxMonName(work, boxData->cursor);
            cursor = boxData->cursor;
            if (boxData->form.species[cursor] != 0) {
                if (VsRecorder_HasAllBits(boxData->form.eggFlags, 1 << cursor) != TRUE) {
                    PlayCry(boxData->form.species[boxData->cursor], boxData->form.forms[boxData->cursor]);
                }
            }
            break;
        }
        case 1:
            VsRecorder_OnTouchConfirm(work);
            OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
            VsRecorder_SetSubState(work, 11);
            break;
        }
    }
}

void VsRecorder_CreateBoxDataButtons(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    VsRecorder_LoadButtonGfx(work, NNS_G2D_VRAM_TYPE_2DSUB);
    VsRecorder_CreateLabeledSprite(work, &boxData->exitButton, NNS_G2D_VRAM_TYPE_2DSUB, 3, 32, 232, 36, -8, 0, TRUE);   // EXIT
    VsRecorder_CreateLabeledSprite(work, &boxData->sendButton, NNS_G2D_VRAM_TYPE_2DSUB, 53, 128, 232, 36, -8, 1, TRUE); // SEND
}

void VsRecorder_FreeBoxDataButtons(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    VsRecorder_FreeLabeledSprite(&boxData->exitButton);
    VsRecorder_FreeLabeledSprite(&boxData->sendButton);
    VsRecorder_FreeButtonGfx(work);
}

void VsRecorder_SelectFirstBoxMon(VsRecorderBoxData *boxData) {
    int i;

    for (i = 0; i < 30; i++) {
        if (boxData->form.species[i] != 0) {
            boxData->cursor = i;
            return;
        }
    }
}

void VsRecorder_PackBoxForm(SaveData *saveData, PCStorage *storage, int box, GlobalTerminalNetBoxForm *form) {
    memset(form, 0, sizeof(GlobalTerminalNetBoxForm));
    GlobalTerminalNet_PackBoxData(saveData, storage, box, form, HEAP_ID_109);
}

// both box data screens, substate 0
BOOL VsRecorder_BoxDataInit(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = Heap_Alloc(HEAP_ID_109, sizeof(VsRecorderBoxData));
    memset(boxData, 0, sizeof(VsRecorderBoxData));
    work->screenWork = boxData;
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
    boxData->storage = SaveArray_PCStorage_Get(work->saveData);
    VsRecorder_PackBoxForm(work->saveData, boxData->storage, boxData->box, &boxData->form);
    VsRecorder_SelectFirstBoxMon(boxData);
    VsRecorder_InitBgBlend(&boxData->eva, &boxData->evb, 0);
    boxData->hitboxes[0] = TouchHitboxController_Create(sVsRecorderBoxDataSendHitboxes, 4, VsRecorder_OnBoxDataSendTouch, work, HEAP_ID_109);
    boxData->hitboxes[1] = TouchHitboxController_Create(sVsRecorderBoxDataCategoryHitboxes, 7, VsRecorder_OnBoxDataSendCategoryTouch, work, HEAP_ID_109);
    boxData->hitboxes[2] = TouchHitboxController_Create(sVsRecorderBoxDataCategoryHitboxes, 7, VsRecorder_OnBoxDataViewCategoryTouch, work, HEAP_ID_109);
    boxData->hitboxes[3] = TouchHitboxController_Create(sVsRecorderBoxListHitboxes, 3, VsRecorder_OnBoxListTouch, work, HEAP_ID_109);
    boxData->hitboxes[4] = TouchHitboxController_Create(sVsRecorderBoxDataHitboxes, 2, VsRecorder_OnBoxDataBoxTouch, work, HEAP_ID_109);
    boxData->card = FrontierCard_New(HEAP_ID_109);
    FrontierCard_InitFromSave(boxData->card, work->saveData);
    VsRecorder_SetSubState(work, 1);
    return FALSE;
}

// both box data screens, substate 1: fade out the menu and flip the screens
BOOL VsRecorder_BoxDataEnter(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        if (VsRecorder_StepFade(&boxData->fade, 1)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)boxData->fade, (u16)work->blendColor);
        break;
    case 1:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeMenuRows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_StartFlipScreens(work, 0);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        work->step++;
        break;
    case 2:
        if (VsRecorder_IsFlipScreensDone(work)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateBoxDataButtons(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_ResetBgPriorities();
            work->step++;
        }
        break;
    default:
        VsRecorder_SetSubState(work, 2);
        break;
    }
    return FALSE;
}

#pragma force_active on

BOOL VsRecorder_BoxDataSendChangeBox(VsRecorderWork *work);

// send screen, substate 4: show the previous / next box of the PC
BOOL VsRecorder_BoxDataSendChangeBox(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_ApplyBlend(&boxData->eva, &boxData->evb, 8, 18, 8, 18, 1);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeBoxMonPic(work);
        VsRecorder_FreeBoxMonIcons(work);
        VsRecorder_ClearBoxDataInfoWindows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
    case 1:
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 1)) {
            VsRecorder_PackBoxForm(work->saveData, boxData->storage, boxData->box, &boxData->form);
            VsRecorder_SelectFirstBoxMon(boxData);
            work->step++;
        }
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_RefreshOwnBox(work);
        VsRecorder_SetBoxMonPicVanished(work, TRUE);
        VsRecorder_SetBoxMonIconsDrawFlag(work, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        break;
    case 3:
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 1)) {
            VsRecorder_PrintBoxMonName(work, boxData->cursor);
            VsRecorder_PrintBoxName(work);
            VsRecorder_PrintBoxDataMessage(work, 270); // Please choose a Box.
            VsRecorder_SetBoxMonPicVanished(work, FALSE);
            VsRecorder_SetBoxMonIconsDrawFlag(work, TRUE);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            reg_G2_BLDCNT = 0;
            VsRecorder_SetSubState(work, 3);
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

void VsRecorder_SetBoxDataSendButtonHidden(VsRecorderWork *work, BOOL hidden) {
    VsRecorderBoxData *boxData = work->screenWork;

    if (hidden == FALSE) {
        ManagedSprite_SetDrawFlag(boxData->sendButton.sprite, TRUE);
        TextOBJ_SetSpritesDrawFlag(boxData->sendButton.text, TRUE);
        ManagedSprite_SetPositionXY(boxData->exitButton.sprite, 32, 232);
    } else {
        ManagedSprite_SetDrawFlag(boxData->sendButton.sprite, FALSE);
        TextOBJ_SetSpritesDrawFlag(boxData->sendButton.text, FALSE);
        ManagedSprite_SetPositionXY(boxData->exitButton.sprite, 80, 232);
    }
    TextOBJ_SetPosition(boxData->exitButton.text, 36, -8);
    TextOBJ_SetPosition(boxData->sendButton.text, 36, -8);
}

#pragma force_active on

BOOL VsRecorder_BoxDataSendShowCategories(VsRecorderWork *work);

// send screen, substate 5: ask for the category
BOOL VsRecorder_BoxDataSendShowCategories(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        VsRecorder_StepFade(&boxData->fade, 1);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 2)) {
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 79, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            VsRecorder_CreateBoxDataCategoryButtons(work);
            VsRecorder_SetBoxDataSendButtonHidden(work, TRUE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, boxData->fade, work->blendColor);
        break;
    case 1:
        VsRecorder_StepFade(&boxData->fade, 0);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 2)) {
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            VsRecorder_SetSubState(work, 6);
            VsRecorder_PrintBoxDataMessage(work, 272); // Where would you like to send this?
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, boxData->fade, work->blendColor);
        break;
    }
    return FALSE;
}

#pragma force_active reset

// send screen, substate 6
BOOL VsRecorder_BoxDataSendSelectCategory(VsRecorderWork *work) {
    TouchHitboxController_IsTriggered(((VsRecorderBoxData *)work->screenWork)->hitboxes[1]);
    return FALSE;
}

#pragma force_active on

BOOL VsRecorder_BoxDataSendUpload(VsRecorderWork *work);

// send screen, substate 8: send the box
BOOL VsRecorder_BoxDataSendUpload(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    GlobalTerminalNetError *error;

    if (!VsRecorder_IsNetIdle(work)) {
        return FALSE;
    }
    switch (work->step) {
    case 0:
        VsRecorder_RemoveBoxDataCategoryButtons(work);
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
    case 1:
        VsRecorder_ApplyBlend(&boxData->eva, &boxData->evb, 8, 18, 8, 18, 1);
        VsRecorder_FreeBoxMonPic(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeBoxMonIcons(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_ClearBoxDataInfoWindows(work);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 2:
        VsRecorder_StepFade(&boxData->fade, 1);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    case 3:
        work->step++;
    case 4:
        VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 128, 96);
        OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 24, 24);
        VsRecorder_ShowTopMessage(work, 288); // Sending your Box Data...
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 5:
        if (GlobalTerminalNet_RequestBoxUpload(VsRecorder_GetNet(work), boxData->category, boxData->card, boxData->storage, boxData->box) == TRUE) {
            work->step++;
        }
        break;
    case 6:
        VsRecorder_HideTopMessage(work);
        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_ShowMessageScreen(work, 2, error->code, error->type);
            work->step++;
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_AddGameStatsScore(work, 37);
            VsRecorder_SetBoxUploaded(work, 1);
            PlaySE(SEQ_SE_PL_BREC12);
            work->step++;
        }
        break;
    case 7:
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        work->step++;
        break;
    default:
        VsRecorder_SetSubState(work, 10);
        break;
    }
    return FALSE;
}

#pragma force_active reset

// send screen, substate 7: back to the box
BOOL VsRecorder_BoxDataSendCancelCategory(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        VsRecorder_RemoveBoxDataCategoryButtons(work);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
    case 1:
        VsRecorder_StepFade(&boxData->fade, 1);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 2)) {
            VsRecorder_SetBoxDataSendButtonHidden(work, FALSE);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 77, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_StepFade(&boxData->fade, 0);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 2)) {
            VsRecorder_PrintBoxDataMessage(work, 270); // Please choose a Box.
            VsRecorder_SetSubState(work, 3);
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    default:
        break;
    }
    return FALSE;
}

// send screen, substate 9: leave (fade out the box)
BOOL VsRecorder_BoxDataSendQuit(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_ApplyBlend(&boxData->eva, &boxData->evb, 8, 18, 8, 18, 1);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeBoxMonPic(work);
        VsRecorder_FreeBoxMonIcons(work);
        VsRecorder_ClearBoxDataInfoWindows(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
    case 1:
        VsRecorder_StepFade(&boxData->fade, 1);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_SetSubState(work, 10);
        break;
    }
    return FALSE;
}

// view screen, substate 12: leave (fade out the categories)
BOOL VsRecorder_BoxDataViewQuit(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_RemoveBoxDataCategoryButtons(work);
        VsRecorder_ApplyBlend(&boxData->eva, &boxData->evb, 8, 18, 8, 18, 1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
    case 1:
        VsRecorder_StepFade(&boxData->fade, 1);
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, TRUE, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_SetSubState(work, 13);
        break;
    }
    return FALSE;
}

void VsRecorder_CreateBoxDataWindows(VsRecorderWork *work) {
    Window *window;
    const VsRecorderWindowRect *rect = sVsRecorderBoxDataWindows;
    const u32 *fill = sVsRecorderBoxDataWindowFillColors;
    int baseTile = 0x101;
    int i;

    for (i = 0, window = ((VsRecorderBoxData *)work->screenWork)->windows; i < 3; i++) {
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, rect->x, rect->y, rect->width, rect->height, 14, baseTile);
        FillWindowPixelBuffer(window, (u8)*fill);
        ScheduleWindowCopyToVram(window);
        baseTile += rect->width * rect->height;
        window++;
        rect++;
        fill++;
    }
}

void VsRecorder_CreateBoxDataCategoryButtons(VsRecorderWork *work) {
    Window *window;
    const VsRecorderWindowRect *rect = sVsRecorderBoxDataCategoryButtonWindows;
    int baseTile = 1;
    u32 i;

    for (i = 0, window = ((VsRecorderBoxData *)work->screenWork)->buttons; i < 6; i++) {
        String *str;
        int x;
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, rect->x, rect->y, rect->width, rect->height, 14, baseTile);
        str = NewString_ReadMsgData(work->msgData, i + 60); // FAVORITE ... SELECT
        x = VsRecorder_GetCenteredTextX(window, str);
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, 0, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
        baseTile += rect->width * rect->height;
        window++;
        rect++;
    }
}

void VsRecorder_RemoveBoxDataCategoryButtons(VsRecorderWork *work) {
    u32 i;
    Window *window;

    for (i = 0, window = ((VsRecorderBoxData *)work->screenWork)->buttons; i < 6; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
}

void VsRecorder_RemoveBoxDataWindows(VsRecorderWork *work) {
    int i;
    Window *window;

    for (i = 0, window = ((VsRecorderBoxData *)work->screenWork)->windows; i < 3; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
}

#pragma force_active on

void VsRecorder_PrintBoxDataMessage(VsRecorderWork *work, int msgId) {
    VsRecorderBoxData *boxData = work->screenWork;
    Window *window = &boxData->windows[2];
    MsgData *msgData = work->msgData;
    String *str = NewString_ReadMsgData(msgData, msgId);
    FillWindowPixelBuffer(window, 0xCC);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

#pragma force_active reset

// "<sender>'S <category> PC BOX"
void VsRecorder_PrintBoxDataOwnerMessage(VsRecorderWork *work, int unused) {
    VsRecorderBoxData *boxData = work->screenWork;
    String *template = NewString_ReadMsgData(work->msgData, boxData->category + 28);
    String *name = String_New(255, HEAP_ID_109);
    String *str = String_New(255, HEAP_ID_109);
    MessageFormat *msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
    Window *window = &boxData->windows[2];
    CopyU16ArrayToString(name, (u16 *)boxData->downloads[work->listWindow.cursor]);
    VsRecorder_SanitizeName(work, name);
    BufferString(msgFormat, 0, name, 0, 1, 2);
    StringExpandPlaceholders(msgFormat, str, template);
    FillWindowPixelBuffer(window, 0xCC);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(template);
    String_Delete(name);
    String_Delete(str);
    MessageFormat_Delete(msgFormat);
}

#pragma force_active on

void VsRecorder_PrintBoxName(VsRecorderWork *work);

void VsRecorder_PrintBoxName(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    Window *window = &boxData->windows[1];
    String *str = String_New(255, HEAP_ID_109);
    CopyU16ArrayToString(str, boxData->form.name);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

#pragma force_active reset

// Pokemon name and box name
void VsRecorder_ClearBoxDataInfoWindows(VsRecorderWork *work) {
    Window *window;
    const u32 *fill = sVsRecorderBoxDataWindowFillColors;
    int i;

    for (i = 0, window = ((VsRecorderBoxData *)work->screenWork)->windows; i < 2; i++) {
        FillWindowPixelBuffer(window, (u8)*fill);
        ScheduleWindowCopyToVram(window);
        window++;
        fill++;
    }
}

BOOL VsRecorder_HasAllBits(u32 flags, u32 mask) {
    return (flags & mask) == mask;
}

void VsRecorder_PrintBoxMonName(VsRecorderWork *work, int slot) {
    u16 name[256];
    VsRecorderBoxData *boxData = work->screenWork;
    Window *window = &boxData->windows[0];
    u16 species;
    String *str;
    int x;

    FillWindowPixelBuffer(window, 0);
    species = boxData->form.species[slot];
    if (species == 0) {
        ScheduleWindowCopyToVram(window);
        return;
    }
    str = String_New(255, HEAP_ID_109);
    if (VsRecorder_HasAllBits(boxData->form.eggFlags, 1 << boxData->cursor) == TRUE) {
        species = SPECIES_EGG;
    }
    GetSpeciesNameIntoArray(species, HEAP_ID_109, name);
    CopyU16ArrayToString(str, name);
    x = VsRecorder_GetCenteredTextX(window, str);
    AddTextPrinterParameterizedWithColor(window, 0, str, x, 6, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

#pragma force_active on

void VsRecorder_CreateBoxMonIcons(VsRecorderWork *work);

void VsRecorder_CreateBoxMonIcons(VsRecorderWork *work) {
    VsRecorderBoxData *boxData;
    PaletteData *plttData;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    int i;
    ManagedSpriteTemplate template;

    boxData = work->screenWork;
    plttData = work->plttData;
    spriteSystem = work->spriteSystem;
    spriteManager = work->spriteManager;
    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, ((u32(*)(VsRecorderWork *))GetMonIconPlttNaix)(work), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconCell64KNaix(), FALSE, 100000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconAnim64KNaix(), FALSE, 100000);
    for (i = 0; i < 30; i++) {
        u16 species = boxData->form.species[i];
        u32 form = boxData->form.forms[i];
        int isEgg = VsRecorder_HasAllBits(boxData->form.eggFlags, 1 << i);
        if (species != 0) {
            SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(species, isEgg, form), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 100000 + i);
            template.x = 110 + (i % 6) * 24;
            template.y = 48 + (i / 6) * 22;
            template.z = 0;
            template.animation = 0;
            template.drawPriority = 0;
            template.pal = 0;
            template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
            template.bgPriority = 0;
            template.vramTransfer = 0;
            template.resIdList[0] = 100000 + i;
            template.resIdList[1] = 100000;
            template.resIdList[2] = 100000;
            template.resIdList[3] = 100000;
            template.resIdList[4] = -1;
            template.resIdList[5] = -1;
            boxData->monIcons[i] = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
            ((void (*)(ManagedSprite *, int))ManagedSprite_SetPaletteOverrideOffset)(boxData->monIcons[i], GetMonIconPaletteEx(species, form, isEgg) + 4);
            ManagedSprite_SetAnim(boxData->monIcons[i], 1);
            ((void (*)(ManagedSprite *, int))ManagedSprite_SetDrawPriority)(boxData->monIcons[i], 30 - i);
        }
    }
}

#pragma force_active reset

void VsRecorder_SetBoxMonIconsDrawFlag(VsRecorderWork *work, BOOL draw) {
    VsRecorderBoxData *boxData = work->screenWork;
    int i;

    for (i = 0; i < 30; i++) {
        if (boxData->form.species[i] != 0) {
            ManagedSprite_SetDrawFlag(boxData->monIcons[i], draw);
        }
    }
}

void VsRecorder_FreeBoxMonIcons(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    int i;

    SpriteManager_UnloadPlttObjById(work->spriteManager, 100000);
    SpriteManager_UnloadCellObjById(work->spriteManager, 100000);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 100000);
    for (i = 0; i < 30; i++) {
        if (boxData->form.species[i] != 0 && boxData->monIcons[i] != NULL) {
            SpriteManager_UnloadCharObjById(work->spriteManager, 100000 + i);
            Sprite_DeleteAndFreeResources(boxData->monIcons[i]);
            boxData->monIcons[i] = NULL;
        }
    }
}

#pragma force_active on

void VsRecorder_CreateBoxMonPic(VsRecorderWork *work);

void VsRecorder_CreateBoxMonPic(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    int cursor = boxData->cursor;
    u32 otId = boxData->form.otIds[cursor];
    int form = boxData->form.forms[cursor];
    u16 species = boxData->form.species[cursor];
    u32 personality = boxData->form.personality[cursor];
    BASE_STATS *personal;
    u8 gender;
    u8 shiny;
    PokepicTemplate template;

    if (species == 0) {
        boxData->pokepic = NULL;
        return;
    }
    if (VsRecorder_HasAllBits(boxData->form.eggFlags, 1 << cursor) == TRUE) {
        if (species == SPECIES_MANAPHY) {
            form = 1;
        } else {
            form = 0;
        }
        species = SPECIES_EGG;
    }
    personal = AllocAndLoadMonPersonal(species, HEAP_ID_109);
    gender = GetGenderBySpeciesAndPersonality(species, personality);
    shiny = CalcShininessByOtIdAndPersonality(otId, personality);
    GetMonPicHeightBySpeciesGenderForm(species, gender, 2, (u8)form, personality);
    GetMonSpriteCharAndPlttNarcIdsEx(&template, species, gender, 2, shiny, (u8)form, personality);
    boxData->pokepic = PokepicManager_CreatePokepic(work->pokepicManager, &template, 42, 91, 0, 0, NULL, NULL);
    FreeMonPersonal(personal);
}

#pragma force_active reset

void VsRecorder_SetBoxMonPicVanished(VsRecorderWork *work, BOOL vanished) {
    VsRecorderBoxData *boxData = work->screenWork;
    Pokepic *pokepic = boxData->pokepic;

    if (pokepic != NULL) {
        Pokepic_SetAttr(pokepic, POKEPIC_VANISHED, vanished);
    }
}

void VsRecorder_FreeBoxMonPic(VsRecorderWork *work) {
    Pokepic *pokepic = ((VsRecorderBoxData *)work->screenWork)->pokepic;
    if (pokepic != NULL) {
        Pokepic_Delete(pokepic);
    }
}

static inline void VsRecorder_LoadBoxGfx(VsRecorderWork *work, VsRecorderBoxData *boxData) {
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, boxData->form.wallpaper * 3 + 138, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, boxData->form.wallpaper * 3 + 140, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
    PaletteData_LoadFromNarc(work->plttData, NARC_a_1_9_1, boxData->form.wallpaper * 3 + 139, HEAP_ID_109, PLTTBUF_MAIN_BG, 0x40, 0xC0, 0xC0);
    VsRecorder_CreateBoxMonPic(work);
    VsRecorder_CreateBoxMonIcons(work);
}

// a box from the server: wallpapers the game cannot show become the first one
void VsRecorder_ShowBox(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    SaveArray_PCStorage_Get(work->saveData);
    if (boxData->form.wallpaper >= 16 && boxData->form.wallpaper < 24) {
        boxData->form.wallpaper = 0;
    }
    if (boxData->form.wallpaper >= 40) {
        boxData->form.wallpaper = 0;
    }
    VsRecorder_LoadBoxGfx(work, boxData);
    VsRecorder_PrintBoxMonName(work, boxData->cursor);
    VsRecorder_PrintBoxName(work);
    VsRecorder_PrintBoxDataMessage(work, 270); // Please choose a Box.
}

// a box of the player's PC: bonus wallpapers only once unlocked
void VsRecorder_RefreshOwnBox(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    PCStorage *storage = SaveArray_PCStorage_Get(work->saveData);

    if (boxData->form.wallpaper >= 16 && boxData->form.wallpaper < 24) {
        boxData->form.wallpaper = 0;
    }
    if (boxData->form.wallpaper >= 24 && boxData->form.wallpaper < 32 && !PCStorage_IsBonusWallpaperUnlocked(storage, boxData->form.wallpaper - 24)) {
        boxData->form.wallpaper = 0;
    }
    VsRecorder_LoadBoxGfx(work, boxData);
}

// send screen, substate 2: show the first box
BOOL VsRecorder_BoxDataSendShow(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_LoadBoxDataMessageBg(work, GF_BG_LYR_MAIN_2);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateBoxDataWindows(work);
        VsRecorder_ShowBox(work);
        VsRecorder_SetBoxMonPicVanished(work, TRUE);
        VsRecorder_SetBoxMonIconsDrawFlag(work, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 77, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 0)) {
            VsRecorder_SetBoxMonPicVanished(work, FALSE);
            VsRecorder_SetBoxMonIconsDrawFlag(work, TRUE);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            *(u16 *)&reg_G2_BLDCNT = 0;
            work->step++;
        }
        break;
    case 2:
        if (VsRecorder_StepFade(&boxData->fade, 0)) {
            boxData->activeHitbox = boxData->hitboxes[0];
            VsRecorder_SetSubState(work, 3);
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, (u8)boxData->fade, (u16)work->blendColor);
        break;
    }
    return FALSE;
}

// send screen, substate 3
BOOL VsRecorder_BoxDataSendMain(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    TouchHitboxController_IsTriggered(boxData->activeHitbox);
    return FALSE;
}

#pragma force_active on

BOOL VsRecorder_BoxDataSendExit(VsRecorderWork *work);

// send screen, substate 10: back to the menu
BOOL VsRecorder_BoxDataSendExit(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_FreeBoxMonIcons(work);
        VsRecorder_FreeBoxMonPic(work);
        VsRecorder_RemoveBoxDataWindows(work);
        VsRecorder_FreeBoxDataButtons(work);
        OrbitCursor_SetActive(work->orbitCursors[1], FALSE);
        OrbitCursor_SetVisible(work->orbitCursors[1], FALSE);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    case 1:
        if (VsRecorder_IsFlipScreensDone(work)) {
            for (int i = 0; i < 5; i++) {
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
        if (VsRecorder_StepFade(&boxData->fade, 0)) {
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

BOOL VsRecorder_BoxDataViewShow(VsRecorderWork *work);

// view screen, substate 2: show the categories
BOOL VsRecorder_BoxDataViewShow(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_LoadBoxDataMessageBg(work, GF_BG_LYR_MAIN_2);
        VsRecorder_CreateBoxDataWindows(work);
        VsRecorder_PrintBoxDataMessage(work, 275); // Which category would you like to see?
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 79, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_CreateBoxDataCategoryButtons(work);
        VsRecorder_SetBoxDataSendButtonHidden(work, TRUE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&boxData->eva, &boxData->evb, FALSE, 0)) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            reg_G2_BLDCNT = 0;
            work->step++;
        }
        break;
    case 2:
        if (VsRecorder_StepFade(&boxData->fade, 0)) {
            boxData->activeHitbox = boxData->hitboxes[2];
            VsRecorder_SetSubState(work, 3);
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 12, boxData->fade, (u16)work->blendColor);
        break;
    }
    return FALSE;
}

#pragma force_active reset

// view screen, substate 3
BOOL VsRecorder_BoxDataViewSelectCategory(VsRecorderWork *work) {
    TouchHitboxController_IsTriggered(((VsRecorderBoxData *)work->screenWork)->hitboxes[2]);
    return FALSE;
}

#pragma force_active on

void VsRecorder_CreateBoxListButtons(VsRecorderWork *work);

// VIEW PROFILE and VIEW BOX DATA
void VsRecorder_CreateBoxListButtons(VsRecorderWork *work) {
    VsRecorderMsgIdPair msgIds;
    Window *window;
    const VsRecorderWindowRect *rect;
    VsRecorderBoxData *boxData = *(VsRecorderBoxData *volatile *)&work->screenWork;
    u32 baseTile;
    int i;

    baseTile = 1;
    msgIds = *(const VsRecorderMsgIdPair *)((const u8 *)ov40_02245268 + 0x14);
    rect = sVsRecorderBoxListButtonWindows;
    i = 0;
    window = boxData->buttons;
    for (; i < 2; i++) {
        String *str;
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, rect->x, rect->y, rect->width, rect->height, 14, (u16)baseTile);
        FillWindowPixelBuffer(window, 0);
        str = NewString_ReadMsgData(work->msgData, msgIds.msgIds[i]);
        AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        baseTile += rect->width * rect->height;
        String_Delete(str);
        window++;
        rect++;
    }
}

#pragma force_active reset

// VIEW PROFILE / CLOSE PROFILE
void VsRecorder_PrintBoxListProfileButton(VsRecorderWork *work, u32 msgId) {
    VsRecorderBoxData *boxData = work->screenWork;
    Window *window = boxData->buttons;
    String *str;
    u32 x;

    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msgId);
    x = VsRecorder_GetCenteredTextX(window, str);
    AddTextPrinterParameterizedWithColor(window, 0, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
}

// "Next Pokemon" button of the box
void VsRecorder_CreateNextMonButton(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    Window *window = boxData->buttons;
    String *str;
    u32 x;

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 12, 6, 8, 4, 14, 1);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, 34); // Next Pokemon
    x = (64 - FontID_String_GetWidthMultiline(0, str, 0)) >> 1;
    AddTextPrinterParameterizedWithColor(window, 0, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

void VsRecorder_RemoveNextMonButton(VsRecorderWork *work) {
    VsRecorderBoxData *boxData = work->screenWork;
    ClearWindowTilemapAndCopyToVram(boxData->buttons);
    RemoveWindow(boxData->buttons);
}

void VsRecorder_RemoveBoxListButtons(VsRecorderWork *work) {
    int i = 0;
    VsRecorderBoxData *boxData = work->screenWork;
    Window *window = boxData->buttons;

    for (; i < 2; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
}

#pragma force_active on
#pragma optimization_level 2

// "LATEST 20 IN THE <category> PC BOX"
void VsRecorder_CreateBoxListTitle(VsRecorderWork *work) {
    VsRecorderBoxData *boxData;
    int baseTile;
    Window *window;
    const VsRecorderWindowRect *rect;
    String *str;
    u32 textX;
    u32 textY;
    u32 i;

    boxData = work->screenWork;
    rect = ov40_02245268;
    baseTile = 1;
    i = 0;
    window = boxData->windows;
    do {
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, rect->x, rect->y, rect->width, rect->height, 14, baseTile);
        FillWindowPixelBuffer(window, 0);
        if (i == 0) {
            str = NewString_ReadMsgData(work->msgData, boxData->category + 310);
            textX = VsRecorder_GetCenteredTextX(window, str);
            textY = 0;
        }
        if (str != NULL) {
            AddTextPrinterParameterizedWithColor(window, 0, str, textX, textY, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            String_Delete(str);
        }
        ScheduleWindowCopyToVram(window);
        baseTile += rect->width * rect->height;
        window++;
        rect++;
        i++;
    } while (i < 1);
}

#pragma optimization_level reset
#pragma force_active reset
