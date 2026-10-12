#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "assert.h"
#include "battle_recording.h"
#include "bg_window.h"
#include "game_stats.h"
#include "global_terminal_net.h"
#include "heap.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "touchscreen.h"

// VS Recorder helpers: list scroll arrows, the touch scroll bar, game stats, battle record import, the screen flip and
// V-blank tasks, the server video list, saved videos and Pokemon icons.

extern const TouchscreenHitbox ov40_02244E08[];
extern const TouchscreenHitbox ov40_02244E0C[];

void WirelessStatusIcon_Refresh(void);

void VsRecorder_RemoveListWindow(VsRecorderListWindow *list) {
    ClearWindowTilemapAndCopyToVram(&list->window);
    RemoveWindow(&list->window);
}

void VsRecorder_ClearListWindow(VsRecorderListWindow *list) {
    memset(list, 0, sizeof(VsRecorderListWindow));
}

#pragma force_active on

// up / down arrows of a list (main screen: vram 1, sub screen: vram 2)
void VsRecorder_CreateListArrows(VsRecorderListWindow *list, VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    ManagedSpriteTemplate template;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7F, FALSE, vram, 200000);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7E, FALSE, 200000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7D, FALSE, 200000);
    template.x = 128;
    template.y = 96;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 1;
    template.bgPriority = 1;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.pal = 0;
    template.vram = vram;
    template.vramTransfer = 0;
    template.resIdList[0] = 200000;
    template.resIdList[2] = 200000;
    template.resIdList[3] = 200000;
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        template.resIdList[1] = 9999;
    } else {
        template.resIdList[1] = 10000;
    }
    list->arrows[0] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    list->arrows[1] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    ManagedSprite_SetPaletteOverride(list->arrows[0], 2);
    ManagedSprite_SetPaletteOverride(list->arrows[1], 2);
    ManagedSprite_SetAnim(list->arrows[0], 0);
    ManagedSprite_SetAnim(list->arrows[1], 0);
    ManagedSprite_TickFrame(list->arrows[0]);
    ManagedSprite_TickFrame(list->arrows[1]);
    ManagedSprite_SetFlipMode(list->arrows[1], 2);
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        ManagedSprite_SetPositionXY(list->arrows[0], 128, 24);
        ManagedSprite_SetPositionXY(list->arrows[1], 128, 120);
    } else {
        ManagedSprite_SetPositionXY(list->arrows[0], 128, 88);
        ManagedSprite_SetPositionXY(list->arrows[1], 128, 184);
    }
    VsRecorder_UpdateListArrows(list);
}

#pragma force_active reset

void VsRecorder_SetListArrowsY(VsRecorderListWindow *list, int upY, int downY) {
    // called through an int prototype, as compiled (no s16 truncation)
    ((void (*)(ManagedSprite *, int, int))ManagedSprite_SetPositionXY)(list->arrows[0], 0x80, upY);
    ((void (*)(ManagedSprite *, int, int))ManagedSprite_SetPositionXY)(list->arrows[1], 0x80, downY);
}

// scrolled lists: hide the arrow of an end that is reached
void VsRecorder_UpdateListArrows(VsRecorderListWindow *list) {
    ManagedSprite_SetDrawFlag(list->arrows[0], TRUE);
    ManagedSprite_SetDrawFlag(list->arrows[1], TRUE);
    if (list->scroll == 0) {
        ManagedSprite_SetDrawFlag(list->arrows[0], FALSE);
    }
    if (list->scroll + list->rows >= list->count) {
        ManagedSprite_SetDrawFlag(list->arrows[1], FALSE);
    }
    ManagedSprite_TickTwoFrames(list->arrows[0]);
    ManagedSprite_TickTwoFrames(list->arrows[1]);
}

// cursor lists: also hide both when everything fits
void VsRecorder_UpdateListArrowsCursor(VsRecorderListWindow *list) {
    ManagedSprite_SetDrawFlag(list->arrows[0], TRUE);
    ManagedSprite_SetDrawFlag(list->arrows[1], TRUE);
    if (list->scroll == 0) {
        ManagedSprite_SetDrawFlag(list->arrows[0], FALSE);
    }
    if (list->scroll + list->rows >= list->count) {
        ManagedSprite_SetDrawFlag(list->arrows[1], FALSE);
    }
    if (list->rows >= list->count) {
        ManagedSprite_SetDrawFlag(list->arrows[0], FALSE);
        ManagedSprite_SetDrawFlag(list->arrows[1], FALSE);
    }
    ManagedSprite_TickTwoFrames(list->arrows[0]);
    ManagedSprite_TickTwoFrames(list->arrows[1]);
}

void VsRecorder_FreeListArrows(VsRecorderListWindow *list, VsRecorderWork *work) {
    SpriteManager_UnloadCharObjById(work->spriteManager, 200000);
    SpriteManager_UnloadCellObjById(work->spriteManager, 200000);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 200000);
    Sprite_DeleteAndFreeResources(list->arrows[0]);
    Sprite_DeleteAndFreeResources(list->arrows[1]);
}

ManagedSprite *ov40_0222F950(void *unused, VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram, int anim) {
    ManagedSpriteTemplate template;
    ManagedSprite *sprite;

    template.x = 0x80;
    template.y = 0x60;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 1;
    template.bgPriority = 1;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.pal = 0;
    template.vram = vram;
    template.vramTransfer = 0;
    template.resIdList[0] = 11924;
    template.resIdList[2] = 11924;
    template.resIdList[3] = 11924;
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        template.resIdList[1] = 9999;
    } else {
        template.resIdList[1] = 10000;
    }
    sprite = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    ManagedSprite_SetPaletteOverride(sprite, 2);
    ManagedSprite_SetAnim(sprite, anim);
    ManagedSprite_TickFrame(sprite);
    return sprite;
}

void VsRecorder_DeleteTouchScrollSprites(VsRecorderTouchScroll *scroll) {
    Sprite_DeleteAndFreeResources(scroll->sprites[0]);
    Sprite_DeleteAndFreeResources(scroll->sprites[1]);
}

void VsRecorder_CreateTouchScrollSprites(VsRecorderTouchScroll *scroll, VsRecorderWork *work) {
    VsRecorder_CreateTouchScrollSpritesWithAnim(scroll, work, 5);
}

#pragma force_active on

void VsRecorder_CreateTouchScrollSpritesWithAnim(VsRecorderTouchScroll *scroll, VsRecorderWork *work, int anim) {
    scroll->sprites[0] = ov40_0222F950(scroll, work, NNS_G2D_VRAM_TYPE_2DSUB, anim);
    scroll->sprites[1] = ov40_0222F950(scroll, work, NNS_G2D_VRAM_TYPE_2DSUB, anim);
    ManagedSprite_SetPositionXY(scroll->sprites[0], 24, 136);
    ManagedSprite_SetPositionXY(scroll->sprites[1], 232, 136);
}

#pragma force_active reset

void VsRecorder_InitTouchScroll(VsRecorderTouchScroll *scroll) {
    memset(scroll, 0, sizeof(VsRecorderTouchScroll));
}

void VsRecorder_FreeTouchScroll(VsRecorderTouchScroll *scroll) {
    VsRecorder_DeleteTouchScrollSprites(scroll);
}

// direction of a drag that moved 16 pixels or more
void VsRecorder_TouchScrollTrack(VsRecorderTouchScroll *scroll, int y) {
    int delta = y - scroll->lastY;
    if (delta * delta >= 0x100) {
        scroll->direction = delta < 0 ? -1 : 1;
        scroll->lastY = y;
    } else {
        scroll->direction = 0;
    }
}

void VsRecorder_TouchScrollSetEnabled(VsRecorderTouchScroll *scroll, VsRecorderListWindow *list) {
    if (list->unk_48 != 0) {
        if (list->count < 2) {
            scroll->enabled = FALSE;
            return;
        }
        scroll->enabled = TRUE;
        return;
    } else {
        if (list->rows >= list->count) {
            scroll->enabled = FALSE;
            return;
        }
        scroll->enabled = TRUE;
        return;
    }
}

void VsRecorder_UpdateTouchScroll(VsRecorderTouchScroll *scroll) {
    u32 x, y;
    BOOL held;
    int hit = 0;
    held = System_GetTouchHeldCoords(&x, &y);
    if (scroll->enabled != FALSE) {
        ManagedSprite_TickTwoFrames(scroll->sprites[0]);
        ManagedSprite_TickTwoFrames(scroll->sprites[1]);
    }
    if (scroll->held == FALSE && held == FALSE) {
        scroll->held = FALSE;
        scroll->timer = 0;
        scroll->startX = 0;
        scroll->lastY = 0;
        scroll->direction = 0;
        return;
    }
    if (TouchscreenHitbox_TouchHeldIsIn(ov40_02244E08) || TouchscreenHitbox_TouchHeldIsIn(ov40_02244E0C)) {
        hit = 1;
    }
    if (hit) {
        if (scroll->held == FALSE) {
            scroll->held = TRUE;
            scroll->startX = x;
            scroll->lastY = y;
            scroll->timer = 2;
        } else {
            scroll->timer--;
            VsRecorder_TouchScrollTrack(scroll, y);
        }
    } else if (scroll->held != FALSE) {
        scroll->held = FALSE;
        scroll->timer = 0;
        scroll->startX = 0;
        scroll->lastY = 0;
    }
}

void VsRecorder_AddGameStatsScore(VsRecorderWork *work, int score) {
    GameStats_AddScore(Save_GameStats_Get(work->saveData), score);
}

// import the downloaded battle video into the save's battle record
void VsRecorder_ImportBattleRecord(VsRecorderWork *work) {
    u32 unused;
    u8 *video;
    BattleRecord_Create(work->saveData, HEAP_ID_126, &unused);
    video = work->net.downloadedVideo;
    BattleRecord_Import(video, video + 0x80, video + 0xE4, NULL, work->saveData);
}

void VsRecorder_FlipScreensTask(SysTask *task, void *data) {
    VsRecorderFlipArgs *args = data;
    VsRecorder_SetScreensFlipped(args->flipped);
    WirelessStatusIcon_Refresh();
    args->done = TRUE;
    SysTask_Destroy(task);
}

SysTask *VsRecorder_StartFlipScreens(VsRecorderWork *work, int flipped) {
    work->flipArgs.flipped = flipped;
    work->flipArgs.done = FALSE;
    return SysTask_CreateOnVBlankQueue(VsRecorder_FlipScreensTask, &work->flipArgs, 4);
}

u32 VsRecorder_IsFlipScreensDone(VsRecorderWork *work) {
    return work->flipArgs.done;
}

void VsRecorder_VBlankTask(SysTask *task, void *data) {
    VsRecorderWork *work = data;
    if (work->spriteManager == NULL) {
        return;
    }
    if (VsRecorder_IsVBlankPaused(work) == 1) {
        return;
    }
    if (work->vblankBusy == 1) {
        return;
    }
    work->vblankBusy = 1;
    PokepicManager_HandleLoadImgAndOrPltt(work->pokepicManager);
    SpriteSystem_DrawSprites(work->spriteManager);
    SpriteSystem_TransferOam();
    work->vblankBusy = 0;
}

void VsRecorder_StartVBlankTask(VsRecorderWork *work) {
    work->task = SysTask_CreateOnVBlankQueue(VsRecorder_VBlankTask, work, 5);
}

BOOL VsRecorder_StepRecordSave(VsRecorderWork *work, u32 a1, u32 a2) {
    u32 result = VsRecorder_WriteRecordAsync(work->saveData, a1, a2, &work->unk_0834, &work->unk_0836);
    return result > 1;
}

void VsRecorder_SetBattleResult(VsRecorderWork *work, u32 result) {
    work->battleResult = result;
}

#pragma force_active on
#pragma opt_strength_reduction off

// server result callback: copy the received video list
void VsRecorder_OnVideoListReceived(VsRecorderWork *work, int *state) {
    VsRecorderNetData *net = &work->net;
    if (*state != 1) {
        int result = GlobalTerminalNet_GetVideoList(VsRecorder_GetNet(work), net->videoList.headers, 30);
        int i = 0;
        net->videoList.count = result;
        if (net->videoList.count > 0) {
            VsRecorderWork *source = work; // headers stride over the work
            GlobalTerminalNetVideoHeader *dest = work->net.videoList.videos;
            do {
                GF_ASSERT(source->net.videoList.headers[0]);
                MI_CpuCopy8(source->net.videoList.headers[0], dest, sizeof(GlobalTerminalNetVideoHeader));
                work->net.videoList.flags[i] = source->net.videoList.headers[0]->data[0x27];
                source = (VsRecorderWork *)((u8 *)source + sizeof(GlobalTerminalNetVideoHeader *));
                dest++;
                i++;
            } while (i < net->videoList.count);
        }
    }
}

#pragma opt_strength_reduction reset
#pragma force_active reset

// the four saved videos (header + owner card); slot 0 owner is the player
void VsRecorder_LoadSavedVideos(VsRecorderWork *work) {
    int i;
    int result;
    u8 *slot;
    slot = (u8 *)work; // savedVideos / videoOwners stride over the work
    for (i = 0; i < 4; i++, slot += sizeof(void *)) {
        BattleRecord_Load(work->saveData, HEAP_ID_109, &result, NULL, i);
        switch (result) {
        case 0:
            ((VsRecorderWork *)slot)->savedVideos[0] = NULL;
            ((VsRecorderWork *)slot)->videoOwners[0] = NULL;
            if (i == 0) {
                ((VsRecorderWork *)slot)->videoOwners[0] = FrontierCard_New(HEAP_ID_109);
                FrontierCard_InitFromSave(((VsRecorderWork *)slot)->videoOwners[0], work->saveData);
            }
            break;
        case 2:
            ((VsRecorderWork *)slot)->savedVideos[0] = NULL;
            ((VsRecorderWork *)slot)->videoOwners[0] = NULL;
            if (i == 0) {
                ((VsRecorderWork *)slot)->videoOwners[0] = FrontierCard_New(HEAP_ID_109);
                FrontierCard_InitFromSave(((VsRecorderWork *)slot)->videoOwners[0], work->saveData);
            }
            break;
        case 3:
            ((VsRecorderWork *)slot)->savedVideos[0] = NULL;
            ((VsRecorderWork *)slot)->videoOwners[0] = NULL;
            if (i == 0) {
                ((VsRecorderWork *)slot)->videoOwners[0] = FrontierCard_New(HEAP_ID_109);
                FrontierCard_InitFromSave(((VsRecorderWork *)slot)->videoOwners[0], work->saveData);
            }
            break;
        case 1:
            ((VsRecorderWork *)slot)->savedVideos[0] = BattleRecord_CopyHeader(HEAP_ID_109);
            if (i == 0) {
                ((VsRecorderWork *)slot)->videoOwners[0] = FrontierCard_New(HEAP_ID_109);
                FrontierCard_InitFromSave(((VsRecorderWork *)slot)->videoOwners[0], work->saveData);
            } else {
                ((VsRecorderWork *)slot)->videoOwners[0] = BattleRecord_CopyProfile(HEAP_ID_109);
            }
            break;
        }
        if (BattleRecord_Exists() == TRUE) {
            BattleRecord_Delete();
        }
    }
}

void VsRecorder_FreeSavedVideos(VsRecorderWork *work) {
    int i;
    u8 *slot = (u8 *)work; // savedVideos / videoOwners stride over the work
    void *zero1 = NULL;
    void *zero2 = NULL;
    for (i = 0; i < 4; i++, slot += sizeof(void *)) {
        if (((VsRecorderWork *)slot)->savedVideos[0] != NULL) {
            Heap_Free(((VsRecorderWork *)slot)->savedVideos[0]);
            ((VsRecorderWork *)slot)->savedVideos[0] = zero1;
        }
        if (((VsRecorderWork *)slot)->videoOwners[0] != NULL) {
            Heap_Free(((VsRecorderWork *)slot)->videoOwners[0]);
            ((VsRecorderWork *)slot)->videoOwners[0] = zero2;
        }
    }
}

// Pokemon icon resources (palette / cell / anim 100000; chars 100000 + index)
void VsRecorder_LoadMonIconGfx(VsRecorderWork *work) {
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    PaletteData *plttData = work->plttData;
    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconCell64KNaix(), FALSE, 100000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconAnim64KNaix(), FALSE, 100000);
}

void VsRecorder_UnloadMonIconGfx(VsRecorderWork *work) {
    SpriteManager_UnloadPlttObjById(work->spriteManager, 100000);
    SpriteManager_UnloadCellObjById(work->spriteManager, 100000);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 100000);
}

NARC *VsRecorder_OpenMonIconNarc(void) {
    return NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_109);
}

void VsRecorder_CloseMonIconNarc(NARC *narc) {
    NARC_Delete(narc);
}

#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_dead_assignments off
#pragma opt_propagation off
#pragma opt_common_subs off

ManagedSprite *VsRecorder_NewMonIcon(VsRecorderWork *work, NARC *narc, int index, int species, int form, int isEgg) {
    int e, f;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    spriteSystem = work->spriteSystem;
    ManagedSpriteTemplate template;
    ManagedSprite *sprite;
    spriteManager = work->spriteManager;
    if (species == 0) {
        return NULL;
    }
    f = form;
    e = isEgg;
    SpriteSystem_LoadCharResObjFromOpenNarcWithHardwareMappingType(spriteSystem, spriteManager, narc, GetMonIconNaixEx(species, e, f), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, index + 100000);
    template.x = 0;
    template.y = -48;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.resIdList[0] = index + 100000;
    template.resIdList[1] = 100000;
    template.resIdList[2] = 100000;
    template.resIdList[3] = 100000;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    ((void (*)(ManagedSprite *, int))ManagedSprite_SetPaletteOverrideOffset)(sprite, GetMonIconPaletteEx(species, f, e) + 4);
    ManagedSprite_SetAnim(sprite, 1);
    ManagedSprite_TickFrame(sprite);
    return sprite;
}

#pragma opt_common_subs reset
#pragma opt_propagation reset
#pragma opt_dead_assignments reset
#pragma opt_lifetimes reset
#pragma force_active reset

void VsRecorder_DeleteMonIcon(VsRecorderWork *work, u32 index, ManagedSprite *sprite) {
    if (sprite != NULL) {
        SpriteManager_UnloadCharObjById(work->spriteManager, 100000 + index);
        Sprite_DeleteAndFreeResources(sprite);
    }
}

void VsRecorder_DeleteSprite(void *unused0, void *unused1, ManagedSprite *sprite) {
    if (sprite != NULL) {
        Sprite_DeleteAndFreeResources(sprite);
    }
}
