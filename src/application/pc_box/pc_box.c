#include "global.h"

#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"

#include "application/pc_box/pc_box_internal.h"
#include "msgdata/msg.naix"

#include "bag.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "grid_input_handler.h"
#include "heap.h"
#include "item.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "render_window.h"
#include "save_arrays.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "yes_no_prompt.h"

extern const GraphicsBanks sPCBoxGfxBanks;
extern const u8 sPCBoxSlotHitboxes[];
extern const u8 sPCBoxPartyHitboxes[];
extern const u8 sPCBoxTrayPartyHitboxes[];
extern const GraphicsModes gPCBoxGraphicsModes;
extern const BgTemplate sPCBoxBgTemplate0;
extern const BgTemplate sPCBoxBgTemplate1;
extern const BgTemplate sPCBoxBgTemplate4;
extern const BgTemplate sPCBoxBgTemplate2;
extern const BgTemplate sPCBoxBgTemplate3;
extern const BgTemplate sPCBoxBgTemplate5;
extern const BgTemplate sPCBoxBgTemplate6;
extern const u16 sPCBoxReleaseCheckMoves[];
extern const u8 sPCBoxSlotGridRect[];
extern const u8 sPCBoxTrayRects[];

BOOL PCBox_Init(OverlayManager *manager, int *state) {
    PCBoxWork *work;

    Heap_Create(HEAP_ID_3, HEAP_ID_9, 1 << 16);
    work = OverlayManager_CreateAndGetData(manager, sizeof(PCBoxWork), HEAP_ID_9);
    MI_CpuFill8(work, 0, sizeof(PCBoxWork));
    work->args = OverlayManager_GetArgs(manager);
    work->storage = SaveArray_PCStorage_Get(work->args->saveData);
    work->activeBox = PCStorage_GetActiveBox(work->storage);
    work->party = SaveArray_Party_Get(work->args->saveData);
    work->bag = Save_Bag_Get(work->args->saveData);
    work->options = Save_PlayerData_GetOptionsAddr(work->args->saveData);
    work->selectedSlot = 0xff;
    work->returnCursorPos = 0;
    work->nextState = 11;
    return TRUE;
}

BOOL PCBox_Main(OverlayManager *manager, int *state) {
    PCBoxWork *work = OverlayManager_GetData(manager);

    if (PCBox_RunStateMachine(work, state) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL PCBox_Exit(OverlayManager *manager, int *state) {
    PCBoxWork *work = OverlayManager_GetData(manager);

    PCStorage_SetActiveBox(work->storage, work->activeBox);
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_9);
    return TRUE;
}

void PCBox_VBlankCallback(SysTask *task, void *data) {
    PCBoxWork *work = data;

    if (work->view->updateFunc != NULL && work->view->updateFunc(work) == FALSE) {
        work->view->updateFunc = NULL;
    }
    PCBox_UpdateDetailPanel(work);
    PaletteData_PushTransparentBuffers(work->view->paletteData);
    DoScheduledBgGpuUpdates(work->view->bgConfig);
    PCBox_TickSprites(work->view);
    if (work->view->spriteDrawLock == 0) {
        SpriteSystem_DrawSprites(work->view->spriteManager);
        SpriteSystem_TransferOam();
    }
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void PCBox_UpdateDetailPanel(PCBoxWork *work) {
    if (work->args->mode != PC_BOX_MODE_MOVE_ITEMS) {
        BgCellManager_UpdateCellMove(work->view->panels, 0x10);
        return;
    }
    BgCellManager_UpdateCellMove(work->view->panels, 0x10);
    PCBox_UpdateInfoItemIconPosition(work->view);
}

void PCBox_SetUpdateFunc(PCBoxView *view, void *v) {
    view->subState = 0;
    view->counter = 0;
    view->updateFunc = v;
    view->pendingUpdateFunc = 0;
}

void PCBox_SetPendingUpdateFunc(PCBoxView *view, void *v) {
    view->pendingUpdateFunc = v;
}

void PCBox_StartPendingUpdateFunc(PCBoxView *view) {
    PCBox_SetUpdateFunc(view, view->pendingUpdateFunc);
}

void PCBox_SetGfxBanks(void) {
    GfGfx_SetBanks(&sPCBoxGfxBanks);
}

void PCBox_InitBgLayers(PCBoxWork *work) {
    GraphicsModes modes;
    BgTemplate bg0;
    BgTemplate bg1;
    BgTemplate bg2;
    BgTemplate bg3;
    BgTemplate bg4;
    BgTemplate bg5;
    BgTemplate bg6;

    work->view->bgConfig = BgConfig_Alloc(HEAP_ID_10);
    modes = gPCBoxGraphicsModes;
    SetBothScreensModesAndDisable(&modes);

    bg0 = sPCBoxBgTemplate0;
    InitBgFromTemplate(work->view->bgConfig, GF_BG_LYR_MAIN_0, &bg0, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(work->view->bgConfig, GF_BG_LYR_MAIN_0);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 0x20, 0, HEAP_ID_10);

    bg1 = sPCBoxBgTemplate1;
    InitBgFromTemplate(work->view->bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(work->view->bgConfig, GF_BG_LYR_MAIN_1);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_10);

    bg2 = sPCBoxBgTemplate2;
    InitBgFromTemplate(work->view->bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);

    bg3 = sPCBoxBgTemplate3;
    InitBgFromTemplate(work->view->bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);

    bg4 = sPCBoxBgTemplate4;
    InitBgFromTemplate(work->view->bgConfig, GF_BG_LYR_SUB_0, &bg4, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(work->view->bgConfig, GF_BG_LYR_SUB_0);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_10);

    bg5 = sPCBoxBgTemplate5;
    InitBgFromTemplate(work->view->bgConfig, GF_BG_LYR_SUB_1, &bg5, GF_BG_TYPE_TEXT);

    bg6 = sPCBoxBgTemplate6;
    InitBgFromTemplate(work->view->bgConfig, GF_BG_LYR_SUB_2, &bg6, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(work->view->bgConfig, GF_BG_LYR_SUB_2);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_10);
}

void PCBox_FreeBgLayers(PCBoxWork *work) {
    FreeBgTilemapBuffer(work->view->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(work->view->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(work->view->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(work->view->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(work->view->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(work->view->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(work->view->bgConfig, GF_BG_LYR_MAIN_0);
    Heap_Free(work->view->bgConfig);
}

void PCBox_LoadBgGraphics(PCBoxWork *work) {
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 0xe, work->view->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 0xe, work->view->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 3, work->view->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_10);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_9, 2, work->view->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 1, work->view->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_10);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_9, 0, work->view->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_10);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_9, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 6, work->view->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_10);
    GfGfxLoader_LoadScrnData(NARC_a_0_1_9, 5, work->view->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_10);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_9, 7, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 6, work->view->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_10);
}

void PCBox_CreateMsgResources(PCBoxWork *work) {
    work->view->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0024_bin, HEAP_ID_10);
    work->view->msgPrinter = MessagePrinter_New(1, 2, 0, HEAP_ID_10);
    work->view->msgFormat = MessageFormat_New(HEAP_ID_10);
    work->view->string = String_New(0x400, HEAP_ID_10);
}

void PCBox_DestroyMsgResources(PCBoxWork *work) {
    String_Delete(work->view->string);
    MessageFormat_Delete(work->view->msgFormat);
    MessagePrinter_Delete(work->view->msgPrinter);
    DestroyMsgData(work->view->msgData);
}

void PCBox_LoadWindowGraphics(PCBoxWork *work) {
    PCBoxView *view = work->view;
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 0x40, view->bgConfig, GF_BG_LYR_MAIN_0, 0x3e8, 0x300, TRUE, HEAP_ID_10);
    GfGfxLoader_LoadCharData(NARC_a_0_1_9, 0x40, view->bgConfig, GF_BG_LYR_MAIN_1, 0x3e8, 0x300, TRUE, HEAP_ID_10);
    GfGfxLoader_GXLoadPal(NARC_a_0_1_9, 0x41, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, 0x40, HEAP_ID_10);
    LoadUserFrameGfx2(view->bgConfig, GF_BG_LYR_MAIN_0, 0x38e, 0xa, (u8)Options_GetFrame(work->options), HEAP_ID_10);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_11_OFFSET, HEAP_ID_10);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_10);
}

void PCBox_CreatePaletteData(PCBoxWork *work) {
    PaletteData *palette = PaletteData_Init(HEAP_ID_10);
    work->view->paletteData = palette;
    PaletteData_AllocBuffers(work->view->paletteData, PLTTBUF_MAIN_BG, 0x200, HEAP_ID_10);
}

void PCBox_FreePaletteData(PCBoxWork *work) {
    PaletteData_FreeBuffers(work->view->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_Free(work->view->paletteData);
}

#pragma force_active on

void PCBox_SetBlendEnabled(int enabled) {
    if (enabled == 1) {
        G2x_SetBlendAlpha_(0x04000050, 0, 42, 6, 10);
    } else {
        *(volatile u16 *)0x04000050 = 0;
    }
}

#pragma force_active reset

void PCBox_CreateYesNoPrompt(PCBoxWork *work) {
    work->view->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_10);
}

void PCBox_DestroyYesNoPrompt(PCBoxWork *work) {
    YesNoPrompt_Destroy(work->view->yesNoPrompt);
}

void PCBox_ShowYesNoPrompt(PCBoxWork *work, u32 cursorPos) {
    YesNoPromptTemplate tmpl;
    tmpl.bgConfig = work->view->bgConfig;
    tmpl.bgId = 0;
    tmpl.tileStart = 0x3AC;
    tmpl.plttSlot = 8;
    tmpl.x = 0x19;
    tmpl.y = 0xC;
    tmpl.ignoreTouchFlag = 0;
    tmpl.initialCursorPos = (u8)cursorPos;
    tmpl.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(work->view->yesNoPrompt, &tmpl);
}

u32 PCBox_AnimatePaletteFlash(PCBoxWork *work) {
    PCBoxView *view = work->view;
    PCBoxPaletteFlash *f = &view->paletteFlash;
    switch (f->state) {
    case 0:
        if (f->isBg == 0) {
            ManagedSprite_SetPaletteOverride(view->sprites[f->index], f->palA);
        } else {
            BgTilemapRectChangePalette(view->bgConfig, f->index, f->x, f->y, f->w, f->h, f->palA);
            ScheduleBgTilemapBufferTransfer(work->view->bgConfig, f->index);
        }
        f->state++;
        break;
    case 1:
        f->timer++;
        if (f->timer == 4) {
            if (f->isBg == 0) {
                ManagedSprite_SetPaletteOverride(work->view->sprites[f->index], f->palB);
            } else {
                BgTilemapRectChangePalette(work->view->bgConfig, f->index, f->x, f->y, f->w, f->h, f->palB);
                ScheduleBgTilemapBufferTransfer(work->view->bgConfig, f->index);
            }
            f->timer = 0;
            f->state++;
        }
        break;
    case 2:
        f->timer++;
        if (f->timer == 2) {
            return 0;
        }
        break;
    }
    return 1;
}

void PCBox_SaveMenuInputState(PCBoxWork *work) {
    if (GridInputHandler_IsButtonInputMode(work->view->gridInput) == TRUE) {
        MenuInputStateMgr_SetState(work->args->menuInputStatePtr, MENU_INPUT_STATE_BUTTONS);
    } else {
        MenuInputStateMgr_SetState(work->args->menuInputStatePtr, MENU_INPUT_STATE_TOUCH);
    }
}

u32 PCBox_GetSlotMonData(PCBoxWork *work, u32 idx, u32 field, void *dest) {
    BoxPokemon *boxmon = PCBox_GetSlotBoxMon(work, work->activeBox, idx);
    if (boxmon == NULL) {
        return 0;
    }
    return GetBoxMonData(boxmon, field, dest);
}

void PCBox_SetSlotMonData(PCBoxWork *work, u32 idx, u32 field, void *src) {
    BoxPokemon *boxmon = PCBox_GetSlotBoxMon(work, work->activeBox, idx);
    if (boxmon != NULL) {
        SetBoxMonData(boxmon, field, src);
        if (idx < 30) {
            PCStorage_SetBoxModified(work->storage, work->activeBox);
        }
    }
}

BoxPokemon *PCBox_GetSlotBoxMon(PCBoxWork *work, u32 boxId, u32 idx) {
    if (boxId == 0xFF || idx >= 30) {
        if (idx >= 30) {
            idx -= 30;
        }
        if (Party_GetCount(work->party) > idx) {
            return Mon_GetBoxMon(Party_GetMonByIndex(work->party, idx));
        }
        return NULL;
    }
    if (idx == 0xFF) {
        return NULL;
    }
    return PCStorage_GetMonByIndexPair(work->storage, boxId, idx);
}

void PCBox_DeleteSlotMon(PCBoxWork *work, u32 boxId, u32 idx) {
    if (idx < 30) {
        PCStorage_DeleteBoxMonByIndexPair(work->storage, boxId, idx);
    } else {
        Party_RemoveMon(work->party, idx - 30);
    }
}

void PCBox_SwapBoxAndPartyMon(PCBoxWork *work, PCBoxMoveEntry *a, PCBoxMoveEntry *b) {
    u8 mood;
    u32 slot;
    CopyBoxPokemonToPokemon(PCBox_GetSlotBoxMon(work, work->activeBox, a->from), a->mon);
    slot = b->from - 30;
    CopyPokemonToPokemon(Party_GetMonByIndex(work->party, slot), b->mon);
    mood = 0;
    SetMonData(b->mon, MON_DATA_MOOD, &mood);
    Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(work->party, slot, a->mon);
    PCStorage_PlaceMonInBoxByIndexPair(work->storage, work->activeBox, b->to, Mon_GetBoxMon(b->mon));
    PCBox_DrawBoxThumbnail(work, work->activeBox);
    PCBox_RefreshTrayBoxThumbnail(work, work->activeBox);
}

void PCBox_MoveBoxMonToParty(PCBoxWork *work, PCBoxMoveEntry *e) {
    CopyBoxPokemonToPokemon(PCBox_GetSlotBoxMon(work, work->activeBox, e->from), e->mon);
    Party_AddMon(work->party, e->mon);
    PCBox_DeleteSlotMon(work, work->activeBox, e->from);
    PCBox_DrawBoxThumbnail(work, work->activeBox);
    PCBox_RefreshTrayBoxThumbnail(work, work->activeBox);
}

void PCBox_MovePartyMonToBox(PCBoxWork *work, PCBoxMoveEntry *e) {
    u8 mood;
    Pokemon *mon;
    mon = Party_GetMonByIndex(work->party, e->from - 30);
    mood = 0;
    SetMonData(mon, MON_DATA_MOOD, &mood);
    PCStorage_PlaceMonInBoxByIndexPair(work->storage, work->activeBox, e->to, Mon_GetBoxMon(mon));
    PCBox_DeleteSlotMon(work, work->activeBox, e->from);
    PCBox_DrawBoxThumbnail(work, work->activeBox);
    PCBox_RefreshTrayBoxThumbnail(work, work->activeBox);
}

void PCBox_SwapPartyMons(PCBoxWork *work, PCBoxMoveEntry *e) {
    PartyExtraSub subA;
    PartyExtraSub subB;
    u32 slotA;
    u32 slotB;
    slotB = e->to - 30;
    slotA = e->from - 30;
    CopyPokemonToPokemon(Party_GetMonByIndex(work->party, slotA), e->mon);
    Party_GetMonAprijuiceModifiers(work->party, &subA, slotA);
    Party_GetMonAprijuiceModifiers(work->party, &subB, slotB);
    Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(work->party, slotA, Party_GetMonByIndex(work->party, slotB));
    Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(work->party, slotB, e->mon);
    Party_SetMonAprijuiceModifiers(work->party, &subB, slotA);
    Party_SetMonAprijuiceModifiers(work->party, &subA, slotB);
}

void PCBox_MovePartyMonToEnd(PCBoxWork *work, PCBoxMoveEntry *e) {
    PartyExtraSub sub;
    u32 slot = e->from - 30;
    u32 last;
    CopyPokemonToPokemon(Party_GetMonByIndex(work->party, slot), e->mon);
    Party_GetMonAprijuiceModifiers(work->party, &sub, slot);
    PCBox_DeleteSlotMon(work, work->activeBox, e->from);
    Party_AddMon(work->party, e->mon);
    last = Party_GetCount(work->party) - 1;
    Party_SetMonAprijuiceModifiers(work->party, &sub, last);
}

void PCBox_MoveBoxMonToOtherBox(PCBoxWork *work, PCBoxMoveEntry *e) {
    BoxPokemon *mon = PCBox_GetSlotBoxMon(work, work->activeBox, e->from);
    u32 box = PCBox_GetTargetBoxId(work, e->to);
    PCStorage_PlaceMonInBoxFirstEmptySlot(work->storage, box, mon);
    PCStorage_DeleteBoxMonByIndexPair(work->storage, work->activeBox, e->from);
    PCBox_DrawBoxThumbnail(work, work->activeBox);
    PCBox_RefreshTrayBoxThumbnail(work, work->activeBox);
    PCBox_DrawBoxThumbnail(work, box);
    PCBox_RefreshTrayBoxThumbnail(work, box);
}

void PCBox_MovePartyMonToOtherBox(PCBoxWork *work, PCBoxMoveEntry *e) {
    u32 box = PCBox_GetTargetBoxId(work, e->to);
    CopyPokemonToPokemon(Party_GetMonByIndex(work->party, e->from - 30), e->mon);
    if (GetMonData(e->mon, MON_DATA_SPECIES, NULL) == SPECIES_SHAYMIN) {
        Mon_UpdateShayminForm(e->mon, 0);
    }
    PCStorage_PlaceMonInBoxFirstEmptySlot(work->storage, box, Mon_GetBoxMon(e->mon));
    PCBox_DeleteSlotMon(work, work->activeBox, e->from);
    PCBox_DrawBoxThumbnail(work, box);
    PCBox_RefreshTrayBoxThumbnail(work, box);
}

void PCBox_ApplyMonMove(PCBoxWork *work) {
    PCBoxMoveWork *w = work->view->temp;
    u32 count;
    if (w->from == 0xFF) {
        return;
    }
    count = Party_GetCount(work->party);
    if ((w->to & 0x80) != 0) {
        PCBox_GetTargetBoxId(work, w->to);
        if (w->from < 30) {
            PCBox_MoveBoxMonToOtherBox(work, &w->entries[0]);
        } else {
            PCBox_MovePartyMonToOtherBox(work, &w->entries[w->from - 30]);
        }
    } else if (w->from < 30) {
        if (w->to < 30) {
            PCStorage_SwapMonsInBoxByIndexPair(work->storage, work->activeBox, w->from, w->to);
            PCBox_DrawBoxThumbnail(work, work->activeBox);
            PCBox_RefreshTrayBoxThumbnail(work, work->activeBox);
        } else if (w->to - 30 < count) {
            PCBox_SwapBoxAndPartyMon(work, &w->entries[0], &w->entries[1]);
        } else {
            PCBox_MoveBoxMonToParty(work, &w->entries[0]);
        }
    } else if (w->to < 30) {
        if (PCBox_GetSlotMonData(work, w->to, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
            PCBox_SwapBoxAndPartyMon(work, &w->entries[1], &w->entries[0]);
        } else {
            PCBox_MovePartyMonToBox(work, &w->entries[w->from - 30]);
        }
    } else if (w->to - 30 < count) {
        PCBox_SwapPartyMons(work, &w->entries[0]);
    } else {
        PCBox_MovePartyMonToEnd(work, &w->entries[w->from - 30]);
    }
}

u32 PCBox_GetTargetBoxId(PCBoxWork *work, u32 v) {
    return (v & 0x7F) + work->trayBox / 6 * 6;
}

u32 PCBox_PartyHasOtherUsableMon(PCBoxWork *work, u32 skip) {
    u32 i;
    Pokemon *mon;
    for (i = 0; i < Party_GetCount(work->party); i++) {
        if (i == skip) {
            continue;
        }
        mon = Party_GetMonByIndex(work->party, i);
        if (GetMonData(mon, MON_DATA_IS_EGG, NULL) != 0) {
            continue;
        }
        if (GetMonData(mon, MON_DATA_HP, NULL) == 0) {
            continue;
        }
        return 1;
    }
    return 0;
}

u32 PCBox_UpdateArceusGiratinaForm(BoxPokemon *boxmon) {
    u16 species = GetBoxMonData(boxmon, MON_DATA_SPECIES, NULL);
    u16 form;
    if (species == SPECIES_ARCEUS) {
        form = GetBoxMonData(boxmon, MON_DATA_FORM, NULL);
        BoxMon_UpdateArceusForm(boxmon);
        if (form != (u16)GetBoxMonData(boxmon, MON_DATA_FORM, NULL)) {
            return 1;
        }
    } else if (species == SPECIES_GIRATINA) {
        form = GetBoxMonData(boxmon, MON_DATA_FORM, NULL);
        BoxMon_UpdateGiratinaForm(boxmon);
        if (form != (u16)GetBoxMonData(boxmon, MON_DATA_FORM, NULL)) {
            return 1;
        }
    }
    return 0;
}

void PCBox_RevertShayminFormOnMove(PCBoxWork *work, u32 a, u32 b) {
    PCBoxView *q;
    if (a < 30 && b < 30) {
        return;
    }
    if (a >= 30 && b >= 30) {
        return;
    }
    if (a >= 30) {
        a = b;
    }
    if (PCBox_GetSlotMonData(work, a, MON_DATA_SPECIES, NULL) != SPECIES_SHAYMIN) {
        return;
    }
    if (PCBox_GetSlotMonData(work, a, MON_DATA_FORM, NULL) == 0) {
        return;
    }
    BoxMon_UpdateShayminForm(PCBox_GetSlotBoxMon(work, work->activeBox, a), 0);
    q = work->view;
    PCBox_RefreshSlotIcon(work, work->activeBox, a, q->slotIcons[a]);
    if (work->selectedSlot == a) {
        PCBox_ShowSlotMonInfo(work, a);
    }
}

u32 PCBox_AnimateMonMove(PCBoxWork *work) {
    s16 pos[2];
    u32 j;
    u32 i;
    u32 s;
    PCBoxMoveWork *w = work->view->temp;
    if (w->timer == 8) {
        for (i = 0; i < 7; i++) {
            if (w->entries[i].active != 0) {
                s = work->view->slotIcons[w->entries[i].from];
                if ((w->entries[i].to & 0x80) == 0) {
                    PCBox_GetSlotPosition(w->entries[i].to, &pos[1], &pos[0], work->partyLayout);
                } else {
                    if (w->entries[i].from >= 30) {
                        PCBox_GetSlotPosition(0x23, &pos[1], &pos[0], work->partyLayout);
                    } else {
                        PCBox_GetSlotPosition(w->entries[i].from, &pos[1], &pos[0], work->partyLayout);
                    }
                    PCBox_SetSpriteVisible(work->view, s, 0);
                }
                ManagedSprite_SetPositionXY(work->view->sprites[s], pos[1], pos[0]);
            }
        }
        return 0;
    }
    w->timer++;
    for (j = 0; j < 7; j++) {
        if (w->entries[j].active != 0) {
            s = work->view->slotIcons[w->entries[j].from];
            pos[1] = w->entries[j].px + (w->entries[j].dx * w->timer >> 16) * w->entries[j].sx;
            pos[0] = w->entries[j].py + (w->entries[j].dy * w->timer >> 16) * w->entries[j].sy;
            ManagedSprite_SetPositionXY(work->view->sprites[s], pos[1], pos[0]);
            PCBox_SetIconLayer(work->view, w->entries[j].from, 0);
        }
    }
    return 1;
}

u32 PCBox_AnimateSelectedSlotMoveEntry(PCBoxWork *work) {
    s16 pos[2];
    u32 i;
    u32 s;
    PCBoxMoveWork *w = work->view->temp;
    if (w->timer == 8) {
        for (i = 0; i < 7; i++) {
            if (w->entries[i].active != 0) {
                s = work->view->slotIcons[w->entries[i].from];
                PCBox_GetSlotPosition(w->entries[i].to, &pos[1], &pos[0], work->partyLayout);
                if (work->selectedSlot != w->entries[i].to) {
                    pos[0] += 0x90;
                }
                ManagedSprite_SetPositionXY(work->view->sprites[s], pos[1], pos[0]);
            }
        }
        return 0;
    }
    w->timer++;
    for (i = 0; i < 7; i++) {
        if (w->entries[i].active != 0 && work->selectedSlot == w->entries[i].to) {
            s = work->view->slotIcons[w->entries[i].from];
            pos[1] = w->entries[i].px + (w->entries[i].dx * w->timer >> 16) * w->entries[i].sx;
            pos[0] = w->entries[i].py + (w->entries[i].dy * w->timer >> 16) * w->entries[i].sy;
            ManagedSprite_SetPositionXY(work->view->sprites[s], pos[1], pos[0]);
            PCBox_SetIconLayer(work->view, w->entries[i].from, 0);
            break;
        }
    }
    return 1;
}

u32 PCBox_AnimateMoveEntries(PCBoxWork *work) {
    s16 pos[2];
    u32 j;
    u32 i;
    u32 s;
    PCBoxMoveWork *w = work->view->temp;
    if (w->timer == 8) {
        for (i = 0; i < 7; i++) {
            if (w->entries[i].active != 0) {
                s = work->view->slotIcons[w->entries[i].from];
                PCBox_GetSlotPosition(w->entries[i].to, &pos[1], &pos[0], work->partyLayout);
                ManagedSprite_SetPositionXY(work->view->sprites[s], pos[1], pos[0]);
            }
        }
        return 0;
    }
    w->timer++;
    for (j = 0; j < 7; j++) {
        if (w->entries[j].active != 0) {
            s = work->view->slotIcons[w->entries[j].from];
            pos[1] = w->entries[j].px + (w->entries[j].dx * w->timer >> 16) * w->entries[j].sx;
            pos[0] = w->entries[j].py + (w->entries[j].dy * w->timer >> 16) * w->entries[j].sy;
            ManagedSprite_SetPositionXY(work->view->sprites[s], pos[1], pos[0]);
            PCBox_SetMovingIconPriority(work->view, w->entries[j].from, w->entries[j].to);
        }
    }
    return 1;
}

u32 PCBox_CanMoveMon(PCBoxWork *work, u32 a, u32 b) {
    u32 box;
    u32 t;
    Pokemon *mon;
    PCBox_GetSlotMonData(work, b, MON_DATA_SPECIES_EXISTS, NULL);
    if ((b & 0x80) != 0) {
        box = PCBox_GetTargetBoxId(work, b);
        if (box == work->activeBox || PCStorage_CountMonsAndEggsInBox(work->storage, box) == 30) {
            return 0;
        }
        b ^= 0x80;
        t = 0;
    } else {
        box = work->activeBox;
        t = PCBox_GetSlotMonData(work, b, MON_DATA_SPECIES_EXISTS, NULL);
    }
    if (a >= 30) {
        if (PCBox_PartyHasOtherUsableMon(work, a - 30) == 0) {
            if (t == 0) {
                if (b < 30 || box != work->activeBox) {
                    return 0;
                }
            } else if (PCBox_GetSlotMonData(work, b, MON_DATA_IS_EGG, NULL) != 0 && b < 30) {
                return 0;
            }
        }
        if (b < 30) {
            mon = Party_GetMonByIndex(work->party, a - 30);
            if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) == TRUE) {
                return 0;
            }
            if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL) != 0) {
                return 0;
            }
        }
    } else if (b >= 30 && t != 0) {
        mon = Party_GetMonByIndex(work->party, b - 30);
        if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) == TRUE) {
            return 0;
        }
        if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL) != 0) {
            return 0;
        }
        if (PCBox_GetSlotMonData(work, a, MON_DATA_IS_EGG, NULL) != 0 && PCBox_PartyHasOtherUsableMon(work, b - 30) == 0) {
            return 0;
        }
    }
    return 1;
}

u32 PCBox_CanMovePartyMonToEmptySlot(PCBoxWork *work, u32 a, u32 b) {
    Pokemon *mon;
    if (PCBox_GetSlotMonData(work, b, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
        return 0;
    }
    if (PCBox_PartyHasOtherUsableMon(work, a - 30) == 0) {
        return 0;
    }
    mon = Party_GetMonByIndex(work->party, a - 30);
    if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) == TRUE) {
        return 0;
    }
    if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL) != 0) {
        return 0;
    }
    return 1;
}

u32 PCBox_CanSwapHeldItems(PCBoxWork *work, u32 a, u32 b) {
    u16 itemA;
    u16 itemB;
    if (PCBox_GetSlotMonData(work, b, MON_DATA_SPECIES_EXISTS, NULL) == 0) {
        return 0;
    }
    if (PCBox_GetSlotMonData(work, b, MON_DATA_IS_EGG, NULL) != 0) {
        return 0;
    }
    itemA = PCBox_GetSlotMonData(work, a, MON_DATA_HELD_ITEM, NULL);
    if (ItemIdIsMail(itemA) == TRUE) {
        return 0;
    }
    itemB = PCBox_GetSlotMonData(work, b, MON_DATA_HELD_ITEM, NULL);
    if (ItemIdIsMail(itemB) == TRUE) {
        return 0;
    }
    if (itemA == ITEM_GRISEOUS_ORB && PCBox_GetSlotMonData(work, b, MON_DATA_SPECIES, NULL) != SPECIES_GIRATINA) {
        return 0;
    }
    if (itemB == ITEM_GRISEOUS_ORB && PCBox_GetSlotMonData(work, a, MON_DATA_SPECIES, NULL) != SPECIES_GIRATINA) {
        return 0;
    }
    return 1;
}

void PCBox_SetupMoveEntryToSlot(PCBoxWork *work, PCBoxMoveEntry *e) {
    s16 pos[2];
    PCBoxView *view = work->view;
    ManagedSprite_GetPositionXY(view->sprites[view->slotIcons[e->from]], &e->px, &e->py);
    if ((e->to & 0x80) == 0) {
        PCBox_GetSlotPosition(e->to, &pos[1], &pos[0], work->partyLayout);
    } else {
        PCBox_GetTrayBoxPosition(work->view, e->to & 0x7F, &pos[1], &pos[0]);
    }
    if (e->px <= pos[1]) {
        e->sx = 1;
        e->dx = ((pos[1] - e->px) << 16) / 8;
    } else {
        e->sx = -1;
        e->dx = ((e->px - pos[1]) << 16) / 8;
    }
    if (e->py <= pos[0]) {
        e->sy = 1;
        e->dy = ((pos[0] - e->py) << 16) / 8;
    } else {
        e->sy = -1;
        e->dy = ((e->py - pos[0]) << 16) / 8;
    }
}

void PCBox_SetupMoveEntryToSprite(PCBoxWork *work, PCBoxMoveEntry *e) {
    s16 pos[2];
    PCBoxView *view = work->view;
    u32 j = view->slotIcons[e->to];
    ManagedSprite_GetPositionXY(view->sprites[view->slotIcons[e->from]], &e->px, &e->py);
    ManagedSprite_GetPositionXY(work->view->sprites[j], &pos[1], &pos[0]);
    if (e->px <= pos[1]) {
        e->sx = 1;
        e->dx = ((pos[1] - e->px) << 16) / 8;
    } else {
        e->sx = -1;
        e->dx = ((e->px - pos[1]) << 16) / 8;
    }
    if (e->py <= pos[0]) {
        e->sy = 1;
        e->dy = ((pos[0] - e->py) << 16) / 8;
    } else {
        e->sy = -1;
        e->dy = ((e->py - pos[0]) << 16) / 8;
    }
}

void PCBox_SetupMonSwapEntries(PCBoxWork *work, u32 a, u32 b) {
    PCBoxMoveWork *w = work->view->temp;
    w->entries[0].from = a;
    w->entries[0].to = b;
    w->entries[0].active = 1;
    PCBox_SetupMoveEntryToSlot(work, &w->entries[0]);
    w->entries[1].from = b;
    w->entries[1].to = a;
    w->entries[1].active = 1;
    PCBox_SetupMoveEntryToSlot(work, &w->entries[1]);
}

void PCBox_SetupMonMove(PCBoxWork *work, u32 a, u32 b) {
    PCBoxMoveWork *w;
    u32 sA;
    u32 sC;
    u32 sB;
    u32 i;
    u32 count;
    w = work->view->temp;
    work->view->showOutlineAfterMove = 1;
    for (i = 0; i < 7; i++) {
        w->entries[i].from = 0;
        w->entries[i].to = 0;
        w->entries[i].active = 0;
    }
    w->sel = b;
    if (b == 0xFF || a == b || PCBox_CanMoveMon(work, a, b) == 0) {
        w->from = 0xFF;
        w->to = 0xFF;
        w->entries[0].from = a;
        w->entries[0].to = a;
        w->entries[0].active = 1;
        PCBox_SetupMoveEntryToSlot(work, &w->entries[0]);
        return;
    }
    w->from = a;
    w->to = b;
    count = Party_GetCount(work->party);
    if ((b & 0x80) != 0) {
        if (a < 30) {
            w->entries[0].from = a;
            w->entries[0].to = b;
            w->entries[0].active = 1;
            PCBox_SetupMoveEntryToSlot(work, &w->entries[0]);
            work->view->showOutlineAfterMove = 0;
            return;
        }
        sA = a - 30;
        for (i = sA + 1; i < 6; i++) {
            w->entries[i].from = i + 30;
            w->entries[i].to = i + 29;
            w->entries[i].active = 1;
            PCBox_SetupMoveEntryToSlot(work, &w->entries[i]);
        }
        w->entries[sA].from = a;
        w->entries[sA].to = b;
        w->entries[sA].active = 1;
        PCBox_SetupMoveEntryToSlot(work, &w->entries[sA]);
        work->view->showOutlineAfterMove = 0;
        return;
    }
    if (a < 30) {
        if (b < 30) {
            PCBox_SetupMonSwapEntries(work, a, b);
            return;
        }
        if (b - 30 < count) {
            PCBox_SetupMonSwapEntries(work, a, b);
            return;
        }
        PCBox_SetupMonSwapEntries(work, a, count + 30);
        return;
    }
    if (b < 30) {
        if (PCBox_GetSlotMonData(work, b, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
            PCBox_SetupMonSwapEntries(work, a, b);
            return;
        }
        sB = a - 30;
        for (i = sB + 1; i < count; i++) {
            w->entries[i].from = i + 30;
            w->entries[i].to = i + 29;
            w->entries[i].active = 1;
            PCBox_SetupMoveEntryToSlot(work, &w->entries[i]);
        }
        w->entries[sB].from = a;
        w->entries[sB].to = b;
        w->entries[sB].active = 1;
        PCBox_SetupMoveEntryToSlot(work, &w->entries[sB]);
        w->entries[count].from = b;
        w->entries[count].to = count + 29;
        w->entries[count].active = 1;
        PCBox_SetupMoveEntryToSlot(work, &w->entries[count]);
        return;
    }
    if (b - 30 < count) {
        PCBox_SetupMonSwapEntries(work, a, b);
        return;
    }
    sC = a - 30;
    for (i = sC + 1; i < count; i++) {
        w->entries[i].from = i + 30;
        w->entries[i].to = i + 29;
        w->entries[i].active = 1;
        PCBox_SetupMoveEntryToSlot(work, &w->entries[i]);
    }
    w->entries[sC].from = a;
    w->entries[sC].to = count + 29;
    w->entries[sC].active = 1;
    PCBox_SetupMoveEntryToSlot(work, &w->entries[sC]);
}

void PCBox_SetupDepositMoveEntries(PCBoxWork *work, u32 a, u32 b) {
    PCBoxMoveWork *w;
    u32 slot;
    u32 i;
    u32 count;
    w = work->view->temp;
    work->view->showOutlineAfterMove = 1;
    for (i = 0; i < 7; i++) {
        w->entries[i].from = 0;
        w->entries[i].to = 0;
        w->entries[i].active = 0;
    }
    if (b == 0xFF || PCBox_CanMovePartyMonToEmptySlot(work, a, b) == 0) {
        w->from = 0xFF;
        w->to = 0xFF;
        w->entries[0].from = a;
        w->entries[0].to = a;
        w->entries[0].active = 1;
        PCBox_SetupMoveEntryToSlot(work, &w->entries[0]);
        return;
    }
    w->from = a;
    w->to = b;
    count = Party_GetCount(work->party);
    if (b < 30) {
        slot = a - 30;
        for (i = slot + 1; i < count; i++) {
            w->entries[i].from = i + 30;
            w->entries[i].to = i + 29;
            w->entries[i].active = 1;
            PCBox_SetupMoveEntryToSprite(work, &w->entries[i]);
        }
        w->entries[slot].from = a;
        w->entries[slot].to = b;
        w->entries[slot].active = 1;
        PCBox_SetupMoveEntryToSprite(work, &w->entries[slot]);
        w->entries[count].from = b;
        w->entries[count].to = count + 29;
        w->entries[count].active = 1;
        PCBox_SetupMoveEntryToSprite(work, &w->entries[count]);
        work->view->showOutlineAfterMove = 0;
    }
}

void PCBox_SetupMonMoveToPartyEnd(PCBoxWork *work, u32 a, u32 b) {
    PCBoxView *view = work->view;
    PCBoxMoveWork *w = view->temp;
    u32 i;
    view->showOutlineAfterMove = 1;
    for (i = 0; i < 7; i++) {
        w->entries[i].from = 0;
        w->entries[i].to = 0;
        w->entries[i].active = 0;
    }
    if (b == 0xFF || PCBox_GetSlotMonData(work, b, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
        w->from = 0xFF;
        w->to = 0xFF;
        w->entries[0].from = a;
        w->entries[0].to = a;
        w->entries[0].active = 1;
        PCBox_SetupMoveEntryToSlot(work, &w->entries[0]);
    } else {
        w->from = a;
        w->to = b;
        PCBox_SetupMonSwapEntries(work, a, Party_GetCount(work->party) + 30);
    }
}

u32 PCBox_GetMoveEntryTarget(PCBoxWork *work, u32 v) {
    PCBoxMoveWork *w = work->view->temp;
    u32 i;
    for (i = 0; i < 7; i++) {
        if (w->entries[i].active == 1 && w->entries[i].from == v) {
            return w->entries[i].to;
        }
    }
    return 0xFF;
}

void PCBox_SetupPartyShiftAfterRelease(PCBoxWork *work, u32 a) {
    PCBoxMoveWork *w = work->view->temp;
    u32 i;
    u32 slot = a - 30;
    for (i = 0; i < 6; i++) {
        w->entries[i].from = i + 30;
        w->entries[i].to = i + 30;
        w->entries[i].active = 0;
    }
    w->entries[i].active = 0;
    for (i = slot + 1; i < 6; i++) {
        w->entries[i].to = i + 29;
        w->entries[i].active = 1;
        PCBox_SetupMoveEntryToSlot(work, &w->entries[i]);
    }
    w->entries[slot].to = 0x23;
    w->entries[slot].active = 1;
    PCBox_SetupMoveEntryToSlot(work, &w->entries[slot]);
}

void PCBox_ApplyMoveEntryIcons(PCBoxWork *work, PCBoxMoveEntry *e) {
    u8 tmp[7];
    u32 i;
    for (i = 0; i < 7; i++) {
        if (e[i].active != 0) {
            tmp[i] = work->view->slotIcons[e[i].from];
        }
    }
    for (i = 0; i < 7; i++) {
        if (e[i].active != 0) {
            if ((e[i].to & 0x80) == 0) {
                work->view->slotIcons[e[i].to] = tmp[i];
                PCBox_SetIconLayer(work->view, e[i].to, 1);
            } else {
                if (e[i].from >= 30) {
                    work->view->slotIcons[35] = tmp[i];
                }
                PCBox_SetIconLayer(work->view, e[i].from, 1);
            }
        }
    }
}

s32 PCBox_GetReleaseCheckMoveIndex(u16 v) {
    u32 i;
    for (i = 0; i < 4; i++) {
        if (v == sPCBoxReleaseCheckMoves[i]) {
            return i;
        }
    }
    return -1;
}

void PCBox_StartReleaseMoveCheck(PCBoxWork *work) {
    PCBoxView *q;
    PCBoxReleaseWork *r;
    BoxPokemon *boxmon;
    u32 i;
    s32 idx;
    work->view->releaseWork = Heap_AllocAtEnd(HEAP_ID_10, sizeof(PCBoxReleaseWork));
    q = work->view;
    r = q->releaseWork;
    r->sprite = q->sprites[q->slotIcons[work->selectedSlot]];
    r->scanIndex = 0;
    r->moveFlags = 0;
    boxmon = PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot);
    for (i = 0; i < 4; i++) {
        idx = PCBox_GetReleaseCheckMoveIndex((u16)GetBoxMonData(boxmon, MON_DATA_MOVE1 + i, NULL));
        if (idx != -1) {
            r->moveFlags |= 1 << idx;
            return;
        }
    }
}

void PCBox_FreeReleaseMoveCheck(PCBoxWork *work) {
    Heap_Free(work->view->releaseWork);
}

u32 PCBox_RunReleaseMoveCheck(PCBoxWork *work) {
    PCBoxReleaseWork *r;
    u16 idx;
    u32 count;
    BoxPokemon *boxmon;
    u32 i;
    s32 bit;
    u32 boxId;
    u32 slot;

    r = work->view->releaseWork;
    idx = r->scanIndex;
    if (idx == 0x222) {
        return 0;
    }
    count = 0;
    do {
        if (idx < 0x21C) {
            boxId = idx / 30;
            slot = idx % 30;
            if (boxId == work->activeBox && slot == work->selectedSlot) {
                boxmon = NULL;
            } else {
                boxmon = PCBox_GetSlotBoxMon(work, boxId, slot);
            }
        } else {
            slot = idx - 0x21C;
            if (slot == work->selectedSlot - 30) {
                boxmon = NULL;
            } else {
                boxmon = PCBox_GetSlotBoxMon(work, 0xFF, slot);
            }
        }
        if (boxmon != NULL && GetBoxMonData(boxmon, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
            for (i = 0; i < 4; i++) {
                bit = PCBox_GetReleaseCheckMoveIndex((u16)GetBoxMonData(boxmon, MON_DATA_MOVE1 + i, NULL));
                if (bit != -1) {
                    r->moveFlags &= 0xFF ^ (1 << bit);
                }
            }
        }
        r->scanIndex++;
        idx = r->scanIndex;
        if (idx == 0x222) {
            return 0;
        }
        count++;
    } while (count < 15);
    return 1;
}

PCBoxMonInfo *PCBox_CreateMonInfo(BoxPokemon *boxMon) {
    PCBoxMonInfo *monInfo;
    u32 i;

    if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL) != 0) {
        monInfo = Heap_Alloc(HEAP_ID_10, sizeof(PCBoxMonInfo));
        monInfo->boxMon = boxMon;
        monInfo->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        monInfo->heldItem = GetBoxMonData(boxMon, MON_DATA_HELD_ITEM, NULL);
        monInfo->personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL);
        monInfo->type1 = GetBoxMonData(boxMon, MON_DATA_TYPE_1, NULL);
        monInfo->type2 = GetBoxMonData(boxMon, MON_DATA_TYPE_2, NULL);
        monInfo->ability = GetBoxMonData(boxMon, MON_DATA_ABILITY, NULL);
        monInfo->nature = GetBoxMonNature(boxMon);
        monInfo->markings = GetBoxMonData(boxMon, MON_DATA_MARKINGS, NULL);
        monInfo->level = GetBoxMonData(boxMon, MON_DATA_LEVEL, NULL);
        monInfo->isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
        monInfo->gender = GetBoxMonGender(boxMon);
        if (monInfo->species != SPECIES_NIDORAN_F && monInfo->species != SPECIES_NIDORAN_M && monInfo->isEgg == 0) {
            monInfo->showGender = 1;
        } else {
            monInfo->showGender = 0;
        }
        for (i = 0; i < 4; i++) {
            monInfo->moves[i] = GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, NULL);
        }
    } else {
        monInfo = NULL;
    }
    return monInfo;
}

void PCBox_FreeMonInfo(PCBoxMonInfo *monInfo) {
    Heap_Free(monInfo);
}

void PCBox_DrawMonInfo(PCBoxWork *work, PCBoxMonInfo *monInfo) {
    u32 base = PCBox_DrawMoveWindows(work, monInfo);
    u32 i;

    for (i = 0; i < 4; i++) {
        BgCellManager_PutWindowTiles(work->view->panels, 0x10, &work->view->windows[base + i]);
    }
    PCBox_HideMovesPanel(work->view->panels);
    PCBox_ShowMonPic(work, monInfo, 2);
    PCBox_SetSpriteVisible(work->view, (work->view->monPicBuffer ^ 1) + 2, 1);
    PCBox_SetSpriteVisible(work->view, work->view->monPicBuffer + 2, 0);
    PCBox_DrawTypeIcons(work->view, monInfo);
    PCBox_PrintMonInfo(work, monInfo);
    PCBox_DrawMarkings(work, monInfo->markings);
}

void PCBox_DrawMonItemInfo(PCBoxWork *work, PCBoxMonInfo *monInfo) {
    u32 base;

    if (monInfo->heldItem != ITEM_NONE) {
        base = PCBox_PrintItemNameAndDesc(work, monInfo->heldItem);
        BgCellManager_PutWindowTiles(work->view->panels, 0x10, &work->view->windows[base]);
        BgCellManager_PutWindowTiles(work->view->panels, 0x10, &work->view->windows[base + 1]);
        PCBox_LoadInfoItemIcon(work->view, monInfo->heldItem);
    }
    PCBox_HideItemInfoPanel(work->view->panels);
    PCBox_ShowMonPic(work, monInfo, 2);
    PCBox_SetSpriteVisible(work->view, (work->view->monPicBuffer ^ 1) + 2, 1);
    PCBox_SetSpriteVisible(work->view, work->view->monPicBuffer + 2, 0);
    PCBox_DrawTypeIcons(work->view, monInfo);
    PCBox_PrintMonInfo(work, monInfo);
    PCBox_DrawMarkings(work, monInfo->markings);
}

BOOL PCBox_ShowSlotMonInfo(PCBoxWork *work, u32 index) {
    PCBoxMonInfo *monInfo;
    u32 lock;
    BoxPokemon *boxMon = PCBox_GetSlotBoxMon(work, work->activeBox, index);

    if (boxMon != NULL) {
        lock = AcquireBoxMonLock(boxMon);
        monInfo = PCBox_CreateMonInfo(boxMon);
        if (monInfo != NULL) {
            if (work->args->mode == PC_BOX_MODE_MOVE_ITEMS) {
                PCBox_DrawMonItemInfo(work, monInfo);
            } else {
                PCBox_DrawMonInfo(work, monInfo);
            }
            PCBox_FreeMonInfo(monInfo);
        } else {
            PCBox_ClearMonInfo(work);
            ReleaseBoxMonLock(boxMon, lock);
            return FALSE;
        }
        ReleaseBoxMonLock(boxMon, lock);
    } else {
        PCBox_ClearMonInfo(work);
        return FALSE;
    }
    return TRUE;
}

void PCBox_RedrawSlotMonInfo(PCBoxWork *work, u32 index) {
    BoxPokemon *boxMon = PCBox_GetSlotBoxMon(work, work->activeBox, index);
    u32 lock = AcquireBoxMonLock(boxMon);
    PCBoxMonInfo *monInfo = PCBox_CreateMonInfo(boxMon);

    PCBox_ShowMonPic(work, monInfo, 2);
    PCBox_SetSpriteVisible(work->view, (work->view->monPicBuffer ^ 1) + 2, 1);
    PCBox_SetSpriteVisible(work->view, work->view->monPicBuffer + 2, 0);
    PCBox_DrawTypeIcons(work->view, monInfo);
    PCBox_PrintMonInfo(work, monInfo);
    PCBox_FreeMonInfo(monInfo);
    ReleaseBoxMonLock(boxMon, lock);
}

void PCBox_ClearMonInfo(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 2, 0);
    PCBox_SetSpriteVisible(work->view, 3, 0);
    PCBox_SetSpriteVisible(work->view, 0xd, 0);
    PCBox_SetSpriteVisible(work->view, 0xe, 0);
    PCBox_ClearMonInfoWindows(work->view);
    PCBox_DrawMarkings(work, 0);
    if (work->args->mode == PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_HideItemInfoPanel(work->view->panels);
    } else {
        PCBox_HideMovesPanel(work->view->panels);
    }
}

void PCBox_ClearMonInfoAndSelection(PCBoxWork *work) {
    PCBox_ClearMonInfo(work);
    PCBox_SetSelectedIconOutlineVisible(work, 0);
    work->selectedSlot = 0xff;
}

void PCBox_LoadWallpaperChars(PCBoxWork *work, s32 member, u32 unused, u32 offset) {
    NNSG2dCharacterData *data;
    void *allocation = GfGfxLoader_GetCharData(NARC_a_0_1_9, member + 0x10, TRUE, &data, HEAP_ID_10);

    BG_LoadCharTilesData(work->view->bgConfig, GF_BG_LYR_MAIN_3, data->pRawData, data->szByte, offset);
    PCBox_DrawBoxNameOnWallpaper(work, (u8 *)data->pRawData + 0x2a0, offset + 0x15, 0x15, 3);
    Heap_Free(allocation);
}

void PCBox_LoadWallpaperPalette(PCBoxWork *work, s32 member, u32 palette) {
    GfGfxLoader_GXLoadPal(NARC_a_0_1_9, member + 0x28, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)(palette << 5), 0x20, HEAP_ID_10);
}

void PCBox_LoadWallpaperTilemap(PCBoxWork *work, s32 unused, u32 tile, u32 addend, u32 palette) {
    NNSG2dScreenData *screen;
    void *allocation;
    u16 *raw;
    u8 row;
    u32 x;
    u8 column;
    u16 *src;
    u16 value;

    allocation = GfGfxLoader_GetScrnData(NARC_a_0_1_9, 0xf, TRUE, &screen, HEAP_ID_10);
    raw = (u16 *)screen->rawData;
    for (row = 0; row < 0x14; row++) {
        src = raw + row * 0x15;
        x = tile;
        column = 0;
        do {
            value = (src[column] & 0xfff) + (palette << 12) + addend;
            LoadRectToBgTilemapRect(work->view->bgConfig, GF_BG_LYR_MAIN_3, &value, x, row + 1, 1, 1);
            x++;
            if (x >= 0x40) {
                x = 0;
            }
            column++;
        } while (column < 0x15);
    }
    Heap_Free(allocation);
    FillBgTilemapRect(work->view->bgConfig, GF_BG_LYR_MAIN_3, 0x1001, x, 0, 2, 0x14, 0x11);
}

void PCBox_LoadWallpaper(PCBoxWork *work, s32 member, u32 a2, u32 a3, u32 palette) {
    PCBox_LoadWallpaperChars(work, member, a2, a3);
    PCBox_LoadWallpaperPalette(work, member, palette);
    PCBox_LoadWallpaperTilemap(work, member, a2, a3, palette);
    ScheduleBgTilemapBufferTransfer(work->view->bgConfig, GF_BG_LYR_MAIN_3);
}

void PCBox_LoadScrolledWallpaper(PCBoxWork *work, s32 member, u32 direction) {
    u32 x;
    u32 tile;

    if (direction == 0) {
        work->view->wallpaperScroll -= 0x17;
        if (work->view->wallpaperScroll < 0) {
            work->view->wallpaperScroll += 0x40;
        }
    } else if (direction == 1) {
        work->view->wallpaperScroll += 0x17;
        if (work->view->wallpaperScroll >= 0x40) {
            work->view->wallpaperScroll -= 0x40;
        }
    }
    if (work->view->wallpaperBuffer == 0) {
        x = 0x25c;
        tile = 0xe;
    } else {
        x = 0xb8;
        tile = 0xf;
    }
    work->view->wallpaperBuffer ^= 1;
    PCBox_LoadWallpaper(work, member, work->view->wallpaperScroll, x, tile);
}

void PCBox_SwapWallpaper(PCBoxWork *work, s32 member) {
    u32 x;
    u32 tile;

    if (work->view->wallpaperBuffer == 0) {
        x = 0x25c;
        tile = 0xe;
    } else {
        x = 0xb8;
        tile = 0xf;
    }
    work->view->wallpaperBuffer ^= 1;
    PCBox_LoadWallpaperChars(work, member, work->view->wallpaperScroll, x);
    PaletteData_LoadNarc(work->view->paletteData, NARC_a_0_1_9, member + 0x28, HEAP_ID_10, PLTTBUF_MAIN_BG, 0x20, tile << 4);
    PCBox_LoadWallpaperTilemap(work, member, work->view->wallpaperScroll, x, tile);
    ScheduleBgTilemapBufferTransfer(work->view->bgConfig, GF_BG_LYR_MAIN_3);
}

u32 PCBox_GetWallpaperGfxIndex(PCBoxWork *work, u32 box) {
    u32 v = PCStorage_GetBoxWallpaper(work->storage, box);
    if (v >= 0x10) {
        v -= 0x10;
    }
    return v;
}

u32 PCBox_IsPointInRect(s32 x, s32 y, const u8 *r) {
    if (x >= r[0] && x <= r[1] && y >= r[2] && y < r[3]) {
        return 1;
    }
    return 0;
}

u32 PCBox_GetTouchedBoxSlot(s16 x, s16 y) {
    if (PCBox_IsPointInRect(x, y, sPCBoxSlotGridRect) == 1) {
        if (x < 12) {
            x = 0;
        } else if (x >= 156) {
            x = 5;
        } else {
            x = (s16)((x - 12) / 24);
        }
        return x + (s16)((y - 40) / 24) * 6;
    }
    return 0xff;
}

u32 PCBox_GetTouchedPartySlot(s16 x, s16 y, const u8 *rect) {
    u32 i;
    for (i = 0; i < 6; i++, rect += 4) {
        if (PCBox_IsPointInRect(x, y, rect) == 1) {
            return i + 0x1e;
        }
    }
    return 0xff;
}

u32 PCBox_GetTouchedTrayBox(s16 x, s16 y) {
    u32 i;
    const u8 *rect = sPCBoxTrayRects;
    for (i = 0; i < 6; i++, rect += 4) {
        if (PCBox_IsPointInRect(x, y, rect) == 1) {
            return i + 0x80;
        }
    }
    return 0xff;
}
