#include "global.h"

#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "application/pc_box/pc_box_internal.h"

#include "bag.h"
#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "palette.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "yes_no_prompt.h"

typedef u32 (*PCBoxStateFunc)(PCBoxWork *work);

// Sub-apps run from the box (summary, bag, naming screen): start, end, state to return to.
typedef struct PCBoxSubApp {
    u32 (*start)(PCBoxWork *work);
    u32 (*end)(PCBoxWork *work);
    u32 nextState;
} PCBoxSubApp;

extern const PCBoxStateFunc sPCBoxStateTable[];
// {yes, no} handler pairs of the yes/no prompts; the asm labels the "no" column separately (ov14_021F7D78).
extern const PCBoxStateFunc sPCBoxYesNoHandlers[];
extern const PCBoxStateFunc ov14_021F7D78[];
extern const PCBoxSubApp sPCBoxSubApps[];
extern const PCBoxMenuEntry ov14_021F7D1C[];
extern const PCBoxMenuEntry ov14_021F7D2C[];
extern const PCBoxMenuEntry ov14_021F7D3C[];

#pragma force_active on

u32 PCBox_State_Init(PCBoxWork *work) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(GX_PLANEMASK_NONE);
    GfGfx_EngineBSetPlanes(GX_PLANEMASK_NONE);
    G2_BlendNone();
    G2S_BlendNone();
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    Heap_Create(HEAP_ID_3, HEAP_ID_10, 0x80000);
    work->view = Heap_Alloc(HEAP_ID_10, sizeof(PCBoxView));
    MI_CpuFill8(work->view, 0, sizeof(PCBoxView));
    work->view->personalNarc = NARC_New(NARC_poketool_personal_personal, HEAP_ID_10);
    work->view->iconNarc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_10);
    PCBox_SetGfxBanks();
    PCBox_InitBgLayers(work);
    PCBox_CreatePaletteData(work);
    PCBox_LoadBgGraphics(work);
    PCBox_CreateMsgResources(work);
    PCBox_LoadWindowGraphics(work);
    PCBox_AddWindows(work);
    ov14_021F297C(work);
    PCBox_LoadBoxIcons(work);
    PCBox_LoadPartyIcons(work);
    PCBox_LoadScrolledWallpaper(work, PCBox_GetWallpaperGfxIndex(work, work->activeBox), 2);
    PCBox_InitPanels(work);
    if (work->args->unk8 != PC_BOX_MODE_WITHDRAW && work->args->unk8 != PC_BOX_MODE_DEPOSIT) {
        PCBox_ShowPartyButton(work);
        ov14_021E825C(work);
    }
    ov14_021E82BC(work);
    PCBox_CreateYesNoPrompt(work);
    ov14_021F5620(work);
    PCBox_PrintInfoLabels(work);
    PCBox_DrawAllBoxThumbnails(work);
    PCBox_CreateModeGridInput(work);
    work->view->vblankTask = SysTask_CreateOnVBlankQueue(PCBox_VBlankCallback, work, 0);
    PCBox_SetBlendEnabled(1);
    return work->nextState;
}

#pragma force_active reset

u32 PCBox_State_Exit(PCBoxWork *work) {
    PCBox_SaveMenuInputState(work);
    SysTask_Destroy(work->view->vblankTask);
    PCBox_FreeGridInput(work);
    PCBox_DestroyYesNoPrompt(work);
    PCBox_FreePanels(work->view);
    ov14_021F29AC(work->view);
    PCBox_RemoveWindows(work);
    PCBox_DestroyMsgResources(work);
    PCBox_FreePaletteData(work);
    PCBox_FreeBgLayers(work);
    NARC_Delete(work->view->iconNarc);
    NARC_Delete(work->view->personalNarc);
    GF_TouchpadStopSampling();
    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04001050 = 0;
    GfGfx_EngineASetPlanes(0);
    GfGfx_EngineBSetPlanes(0);
    Heap_Free(work->view);
    Heap_Destroy(HEAP_ID_10);
    return work->nextState;
}

u32 PCBox_State_WaitFade(PCBoxWork *work) {
    if (IsPaletteFadeFinished() == 1) {
        return work->view->fadeNextState;
    }
    return 2;
}

u32 PCBox_State_WaitPaletteBuffers(PCBoxWork *work) {
    if (PaletteData_GetSelectedBuffersBitmask(work->view->paletteData) == 0) {
        return work->nextState;
    }
    return 3;
}

u32 PCBox_State_Delay(PCBoxWork *work) {
    u32 delay = work->view->delay;

    if (delay == 0) {
        return work->nextState;
    }
    work->view->delay = delay - 1;
    return 4;
}

u32 PCBox_State_WaitUpdateFunc(PCBoxWork *work) {
    if (work->view->updateFunc == NULL) {
        return sPCBoxStateTable[work->nextState](work);
    }
    return 5;
}

u32 PCBox_State_WaitButton(PCBoxWork *work) {
    if (System_GetTouchNew() == TRUE) {
        PlaySE(SEQ_SE_DP_DECIDE);
        return work->nextState;
    }
    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) != 0) {
        PlaySE(SEQ_SE_DP_DECIDE);
        return work->nextState;
    }
    return 6;
}

u32 PCBox_State_HandleYesNo(PCBoxWork *work) {
    int input = YesNoPrompt_HandleInput(work->view->yesNoPrompt);

    switch (input) {
    case YESNORESPONSE_YES:
        YesNoPrompt_Reset(work->view->yesNoPrompt);
        return sPCBoxYesNoHandlers[work->view->yesNoHandler * 2](work);
    case YESNORESPONSE_NO:
        YesNoPrompt_Reset(work->view->yesNoPrompt);
        return ov14_021F7D78[work->view->yesNoHandler * 2](work);
    default:
        return 7;
    }
}

u32 PCBox_State_WaitPaletteFlash(PCBoxWork *work) {
    if (PCBox_AnimatePaletteFlash(work) == 0) {
        return work->nextState;
    }
    return 8;
}

u32 PCBox_State_StartSubApp(PCBoxWork *work) {
    sPCBoxSubApps[work->subAppIndex].start(work);
    return 10;
}

u32 PCBox_State_RunSubApp(PCBoxWork *work) {
    if (!OverlayManager_Run(work->subApp)) {
        return 10;
    }
    OverlayManager_Delete(work->subApp);
    sPCBoxSubApps[work->subAppIndex].end(work);
    work->nextState = sPCBoxSubApps[work->subAppIndex].nextState;
    return 0;
}

void PCBox_State_Start(PCBoxWork *work) {
    u32 msg;

    PlaySE(SEQ_SE_DP_PC_LOGIN);
    switch (work->args->unk8) {
    case 0:
        ov14_021F0BF4(work);
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
        PCBox_SetBoxArrowsVisible(work->view, 0);
        PCBox_SetIconsTranslucent(work, 1, 1);
        PCBox_ShowSlotMonInfo(work, 0x1e);
        msg = 0x5b;
        break;
    case 1:
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
        PCBox_ShowSlotMonInfo(work, 0);
        msg = 0x51;
        break;
    case 2:
        PCBox_ShowSlotMonInfo(work, 0);
        msg = 0xc;
        break;
    case 3:
        PCBox_SetIconsTranslucent(work, 0x81, 1);
        PCBox_ShowSlotMonInfo(work, 0);
        msg = 0x75;
        break;
    }
    PCBox_StartFadeIn(work, msg);
}

u32 PCBox_State_MoveModeInput(PCBoxWork *work) {
    u32 input = PCBox_FindTouchedBoxSlot();

    if (input != -1) {
        if (PCBox_GetSlotMonData(work, input, MON_DATA_SPECIES_EXISTS, 0) != 0) {
            PlaySE(SEQ_SE_DP_BOX02);
            System_GetTouchNewCoords((u32 *)(&work->view->touchX),
                (u32 *)(&work->view->touchY));
            if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
                PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            }
            PCBox_ShowSlotMonInfo(work, input);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return PCBox_StartMoveModeBoxDrag(work, input);
        }
        if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
            u32 old = work->selectedSlot;
            u32 next = GridInputHandler_GetNextInput(work->view->gridInput);
            PCBox_StartCursorSlide(work->view, old, next);
            GridInputHandler_SetNextInput(work->view->gridInput, old);
            PCBox_SlideOutActionMenu(work->view->panels);
            PCBox_SlideInPartyButton(work->view->panels);
            ov14_021E82A8(work->view->panels);
            ov14_021E8328(work->view->panels);
            ov14_021E884C(work->view);
            PCBox_SetSelectedIconOutlineVisible(work, 0);
            return PCBox_WaitForUpdateFunc(work, (void *)ov14_021EA180, 0x4a);
        }
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)input);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        PCBox_ClearMonInfo(work);
        return 0xc;
    }

    input = PCBox_GetMoveModeMenuInput(work);
    switch (input) {
    case 30:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F1128(work);
    case 31:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(work->view, 9, 0);
        GridInputHandler_SetNextInput(work->view->gridInput, 0x1e);
        return PCBox_ScrollToPrevBox(work, 0xc);
    case 32:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(work->view, 9, 0);
        GridInputHandler_SetNextInput(work->view->gridInput, 0x1e);
        return PCBox_ScrollToNextBox(work, 0xc);
    case 33:
        PlaySE(SEQ_SE_DP_OPEN7);
        return PCBox_FlashPanel(work, 8, 0x95);
    case 34:
        PlaySE(SEQ_SE_DP_OPEN7);
        work->unk_2A = 0;
        work->unk_2B = 0;
        return ov14_021F2330(work, 9, 0x96);
    case 36:
        PlaySE(SEQ_SE_DP_DECIDE);
        if (System_GetTouchNew() == 0) {
            work->unk_2A = 1;
        }
        work->unk_2B = work->selectedSlot;
        return ov14_021F2330(work, 3, 0xb2);
    case 37:
        PlaySE(SEQ_SE_DP_DECIDE);
        work->unk_2C = 0x25;
        return PCBox_FlashPanel(work, 4, 0x97);
    case 38:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 5, 0x98);
    case 39:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 6, 0x99);
    case 40:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 7, 0x9b);
    case 41: {
        u32 old;
        PlaySE(SEQ_SE_DP_SELECT);
        old = work->selectedSlot;
        PCBox_StartCursorSlide(work->view, old, GridInputHandler_GetNextInput(work->view->gridInput));
        GridInputHandler_SetNextInput(work->view->gridInput, old);
        return PCBox_FlashPanel(work, 0xb, 0x9c);
    }
    case -1: {
        u32 next = GridInputHandler_GetNextInput(work->view->gridInput);
        if (next == 0x1e) {
            if (gSystem.newKeys & 0x20) {
                PlaySE(SEQ_SE_DP_SELECT);
                PCBox_ClearMonInfoAndSelection(work);
                return PCBox_ScrollToPrevBox(work, 0xc);
            }
            if (gSystem.newKeys & 0x10) {
                PlaySE(SEQ_SE_DP_SELECT);
                PCBox_ClearMonInfoAndSelection(work);
                return PCBox_ScrollToNextBox(work, 0xc);
            }
        }
        break;
    }
    case -3: {
        u32 next = GridInputHandler_GetNextInput(work->view->gridInput);
        if (next < 0x1e) {
            PCBox_ShowSlotMonInfo(work, next);
        } else if (next != 0x24 && next != 0x25 && next != 0x26 && next != 0x27 && next != 0x28 && next != 0x29) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_WaitForPendingUpdateFunc(work, 0x4a);
    }
    case 35:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfo(work);
        return PCBox_FlashPanel(work, 10, 0x93);
    case -2:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 10, 0x94);
    case -4: {
        u32 next = GridInputHandler_GetNextInput(work->view->gridInput);
        if (next < 0x1e) {
            PCBox_ShowSlotMonInfo(work, next);
        } else if (next != 0x24 && next != 0x25 && next != 0x26 && next != 0x27 && next != 0x28 && next != 0x29) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
    default:
        if (PCBox_GetSlotMonData(work, input, MON_DATA_SPECIES_EXISTS, 0) != 0) {
            PlaySE(SEQ_SE_DP_DECIDE);
            PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            PCBox_ShowSlotMonInfo(work, input);
            PCBox_StartCursorSlide(work->view, 0x24, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, 0x24);
            return ov14_021F0530(work, input);
        }
        break;
    }
    return 0xc;
}

u32 PCBox_State_ApplyMonMove(PCBoxWork *work) {
    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_ApplyMonMove(work);
    PCBox_FreeMoveWork(work);
    GridInputHandler_SetNextInput(work->view->gridInput, 0x24);
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0xc;
}

void ov14_021EB7E4(PCBoxWork *work) {
    u32 msg;
    u8 *field;

    ov14_021F6688(work->view, 0x25);
    switch (work->args->unk8) {
    case 1:
        PCBox_DrawActionMenu(work, ov14_021F7D2C, 4);
        PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
        msg = 0x5a;
        break;
    case 0:
        PCBox_DrawActionMenu(work, ov14_021F7D1C, 4);
        PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
        msg = 0x72;
        break;
    case 2:
    default:
        PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
        msg = work->selectedSlot < 0x1e ? 0x4d : 0x4e;
        break;
    }
    field = ((u8 *)work->view + 0x44e);
    if (((u32)(*field << 24) >> 31) == 1) {
        *field &= ~0x80;
        if (work->selectedSlot >= 0x1e) {
            PCBox_SetIconsTranslucent(work, 2, 0);
        } else {
            PCBox_SetBoxArrowsVisible(work->view, 1);
            PCBox_SetIconsTranslucent(work, 1, 0);
        }
    }
    PCBox_SlideInActionMenu(work->view->panels);
    PCBox_WaitForUpdateFunc(work, ov14_021E9434, msg);
}

void ov14_021EB8C0(PCBoxWork *work) {
    u32 msg;

    PCBox_ShowSlotMonInfo(work, work->selectedSlot);
    if (work->unk_26 == 0) {
        if (work->args->unk8 == PC_BOX_MODE_WITHDRAW) {
            PCBox_DrawActionMenu(work, ov14_021F7D2C, 4);
        } else if (work->args->unk8 == PC_BOX_MODE_DEPOSIT) {
            PCBox_SetBoxArrowsVisible(work->view, 0);
            PCBox_SetIconsTranslucent(work, 1, 1);
            PCBox_DrawActionMenu(work, ov14_021F7D1C, 4);
        } else {
            if (work->selectedSlot >= 0x1e) {
                PCBox_SetIconsTranslucent(work, 1, 1);
                if (work->trayOpen != 0) {
                    ov14_021E8664(work);
                }
            } else if (work->trayOpen != 0) {
                ov14_021E8664(work);
            }
            PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            ov14_021E87F4(work);
        }
        ov14_021E83C4(work->view->panels);
        if (work->selectedSlot < 0x1e) {
            if (work->args->unk8 == PC_BOX_MODE_WITHDRAW) {
                PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
                msg = 0x51;
            } else {
                msg = 0xc;
            }
        } else if (work->args->unk8 == PC_BOX_MODE_DEPOSIT) {
            ov14_021F0BF4(work);
            PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
            msg = 0x5b;
        } else {
            ov14_021F0B70(work);
            PCBox_SetBoxArrowsVisible(work->view, 0);
            PCBox_CreateGridInput(work, 5, 9);
            msg = 0x24;
        }
        PCBox_CreateSelectedIconOutline(work);
        if (work->args->unk8 != PC_BOX_MODE_MOVE_ITEMS) {
            ov14_021E8874(work->view);
        }
    } else {
        ov14_021F0BB4(work);
        PCBox_ShowSummaryButton(work->view->panels);
        PCBox_SetupBoxTraySprites(work);
        PCBox_UpdateCurrentBoxMarker(work);
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        PCBox_SlideInBoxTray(work->view->panels);
        while (PCBox_UpdateBoxTraySlideIn(work) != 0) {
        }
        if (work->unk_27 == 0) {
            PCBox_CreateGridInput(work, 4, work->selectedSlot);
        } else {
            PCBox_CreateGridInput(work, 4, work->unk_28);
            work->view->unk_044B = 1;
            ov14_021EA1F0(work);
        }
        work->view->unk_043C = work->trayBox % 6 + 0x25;
        if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
            PCBox_SetIconsTranslucent(work, 0x81, 1);
            PCBox_SetIconsTranslucent(work, 0x82, 1);
            msg = 0x82;
        } else if (work->unk_27 == 0) {
            msg = 0x29;
        } else {
            msg = 0x73;
        }
        if (work->args->unk8 != PC_BOX_MODE_MOVE_ITEMS && work->unk_27 != 0) {
            ov14_021E8874(work->view);
        }
        if (work->unk_29 == 1) {
            PCBox_SetBoxArrowsVisible(work->view, 0);
        }
        work->unk_26 = 0;
        work->unk_28 = 0;
        work->unk_27 = 0;
    }
    PCBox_StartFadeIn(work, msg);
}

void ov14_021EBAEC(PCBoxWork *work) {
    PCBox_CreateGridInput(work, 9, 10);
    work->view->unk_043C = work->trayBox % 6;
    ov14_021E8740(work);
    PCBox_PrintMenuPrompt(work, 0, 0x27);
    if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_SetIconsTranslucent(work, 0x81, 1);
    }
    PCBox_StartFadeIn(work, 0x3d);
}

void PCBox_State_GiveItemFromBag(PCBoxWork *work) {
    u32 msg;
    u32 special;
    void *entry;
    u8 pos[2];

    if (work->selectedSlot >= 0x1e) {
        ov14_021F0B70(work);
        PCBox_SetBoxArrowsVisible(work->view, 0);
        PCBox_SetIconsTranslucent(work, 1, 1);
    }
    if (work->args->unk8 != PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_CreateSelectedIconOutline(work);
    }
    if ((u32)(work->args->unk8 - 2) <= 1) {
        ov14_021E87F4(work);
        ov14_021E82FC(work->view->panels);
    }
    if (work->itemId == 0) {
        if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
            PCBox_DrawBagGiveButton(work, 0);
            if (work->selectedSlot < 0x1e) {
                PCBox_SetIconsTranslucent(work, 0x81, 1);
                msg = 0x75;
            } else {
                PCBox_SetIconsTranslucent(work, 0x82, 1);
                PCBox_CreateGridInput(work, 7, 8);
                msg = 0x8b;
            }
        } else {
            PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            if (work->selectedSlot < 0x1e) {
                msg = 0xc;
            } else {
                PCBox_CreateGridInput(work, 5, 10);
                msg = 0x24;
            }
        }
        ov14_021E83C4(work->view->panels);
    } else {
        special = 0;
        if (work->itemId == ITEM_GRISEOUS_ORB && PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_SPECIES, 0) != SPECIES_GIRATINA) {
            special = 1;
        } else {
            entry = PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot);
            PCBox_SetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, &work->itemId);
            if (PCBox_UpdateArceusGiratinaForm(entry) == 1) {
                PCBox_RefreshSlotIcon(work, work->activeBox, work->selectedSlot, work->view->slotIcons[work->selectedSlot]);
            }
            Bag_TakeItem(work->bag, work->itemId, 1, HEAP_ID_10);
        }
        if (work->selectedSlot >= 0x1e) {
            if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
                PCBox_CreateGridInput(work, 7, work->selectedSlot - 0x1e);
            } else {
                PCBox_CreateGridInput(work, 5, 10);
            }
        } else if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
            GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
            DpadMenuBox_GetPosition(
                GridInputHandler_GetDpadBox(work->view->gridInput, work->selectedSlot),
                &pos[1],
                &pos[0]);
            ManagedSprite_SetPositionXY(work->view->sprites[9], pos[1], pos[0]);
        }
        PCBox_SetSpriteVisible(work->view, 9, 0);
        if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
            if (special == 0) {
                work->view->heldItem = work->itemId;
                PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
                PCBox_SetSpriteVisible(work->view, 0xb, 1);
                if (work->selectedSlot >= 0x1e) {
                    PCBox_SetIconsTranslucent(work, 0x82, 1);
                    PCBox_MoveHeldItemIconToSlot(work->view, work->selectedSlot, 1);
                } else {
                    PCBox_SetIconsTranslucent(work, 0x81, 1);
                    PCBox_MoveHeldItemIconToSlot(work->view, work->selectedSlot, 0);
                }
                PCBox_CreateHeldItemIconOutline(work->view);
                PCBox_ShowHeldItemIconOutline(work->view);
            } else {
                work->itemId = 0;
                if (work->selectedSlot >= 0x1e) {
                    PCBox_SetIconsTranslucent(work, 0x82, 1);
                } else {
                    PCBox_SetIconsTranslucent(work, 0x81, 1);
                }
            }
            msg = 0x7d;
        } else {
            if (special == 1) {
                work->itemId = 0;
            }
            msg = 0x12;
        }
    }
    PCBox_ShowSlotMonInfo(work, work->selectedSlot);
    if (work->args->unk8 != PC_BOX_MODE_MOVE_ITEMS) {
        ov14_021E8874(work->view);
    } else if (work->itemId != 0) {
        ov14_021E892C(work->view->panels);
    }
    PCBox_StartFadeIn(work, msg);
}

u32 ov14_021EBDCC(PCBoxWork *work) {
    PCBox_PrintItemHeld(work, work->itemId, 0x25);
    work->nextState = 0xe;
    return 6;
}

void ov14_021EBDE0(PCBoxWork *work) {
    PCBox_State_OpenMarkingMenu(work);
}

void PCBox_State_OpenMarkingMenu(PCBoxWork *work) {
    work->view->markings = PCBox_GetSlotMonData(work, *(&work->selectedSlot), MON_DATA_MARKINGS, 0);
    PCBox_DrawMarkingMenu(work);
    PCBox_DrawConfirmCancel(work->view);
    PCBox_SlideInMarkingMenu(*(void **)(&work->view->panels));
    PCBox_WaitForUpdateFunc(work, PCBox_UpdateMarkingMenuSlide, 0x15);
}

u32 ov14_021EBE2C(PCBoxWork *work) {
    PCBox_CreateGridInput(work, 8, 0);
    PCBox_SetIconsTranslucent(work, 1, 1);
    PCBox_SetIconsTranslucent(work, 2, 1);
    PCBox_SetSlotIconTranslucent(work->view, *(&work->selectedSlot), 0);
    PCBox_PrintTouchTheMark(work, 0x26);
    return 0x16;
}

u32 PCBox_State_MarkingMenuInput(PCBoxWork *work) {
    u32 input = GridInputHandler_HandleInput_AllowHold(*(void **)(&work->view->gridInput));
    switch (input) {
    case 0:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ToggleMarking(work, 0);
        break;
    case 1:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ToggleMarking(work, 1);
        break;
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ToggleMarking(work, 2);
        break;
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ToggleMarking(work, 3);
        break;
    case 4:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ToggleMarking(work, 4);
        break;
    case 5:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ToggleMarking(work, 5);
        break;
    case 6: {
        u8 value = *(u32 *)(&work->view->markings);
        PCBox_SetSlotMonData(work, *(&work->selectedSlot), MON_DATA_MARKINGS, &value);
        PCBox_DrawMarkings(work, value);
        PlaySE(SEQ_SE_DP_DECIDE);
        return ov14_021F23F0(work, 0, 0x9a);
    }
    case 7:
    case (u32)-2:
        PlaySE(SEQ_SE_DP_SELECT);
        return ov14_021F23F0(work, 1, 0x9a);
    case (u32)-3:
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_WaitForPendingUpdateFunc(work, 0x18);
    case (u32)-4:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
    return 0x16;
}

u32 ov14_021EBF8C(PCBoxWork *work) {
    return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0xe);
}

u32 PCBox_State_TryRelease(PCBoxWork *work) {
    u32 slot;
    void *mon;
    if (work->selectedSlot >= 0x1e) {
        PCBox_SetIconsTranslucent(work, 2, 1);
        PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 0);
        slot = work->selectedSlot - 0x1e;
        if (PCBox_PartyHasOtherUsableMon(work, slot) == 0) {
            PlaySE(SEQ_SE_DP_BOX03);
            PCBox_PrintReleaseMessage(work, 6, 0x25);
            work->view->unk_044E_7 = 1;
            work->nextState = 0xe;
            return 6;
        }
        mon = Party_GetMonByIndex(work->party, slot);
        if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, 0)) == 1) {
            PlaySE(SEQ_SE_DP_BOX03);
            PCBox_PrintMonActionMessage(work, 0, 6, 0x25);
            work->view->unk_044E_7 = 1;
            work->nextState = 0xe;
            return 6;
        }
        if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, 0) != 0) {
            PlaySE(SEQ_SE_DP_BOX03);
            PCBox_PrintMonActionMessage(work, 0, 5, 0x25);
            work->view->unk_044E_7 = 1;
            work->nextState = 0xe;
            return 6;
        }
    } else {
        PCBox_SetBoxArrowsVisible(work->view, 0);
        PCBox_SetIconsTranslucent(work, 1, 1);
        PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 0);
    }
    if (GetBoxMonData(PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot), MON_DATA_IS_EGG, 0) != 0) {
        PlaySE(SEQ_SE_DP_BOX03);
        PCBox_PrintReleaseMessage(work, 3, 0x25);
        work->view->unk_044E_7 = 1;
        work->nextState = 0xe;
        return 6;
    }
    PCBox_PrintReleaseMessage(work, 0, 0x25);
    return PCBox_ShowYesNo(work, 1);
}

u32 PCBox_State_ReleaseMoveCheck(PCBoxWork *work) {
    PCBoxReleaseWork *x;
    PCBox_RunReleaseMoveCheck(work);
    x = work->view->releaseWork;
    if (PCBox_UpdateReleaseShrink(x) == 0) {
        x = work->view->releaseWork;
        if (x->moveFlags != 0) {
            return 0x1e;
        }
        PCBox_EndReleaseShrink(x);
        PCBox_FreeReleaseMoveCheck(work);
        return 0x1b;
    }
    return 0x1a;
}

u32 PCBox_State_PrintReleased(PCBoxWork *work) {
    PCBox_PrintReleaseMessage(work, 1, 0x25);
    work->nextState = 0x1c;
    return 6;
}

u32 PCBox_State_PrintByeBye(PCBoxWork *work) {
    PCBox_PrintReleaseMessage(work, 2, 0x25);
    work->nextState = 0x1d;
    return 6;
}

u32 PCBox_State_RemoveReleasedMon(PCBoxWork *work) {
    void *next;
    PCBox_DeleteSlotMon(work, work->activeBox, work->selectedSlot);
    PCBox_CloseMessageWindow(work->view, 0x25);
    PCBox_ClearMonInfo(work);
    if (work->selectedSlot < 0x1e) {
        PCBox_DrawBoxThumbnail(work, work->activeBox);
        PCBox_RefreshTrayBoxThumbnail(work, work->activeBox);
        if (work->args->unk8 == PC_BOX_MODE_WITHDRAW) {
            PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
            work->nextState = 0x51;
        } else {
            work->nextState = 0xc;
            PCBox_SlideInPartyButton(work->view->panels);
            ov14_021E82A8(work->view->panels);
        }
        ov14_021E8328(work->view->panels);
        GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        PCBox_SetBoxArrowsVisible(work->view, 1);
        PCBox_SetIconsTranslucent(work, 1, 0);
        work->selectedSlot = 0xff;
        next = ov14_021E9450;
    } else {
        PCBox_AllocMoveWork(work);
        work->partyLayout = 1;
        work->nextState = 0x21;
        ov14_021E8328(work->view->panels);
        PCBox_SetIconsTranslucent(work, 2, 0);
        next = ov14_021E9194;
    }
    return PCBox_WaitForUpdateFunc(work, next, work->nextState);
}

u32 ov14_021EC23C(PCBoxWork *work) {
    PCBox_FreeMoveWork(work);
    if (work->trayOpen != 0) {
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    PCBox_ShowSlotMonInfo(work, work->selectedSlot);
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->selectedSlot - 0x1e));
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    work->selectedSlot = 0xff;
    if (work->args->unk8 == PC_BOX_MODE_DEPOSIT) {
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
        return 0x5b;
    }
    return 0x24;
}

u32 PCBox_State_ReleaseCameBack(PCBoxWork *work) {
    PCBoxReleaseWork *x = work->view->releaseWork;
    if (PCBox_UpdateReleaseGrow(x) == 0) {
        x = work->view->releaseWork;
        PCBox_ResetSpriteAffine(x);
        PCBox_FreeReleaseMoveCheck(work);
        PCBox_CreateSelectedIconOutline(work);
        PCBox_PrintReleaseMessage(work, 4, 0x25);
        ov14_021E8824(work->view);
        work->nextState = 0x1f;
        return 6;
    }
    return 0x1e;
}

u32 PCBox_State_PrintWorried(PCBoxWork *work) {
    PCBox_PrintReleaseMessage(work, 5, 0x25);
    work->nextState = 0x20;
    return 6;
}

u32 ov14_021EC300(PCBoxWork *work) {
    PCBox_SetSelectedIconOutlineVisible(work, 1);
    if (work->selectedSlot < 0x1e) {
        PCBox_SetBoxArrowsVisible(work->view, 1);
        PCBox_SetIconsTranslucent(work, 1, 0);
    } else {
        PCBox_SetIconsTranslucent(work, 2, 0);
    }
    return PCBox_WaitForUpdateFunc(work, (void *)ov14_021E9450, 0xe);
}

u32 ov14_021EC344(PCBoxWork *work) {
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x5b;
}

u32 ov14_021EC354(PCBoxWork *work) {
    ov14_021E81A8(work->view->panels, 1);
    ov14_021E7EC0(work->view->panels);
    PCBox_SlideInPartyPanel(work->view->panels);
    ov14_021F63F0(work->view);
    ov14_021F63A8(work->view);
    PCBox_DrawExitButton(work, 0);
    ov14_021F2FDC(work);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9518, 0x23);
}

u32 ov14_021EC3A8(PCBoxWork *work) {
    PCBox_ShowSlotMonInfo(work, 0x1e);
    if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_CreateGridInput(work, 7, 0);
        return 0x8b;
    }
    PCBox_CreateGridInput(work, 5, 0);
    return 0x24;
}

#pragma force_active on

static inline u32 plus30(u32 n) {
    return n + 30;
}

int PCBox_State_MoveModePartyInput(PCBoxWork *work) {
    u32 touch;
    touch = PCBox_FindTouchedPartySlot();
    if (touch != -1) {
        if (PCBox_GetSlotMonData(work, touch + 30, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_BOX02);
            System_GetTouchNewCoords((u32 *)&work->view->touchX, (u32 *)&work->view->touchY);
            if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
                PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
            }
            PCBox_ShowSlotMonInfo(work, touch + 30);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return PCBox_StartMoveModePartyDrag(work, touch += 30);
        } else if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
            touch = (u8)(work->selectedSlot - 30);
            PCBox_StartCursorSlide(work->view, touch, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, touch);
            PCBox_SlideOutActionMenu(work->view->panels);
            ov14_021E8328(work->view->panels);
            ov14_021E884C(work->view);
            PCBox_SetSelectedIconOutlineVisible(work, 0);
            return PCBox_WaitForUpdateFunc(work, ov14_021EA180, 76);
        } else {
            GridInputHandler_SetNextInput(work->view->gridInput, (u8)touch);
            GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
            PCBox_ClearMonInfo(work);
            return 36;
        }
    }
    {
        u32 input = PCBox_GetMoveModePartyMenuInput(work);
        switch ((u32)input) {
        case 6:
            PlaySE(SEQ_SE_DP_DECIDE);
            work->unk_2A = 0;
            work->unk_2B = 0;
            return ov14_021F2490(work, 0, 157);
        case 8:
            PlaySE(SEQ_SE_DP_DECIDE);
            if (!System_GetTouchNew()) {
                work->unk_2A = 2;
            }
            work->unk_2B = work->selectedSlot;
            return ov14_021F2330(work, 3, 157);
        case 9:
            PlaySE(SEQ_SE_DP_DECIDE);
            work->unk_2C = 9;
            return PCBox_FlashPanel(work, 4, 151);
        case 10:
            PlaySE(SEQ_SE_DP_DECIDE);
            return PCBox_FlashPanel(work, 5, 152);
        case 11:
            PlaySE(SEQ_SE_DP_DECIDE);
            return PCBox_FlashPanel(work, 6, 153);
        case 12:
            PlaySE(SEQ_SE_DP_DECIDE);
            return PCBox_FlashPanel(work, 7, 155);
        case (u32)-3: {
            u32 i = GridInputHandler_GetNextInput(work->view->gridInput);
            if (i <= 5) {
                PCBox_ShowSlotMonInfo(work, i + 30);
            } else if (i != 8 && i != 9 && i != 10 && i != 11 && i != 12 && i != 13) {
                PCBox_ClearMonInfo(work);
            }
            PlaySE(SEQ_SE_DP_SELECT);
            return PCBox_WaitForPendingUpdateFunc(work, 76);
        }
        case 7:
        case (u32)-2:
            PlaySE(SEQ_SE_DP_CLOSE7);
            return PCBox_FlashPanel(work, 10, 159);
        case 13:
            PlaySE(SEQ_SE_DP_SELECT);
            input = (u8)(work->selectedSlot - 30);
            PCBox_StartCursorSlide(work->view, input, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, input);
            return PCBox_FlashPanel(work, 11, 158);
        case (u32)-4: {
            u32 i = GridInputHandler_GetNextInput(work->view->gridInput);
            if (i <= 5) {
                PCBox_ShowSlotMonInfo(work, i + 30);
            } else if (i != 8 && i != 9 && i != 10 && i != 11 && i != 12 && i != 13) {
                PCBox_ClearMonInfo(work);
            }
            PlaySE(SEQ_SE_DP_SELECT);
            break;
        }
        case (u32)-1:
            break;
        default:
            if (PCBox_GetSlotMonData(work, input + 30, MON_DATA_SPECIES_EXISTS, 0)) {
                PlaySE(SEQ_SE_DP_DECIDE);
                PCBox_DrawActionMenu(work, ov14_021F7D3C, 5);
                PCBox_ShowSlotMonInfo(work, input + 30);
                PCBox_StartCursorSlide(work->view, 8, GridInputHandler_GetNextInput(work->view->gridInput));
                GridInputHandler_SetNextInput(work->view->gridInput, 8);
                return ov14_021F07F0(work, input += 30);
            }
            break;
        }
        return 36;
    }
}

#pragma force_active reset

u32 ov14_021EC710(PCBoxWork *work) {
    PCBox_SlideOutPartyPanel(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9518, 0x26);
}

u32 ov14_021EC730(PCBoxWork *work) {
    ov14_021E7E98(work);
    PCBox_SetBoxArrowsVisible(work->view, 1);
    PCBox_DrawPartyButton(work, 0);
    ov14_021F5E94(work, 0);
    if (work->trayOpen == 0) {
        PCBox_DrawReturnButton(work, 0);
    }
    PCBox_SlideInPartyButton(work->view->panels);
    ov14_021E82A8(work->view->panels);
    ov14_021E8328(work->view->panels);
    if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_SetIconsTranslucent(work, 0x81, 1);
        PCBox_CreateGridInput(work, 6, 0x21);
        return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x75);
    }
    PCBox_SetIconsTranslucent(work, 1, 0);
    PCBox_CreateGridInput(work, 3, 0x21);
    return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0xc);
}

u32 ov14_021EC7D4(PCBoxWork *work) {
    PCBoxView *view = work->view;
    PCBoxMoveWork *moveWork = view->temp;
    u32 selected = moveWork->from;
    PCBox_CloseMessageWindow(view, 0x28);
    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_ApplyMonMove(work);
    PCBox_RevertShayminFormOnMove(work, moveWork->from, moveWork->to);
    PCBox_FreeMoveWork(work);
    if (work->selectedSlot == 0xff) {
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(selected - 0x1e));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        PCBox_ShowSlotMonInfo(work, selected);
    } else {
        GridInputHandler_SetNextInput(work->view->gridInput, 8);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    }
    return 0x24;
}

u32 ov14_021EC854(PCBoxWork *work) {
    s32 row = work->trayBox % 6;
    work->view->unk_043C = row + 0x25;
    if (work->unk_2A != 0) {
        PCBox_CreateGridInput(work, 4, work->unk_2B);
        work->selectedSlot = work->unk_2B;
        ov14_021F1580(work, work->unk_2B);
        row = 0;
        work->unk_2A = row;
    } else {
        PCBox_CreateGridInput(work, 4, work->unk_2B);
        PCBox_ShowSlotMonInfo(work, work->unk_2B);
        return work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS ? 0x82 : 0x29;
    }
}

u32 PCBox_State_BoxTrayInput(PCBoxWork *work) {
    u32 v = ov14_021F6A34();
    if (v != -1) {
        if (PCBox_GetSlotMonData(work, v + 0x1e, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_BOX02);
            System_GetTouchNewCoords((u32 *)(&work->view->touchX), (u32 *)(&work->view->touchY));
            PCBox_ShowSlotMonInfo(work, v + 0x1e);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return PCBox_StartBoxTrayDrag(work, v + 0x1e);
        }
        PCBox_ClearMonInfo(work);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(v + 0x1e));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            return ov14_021F0EE8(work, 0x29);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return PCBox_HideInfoPanelAndWait(work, 0x29);
        }
        return 0x29;
    }
    v = PCBox_FindTouchedBoxSlot();
    if (v != -1) {
        if (PCBox_GetSlotMonData(work, v, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_BOX02);
            System_GetTouchNewCoords((u32 *)(&work->view->touchX), (u32 *)(&work->view->touchY));
            PCBox_ShowSlotMonInfo(work, v);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return PCBox_StartBoxTrayDrag(work, v);
        }
        PCBox_ClearMonInfo(work);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            return ov14_021F0EE8(work, 0x29);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return PCBox_HideInfoPanelAndWait(work, 0x29);
        }
        return 0x29;
    }
    if (PCBox_IsSummaryShortcutPressed(work) == 1) {
        v = GridInputHandler_GetNextInput(work->view->gridInput);
        if (PCBox_GetSlotMonData(work, v, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            work->selectedSlot = (u8)v;
            work->unk_26 = 1;
            return ov14_021F2330(work, 0xf, 0x97);
        }
        return 0x29;
    }
    v = PCBox_GetBoxTrayInput(work);
    switch (v) {
    case 0x25:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 0);
    case 0x26:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 1);
    case 0x27:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 2);
    case 0x28:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 3);
    case 0x29:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 4);
    case 0x2a:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 5);
    case 0x2b:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_ChangeTrayPage(work, -1);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            return ov14_021F0EE8(work, 0x29);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return PCBox_HideInfoPanelAndWait(work, 0x29);
        }
        return 0x29;
    case 0x2c:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_ChangeTrayPage(work, 1);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            return ov14_021F0EE8(work, 0x29);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return PCBox_HideInfoPanelAndWait(work, 0x29);
        }
        return 0x29;
    case 0x2d:
        PlaySE(SEQ_SE_DP_SELECT);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
        return PCBox_FlashPanel(work, 0xe, 0xa0);
    case -3: {
        u32 n = GridInputHandler_GetNextInput(work->view->gridInput);
        if (n < 0x25) {
            if (PCBox_ShowSlotMonInfo(work, n) == 1) {
                if (PCBox_IsSummaryButtonShown(work->view->panels) == 0) {
                    PCBox_DrawSummaryButton(work, 0);
                    PCBox_SlideInSummaryButton(work->view->panels);
                }
            } else if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
                PCBox_SlideOutSummaryButton(work->view->panels);
            }
        } else {
            PCBox_ClearMonInfo(work);
            if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
                PCBox_SlideOutSummaryButton(work->view->panels);
            }
        }
        PlaySE(SEQ_SE_DP_SELECT);
        if (work->view->pendingUpdateFunc != 0) {
            return PCBox_WaitForPendingUpdateFunc(work, 0x4b);
        }
        break;
    }
    case 0x24:
        PlaySE(SEQ_SE_DP_CLOSE7);
        PCBox_SetSpriteAnim(work->view, 9, 8);
        return ov14_021F2490(work, 1, 0xa1);
    case -2:
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 0) {
            PlaySE(SEQ_SE_DP_CLOSE7);
            return ov14_021F2490(work, 1, 0xa1);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        return ov14_021F0EE8(work, 0x29);
    default:
        if (PCBox_GetSlotMonData(work, v, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_BOX02);
            PCBox_ShowSlotMonInfo(work, v);
            return ov14_021F1580(work, v);
        }
        break;
    case -4:
    case -1:
        break;
    }
    return 0x29;
}

u32 ov14_021ECDA8(PCBoxWork *work) {
    PCBoxMoveWork *moveWork = work->view->temp;
    s32 selected = moveWork->to;
    u32 current = moveWork->sel;
    u32 high;

    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_ApplyMonMove(work);
    high = selected & 0x80;
    if (high == 0) {
        PCBox_RevertShayminFormOnMove(work, moveWork->from, moveWork->to);
    }
    PCBox_FreeMoveWork(work);
    GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
    if (work->trayOpen != 0) {
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    if (PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_SPECIES_EXISTS, 0) == 0) {
        PCBox_ClearMonInfo(work);
        goto done;
    }
    if (selected != 0xff) {
        if (high == 0) {
            goto done;
        }
        PCBox_ShowSlotMonInfo(work, work->selectedSlot);
        goto done;
    } else {
        selected = work->selectedSlot;
        if ((u32)selected >= 0x1e) {
            if (current != selected) {
                selected -= 0x1e;
                void *mon = Party_GetMonByIndex(work->party, (u32)selected);
                if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, 0)) == 1) {
                    PlaySE(SEQ_SE_DP_BOX03);
                    PCBox_PrintMonActionMessage(work, 0, 6, 0x25);
                    work->nextState = 0x2c;
                    return 6;
                }
                if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, 0) != 0) {
                    PlaySE(SEQ_SE_DP_BOX03);
                    PCBox_PrintMonActionMessage(work, 0, 5, 0x25);
                    work->nextState = 0x2c;
                    return 6;
                }
                if (PCBox_PartyHasOtherUsableMon(work, selected) == 0) {
                    PlaySE(SEQ_SE_DP_BOX03);
                    PCBox_PrintReleaseMessage(work, 6, 0x25);
                    work->nextState = 0x2c;
                    return 6;
                }
            }
        }
    }
    if (current != 0xff && (current & 0x80) != 0) {
        u32 next = (current ^ 0x80) + (work->trayBox / 6) * 6;
        if (work->activeBox != next) {
            s32 index = work->selectedSlot;
            if ((u32)index < 0x1e) {
                PCBox_PrintMonActionMessage(work, 0, 4, 0x25);
            } else if (PCBox_PartyHasOtherUsableMon(work, index - 0x1e) == 1) {
                PCBox_PrintMonActionMessage(work, 0, 4, 0x25);
            } else {
                PCBox_PrintReleaseMessage(work, 6, 0x25);
            }
            PlaySE(SEQ_SE_DP_BOX03);
            work->nextState = 0x2c;
            return 6;
        }
    }
done:
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x29;
}

u32 ov14_021ECF58(PCBoxWork *work) {
    PCBoxMoveWork *moveWork = work->view->temp;
    s32 selected = moveWork->to;
    u32 input;
    s32 index;

    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_ApplyMonMove(work);
    if ((selected & 0x80) == 0) {
        PCBox_RevertShayminFormOnMove(work, moveWork->from, moveWork->to);
    }
    PCBox_FreeMoveWork(work);
    if (work->trayOpen != 0) {
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    input = GridInputHandler_GetNextInput(work->view->gridInput);
    if (work->view->unk_88CC == 0 && selected == 0xff) {
        index = work->selectedSlot;
        if (input != index) {
            if ((u32)index >= 0x1e) {
                index -= 0x1e;
                void *mon = Party_GetMonByIndex(work->party, (u32)index);
                if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, 0)) == 1) {
                    PlaySE(SEQ_SE_DP_BOX03);
                    GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
                    GridInputHandler_SetButtonInputMode(work->view->gridInput, 0);
                    PCBox_SetSpriteAnim(work->view, 9, 8);
                    PCBox_PrintMonActionMessage(work, 0, 6, 0x25);
                    work->nextState = 0x2c;
                    return 6;
                }
                if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, 0) != 0) {
                    PlaySE(SEQ_SE_DP_BOX03);
                    GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
                    GridInputHandler_SetButtonInputMode(work->view->gridInput, 0);
                    PCBox_SetSpriteAnim(work->view, 9, 8);
                    PCBox_PrintMonActionMessage(work, 0, 5, 0x25);
                    work->nextState = 0x2c;
                    return 6;
                }
                if (PCBox_PartyHasOtherUsableMon(work, index) == 0) {
                    PlaySE(SEQ_SE_DP_BOX03);
                    GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
                    GridInputHandler_SetButtonInputMode(work->view->gridInput, 0);
                    PCBox_SetSpriteAnim(work->view, 9, 8);
                    PCBox_PrintReleaseMessage(work, 6, 0x25);
                    work->nextState = 0x2c;
                    return 6;
                }
            }
            if (input >= 0x25 && input <= 0x2a) {
                u32 next = (input - 0x25) + (work->trayBox / 6) * 6;
                if (work->activeBox != next) {
                    index = work->selectedSlot;
                    if ((u32)index < 0x1e) {
                        PCBox_PrintMonActionMessage(work, 0, 4, 0x25);
                    } else if (PCBox_PartyHasOtherUsableMon(work, index - 0x1e) == 1) {
                        PCBox_PrintMonActionMessage(work, 0, 4, 0x25);
                    } else {
                        PCBox_PrintReleaseMessage(work, 6, 0x25);
                    }
                    PlaySE(SEQ_SE_DP_BOX03);
                    GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
                    GridInputHandler_SetButtonInputMode(work->view->gridInput, 0);
                    PCBox_SetSpriteAnim(work->view, 9, 8);
                    work->nextState = 0x2c;
                    return 6;
                }
            }
            if (work->unk_2A == 0) {
                PCBox_ShowSlotMonInfo(work, GridInputHandler_GetNextInput(work->view->gridInput));
            }
        }
    }
    if (work->unk_2A != 0) {
        PCBox_SetSpriteAnim(work->view, 9, 8);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0CD8(work);
    }
    if (input >= 0x25 && input <= 0x2a) {
        PCBox_SetSpriteAnim(work->view, 9, 0xe);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
    return 0x29;
}

u32 ov14_021ED1AC(PCBoxWork *work) {
    ov14_021F6688(work->view, 0x25);
    if (work->unk_2A != 0) {
        PCBox_SetSpriteAnim(work->view, 9, 8);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0CD8(work);
    }
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x29;
}

u32 ov14_021ED1E8(PCBoxWork *work) {
    ov14_021F63B8(work->view);
    if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_SetIconsTranslucent(work, 0x81, 1);
    } else {
        PCBox_SetIconsTranslucent(work, 1, 0);
    }
    if (PCBox_GetSlotMonData(work, work->unk_2B, MON_DATA_SPECIES_EXISTS, 0) != 0) {
        PCBox_DrawSummaryButton(work, 0);
        PCBox_SlideInSummaryButton(work->view->panels);
    }
    work->trayOpen = 1;
    work->unk_29 = 1;
    work->trayBox = work->activeBox;
    return PCBox_OpenBoxTray(work, 0x28);
}

u32 ov14_021ED258(PCBoxWork *work) {
    ov14_021F63A8(work->view);
    if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_CreateGridInput(work, 7, 0);
        PCBox_ShowSlotMonInfo(work, GridInputHandler_GetNextInput(work->view->gridInput) + 0x1e);
        return 0x8b;
    }
    if (work->unk_2A != 0) {
        PCBox_CreateGridInput(work, 5, work->unk_2B - 0x1e);
    } else {
        PCBox_CreateGridInput(work, 5, 0);
    }
    PCBox_ShowSlotMonInfo(work, GridInputHandler_GetNextInput(work->view->gridInput) + 0x1e);
    return 0x24;
}

u32 ov14_021ED2C8(PCBoxWork *work) {
    work->trayBox = work->activeBox;
    return PCBox_OpenBoxTray(work, 0x30);
}

#pragma force_active on

void ov14_021ED2DC(PCBoxWork *work) {
    ov14_021E81A8(work->view->panels, 1);
    ov14_021E7ED0(work->view->panels);
    PCBox_SlideInPartyPanel(work->view->panels);
    ov14_021F63F0(work->view);
    ov14_021F63B8(work->view);
    ov14_021F3044(work);
    PCBox_DrawSummaryButton(work, 0);
    if (PCBox_GetSlotMonData(work, work->unk_2B, MON_DATA_SPECIES_EXISTS, 0)) {
        PCBox_SlideInSummaryButton(work->view->panels);
    }
    PCBox_WaitForUpdateFunc(work, ov14_021E9518, 40);
}

#pragma force_active reset

u32 ov14_021ED350(PCBoxWork *work) {
    return PCBox_CloseBoxTray(work, 0x32);
}

void ov14_021ED35C(PCBoxWork *work) {
    if (work->unk_29 == 1) {
        ov14_021F0C58(work);
        return;
    }
    PCBox_SlideOutPartyPanel(work->view->panels);
    PCBox_WaitForUpdateFunc(work, ov14_021E9518, 0x33);
}

void ov14_021ED38C(PCBoxWork *work) {
    u32 a;
    u32 b;
    u32 next;
    if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
        a = 6;
        b = 0x22;
        next = 0x75;
    } else {
        a = 3;
        b = 0x22;
        next = 0xc;
        if (work->unk_2A != 0) {
            b = work->unk_2B;
            PCBox_ShowSlotMonInfo(work, b);
        }
    }
    PCBox_CreateGridInput(work, a, b);
    PCBox_DrawPartyButton(work, 0);
    ov14_021F5E94(work, 0);
    PCBox_DrawReturnButton(work, 0);
    PCBox_SlideInPartyButton(work->view->panels);
    ov14_021E82A8(work->view->panels);
    ov14_021E8328(work->view->panels);
    PCBox_WaitForUpdateFunc(work, ov14_021E94BC, next);
}

u32 ov14_021ED414(PCBoxWork *work) {
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);
    switch (input) {
    case 0:
        PlaySE(SEQ_SE_DP_DECIDE);
        return ov14_021F1170(work, 0);
    case 1:
        PlaySE(SEQ_SE_DP_DECIDE);
        return ov14_021F1170(work, 1);
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        return ov14_021F1170(work, 2);
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        return ov14_021F1170(work, 3);
    case 4:
        PlaySE(SEQ_SE_DP_DECIDE);
        return ov14_021F1170(work, 4);
    case 5:
        PlaySE(SEQ_SE_DP_DECIDE);
        return ov14_021F1170(work, 5);
    case 6:
        PlaySE(SEQ_SE_DP_SELECT);
        return ov14_021F11F8(work, -1);
    case 7:
        PlaySE(SEQ_SE_DP_SELECT);
        return ov14_021F11F8(work, 1);
    case 8:
        PCBox_SetSpriteAnim(work->view, 9, 8);
        if (work->activeBox == work->trayBox) {
            PlaySE(SEQ_SE_DP_BOX03);
            return PCBox_FlashPanel(work, 0xc, 0x3d);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_FlashPanel(work, 0xc, 0xa2);
    case 9:
        PCBox_SetSpriteAnim(work->view, 9, 8);
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 6, 0xa3);
    case 10:
        PCBox_SetSpriteAnim(work->view, 9, 8);
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 7, 0xa4);
    case -3:
        PlaySE(SEQ_SE_DP_SELECT);
        if (work->view->pendingUpdateFunc != 0) {
            PlaySE(SEQ_SE_DP_SELECT);
            return PCBox_WaitForPendingUpdateFunc(work, 0x3e);
        }
        break;
    case -4:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case 11:
        PCBox_SetSpriteAnim(work->view, 9, 8);

    case -2:
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_FlashPanel(work, 0xa, 0x39);
    }
    return 0x3d;
}

u32 ov14_021ED5B0(PCBoxWork *work) {
    PCBoxView *view = work->view;
    if (work->args->unk8 == PC_BOX_MODE_WITHDRAW) {
        if (ov14_021E834C(view->panels) == 0) {
            ov14_021E8314(work->view->panels);
        }
    } else {
        PCBox_SlideOutPartyButton(view->panels);
        ov14_021E8294(work->view->panels);
        ov14_021E8314(work->view->panels);
    }
    PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x35);
}

u32 ov14_021ED60C(PCBoxWork *work) {
    work->trayBox = work->activeBox;
    return PCBox_OpenBoxTray(work, 0x36);
}

u32 ov14_021ED620(PCBoxWork *work) {
    return ov14_021F10B4(work, 0x37);
}

void ov14_021ED62C(PCBoxWork *work) {
    PCBox_DrawBoxMenu(work);
    PCBox_SlideInBoxMenu(work->view->panels);
    PCBox_WaitForUpdateFunc(work, PCBox_UpdateBoxMenuSlide, 0x38);
}

u32 ov14_021ED650(PCBoxWork *work) {
    PCBox_CreateGridInput(work, 9, work->unk_2C);
    if (work->unk_2C <= 5) {
        PCBox_SetSpriteAnim(work->view, 9, 0xe);
    } else {
        PCBox_SetSpriteAnim(work->view, 9, 8);
    }
    PCBox_PrintMenuPrompt(work, 0, 0x27);
    return 0x3d;
}

void ov14_021ED684(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_CloseMessageWindow(work->view, 0x27);
    ov14_021F10DC(work, 0x3a);
}

void ov14_021ED6A4(PCBoxWork *work) {
    PCBox_SlideOutBoxMenu(work->view->panels);
    PCBox_WaitForUpdateFunc(work, PCBox_UpdateBoxMenuSlide, 0x3b);
}

u32 ov14_021ED6C4(PCBoxWork *work) {
    return PCBox_CloseBoxTray(work, 0x3c);
}

u32 ov14_021ED6D0(PCBoxWork *work) {
    u32 mode;

    PCBox_SetSpriteAnim(work->view, 9, 8);
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_DrawReturnButton(work, 0);
    ov14_021E8328(work->view->panels);
    mode = work->args->unk8;
    if (mode == 1) {
        PCBox_CreateGridInput(work, 2, 0x1e);
        return PCBox_WaitForUpdateFunc(work, ov14_021E95B4, 0x52);
    }
    if (mode == 3) {
        PCBox_CreateGridInput(work, 6, 0x1e);
    } else {
        PCBox_CreateGridInput(work, 3, 0x1e);
    }
    PCBox_SlideInPartyButton(work->view->panels);
    ov14_021E82A8(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x4d);
}

u32 PCBox_State_OpenWallpaperMenu(PCBoxWork *work) {
    work->view->wallpaperSelection = PCBox_GetWallpaperGfxIndex(work, work->trayBox);
    PCBox_SetupWallpaperMenuSprites(work);
    PCBox_UpdateCurrentWallpaperMarker(work);
    PCBox_UpdateWallpaperCursor(work);
    PCBox_DrawWallpaperName(work);
    PCBox_SlideOutBoxMenu(work->view->panels);
    PCBox_SlideInWallpaperList(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9660, 0x40);
}

u32 ov14_021ED7B8(PCBoxWork *work) {
    PCBox_DrawChangeWallpaperTitle(work);
    ov14_021E8560(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E95C8, 0x41);
}

u32 PCBox_State_SetupWallpaperMenuInput(PCBoxWork *work) {
    work->view->unk_043C = work->view->wallpaperSelection % 4;
    PCBox_CreateGridInput(work, 0xa, work->view->unk_043C);
    PCBox_SetSpriteAnim(work->view, 9, 0xe);
    PCBox_PrintMenuPrompt(work, 1, 0x27);
    return 0x42;
}

u32 PCBox_State_WallpaperMenuInput(PCBoxWork *work) {
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);

    switch (input) {
    case 0:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_SelectWallpaperInPage(work, 0);
        return 0x42;
    case 1:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_SelectWallpaperInPage(work, 1);
        return 0x42;
    case 2:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_SelectWallpaperInPage(work, 2);
        return 0x42;
    case 3:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_SelectWallpaperInPage(work, 3);
        return 0x42;
    case 4:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ChangeWallpaperPage(work, -1);
        return 0x42;
    case 5:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ChangeWallpaperPage(work, 1);
        return 0x42;
    case 6:
        PCBox_SetSpriteAnim(work->view, 9, 8);
        return PCBox_ConfirmWallpaperChoice(work);
    case -3:
        PlaySE(SEQ_SE_DP_SELECT);
        if (work->view->pendingUpdateFunc != 0) {
            return PCBox_WaitForPendingUpdateFunc(work, 0x49);
        }
        break;
    case 7:
        PCBox_SetSpriteAnim(work->view, 9, 8);
    case -2:
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_FlashPanel(work, 0xa, 0xa6);
    default:
        break;
    }
    return 0x42;
}

u32 ov14_021ED920(PCBoxWork *work) {
    ov14_021E8588(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9604, 0x44);
}

u32 ov14_021ED940(PCBoxWork *work) {
    PCBox_SlideOutWallpaperList(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E96A8, 0x45);
}

u32 ov14_021ED960(PCBoxWork *work) {
    return PCBox_OpenBoxTray(work, 0x37);
}

u32 PCBox_State_ApplyWallpaper(PCBoxWork *work) {
    u8 wallpaper;

    wallpaper = work->view->wallpaperSelection;
    ov14_021E78AC(work, wallpaper);
    wallpaper = work->view->wallpaperSelection;
    if (wallpaper < 16) {
        PCStorage_SetBoxWallpaper(work->storage, work->activeBox, wallpaper);
    } else {
        // the bonus wallpaper id goes on as a full register
        ((void (*)(PCStorage *, u32, u32))PCStorage_SetBoxWallpaper)(work->storage, work->activeBox, wallpaper + 16);
    }
    PCBox_UpdateCurrentWallpaperMarker(work);
    PCBox_DrawBoxThumbnail(work, work->activeBox);
    return 0x48;
}

u32 PCBox_State_WallpaperFadeOut(PCBoxWork *work) {
    PaletteData_LoadPaletteSlotFromHardware(work->view->paletteData, PLTTBUF_MAIN_BG, 0, 0x200);
    PaletteData_BeginPaletteFade(work->view->paletteData, PLTTBUF_MAIN_BG_F, 0xC000, 0, 0, 16, RGB_WHITE);
    work->nextState = 0x46;
    return 3;
}

u32 PCBox_State_WallpaperFadeIn(PCBoxWork *work) {
    PaletteData_BeginPaletteFade(work->view->paletteData, PLTTBUF_MAIN_BG_F, 0xC000, 0, 16, 0, RGB_WHITE);
    work->nextState = 0x42;
    return 3;
}

u32 ov14_021EDA1C(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 1);
    if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
        return 0x75;
    }
    return 0xC;
}

u32 ov14_021EDA3C(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 1);
    return 0x24;
}

#pragma force_active on

int PCBox_State_WithdrawInput(PCBoxWork *work) {
    u32 input = PCBox_FindTouchedBoxSlot();
    if (input != -1) {
        if (PCBox_GetSlotMonData(work, input, MON_DATA_SPECIES_EXISTS, 0)) {
            System_GetTouchNewCoords((u32 *)&work->view->touchX, (u32 *)&work->view->touchY);
            if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
                PCBox_DrawActionMenu(work, ov14_021F7D2C, 4);
            }
            PlaySE(SEQ_SE_DP_BOX02);
            PCBox_ShowSlotMonInfo(work, input);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return PCBox_StartWithdrawDrag(work, input);
        } else if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
            input = work->selectedSlot;
            PCBox_StartCursorSlide(work->view, input, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, input);
            PCBox_SlideOutActionMenu(work->view->panels);
            ov14_021E8328(work->view->panels);
            ov14_021E884C(work->view);
            PCBox_SetSelectedIconOutlineVisible(work, 0);
            return PCBox_WaitForUpdateFunc(work, ov14_021EA130, 89);
        } else {
            GridInputHandler_SetNextInput(work->view->gridInput, (u8)input);
            GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
            PCBox_ClearMonInfo(work);
            return 81;
        }
    }
    input = PCBox_GetWithdrawMenuInput(work);
    switch (input) {
    case 34:
        if (Party_GetCount(work->party) != 6) {
            PlaySE(SEQ_SE_DP_DECIDE);
        } else {
            PlaySE(SEQ_SE_DP_BOX03);
        }
        return PCBox_FlashPanel(work, 4, 167);
    case 35:
        PlaySE(SEQ_SE_DP_DECIDE);
        work->unk_2C = 35;
        return PCBox_FlashPanel(work, 5, 151);
    case 36:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_CloseMessageWindow(work->view, 39);
        return PCBox_FlashPanel(work, 6, 153);
    case 37:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 7, 155);
    case 38:
        input = work->selectedSlot;
        PCBox_StartCursorSlide(work->view, input, GridInputHandler_GetNextInput(work->view->gridInput));
        GridInputHandler_SetNextInput(work->view->gridInput, input);
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_FlashPanel(work, 11, 168);
    case 30:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_CloseMessageWindow(work->view, 39);
        return ov14_021F1128(work);
    case 31:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(work->view, 9, 0);
        GridInputHandler_SetNextInput(work->view->gridInput, 30);
        PCBox_PrintMonActionMessage(work, 0, 0, 39);
        return PCBox_ScrollToPrevBox(work, 81);
    case 32:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(work->view, 9, 0);
        GridInputHandler_SetNextInput(work->view->gridInput, 30);
        PCBox_PrintMonActionMessage(work, 0, 0, 39);
        return PCBox_ScrollToNextBox(work, 81);
    case (u32)-3: {
        u32 i = GridInputHandler_GetNextInput(work->view->gridInput);
        if (i < 30) {
            PCBox_ShowSlotMonInfo(work, i);
        } else if (i != 34 && i != 35 && i != 36 && i != 37 && i != 38) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_WaitForPendingUpdateFunc(work, 88);
    }
    case 33:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfo(work);
        return PCBox_FlashPanel(work, 10, 147);
    case (u32)-2:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 10, 148);
    case (u32)-1:
        if (GridInputHandler_GetNextInput(work->view->gridInput) == 30) {
            if (gSystem.newKeys & 32) {
                PlaySE(SEQ_SE_DP_SELECT);
                PCBox_ClearMonInfoAndSelection(work);
                return PCBox_ScrollToPrevBox(work, 81);
            } else if (gSystem.newKeys & 16) {
                PlaySE(SEQ_SE_DP_SELECT);
                PCBox_ClearMonInfoAndSelection(work);
                return PCBox_ScrollToNextBox(work, 81);
            }
        }
        break;
    case (u32)-4: {
        u32 i = GridInputHandler_GetNextInput(work->view->gridInput);
        if (i < 30) {
            PCBox_ShowSlotMonInfo(work, i);
        } else if (i != 34 && i != 35 && i != 36 && i != 37 && i != 38) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
    default:
        if (PCBox_GetSlotMonData(work, input, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            PCBox_DrawActionMenu(work, ov14_021F7D2C, 4);
            PCBox_ShowSlotMonInfo(work, input);
            PCBox_StartCursorSlide(work->view, 34, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, 34);
            return ov14_021F04D4(work, input);
        }
        break;
    }
    return 81;
}

#pragma force_active reset

void ov14_021EDE20(PCBoxWork *work) {
    PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
    ov14_021EDF90(work);
}

void ov14_021EDE38(PCBoxWork *work) {
    ov14_021F3044(work);
    ov14_021E7ED0(work->view->panels);
    PCBox_SlideInPartyPanel(work->view->panels);
    work->partyLayout = 2;
    PCBox_WaitForUpdateFunc(work, ov14_021E9518, 0x54);
}

void ov14_021EDE70(PCBoxWork *work) {
    PCBox_AllocMoveWork(work);
    PCBox_WaitForUpdateFunc(work, PCBox_AnimateMoveToPartyEnd, 0x55);
}

void ov14_021EDE88(PCBoxWork *work) {
    struct {
        u8 y;
        u8 x;
    } pos;

    PlaySE(SEQ_SE_DP_BOX01);
    GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, work->selectedSlot), &pos.x, &pos.y);
    ManagedSprite_SetPositionXY(work->view->sprites[9], pos.x, pos.y);
    work->selectedSlot = 0xff;
    PCBox_ApplyMonMove(work);
    PCBox_FreeMoveWork(work);
    PCBox_ClearMonInfo(work);
    PCBox_SlideOutPartyPanel(work->view->panels);
    PCBox_WaitForUpdateFunc(work, ov14_021E9518, 0x56);
}

void ov14_021EDF08(PCBoxWork *work) {
    ov14_021E8328(work->view->panels);
    PCBox_WaitForUpdateFunc(work, ov14_021E95B4, 0x52);
}

u32 ov14_021EDF28(PCBoxWork *work) {
    u32 input = ((PCBoxMoveWork *)work->view->temp)->from;

    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_ApplyMonMove(work);
    PCBox_FreeMoveWork(work);
    ov14_021F6678(work->view, 0x28);
    if (work->selectedSlot == 0xff) {
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
    } else {
        PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
        input = 0x22;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)input);
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x51;
}

u32 ov14_021EDF90(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 1);
    return 0x51;
}

#pragma force_active on

int PCBox_State_DepositInput(PCBoxWork *work) {
    u32 choice = PCBox_FindTouchedPartySlot();
    if (choice != -1) {
        if (PCBox_GetSlotMonData(work, (s32)choice + 30, MON_DATA_SPECIES_EXISTS, 0)) {
            System_GetTouchNewCoords(&work->view->touchX, &work->view->touchY);
            if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
                PCBox_DrawActionMenu(work, ov14_021F7D1C, 4);
            }
            PlaySE(SEQ_SE_DP_BOX02);
            PCBox_ShowSlotMonInfo(work, (u32)(choice + 30));
            PCBox_SetSpriteVisible(work->view, 9, 0);
            choice += 30;
            return PCBox_StartDepositDrag(work, choice);
        }
        if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
            choice = (u8)(work->selectedSlot - 30);
            PCBox_StartCursorSlide(work->view, choice, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, choice);
            PCBox_DrawReturnButton(work, 0);
            PCBox_SlideOutActionMenu(work->view->panels);
            ov14_021E8328(work->view->panels);
            ov14_021E884C(work->view);
            PCBox_SetSelectedIconOutlineVisible(work, 0);
            return PCBox_WaitForUpdateFunc(work, ov14_021EA130, 112);
        }
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)choice);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        PCBox_ClearMonInfo(work);
        return 91;
    }
    choice = PCBox_GetDepositMenuInput(work);
    switch (choice) {
    case 7:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 4, 169);
    case 8:
        PlaySE(SEQ_SE_DP_DECIDE);
        work->unk_2C = 8;
        return PCBox_FlashPanel(work, 5, 151);
    case 9:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_CloseMessageWindow(work->view, 39);
        return PCBox_FlashPanel(work, 6, 153);
    case 10:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 7, 155);
    case 11:
        choice = (u8)(work->selectedSlot - 30);
        PCBox_StartCursorSlide(work->view, choice, GridInputHandler_GetNextInput(work->view->gridInput));
        GridInputHandler_SetNextInput(work->view->gridInput, choice);
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_FlashPanel(work, 11, 170);
    case (u32)-4: {
        u32 input = GridInputHandler_GetNextInput(work->view->gridInput);
        if (input <= 6) {
            PCBox_ShowSlotMonInfo(work, input + 30);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return 91;
    }
    case (u32)-3: {
        u32 input = GridInputHandler_GetNextInput(work->view->gridInput);
        if (input <= 6) {
            PCBox_ShowSlotMonInfo(work, input + 30);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_WaitForPendingUpdateFunc(work, 111);
    }
    case 6:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfo(work);
        return PCBox_FlashPanel(work, 10, 147);
    case (u32)-2:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 10, 148);
    case (u32)-1:
        break;
    default:
        if (PCBox_GetSlotMonData(work, (s32)choice + 30, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            PCBox_DrawActionMenu(work, ov14_021F7D1C, 4);
            PCBox_ShowSlotMonInfo(work, (u32)(choice + 30));
            PCBox_StartCursorSlide(work->view, 7, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, 7);
            choice += 30;
            return ov14_021F0794(work, choice);
        }
        break;
    }
    return 91;
}

#pragma force_active reset

#pragma force_active on

int PCBox_State_TryDeposit(PCBoxWork *work) {
    int slot = work->selectedSlot - 30;
    void *mon;
    if (PCBox_PartyHasOtherUsableMon(work, slot) == 0) {
        PlaySE(SEQ_SE_DP_BOX03);
        PCBox_PrintReleaseMessage(work, 6, 37);
        work->nextState = 93;
        return 6;
    }
    mon = Party_GetMonByIndex(work->party, slot);
    if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, 0)) == 1) {
        PlaySE(SEQ_SE_DP_BOX03);
        PCBox_PrintMonActionMessage(work, 0, 6, 37);
        work->nextState = 93;
        return 6;
    }
    if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, 0) != 0) {
        PlaySE(SEQ_SE_DP_BOX03);
        PCBox_PrintMonActionMessage(work, 0, 5, 37);
        work->nextState = 93;
        return 6;
    }
    work->trayBox = work->activeBox;
    PCBox_SetIconsTranslucent(work, 2, 1);
    PCBox_HideSelectedIconOutline(work);
    return PCBox_WaitForUpdateFunc(work, ov14_021E96C8, 94);
}

#pragma force_active reset

u32 ov14_021EE328(PCBoxWork *work) {
    return PCBox_WaitForUpdateFunc(work, ov14_021E9450, 0xe);
}

u32 ov14_021EE338(PCBoxWork *work) {
    ov14_021F4BC0(work);
    PCBox_UpdateCurrentBoxMarker(work);
    PCBox_UpdateTrayCursor(work);
    PCBox_DrawTrayBoxNameAndCount(work);
    return ov14_021F10B4(work, 0x5f);
}

u32 ov14_021EE35C(PCBoxWork *work) {
    PCBox_DrawDepositTitle(work);
    ov14_021E8560(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E95C8, 0x60);
}

u32 ov14_021EE380(PCBoxWork *work) {
    work->view->unk_043C = work->trayBox % 6;
    PCBox_CreateGridInput(work, 1, work->view->unk_043C);
    PCBox_SetSpriteAnim(work->view, 9, 0xe);
    PCBox_CloseMessageWindow(work->view, 0x25);
    PCBox_PrintMonActionMessage(work, 0, 3, 0x27);
    return 0x61;
}

u32 ov14_021EE3C8(PCBoxWork *work) {
    u32 input = GridInputHandler_HandleInput_AllowHold(work->view->gridInput);

    switch (input) {
    case 0:
        return ov14_021F1448(work, 0);
    case 1:
        return ov14_021F1448(work, 1);
    case 2:
        return ov14_021F1448(work, 2);
    case 3:
        return ov14_021F1448(work, 3);
    case 4:
        return ov14_021F1448(work, 4);
    case 5:
        return ov14_021F1448(work, 5);
    case 6:
        PlaySE(SEQ_SE_DP_SELECT);
        return ov14_021F1504(work, -1);
    case 7:
        PlaySE(SEQ_SE_DP_SELECT);
        return ov14_021F1504(work, 1);
    case 8:
        return ov14_021F1540(work);
    case -3:
        PlaySE(SEQ_SE_DP_SELECT);
        if (work->view->pendingUpdateFunc != 0) {
            return PCBox_WaitForPendingUpdateFunc(work, 0x71);
        }
        break;
    case 9:
        PCBox_SetSpriteAnim(work->view, 9, 8);
    case -2:
        PlaySE(SEQ_SE_DP_SELECT);
        return ov14_021F1534(work, 0);
    default:
        break;
    }
    return 0x61;
}

u32 ov14_021EE4AC(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    ov14_021E8588(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9604, 0x63);
}

u32 ov14_021EE4D8(PCBoxWork *work) {
    return ov14_021F10DC(work, 0x64);
}

u32 ov14_021EE4E4(PCBoxWork *work) {
    PCBox_HideBoxTraySprites(work);
    return PCBox_WaitForUpdateFunc(work, ov14_021E98AC, 0x65);
}

u32 ov14_021EE4FC(PCBoxWork *work) {
    PCBox_CreateGridInput(work, 0, 7);
    PCBox_SetSpriteAnim(work->view, 9, 8);
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_CreateSelectedIconOutline(work);
    PCBox_SetSelectedIconOutlineVisible(work, 1);
    PCBox_SetIconsTranslucent(work, 2, 0);
    return 0xe;
}

u32 ov14_021EE538(PCBoxWork *work) {
    PCBox_SetSpriteAnim(work->view, 9, 8);
    PCBox_SetSpriteVisible(work->view, 9, 0);
    ov14_021E8588(work->view->panels);
    ov14_021E8314(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E99F0, 0x67);
}

u32 ov14_021EE578(PCBoxWork *work) {
    if (PCStorage_CountMonsAndEggsInBox(work->storage, work->trayBox) == 0x1e) {
        PCBox_PrintMonActionMessage(work, 0, 4, 0x25);
        work->nextState = 0x5e;
        return 6;
    }
    PCBox_SetIconsTranslucent(work, 2, 0);
    PCBox_CloseMessageWindow(work->view, 0x27);
    PCBox_HideBoxTraySprites(work);
    return PCBox_WaitForUpdateFunc(work, ov14_021E98AC, 0x68);
}

u32 ov14_021EE5C8(PCBoxWork *work) {
    ov14_021E8048(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E952C, 0x69);
}

u32 PCBox_State_SwitchBox(PCBoxWork *work) {
    void *cb;
    u8 old = work->activeBox;
    work->activeBox = work->trayBox;
    if (old == work->trayBox) {
        return 0x6a;
    }
    if (old > work->trayBox) {
        PCBox_LoadBoxIconImages(work, work->activeBox);
        PCBox_LoadScrolledWallpaper(work, PCBox_GetWallpaperGfxIndex(work, work->activeBox), 0);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 0);
        cb = PCBox_UpdateScrollToPrevBox;
    } else {
        PCBox_LoadBoxIconImages(work, work->activeBox);
        PCBox_LoadScrolledWallpaper(work, PCBox_GetWallpaperGfxIndex(work, work->activeBox), 1);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 0);
        cb = PCBox_UpdateScrollToNextBox;
    }
    return PCBox_WaitForUpdateFunc(work, cb, 0x6a);
}

u32 ov14_021EE664(PCBoxWork *work) {
    work->partyLayout = 2;
    PCBox_AllocMoveWork(work);
    return PCBox_WaitForUpdateFunc(work, PCBox_AnimateMoveToFirstEmptySlot, 0x6b);
}

u32 ov14_021EE684(PCBoxWork *work) {
    PCBoxMoveWork *moveWork = work->view->temp;
    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_ApplyMonMove(work);
    PCBox_RevertShayminFormOnMove(work, moveWork->from, moveWork->to);
    PCBox_FreeMoveWork(work);
    ov14_021E8094(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E954C, 0x6c);
}

u32 ov14_021EE6D0(PCBoxWork *work) {
    PCBox_DrawReturnButton(work, 0);
    ov14_021E8328(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E95B4, 0x6d);
}

u32 ov14_021EE6F8(PCBoxWork *work) {
    PCBox_CreateGridInput(work, 0, 0);
    PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
    PCBox_ShowSlotMonInfo(work, 0x1e);
    PCBox_SetSpriteAnim(work->view, 9, 8);
    return 0x5b;
}

u32 ov14_021EE728(PCBoxWork *work) {
    PCBoxMoveWork *moveWork = work->view->temp;
    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_ApplyMonMove(work);
    PCBox_RevertShayminFormOnMove(work, moveWork->from, moveWork->to);
    PCBox_FreeMoveWork(work);
    ov14_021F6678(work->view, 0x28);
    if (work->selectedSlot == 0xff) {
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
        GridInputHandler_SetNextInput(work->view->gridInput, 0);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        PCBox_ShowSlotMonInfo(work, 0x1e);
    } else {
        PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
        GridInputHandler_SetNextInput(work->view->gridInput, 7);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    }
    return 0x5b;
}

u32 ov14_021EE7B4(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0xc;
}

u32 ov14_021EE7C4(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x29;
}

u32 ov14_021EE7D4(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x24;
}

u32 ov14_021EE7E4(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x5b;
}

u32 ov14_021EE7F4(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
    return 0x5b;
}

u32 ov14_021EE810(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x61;
}

u32 ov14_021EE820(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x16;
}

u32 ov14_021EE830(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x3d;
}

u32 ov14_021EE840(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x42;
}

u32 ov14_021EE850(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x51;
}

u32 ov14_021EE860(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
    return 0x51;
}

u32 PCBox_State_ItemModeInput(PCBoxWork *work) {
    u32 v = PCBox_FindTouchedBoxSlot();
    if (v != -1) {
        if (PCBox_GetSlotMonData(work, v, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_BOX02);
            System_GetTouchNewCoords(&work->view->touchX, &work->view->touchY);
            PCBox_ShowSlotMonInfo(work, v);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return ov14_021F18B0(work, v);
        }
        if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
            v = work->selectedSlot;
            PCBox_StartCursorSlide(work->view, v, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, v);
            PCBox_SlideOutActionMenu(work->view->panels);
            PCBox_SlideInPartyButton(work->view->panels);
            ov14_021E82A8(work->view->panels);
            ov14_021E8328(work->view->panels);
            PCBox_HideSelectedIconOutline(work);
            if (work->view->heldItem != 0) {
                ov14_021E88F8(work->view->panels);
            }
            return PCBox_WaitForUpdateFunc(work, ov14_021EA674, 0x76);
        }
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        PCBox_ClearMonInfo(work);
        return 0x75;
    }
    v = PCBox_GetItemModeMenuInput(work);
    switch (v) {
    case 0x1e:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(work->view, 0xb, 0);
        return ov14_021F1128(work);
    case 0x1f:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(work->view, 0xb, 0);
        PCBox_SetSpriteVisible(work->view, 9, 0);
        GridInputHandler_SetNextInput(work->view->gridInput, 0x1e);
        return PCBox_ScrollToPrevBox(work, 0x75);
    case 0x20:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_SetSpriteVisible(work->view, 0xb, 0);
        PCBox_SetSpriteVisible(work->view, 9, 0);
        GridInputHandler_SetNextInput(work->view->gridInput, 0x1e);
        return PCBox_ScrollToNextBox(work, 0x75);
    case 0x21:
        PlaySE(SEQ_SE_DP_OPEN7);
        return PCBox_FlashPanel(work, 8, 0xab);
    case 0x22:
        PlaySE(SEQ_SE_DP_OPEN7);
        return ov14_021F2330(work, 9, 0xac);
    case 0x24:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 7, 0xad);
    case 0x25:
        PlaySE(SEQ_SE_DP_SELECT);
        v = work->selectedSlot;
        PCBox_StartCursorSlide(work->view, v, GridInputHandler_GetNextInput(work->view->gridInput));
        GridInputHandler_SetNextInput(work->view->gridInput, v);
        return PCBox_FlashPanel(work, 0xb, 0xae);
    case -1:
        if (GridInputHandler_GetNextInput(work->view->gridInput) == 0x1e) {
            if (gSystem.newKeys & PAD_KEY_LEFT) {
                PlaySE(SEQ_SE_DP_SELECT);
                PCBox_ClearMonInfoAndSelection(work);
                return PCBox_ScrollToPrevBox(work, 0x75);
            }
            if (gSystem.newKeys & PAD_KEY_RIGHT) {
                PlaySE(SEQ_SE_DP_SELECT);
                PCBox_ClearMonInfoAndSelection(work);
                return PCBox_ScrollToNextBox(work, 0x75);
            }
        }
        break;
    case -3: {
        u32 n = GridInputHandler_GetNextInput(work->view->gridInput);
        if (n < 0x1e) {
            PCBox_ShowSlotMonInfo(work, n);
        } else if (n != 0x24 && n != 0x25) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_WaitForPendingUpdateFunc(work, 0x76);
    }
    case 0x23:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfo(work);
        return PCBox_FlashPanel(work, 0xa, 0x93);
    case -2:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 0xa, 0x94);
    case -4: {
        u32 n = GridInputHandler_GetNextInput(work->view->gridInput);
        if (n < 0x1e) {
            PCBox_ShowSlotMonInfo(work, n);
        } else if (n != 0x24 && n != 0x25) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
    default:
        if (PCBox_GetSlotMonData(work, v, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            PCBox_ShowSlotMonInfo(work, v);
            PCBox_StartCursorSlide(work->view, 0x24, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, 0x24);
            return ov14_021F1808(work, v);
        }
        break;
    }
    return 0x75;
}

u32 ov14_021EEBFC(PCBoxWork *work) {
    if (PCBox_IsSpriteVisible(work->view, 0xb) == 1) {
        PCBox_HideSelectedIconOutline(work);
        ov14_021F391C(work->view, 1);
        PCBox_SetSpriteAnim(work->view, 0xb, 2);
        ov14_021E88F8(work->view->panels);
    }
    PCBox_CloseMessageWindow(work->view, 0x25);
    ov14_021E8328(work->view->panels);
    if (work->selectedSlot < 0x1e) {
        PCBox_SlideInPartyButton(work->view->panels);
        ov14_021E82A8(work->view->panels);
        PCBox_SetIconsTranslucent(work, 0x81, 1);
    } else {
        PCBox_SetIconsTranslucent(work, 0x82, 1);
    }
    return PCBox_WaitForUpdateFunc(work, ov14_021E9450, 0x7b);
}

u32 ov14_021EEC8C(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x75;
}

u32 ov14_021EEC9C(PCBoxWork *work) {
    u8 x;
    u8 y;
    u16 idx;
    if (work->selectedSlot >= 0x1e && ItemIdIsMail(work->view->heldItem) == 1) {
        idx = (u16)(work->selectedSlot - 0x1e);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)idx);
        DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, idx), &x, &y);
        ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
        PlaySE(SEQ_SE_DP_BOX03);
        PCBox_PrintItemActionMessage(work, 4, 0x25);
        work->nextState = 0x77;
        return 6;
    }
    PCBox_PrintItemActionMessage(work, 2, 0x25);
    return PCBox_ShowYesNo(work, 2);
}

u32 ov14_021EED28(PCBoxWork *work) {
    u8 x;
    u8 y;
    u16 idx;
    if (PCBox_IsSpriteAnimated(work->view, 0xb) == 0) {
        idx = work->selectedSlot;
        if (idx >= 0x1e) {
            idx = (u16)(idx - 0x1e);
        }
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)idx);
        DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, idx), &x, &y);
        ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
        ov14_021F391C(work->view, 0);
        PCBox_SetSpriteVisible(work->view, 0xb, 0);
        PCBox_PrintItemActionMessage(work, 3, 0x25);
        PCBox_DrawBagGiveButton(work, 0);
        work->view->heldItem = 0;
        work->nextState = 0x77;
        return 6;
    }
    return 0x79;
}

u32 ov14_021EEDB8(PCBoxWork *work) {
    u8 x;
    u8 y;
    u16 idx = work->selectedSlot;
    if (idx < 0x1e) {
        PCBox_SlideInPartyButton(work->view->panels);
        ov14_021E82A8(work->view->panels);
        PCBox_SetIconsTranslucent(work, 0x81, 1);
    } else {
        idx = (u16)(idx - 0x1e);
        PCBox_SetIconsTranslucent(work, 0x82, 1);
    }
    ov14_021E8328(work->view->panels);
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)idx);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, idx), &x, &y);
    ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
    PCBox_HideSelectedIconOutline(work);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 2);
    ov14_021E88F8(work->view->panels);
    PCBox_CloseMessageWindow(work->view, 0x25);
    if (work->selectedSlot < 0x1e) {
        PCBox_SetIconsTranslucent(work, 0x81, 1);
    } else {
        PCBox_SetIconsTranslucent(work, 0x82, 1);
    }
    return PCBox_WaitForUpdateFunc(work, ov14_021E9450, 0x7b);
}

u32 ov14_021EEE94(PCBoxWork *work) {
    if (PCBox_IsSpriteAnimated(work->view, 0xb) == 1) {
        return 0x7b;
    }
    ov14_021F391C(work->view, 0);
    PCBox_SetSpriteVisible(work->view, 0xb, 0);
    PCBox_SetSpriteVisible(work->view, 9, 1);
    if (work->selectedSlot >= 0x1e) {
        return 0x8b;
    }
    return 0x75;
}

u32 ov14_021EEED4(PCBoxWork *work) {
    u8 x;
    u8 y;
    u16 idx = work->selectedSlot;
    if (idx >= 0x1e) {
        idx -= 0x1e;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)idx);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, idx), &x, &y);
    ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
    PlaySE(SEQ_SE_DP_BOX03);
    PCBox_PrintItemActionMessage(work, 5, 0x25);
    work->nextState = 0x77;
    return 6;
}

u32 ov14_021EEF34(PCBoxWork *work) {
    u8 x;
    u8 y;
    u16 idx = work->selectedSlot;
    if (idx >= 0x1e) {
        idx -= 0x1e;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)idx);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, idx), &x, &y);
    ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
    PCBox_PrintItemHeld(work, work->itemId, 0x25);
    work->nextState = 0x77;
    return 6;
}

u32 ov14_021EEF8C(PCBoxWork *work) {
    PCBoxHeldItemMove *blk = work->view->temp;
    if (blk->toSlot == work->selectedSlot || work->view->heldItem == 0) {
        ov14_021F1F38(work);
        if (work->view->heldItem != 0) {
            PlaySE(SEQ_SE_DP_BOX01);
            PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 0);
        }
        GridInputHandler_SetNextInput(work->view->gridInput, 0x24);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        return 0x75;
    }
    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_SetSlotIconTranslucent(work->view, blk->toSlot, 0);
    PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 1);
    PCBox_HideSelectedIconOutline(work);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 2);
    return 0x7f;
}

u32 PCBox_State_SwapHeldItems(PCBoxWork *work) {
    PCBoxHeldItemMove *blk;
    u16 held;
    if (PCBox_IsSpriteAnimated(work->view, 0xb) == 1) {
        return 0x7f;
    }
    ov14_021F391C(work->view, 0);
    PCBox_SetSpriteVisible(work->view, 0xb, 0);
    blk = work->view->temp;
    held = (u16)PCBox_GetSlotMonData(work, blk->toSlot, MON_DATA_HELD_ITEM, 0);
    PCBox_SetSlotMonData(work, blk->toSlot, MON_DATA_HELD_ITEM, &work->view->heldItem);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, blk->toSlot)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, blk->toSlot, work->view->slotIcons[blk->toSlot]);
    }
    PCBox_ShowSlotMonInfo(work, blk->toSlot);
    work->view->heldItem = held;
    PCBox_SetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, &work->view->heldItem);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, work->selectedSlot, work->view->slotIcons[work->selectedSlot]);
    }
    if (work->view->heldItem == 0) {
        work->selectedSlot = (u8)blk->toSlot;
        ov14_021F1F38(work);
        PCBox_SlideInPartyButton(work->view->panels);
        ov14_021E82A8(work->view->panels);
        ov14_021E8328(work->view->panels);
        GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x75);
    }
    PlaySE(SEQ_SE_DP_BOX02);
    PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 1);
    PCBox_SetSpriteVisible(work->view, 0xb, 1);
    PCBox_CreateHeldItemIconOutline(work->view);
    return PCBox_WaitForUpdateFunc(work, ov14_021EA728, 0x80);
}

u32 ov14_021EF190(PCBoxWork *work) {
    PCBoxHeldItemMove *heldItemMove = work->view->temp;
    PCBox_SetSlotIconTranslucent(work->view, heldItemMove->toSlot, 0);
    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_HideSelectedIconOutline(work);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 2);
    GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    work->selectedSlot = heldItemMove->toSlot;
    ov14_021F1F38(work);
    return 0x81;
}

u32 ov14_021EF1EC(PCBoxWork *work) {
    if (PCBox_IsSpriteAnimated(work->view, 0xb) == 1) {
        return 0x81;
    }
    ov14_021F391C(work->view, 0);
    PCBox_SetSpriteVisible(work->view, 0xb, 0);
    PCBox_SlideInPartyButton(work->view->panels);
    ov14_021E82A8(work->view->panels);
    ov14_021E8328(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x75);
}

u32 PCBox_State_ItemModeBoxTrayInput(PCBoxWork *work) {
    u32 v = ov14_021F6A34();
    if (v != -1) {
        if (PCBox_GetSlotMonData(work, v + 0x1e, MON_DATA_SPECIES_EXISTS, 0)) {
            PCBox_ShowSlotMonInfo(work, v + 0x1e);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return ov14_021F19F0(work, v + 0x1e);
        }
        PCBox_ClearMonInfo(work);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(v + 0x1e));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            return ov14_021F0EE8(work, 0x82);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return PCBox_HideInfoPanelAndWait(work, 0x82);
        }
        return 0x82;
    }
    v = PCBox_FindTouchedBoxSlot();
    if (v != -1) {
        if (PCBox_GetSlotMonData(work, v, MON_DATA_SPECIES_EXISTS, 0)) {
            PCBox_ShowSlotMonInfo(work, v);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return ov14_021F19F0(work, v);
        }
        PCBox_ClearMonInfo(work);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            return ov14_021F0EE8(work, 0x82);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return PCBox_HideInfoPanelAndWait(work, 0x82);
        }
        return 0x82;
    }
    if (PCBox_IsSummaryShortcutPressed(work) == 1) {
        v = GridInputHandler_GetNextInput(work->view->gridInput);
        if (PCBox_GetSlotMonData(work, v, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            work->selectedSlot = (u8)v;
            work->unk_26 = 1;
            return ov14_021F2330(work, 0xf, 0x97);
        }
        return 0x82;
    }
    v = PCBox_GetBoxTrayInput(work);
    switch (v) {
    case 0x25:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 0);
    case 0x26:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 1);
    case 0x27:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 2);
    case 0x28:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 3);
    case 0x29:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 4);
    case 0x2a:
        PlaySE(SEQ_SE_DP_DECIDE);
        PCBox_ClearMonInfoAndSelection(work);
        return ov14_021F0D58(work, 5);
    case 0x2b:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_ChangeTrayPage(work, -1);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            return ov14_021F0EE8(work, 0x82);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return PCBox_HideInfoPanelAndWait(work, 0x82);
        }
        return 0x82;
    case 0x2c:
        PlaySE(SEQ_SE_DP_SELECT);
        PCBox_ClearMonInfoAndSelection(work);
        PCBox_ChangeTrayPage(work, 1);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            return ov14_021F0EE8(work, 0x82);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return PCBox_HideInfoPanelAndWait(work, 0x82);
        }
        return 0x82;
    case 0x2d:
        PlaySE(SEQ_SE_DP_SELECT);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
        return PCBox_FlashPanel(work, 0xe, 0xaf);
    case -3: {
        u32 n = GridInputHandler_GetNextInput(work->view->gridInput);
        if (n < 0x24) {
            if (PCBox_ShowSlotMonInfo(work, n) == 1) {
                if (PCBox_IsSummaryButtonShown(work->view->panels) == 0) {
                    PCBox_DrawSummaryButton(work, 0);
                    PCBox_SlideInSummaryButton(work->view->panels);
                }
            } else if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
                PCBox_SlideOutSummaryButton(work->view->panels);
            }
        } else {
            PCBox_ClearMonInfo(work);
            if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
                PCBox_SlideOutSummaryButton(work->view->panels);
            }
        }
        PlaySE(SEQ_SE_DP_SELECT);
        if (work->view->pendingUpdateFunc != 0) {
            return PCBox_WaitForPendingUpdateFunc(work, 0x83);
        }
        break;
    }
    case 0x24:
        PlaySE(SEQ_SE_DP_CLOSE7);
        return ov14_021F2490(work, 1, 0xa1);
    case -2:
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 0) {
            PlaySE(SEQ_SE_DP_CLOSE7);
            return ov14_021F2490(work, 1, 0xa1);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        return ov14_021F0EE8(work, 0x82);
    default:
        if (PCBox_GetSlotMonData(work, v, MON_DATA_SPECIES_EXISTS, 0)) {
            PCBox_ShowSlotMonInfo(work, v);
            return ov14_021F1B4C(work, v);
        }
        break;
    case -4:
    case -1:
        break;
    }
    return 0x82;
}

u32 ov14_021EF6D4(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x82;
}

u32 ov14_021EF6E4(PCBoxWork *work) {
    PCBox_HideSelectedIconOutline(work);
    ov14_021E88F8(work->view->panels);
    return 0x85;
}

u32 ov14_021EF6FC(PCBoxWork *work) {
    PCBoxHeldItemMove *blk = work->view->temp;
    u16 prev;
    u16 held;
    if (BgCellManager_IsCellMoving(work->view->panels, 0x10) != 0) {
        return 0x85;
    }
    if (blk->toSlot == work->selectedSlot) {
        prev = blk->fromSlot;
        ov14_021F1F38(work);
        PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 0);
        GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
        if (ItemIdIsMail(work->view->heldItem) == 1 && prev != work->selectedSlot) {
            PlaySE(SEQ_SE_DP_BOX03);
            PCBox_PrintMailCantBeMoved(work, 0x25);
            work->nextState = 0x87;
            return 6;
        }
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        return 0x82;
    }
    PCBox_SetSlotIconTranslucent(work->view, blk->toSlot, 0);
    PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 1);
    held = (u16)PCBox_GetSlotMonData(work, blk->toSlot, MON_DATA_HELD_ITEM, 0);
    PCBox_SetSlotMonData(work, blk->toSlot, MON_DATA_HELD_ITEM, &work->view->heldItem);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, blk->toSlot)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, blk->toSlot, work->view->slotIcons[blk->toSlot]);
    }
    PCBox_ShowSlotMonInfo(work, blk->toSlot);
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)blk->toSlot);
    work->view->heldItem = held;
    PCBox_SetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, &work->view->heldItem);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, work->selectedSlot, work->view->slotIcons[work->selectedSlot]);
    }
    if (work->view->heldItem == 0) {
        work->selectedSlot = (u8)blk->toSlot;
        ov14_021F1F38(work);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        return 0x82;
    }
    PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 1);
    PCBox_SetSpriteVisible(work->view, 0xb, 1);
    PCBox_CreateHeldItemIconOutline(work->view);
    PlaySE(SEQ_SE_DP_BOX02);
    return PCBox_WaitForUpdateFunc(work, ov14_021EA928, 0x86);
}

u32 ov14_021EF8AC(PCBoxWork *work) {
    PCBoxHeldItemMove *heldItemMove = work->view->temp;
    u16 item;
    u8 old;
    u16 prev;
    PCBox_SetSlotIconTranslucent(work->view, heldItemMove->toSlot, 0);
    old = work->selectedSlot;
    prev = heldItemMove->fromSlot;
    work->selectedSlot = heldItemMove->toSlot;
    ov14_021F1F38(work);
    item = work->view->heldItem;
    work->view->heldItem = 0;
    if (ItemIdIsMail(item) == 1 && prev != old) {
        PlaySE(SEQ_SE_DP_BOX03);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 0);
        PCBox_PrintMailCantBeMoved(work, 0x25);
        work->nextState = 0x87;
        return 6;
    }
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x82;
}

u32 ov14_021EF920(PCBoxWork *work) {
    ov14_021F6688(work->view, 0x25);
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x82;
}

u32 ov14_021EF93C(PCBoxWork *work) {
    u32 v = ov14_021F7A50(work);
    if (!GridInputHandler_IsButtonInputMode(work->view->gridInput)) {
        v = -2;
    }
    switch (v) {
    case -3:
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_WaitForPendingUpdateFunc(work, 0x89);
    case -4:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case -2:
    case 0x24:
        PlaySE(SEQ_SE_DP_BOX01);
        return ov14_021F1C4C(work, 0xff);
    default:
        PlaySE(SEQ_SE_DP_BOX01);
        return ov14_021F1C4C(work, v);
    case -1:
        break;
    }
    return 0x88;
}

u32 ov14_021EF9BC(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x88;
}

u32 ov14_021EF9CC(PCBoxWork *work) {
    u16 held;
    u32 n;
    PCBox_SetSlotIconTranslucent(work->view, work->view->unk_044C, 0);
    if (work->view->unk_044C == work->view->unk_88CA) {
        work->view->heldItem = 0;
        PCBox_ShowSlotMonInfo(work, work->view->unk_044C);
        PCBox_SlideInSummaryButton(work->view->panels);
        return PCBox_WaitForUpdateFunc(work, ov14_021E9970, 0x82);
    }
    held = (u16)PCBox_GetSlotMonData(work, work->view->unk_044C, MON_DATA_HELD_ITEM, 0);
    PCBox_SetSlotMonData(work, work->view->unk_044C, MON_DATA_HELD_ITEM, &work->view->heldItem);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, work->view->unk_044C)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, work->view->unk_044C, work->view->slotIcons[work->view->unk_044C]);
    }
    PCBox_ShowSlotMonInfo(work, work->view->unk_044C);
    work->view->heldItem = held;
    PCBox_SetSlotMonData(work, work->view->unk_88CA, MON_DATA_HELD_ITEM, &work->view->heldItem);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, work->view->unk_88CA)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, work->view->unk_88CA, work->view->slotIcons[work->view->unk_88CA]);
    }
    if (work->view->heldItem == 0) {
        PCBox_SlideInSummaryButton(work->view->panels);
        return PCBox_WaitForUpdateFunc(work, ov14_021E9970, 0x82);
    }
    work->view->unk_044B = 1;
    PCBox_SetSpriteVisible(work->view, 0xb, 0);
    ov14_021F39A0(work->view, work->view->unk_044C, 2);
    PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
    n = PCBox_PrintItemNameAndDesc(work, work->view->heldItem);
    BgCellManager_PutWindowTiles(work->view->panels, 0x10, &work->view->windows[n]);
    BgCellManager_PutWindowTiles(work->view->panels, 0x10, &work->view->windows[n + 1]);
    PCBox_LoadInfoItemIcon(work->view, work->view->heldItem);
    ov14_021E88BC(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021EAA04, 0x88);
}

u32 ov14_021EFB64(PCBoxWork *work) {
    u32 v = PCBox_FindTouchedPartySlot();
    if (v != -1) {
        if (PCBox_GetSlotMonData(work, v + 0x1e, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_BOX02);
            System_GetTouchNewCoords(&work->view->touchX, &work->view->touchY);
            PCBox_ShowSlotMonInfo(work, v + 0x1e);
            PCBox_SetSpriteVisible(work->view, 9, 0);
            return ov14_021F1D6C(work, v + 0x1e);
        }
        if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
            v = (u8)(work->selectedSlot - 0x1e);
            PCBox_StartCursorSlide(work->view, v, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, v);
            PCBox_SlideOutActionMenu(work->view->panels);
            ov14_021E8328(work->view->panels);
            PCBox_HideSelectedIconOutline(work);
            if (work->view->heldItem != 0) {
                ov14_021E88F8(work->view->panels);
            }
            return PCBox_WaitForUpdateFunc(work, ov14_021EA674, 0x8c);
        }
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)v);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        PCBox_ClearMonInfo(work);
        return 0x8b;
    }
    v = PCBox_GetItemModePartyMenuInput(work);
    switch (v) {
    case 6:
        PlaySE(SEQ_SE_DP_DECIDE);
        return ov14_021F2490(work, 0, 0xb0);
    case 8:
        PlaySE(SEQ_SE_DP_DECIDE);
        return PCBox_FlashPanel(work, 7, 0xad);
    case -3: {
        u32 n = GridInputHandler_GetNextInput(work->view->gridInput);
        if (n <= 5) {
            PCBox_ShowSlotMonInfo(work, n + 0x1e);
        } else if (n != 8 && n != 9) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_WaitForPendingUpdateFunc(work, 0x8c);
    }
    case 7:
    case -2:
        PlaySE(SEQ_SE_DP_CLOSE7);
        return PCBox_FlashPanel(work, 0xa, 0x9f);
    case 9:
        PlaySE(SEQ_SE_DP_SELECT);
        v = (u8)(work->selectedSlot - 0x1e);
        PCBox_StartCursorSlide(work->view, v, GridInputHandler_GetNextInput(work->view->gridInput));
        GridInputHandler_SetNextInput(work->view->gridInput, v);
        return PCBox_FlashPanel(work, 0xb, 0xb1);
    case -4: {
        u32 n = GridInputHandler_GetNextInput(work->view->gridInput);
        if (n <= 5) {
            PCBox_ShowSlotMonInfo(work, n + 0x1e);
        } else if (n != 8 && n != 9) {
            PCBox_ClearMonInfo(work);
        }
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    }
    default:
        if (PCBox_GetSlotMonData(work, v + 0x1e, MON_DATA_SPECIES_EXISTS, 0)) {
            PlaySE(SEQ_SE_DP_DECIDE);
            PCBox_ShowSlotMonInfo(work, v + 0x1e);
            PCBox_StartCursorSlide(work->view, 8, GridInputHandler_GetNextInput(work->view->gridInput));
            GridInputHandler_SetNextInput(work->view->gridInput, 8);
            return ov14_021F1CDC(work, v + 0x1e);
        }
        break;
    case -1:
        break;
    }
    return 0x8b;
}

u32 ov14_021EFDE4(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x8b;
}

u32 ov14_021EFDF4(PCBoxWork *work) {
    PCBoxHeldItemMove *blk = work->view->temp;
    u16 prev;
    if (blk->toSlot == work->selectedSlot || work->view->heldItem == 0) {
        prev = blk->fromSlot;
        ov14_021F1F38(work);
        if (work->view->unk_044A == 0) {
            if (ItemIdIsMail(work->view->heldItem) == 1 && prev != work->selectedSlot) {
                PlaySE(SEQ_SE_DP_BOX03);
                BgCellManager_EraseCell(work->view->panels, 7);
                PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 0);
                PCBox_PrintItemActionMessage(work, 7, 0x28);
                work->nextState = 0x92;
                return 6;
            }
        } else {
            PCBox_CloseMessageWindow(work->view, 0x28);
            work->view->unk_044A = 0;
        }
        if (work->view->heldItem != 0) {
            PlaySE(SEQ_SE_DP_BOX01);
            PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 0);
        }
        GridInputHandler_SetNextInput(work->view->gridInput, 8);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        return 0x8b;
    }
    PCBox_CloseMessageWindow(work->view, 0x28);
    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_SetSlotIconTranslucent(work->view, blk->toSlot, 0);
    PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 1);
    PCBox_HideSelectedIconOutline(work);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 2);
    return 0x8e;
}

u32 ov14_021EFF00(PCBoxWork *work) {
    PCBoxHeldItemMove *blk;
    u16 held;
    if (PCBox_IsSpriteAnimated(work->view, 0xb) == 1) {
        return 0x8e;
    }
    ov14_021F391C(work->view, 0);
    PCBox_SetSpriteVisible(work->view, 0xb, 0);
    blk = work->view->temp;
    held = (u16)PCBox_GetSlotMonData(work, blk->toSlot, MON_DATA_HELD_ITEM, 0);
    PCBox_SetSlotMonData(work, blk->toSlot, MON_DATA_HELD_ITEM, &work->view->heldItem);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, blk->toSlot)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, blk->toSlot, work->view->slotIcons[blk->toSlot]);
    }
    PCBox_ShowSlotMonInfo(work, blk->toSlot);
    work->view->heldItem = held;
    PCBox_SetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, &work->view->heldItem);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, work->selectedSlot, work->view->slotIcons[work->selectedSlot]);
    }
    if (work->view->heldItem == 0) {
        if (work->view->unk_044A == 0) {
            work->selectedSlot = (u8)blk->toSlot;
            ov14_021F1F38(work);
            GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->selectedSlot - 0x1e));
            ov14_021E8328(work->view->panels);
            return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x8f);
        }
        work->view->unk_044A = 0;
        ov14_021F1F38(work);
        GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->selectedSlot - 0x1e));
        PCBox_SlideInPartyPanel(work->view->panels);
        PCBox_SetIconsTranslucent(work, 1, 1);
        return PCBox_WaitForUpdateFunc(work, ov14_021E9518, 0x8f);
    }
    PlaySE(SEQ_SE_DP_BOX02);
    PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 1);
    PCBox_SetSpriteVisible(work->view, 0xb, 1);
    PCBox_CreateHeldItemIconOutline(work->view);
    return PCBox_WaitForUpdateFunc(work, ov14_021EAF08, 0x90);
}

u32 ov14_021F00A0(PCBoxWork *work) {
    PCBox_ShowSlotMonInfo(work, work->selectedSlot);
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x8b;
}

u32 ov14_021F00BC(PCBoxWork *work) {
    PCBoxHeldItemMove *heldItemMove = work->view->temp;
    PCBox_SetSlotIconTranslucent(work->view, heldItemMove->toSlot, 0);
    PlaySE(SEQ_SE_DP_BOX01);
    PCBox_HideSelectedIconOutline(work);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 2);
    if (work->selectedSlot < 0x1e) {
        work->selectedSlot = heldItemMove->toSlot;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->selectedSlot - 0x1e));
    ov14_021F1F38(work);
    return 0x91;
}

u32 ov14_021F0120(PCBoxWork *work) {
    if (PCBox_IsSpriteAnimated(work->view, 0xb) == 1) {
        return 0x91;
    }
    ov14_021F391C(work->view, 0);
    PCBox_SetSpriteVisible(work->view, 0xb, 0);
    ov14_021E8328(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x8f);
}

u32 ov14_021F0164(PCBoxWork *work) {
    PCBox_CloseMessageWindow(work->view, 0x28);
    BgCellManager_DrawCell(work->view->panels, 7);
    GridInputHandler_SetNextInput(work->view->gridInput, 8);
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x8b;
}

u32 ov14_021F0198(PCBoxWork *work) {
    PlaySE(SEQ_SE_DP_BOX03);
    PCBox_PrintItemActionMessage(work, 5, 0x25);
    work->nextState = 0xe;
    return 6;
}

u32 ov14_021F01B8(PCBoxWork *work) {
    PlaySE(SEQ_SE_DP_BOX03);
    PCBox_PrintMailCantBeTaken(work, 0x25);
    work->nextState = 0xe;
    return 6;
}

u32 PCBox_StartFadeIn(PCBoxWork *work, u32 v) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_10);
    work->view->fadeNextState = v;
    return 2;
}

u32 PCBox_StartFadeOut(PCBoxWork *work, u32 v) {
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_10);
    work->view->fadeNextState = v;
    return 2;
}

u32 PCBox_WaitForUpdateFunc(PCBoxWork *work, void *cb, u32 value) {
    work->nextState = value;
    PCBox_SetUpdateFunc(work->view, cb);
    return 5;
}

u32 PCBox_WaitForPendingUpdateFunc(PCBoxWork *work, u32 value) {
    work->nextState = value;
    PCBox_StartPendingUpdateFunc(work->view);
    return 5;
}

#pragma force_active on

int PCBox_ShowYesNo(PCBoxWork *work, int state) {
    if (state == 1) {
        PCBox_ShowYesNoPrompt(work, 1);
    } else {
        PCBox_ShowYesNoPrompt(work, 0);
    }
    work->view->yesNoHandler = state;
    return 7;
}

#pragma force_active reset

u32 PCBox_StartSubApp(PCBoxWork *work, u32 value) {
    work->subAppIndex = value;
    work->nextState = 9;
    return PCBox_StartFadeOut(work, 1);
}

u32 PCBox_ScrollToPrevBox(PCBoxWork *work, u32 next) {
    work->activeBox = (work->activeBox == 0) ? 0x11 : work->activeBox - 1;
    PCBox_LoadBoxIconImages(work, work->activeBox);
    PCBox_LoadScrolledWallpaper(work, PCBox_GetWallpaperGfxIndex(work, work->activeBox), 0);
    PCBox_SetSpriteAnim(work->view, 0, 2);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        PCBox_SlideOutActionMenu(work->view->panels);
        if (work->args->unk8 == PC_BOX_MODE_MOVE_POKEMON || work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
            PCBox_SlideInPartyButton(work->view->panels);
            ov14_021E82A8(work->view->panels);
        }
    }
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateScrollToPrevBox, next);
}

u32 PCBox_ScrollToNextBox(PCBoxWork *work, u32 next) {
    work->activeBox = (work->activeBox == 0x11) ? 0 : work->activeBox + 1;
    PCBox_LoadBoxIconImages(work, work->activeBox);
    PCBox_LoadScrolledWallpaper(work, PCBox_GetWallpaperGfxIndex(work, work->activeBox), 1);
    PCBox_SetSpriteAnim(work->view, 1, 4);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        PCBox_SlideOutActionMenu(work->view->panels);
        if (work->args->unk8 == PC_BOX_MODE_MOVE_POKEMON || work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
            PCBox_SlideInPartyButton(work->view->panels);
            ov14_021E82A8(work->view->panels);
        }
    }
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateScrollToNextBox, next);
}

u32 PCBox_StartMoveModeBoxDrag(PCBoxWork *work, u32 idx) {
    work->selectedSlot = idx;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 0);
    PCBox_CreateSelectedIconOutline(work);
    PCBox_AllocMoveWork(work);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        PCBox_SlideOutPartyButton(work->view->panels);
        ov14_021E8294(work->view->panels);
        ov14_021E8314(work->view->panels);
    }
    PCBox_SlideOutActionMenu(work->view->panels);
    work->partyLayout = 0;
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateMoveModeBoxDrag, 0xd);
}

u32 PCBox_StartWithdrawDrag(PCBoxWork *work, u32 idx) {
    work->selectedSlot = idx;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 0);
    PCBox_CreateSelectedIconOutline(work);
    ov14_021F3044(work);
    PCBox_SetIconsTranslucent(work, 1, 1);
    PCBox_SetIconsTranslucent(work, 2, 1);
    PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 0);
    PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
    if (Party_GetCount(work->party) != 6) {
        PCBox_PrintPanelHint(work, 0x28, 1);
    } else {
        PCBox_PrintPanelHint(work, 0x28, 3);
    }
    PCBox_AllocMoveWork(work);
    PCBox_SlideOutActionMenu(work->view->panels);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        ov14_021E8314(work->view->panels);
    }
    work->partyLayout = 2;
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateWithdrawDrag, 0x57);
}

u32 ov14_021F04D4(PCBoxWork *work, u32 idx) {
    work->selectedSlot = idx;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 1);
    PCBox_CreateSelectedIconOutline(work);
    PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
    PCBox_SlideInActionMenu(work->view->panels);
    ov14_021E8314(work->view->panels);
    ov14_021E8824(work->view);
    return PCBox_WaitForUpdateFunc(work, ov14_021EA068, 0x58);
}

u32 ov14_021F0530(PCBoxWork *work, u32 idx) {
    work->selectedSlot = idx;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 1);
    PCBox_CreateSelectedIconOutline(work);
    PCBox_SlideInActionMenu(work->view->panels);
    PCBox_SlideOutPartyButton(work->view->panels);
    ov14_021E8294(work->view->panels);
    ov14_021E8314(work->view->panels);
    ov14_021E8824(work->view);
    return PCBox_WaitForUpdateFunc(work, ov14_021EA0B8, 0x4a);
}

u32 PCBox_StartMoveModePartyDrag(PCBoxWork *work, u32 idx) {
    void *mon;
    work->selectedSlot = idx;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 0);
    PCBox_CreateSelectedIconOutline(work);
    mon = Party_GetMonByIndex(work->party, idx - 0x1e);
    if (PCBox_PartyHasOtherUsableMon(work, idx - 0x1e) == 0 && PCStorage_CountMonsInBox(work->storage, work->activeBox) == 0) {
        PCBox_PrintPanelHint(work, 0x28, 8);
    } else if (ItemIdIsMail(GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) == 1) {
        PCBox_PrintPanelHint(work, 0x28, 6);
    } else if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL) != 0) {
        PCBox_PrintPanelHint(work, 0x28, 7);
    } else {
        PCBox_PrintPanelHint(work, 0x28, 0);
    }
    PCBox_AllocMoveWork(work);
    PCBox_SlideOutActionMenu(work->view->panels);
    ov14_021E8314(work->view->panels);
    work->partyLayout = 1;
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateMoveModePartyDrag, 0x27);
}

u32 PCBox_StartDepositDrag(PCBoxWork *work, u32 idx) {
    void *mon;
    u32 i;
    work->selectedSlot = idx;
    for (i = 0x1e; i < 0x24; i++) {
        if (i == work->selectedSlot) {
            PCBox_SetIconLayer(work->view, i, 0);
        } else {
            PCBox_SetIconLayer(work->view, i, 1);
        }
    }
    PCBox_CreateSelectedIconOutline(work);
    PCBox_SetIconsTranslucent(work, 2, 1);
    PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 0);
    PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
    mon = Party_GetMonByIndex(work->party, idx - 0x1e);
    if (PCBox_PartyHasOtherUsableMon(work, idx - 0x1e) == 0) {
        PCBox_PrintPanelHint(work, 0x28, 8);
    } else if (ItemIdIsMail(GetMonData(mon, MON_DATA_HELD_ITEM, NULL)) == 1) {
        PCBox_PrintPanelHint(work, 0x28, 6);
    } else if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, NULL) != 0) {
        PCBox_PrintPanelHint(work, 0x28, 7);
    } else if (PCStorage_CountEmptySpotsInBox(work->storage, work->activeBox) == 0) {
        PCBox_PrintPanelHint(work, 0x28, 2);
    } else {
        PCBox_PrintPanelHint(work, 0x28, 0);
    }
    PCBox_AllocMoveWork(work);
    PCBox_SlideOutActionMenu(work->view->panels);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        ov14_021E8314(work->view->panels);
    }
    work->partyLayout = 1;
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateDepositDrag, 0x6e);
}

u32 ov14_021F0794(PCBoxWork *work, u32 idx) {
    work->selectedSlot = idx;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 1);
    PCBox_CreateSelectedIconOutline(work);
    PCBox_PrintMonActionMessage(work, work->selectedSlot, 1, 0x27);
    PCBox_SlideInActionMenu(work->view->panels);
    ov14_021E8314(work->view->panels);
    ov14_021E8824(work->view);
    return PCBox_WaitForUpdateFunc(work, ov14_021EA068, 0x6f);
}

u32 ov14_021F07F0(PCBoxWork *work, u32 idx) {
    work->selectedSlot = idx;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 1);
    PCBox_CreateSelectedIconOutline(work);
    PCBox_SlideInActionMenu(work->view->panels);
    ov14_021E8314(work->view->panels);
    ov14_021E8824(work->view);
    return PCBox_WaitForUpdateFunc(work, ov14_021EA0B8, 0x4c);
}

u32 PCBox_StartBoxTrayDrag(PCBoxWork *work, u32 idx) {
    work->selectedSlot = idx;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 0);
    PCBox_CreateSelectedIconOutline(work);
    if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
        PCBox_SlideOutChangeBoxButton(work->view->panels);
    } else if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
        PCBox_SlideOutSummaryButton(work->view->panels);
    }
    ov14_021E8824(work->view);
    PCBox_DrawSummaryButton(work, 0);
    PCBox_AllocMoveWork(work);
    work->partyLayout = 2;
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateBoxTrayDrag, 0x2a);
}

void PCBox_AllocMoveWork(PCBoxWork *work) {
    PCBoxMoveWork *moveWork;
    u32 i;
    PCBoxMoveEntry *entry;

    moveWork = Heap_Alloc(HEAP_ID_10, sizeof(PCBoxMoveWork));
    for (i = 0, entry = moveWork->entries; i < 7; i++, entry++) {
        entry->mon = AllocMonZeroed(HEAP_ID_10);
    }
    moveWork->timer = 0;
    work->view->temp = moveWork;
}

void PCBox_FreeMoveWork(PCBoxWork *work) {
    PCBoxMoveWork *moveWork;
    u32 i;
    PCBoxMoveEntry *entry;

    moveWork = work->view->temp;
    entry = moveWork->entries;
    for (i = 0; i < 7; i++, entry++) {
        Heap_Free(entry->mon);
    }
    Heap_Free(moveWork);
}

u32 PCBox_HandleItemCommand(PCBoxWork *work) {
    u32 item;
    if (PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_IS_EGG, 0) != 0) {
        PCBox_SetSpriteVisible(work->view, 9, 0);
        PCBox_SlideOutActionMenu(work->view->panels);
        return PCBox_WaitForUpdateFunc(work, ov14_021E9450, 0x4f);
    }
    item = PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, 0);
    if (item == 0) {
        work->unk_2C = 0x26;
        return PCBox_StartSubApp(work, 1);
    }
    PCBox_SetSpriteVisible(work->view, 9, 0);
    if (ItemIdIsMail(item) == 1) {
        PCBox_SlideOutActionMenu(work->view->panels);
        return PCBox_WaitForUpdateFunc(work, ov14_021E9450, 0x50);
    }
    ov14_021E83F4(work->view->panels);
    PCBox_PrintTakeItemPrompt(work, item, 0x25);
    return PCBox_ShowYesNo(work, 0);
}

u32 ov14_021F09BC(PCBoxWork *work) {
    switch (work->args->unk8) {
    case 0:
        PCBox_CreateGridInput(work, 0, 9);
        break;
    case 1:
        PCBox_CreateGridInput(work, 2, 0x24);
        break;
    case 2:
    default:
        if (work->selectedSlot < 0x1e) {
            PCBox_CreateGridInput(work, 3, 0x27);
        } else {
            PCBox_CreateGridInput(work, 5, 0xb);
        }
        break;
    }
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_CloseMessageWindow(work->view, 0x26);
    if (work->trayOpen != 0) {
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    if (work->selectedSlot < 0x1e) {
        PCBox_SetIconsTranslucent(work, 1, 0);
    }
    PCBox_SetIconsTranslucent(work, 2, 0);
    PCBox_SlideOutMarkingMenu(work->view->panels);
    if (work->args->unk8 == PC_BOX_MODE_MOVE_POKEMON && work->selectedSlot < 0x1e) {
        return PCBox_WaitForUpdateFunc(work, PCBox_UpdateMarkingMenuSlide, 0x17);
    }
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateMarkingMenuSlide, 0xe);
}

u32 ov14_021F0A80(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_SlideOutActionMenu(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9434, 0x13);
}

u32 ov14_021F0AAC(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_SlideOutActionMenu(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9450, 0x19);
}

u32 ov14_021F0AD8(PCBoxWork *work) {
    work->unk_23 = 1;
    PCBox_SetBoxArrowsVisible(work->view, 0);
    PCBox_SetIconsTranslucent(work, 1, 1);
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_SlideOutPartyButton(work->view->panels);
    ov14_021E8294(work->view->panels);
    ov14_021E8314(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x22);
}

u32 ov14_021F0B34(PCBoxWork *work) {
    work->unk_23 = 0;
    PCBox_SetSpriteVisible(work->view, 9, 0);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        return ov14_021EC710(work);
    }
    PCBox_ClearMonInfoAndSelection(work);
    return ov14_021F1100(work, 0x25);
}

void ov14_021F0B70(PCBoxWork *work) {
    ov14_021E81A8(work->view->panels, 1);
    ov14_021F63F0(work->view);
    ov14_021F63A8(work->view);
    ov14_021E8368(work->view->panels);
    ov14_021E82DC(work);
    ov14_021E7E78(work->view->panels);
    ov14_021F30B0(work);
}

void ov14_021F0BB4(PCBoxWork *work) {
    ov14_021E81A8(work->view->panels, 1);
    ov14_021F63F0(work->view);
    ov14_021F63B8(work->view);
    ov14_021E8368(work->view->panels);
    ov14_021E7E88(work->view->panels);
    ov14_021F311C(work);
}

void ov14_021F0BF4(PCBoxWork *work) {
    ov14_021F30B0(work);
    ov14_021E7E78(work->view->panels);
}

u32 ov14_021F0C0C(PCBoxWork *work) {
    work->selectedSlot = 0xff;
    PCBox_SetSpriteVisible(work->view, 9, 0);
    ov14_021E8020(work->view->panels);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        PCBox_SlideOutActionMenu(work->view->panels);
    }
    return PCBox_WaitForUpdateFunc(work, ov14_021E952C, 0x2d);
}

u32 ov14_021F0C58(PCBoxWork *work) {
    work->selectedSlot = 0xff;
    PCBox_SetIconsTranslucent(work, 1, 1);
    ov14_021E805C(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E954C, 0x2e);
}

u32 ov14_021F0C88(PCBoxWork *work) {
    work->trayOpen = 1;
    work->unk_29 = 0;
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_SlideOutPartyButton(work->view->panels);
    ov14_021E8294(work->view->panels);
    ov14_021E8314(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E94BC, 0x2f);
}

u32 ov14_021F0CD8(PCBoxWork *work) {
    work->trayOpen = 0;
    PCBox_SetSpriteAnim(work->view, 9, 8);
    PCBox_SetSpriteVisible(work->view, 9, 0);
    if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
        return ov14_021F0EE8(work, 0x31);
    }
    if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
        return PCBox_HideInfoPanelAndWait(work, 0x31);
    }
    return PCBox_CloseBoxTray(work, 0x32);
}

u32 PCBox_HideInfoPanelAndWait(PCBoxWork *work, u32 next) {
    PCBox_SlideOutSummaryButton(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9970, next);
}

u32 ov14_021F0D58(PCBoxWork *work, s32 delta) {
    u32 st;
    s32 cur;
    s32 nv;
    u8 x;
    u8 y;
    st = (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) ? 0x82 : 0x29;
    cur = work->trayBox;
    nv = delta + cur / 6 * 6;
    if (nv != cur) {
        work->trayBox = nv;
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
    }
    if (work->trayBox == work->activeBox) {
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
            GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
            PCBox_SetSpriteAnim(work->view, 9, 0xe);
            return ov14_021F0EE8(work, st);
        }
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6 + 0x25));
            GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
            PCBox_SetSpriteAnim(work->view, 9, 0xe);
            PCBox_SlideOutSummaryButton(work->view->panels);
            return PCBox_WaitForUpdateFunc(work, ov14_021E9970, st);
        }
        PCBox_SetSpriteAnim(work->view, 9, 0xe);
        return st;
    }
    GridInputHandler_SetNextInput(work->view->gridInput, 0x2d);
    if (GridInputHandler_IsButtonInputMode(work->view->gridInput) == 1) {
        DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, 0x2d), &x, &y);
        ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
        PCBox_SetSpriteAnim(work->view, 9, 8);
        PCBox_SetSpriteVisible(work->view, 9, 1);
    }
    if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
        return st;
    }
    PCBox_DrawChangeBoxButton(work);
    if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
        PCBox_SlideOutSummaryButton(work->view->panels);
    }
    return PCBox_WaitForUpdateFunc(work, ov14_021E9920, st);
}

u32 ov14_021F0EE8(PCBoxWork *work, u32 next) {
    PCBox_SlideOutChangeBoxButton(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9970, next);
}

u32 PCBox_JumpToTrayBox(PCBoxWork *work, u32 which) {
    void *cb;
    u32 prev;
    prev = work->activeBox;
    work->activeBox = work->trayBox;
    if (prev > work->trayBox) {
        PCBox_LoadBoxIconImages(work, work->activeBox);
        PCBox_LoadScrolledWallpaper(work, PCBox_GetWallpaperGfxIndex(work, work->activeBox), 0);
        cb = PCBox_UpdateScrollToPrevBox;
    } else {
        PCBox_LoadBoxIconImages(work, work->activeBox);
        PCBox_LoadScrolledWallpaper(work, PCBox_GetWallpaperGfxIndex(work, work->activeBox), 1);
        cb = PCBox_UpdateScrollToNextBox;
    }
    switch (which) {
    case 0:
        PCBox_UpdateCurrentBoxMarker(work);
        if (work->unk_23 == 0) {
            work->nextState = 0xc;
        } else {
            work->nextState = 0x24;
        }
        break;
    case 1:
        PCBox_UpdateCurrentBoxMarker(work);
        work->nextState = 0x3d;
        break;
    case 2:
        work->nextState = 0x47;
        break;
    case 3:
        PCBox_UpdateCurrentBoxMarker(work);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            PCBox_SlideOutChangeBoxButton(work->view->panels);
        }
        work->nextState = 0x29;
        break;
    case 4:
        PCBox_UpdateCurrentBoxMarker(work);
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            PCBox_SlideOutChangeBoxButton(work->view->panels);
        }
        work->nextState = 0x82;
        break;
    }
    return PCBox_WaitForUpdateFunc(work, cb, work->nextState);
}

void PCBox_ChangeTrayPage(PCBoxWork *work, s32 delta) {
    s32 v;
    v = work->trayBox + delta * 6;
    if (v < 0) {
        v += 0x12;
    } else if (v >= 0x12) {
        v -= 0x12;
    }
    work->trayBox = v;
    PCBox_LoadTrayPageThumbnails(work);
    PCBox_UpdateTrayCursor(work);
    PCBox_UpdateCurrentBoxMarker(work);
    PCBox_DrawTrayBoxNameAndCount(work);
    if (delta > 0) {
        PCBox_SetSpriteAnim(work->view, 5, 4);
    } else {
        PCBox_SetSpriteAnim(work->view, 4, 2);
    }
}

u32 PCBox_OpenBoxTray(PCBoxWork *work, u32 next) {
    PCBox_SetupBoxTraySprites(work);
    PCBox_UpdateCurrentBoxMarker(work);
    PCBox_UpdateTrayCursor(work);
    PCBox_DrawTrayBoxNameAndCount(work);
    PCBox_SlideInBoxTray(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateBoxTraySlideIn, next);
}

u32 PCBox_CloseBoxTray(PCBoxWork *work, u32 next) {
    PCBox_SlideOutBoxTray(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, PCBox_UpdateBoxTraySlideOut, next);
}

u32 ov14_021F10B4(PCBoxWork *work, u32 next) {
    PCBox_DrawExitButton(work, 0);
    ov14_021E8328(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E95B4, next);
}

u32 ov14_021F10DC(PCBoxWork *work, u32 next) {
    ov14_021E8314(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E95B4, next);
}

u32 ov14_021F1100(PCBoxWork *work, u32 next) {
    work->selectedSlot = 0xff;
    PCBox_SlideOutActionMenu(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9434, next);
}

u32 ov14_021F1128(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    work->view->unk_043C = work->activeBox % 6;
    work->unk_2C = work->view->unk_043C;
    if (PCBox_IsActionMenuOpen(work->view->panels) == 0) {
        return ov14_021ED5B0(work);
    } else {
        return ov14_021F1100(work, 0x35);
    }
}

u32 ov14_021F1170(PCBoxWork *work, s32 delta) {
    s32 v;
    u8 x;
    u8 y;
    work->trayBox = delta + work->trayBox / 6 * 6;
    PCBox_UpdateTrayCursor(work);
    PCBox_DrawTrayBoxNameAndCount(work);
    v = GridInputHandler_GetNextInput(work->view->gridInput);
    work->view->unk_043C = v;
    GridInputHandler_SetNextLastUnk0FInputs(work->view->gridInput, 8, v, v);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, 8), &x, &y);
    ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
    PCBox_SetSpriteAnim(work->view, 9, 8);
    PCBox_SetSpriteVisible(work->view, 9, 1);
    return 0x3d;
}

u32 ov14_021F11F8(PCBoxWork *work, s32 delta) {
    PCBox_ChangeTrayPage(work, delta);
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6));
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x3d;
}

void PCBox_ChangeWallpaperPage(PCBoxWork *work, s32 delta) {
    s32 v;
    v = work->view->wallpaperSelection + delta * 4;
    if (v < 0) {
        v += 0x18;
    } else if (v >= 0x18) {
        v -= 0x18;
    }
    work->view->wallpaperSelection = v;
    if (delta > 0) {
        PCBox_SetSpriteAnim(work->view, 5, 4);
    } else {
        PCBox_SetSpriteAnim(work->view, 4, 2);
    }
    PCBox_LoadWallpaperThumbnails(work);
    PCBox_UpdateCurrentWallpaperMarker(work);
    PCBox_DrawWallpaperName(work);
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->view->wallpaperSelection % 4));
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
}

void PCBox_SelectWallpaperInPage(PCBoxWork *work, u32 delta) {
    u8 x;
    u8 y;
    work->view->wallpaperSelection = delta + work->view->wallpaperSelection / 4 * 4;
    PCBox_UpdateWallpaperCursor(work);
    PCBox_DrawWallpaperName(work);
    work->view->unk_043C = GridInputHandler_GetNextInput(work->view->gridInput);
    ((void (*)(void *, s32, u8, u8))GridInputHandler_SetNextLastUnk0FInputs)(
        work->view->gridInput,
        6,
        (u8)work->view->unk_043C,
        (u8)work->view->unk_043C);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, 6), &x, &y);
    ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
    PCBox_SetSpriteAnim(work->view, 9, 8);
    PCBox_SetSpriteVisible(work->view, 9, 1);
}

u32 PCBox_ConfirmWallpaperChoice(PCBoxWork *work) {
    if (work->view->wallpaperSelection >= 0x10
        && PCStorage_IsBonusWallpaperUnlocked(work->storage, work->view->wallpaperSelection - 0x10) == 0) {
        PlaySE(SEQ_SE_DP_BOX03);
        return PCBox_FlashPanel(work, 0xc, 0x42);
    }
    if (work->view->wallpaperSelection == PCBox_GetWallpaperGfxIndex(work, work->trayBox)) {
        PlaySE(SEQ_SE_DP_BOX03);
        return PCBox_FlashPanel(work, 0xc, 0x42);
    }
    if (work->trayBox != work->activeBox) {
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_FlashPanel(work, 0xc, 0xa5);
    }
    PlaySE(SEQ_SE_DP_DECIDE);
    return PCBox_FlashPanel(work, 0xc, 0x47);
}

u32 PCBox_TryWithdraw(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    if (Party_GetCount(work->party) != 6) {
        PCBox_CloseMessageWindow(work->view, 0x27);
        PCBox_SlideOutActionMenu(work->view->panels);
        ov14_021E884C(work->view);
        return PCBox_WaitForUpdateFunc(work, ov14_021E9434, 0x53);
    }
    ov14_021E83F4(work->view->panels);
    PCBox_PrintMonActionMessage(work, 0, 2, 0x25);
    work->nextState = 0xe;
    return 6;
}

u32 ov14_021F1414(PCBoxWork *work) {
    PCBox_CloseMessageWindow(work->view, 0x27);
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_SlideOutActionMenu(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021E9450, 0x5c);
}

u32 ov14_021F1448(PCBoxWork *work, s32 delta) {
    s32 v;
    u8 x;
    u8 y;
    work->trayBox = delta + work->trayBox / 6 * 6;
    if (System_GetTouchNew() == 0) {
        if (PCStorage_CountMonsAndEggsInBox(work->storage, work->trayBox) == 0x1e) {
            PlaySE(SEQ_SE_DP_BOX03);
        } else {
            PlaySE(SEQ_SE_DP_DECIDE);
        }
        return 0x66;
    }
    PlaySE(SEQ_SE_DP_DECIDE);
    PCBox_UpdateTrayCursor(work);
    PCBox_DrawTrayBoxNameAndCount(work);
    v = GridInputHandler_GetNextInput(work->view->gridInput);
    work->view->unk_043C = v;
    GridInputHandler_SetNextLastUnk0FInputs(work->view->gridInput, 8, v, v);
    DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(work->view->gridInput, 8), &x, &y);
    ManagedSprite_SetPositionXY(work->view->sprites[9], x, y);
    PCBox_SetSpriteAnim(work->view, 9, 8);
    PCBox_SetSpriteVisible(work->view, 9, 1);
    return 0x61;
}

u32 ov14_021F1504(PCBoxWork *work, s32 delta) {
    PCBox_ChangeTrayPage(work, delta);
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)(work->trayBox % 6));
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    return 0x61;
}

u32 ov14_021F1534(PCBoxWork *work, u32 unused) {
    return PCBox_FlashPanel(work, 0xa, 0x62);
}

u32 ov14_021F1540(PCBoxWork *work) {
    PCBox_SetSpriteAnim(work->view, 9, 8);
    if (PCStorage_CountMonsAndEggsInBox(work->storage, work->trayBox) == 30) {
        PlaySE(SEQ_SE_DP_BOX03);
    } else {
        PlaySE(SEQ_SE_DP_DECIDE);
    }
    return PCBox_FlashPanel(work, 12, 0x66);
}

u32 ov14_021F1580(PCBoxWork *work, u32 value) {
    work->selectedSlot = value;
    work->view->unk_044B = 1;
    PCBox_SetIconLayer(work->view, work->selectedSlot, 0);
    PCBox_HideSelectedIconOutline(work);
    if (work->unk_2A == 0) {
        ov14_021E8824(work->view);
    }
    return PCBox_WaitForUpdateFunc(work, ov14_021EA254, 0x73);
}

#pragma force_active on

u32 ov14_021F15C8(PCBoxWork *work, u32 index) {
    struct {
        s16 y, x;
    } xy;

    work->view->unk_88CC = index == 255;
    ManagedSprite_GetPositionXY(work->view->sprites[9], &xy.x, &xy.y);
    ManagedSprite_SetPositionXY(work->view->sprites[work->view->slotIcons[work->selectedSlot]], xy.x, xy.y + 4);
    if (index < 36) {
        if (index >= 30) {
            int count = Party_GetCount(work->party);
            if (work->selectedSlot < 30) {
                if (index - 30 > count) {
                    PCBox_ClearMonInfo(work);
                    PCBox_SlideOutSummaryButton(work->view->panels);
                } else {
                    ov14_021E884C(work->view);
                }
            } else {
                if (index - 30 >= count) {
                    PCBox_ClearMonInfo(work);
                    PCBox_SlideOutSummaryButton(work->view->panels);
                } else {
                    ov14_021E884C(work->view);
                }
            }
        } else {
            ov14_021E884C(work->view);
        }
    } else if (index != 255) {
        u32 selected;
        work->trayBox = (work->trayBox / 6) * 6 + index - 37;
        PCBox_UpdateTrayCursor(work);
        PCBox_DrawTrayBoxNameAndCount(work);
        selected = work->selectedSlot;
        if (selected >= 30) {
            void *mon;
            selected -= 30;
            mon = Party_GetMonByIndex(work->party, selected);
            if (ItemIdIsMail((u16)GetMonData(mon, MON_DATA_HELD_ITEM, 0)) == 1) {
                ov14_021E884C(work->view);
            } else if (GetMonData(mon, MON_DATA_BALL_CAPSULE_ID, 0) != 0) {
                ov14_021E884C(work->view);
            } else if (!PCBox_PartyHasOtherUsableMon(work, (int)selected)) {
                ov14_021E884C(work->view);
            } else if (work->trayBox == work->activeBox) {
                PCBox_ClearMonInfo(work);
                PCBox_SlideOutSummaryButton(work->view->panels);
            } else if (!PCStorage_CountEmptySpotsInBox(work->storage, work->trayBox)) {
                ov14_021E884C(work->view);
            } else {
                PCBox_ClearMonInfo(work);
                PCBox_SlideOutSummaryButton(work->view->panels);
            }
        } else if (work->trayBox == work->activeBox) {
            PCBox_ClearMonInfo(work);
            PCBox_SlideOutSummaryButton(work->view->panels);
        } else if (!PCStorage_CountEmptySpotsInBox(work->storage, work->trayBox)) {
            ov14_021E884C(work->view);
        } else {
            PCBox_ClearMonInfo(work);
            PCBox_SlideOutSummaryButton(work->view->panels);
        }
    } else {
        u32 next = GridInputHandler_GetNextInput(work->view->gridInput);
        if (next < 36) {
            if (!PCBox_ShowSlotMonInfo(work, next)) {
                PCBox_SlideOutSummaryButton(work->view->panels);
            }
        } else {
            PCBox_ClearMonInfo(work);
            PCBox_SlideOutSummaryButton(work->view->panels);
        }
    }
    work->view->unk_044C = (u8)index;
    work->view->unk_044B = 0;
    PCBox_AllocMoveWork(work);
    work->partyLayout = 2;
    return PCBox_WaitForUpdateFunc(work, ov14_021EA378, 43);
}

#pragma force_active reset

u32 ov14_021F1808(PCBoxWork *work, u32 value) {
    work->selectedSlot = value;
    work->view->heldItem = PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, 0);
    PCBox_DrawBagGiveButton(work, work->view->heldItem);
    PCBox_SlideInActionMenu(work->view->panels);
    PCBox_SlideOutPartyButton(work->view->panels);
    ov14_021E8294(work->view->panels);
    ov14_021E8314(work->view->panels);
    if (work->view->heldItem != 0) {
        PCBox_SetSpriteVisible(work->view, 11, 0);
        PCBox_MoveHeldItemIconToSlot(work->view, work->selectedSlot, 0);
        PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
        PCBox_CreateHeldItemIconOutline(work->view);
        ov14_021E88BC(work->view->panels);
    }
    return PCBox_WaitForUpdateFunc(work, ov14_021EA408, 0x76);
}

u32 ov14_021F18B0(PCBoxWork *work, u32 value) {
    s16 xy[2];
    u16 oldItem = work->view->heldItem;
    work->selectedSlot = value;
    work->view->heldItem = PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, 0);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        if (oldItem != 0 || work->view->heldItem != 0) {
            ov14_021E83F4(work->view->panels);
        } else if (work->view->heldItem != 0) {
            PCBox_SlideOutActionMenu(work->view->panels);
        }
    } else {
        PCBox_SlideOutPartyButton(work->view->panels);
        ov14_021E8294(work->view->panels);
        ov14_021E8314(work->view->panels);
    }
    PCBox_DrawBagGiveButton(work, work->view->heldItem);
    if (work->view->heldItem != 0) {
        PCBox_SetSpriteVisible(work->view, 11, 0);
        PCBox_MoveHeldItemIconToSlot(work->view, work->selectedSlot, 0);
        PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
        PCBox_CreateHeldItemIconOutline(work->view);
        PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 1);
        ov14_021E88BC(work->view->panels);
    } else if (PCBox_IsSpriteVisible(work->view, 11) == 1) {
        PCBox_SetSpriteVisible(work->view, 11, 0);
        PCBox_HideSelectedIconOutline(work);
    }
    PCBox_GetSlotPosition(work->selectedSlot, &xy[1], &xy[0], 0);
    work->view->touchX = xy[1] + 8;
    work->view->touchY = xy[0] + 8;
    ov14_021F1F24(work);
    return PCBox_WaitForUpdateFunc(work, ov14_021EA4C8, 0x7e);
}

u32 ov14_021F19F0(PCBoxWork *work, u32 value) {
    s16 xy[2];
    work->selectedSlot = value;
    work->view->heldItem = PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, 0);
    PCBox_DrawSummaryButton(work, 0);
    if (work->view->heldItem == 0) {
        GridInputHandler_SetNextInput(work->view->gridInput, work->selectedSlot);
        GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
        if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
            return 0x82;
        }
        if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
            PCBox_SlideOutChangeBoxButton(work->view->panels);
        }
        return PCBox_WaitForUpdateFunc(work, ov14_021E99A0, 0x82);
    }
    PlaySE(SEQ_SE_DP_BOX02);
    PCBox_SetSpriteVisible(work->view, 11, 0);
    PCBox_MoveHeldItemIconToSlot(work->view, work->selectedSlot, 2);
    PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
    PCBox_CreateHeldItemIconOutline(work->view);
    PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 1);
    ov14_021E88BC(work->view->panels);
    if (PCBox_IsChangeBoxButtonShown(work->view->panels) == 1) {
        PCBox_SlideOutChangeBoxButton(work->view->panels);
    } else if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
        PCBox_SlideOutSummaryButton(work->view->panels);
    }
    PCBox_GetSlotPosition(work->selectedSlot, &xy[1], &xy[0], 2);
    work->view->touchX = xy[1] + 8;
    work->view->touchY = xy[0] + 8;
    ov14_021F1F24(work);
    return PCBox_WaitForUpdateFunc(work, ov14_021EA778, 0x84);
}

u32 ov14_021F1B4C(PCBoxWork *work, u32 value) {
    work->selectedSlot = value;
    work->view->unk_88CA = value;
    work->view->heldItem = PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, 0);
    if (work->view->heldItem == 0) {
        return 0x82;
    }
    PlaySE(SEQ_SE_DP_BOX02);
    work->view->unk_044B = 1;
    PCBox_SetSpriteVisible(work->view, 11, 0);
    ov14_021F39A0(work->view, work->selectedSlot, 2);
    PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
    PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 1);
    ov14_021E88BC(work->view->panels);
    if (PCBox_IsSummaryButtonShown(work->view->panels) == 1) {
        PCBox_SlideOutSummaryButton(work->view->panels);
    }
    return PCBox_WaitForUpdateFunc(work, ov14_021EAA04, 0x88);
}

#pragma force_active on

u32 ov14_021F1C04(PCBoxWork *work) {
    u32 input = GridInputHandler_GetNextInput(work->view->gridInput);
    if (input < 36) {
        if (PCBox_ShowSlotMonInfo(work, input) == 1) {
            PCBox_SlideInSummaryButton(work->view->panels);
        }
    } else {
        PCBox_ClearMonInfo(work);
    }
    ov14_021F1F24(work);
    return PCBox_WaitForUpdateFunc(work, ov14_021EAC24, 134);
}

#pragma force_active reset

u32 ov14_021F1C4C(PCBoxWork *work, u32 value) {
    s16 xy[2];
    work->view->unk_044C = value;
    work->view->unk_044B = 0;
    ManagedSprite_GetPositionXY(work->view->sprites[9], &xy[1], &xy[0]);
    ManagedSprite_SetPositionXY(work->view->sprites[11], xy[1], (s16)(xy[0] + 8));
    if (work->view->unk_044C == 0xff) {
        return ov14_021F1C04(work);
    }
    if (PCBox_CanSwapHeldItems(work, work->selectedSlot, value) == 0) {
        return ov14_021F1C04(work);
    }
    ov14_021E88F8(work->view->panels);
    return PCBox_WaitForUpdateFunc(work, ov14_021EAB54, 0x8a);
}

u32 ov14_021F1CDC(PCBoxWork *work, u32 value) {
    work->selectedSlot = value;
    work->view->heldItem = PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, 0);
    PCBox_DrawBagGiveButton(work, work->view->heldItem);
    PCBox_SlideInActionMenu(work->view->panels);
    ov14_021E8314(work->view->panels);
    if (work->view->heldItem != 0) {
        PCBox_SetSpriteVisible(work->view, 11, 0);
        PCBox_MoveHeldItemIconToSlot(work->view, work->selectedSlot, 1);
        PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
        PCBox_CreateHeldItemIconOutline(work->view);
        ov14_021E88BC(work->view->panels);
    }
    return PCBox_WaitForUpdateFunc(work, ov14_021EA408, 0x8c);
}

u32 ov14_021F1D6C(PCBoxWork *work, u32 value) {
    s16 xy[2];
    u16 oldItem;
    work->selectedSlot = value;
    oldItem = work->view->heldItem;
    work->view->heldItem = PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, 0);
    if (PCBox_IsActionMenuOpen(work->view->panels) == 1) {
        if (oldItem != 0 || work->view->heldItem != 0) {
            ov14_021E83F4(work->view->panels);
        } else if (work->view->heldItem != 0) {
            PCBox_SlideOutActionMenu(work->view->panels);
        }
    } else {
        ov14_021E8314(work->view->panels);
    }
    PCBox_DrawBagGiveButton(work, work->view->heldItem);
    if (work->view->heldItem != 0) {
        PCBox_SetSpriteVisible(work->view, 11, 0);
        PCBox_MoveHeldItemIconToSlot(work->view, work->selectedSlot, 1);
        PCBox_LoadHeldItemIcon(work->view, work->view->heldItem);
        PCBox_CreateHeldItemIconOutline(work->view);
        PCBox_SetSlotIconTranslucent(work->view, work->selectedSlot, 1);
        ov14_021E88BC(work->view->panels);
        if (ItemIdIsMail(work->view->heldItem) == 1) {
            PCBox_PrintPanelHint(work, 0x28, 9);
        } else {
            PCBox_PrintPanelHint(work, 0x28, 10);
        }
    } else if (PCBox_IsSpriteVisible(work->view, 11) == 1) {
        PCBox_SetSpriteVisible(work->view, 11, 0);
        PCBox_HideSelectedIconOutline(work);
    }
    PCBox_GetSlotPosition(work->selectedSlot, &xy[1], &xy[0], 1);
    work->view->touchX = xy[1] + 8;
    work->view->touchY = xy[0] + 8;
    ov14_021F1F24(work);
    return PCBox_WaitForUpdateFunc(work, ov14_021EACD4, 0x8d);
}

void ov14_021F1EB8(PCBoxWork *work) {
    u32 next;
    if (work->view->heldItem == 0) {
        void *boxMon;
        PCBox_SetSpriteVisible(work->view, 9, 0);
        boxMon = (void *)PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot);
        if (GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL) == 0) {
            work->unk_2C = 0x24;
            PCBox_StartSubApp(work, 1);
            return;
        }
        next = 0x7c;
    } else {
        PCBox_SetSpriteVisible(work->view, 9, 0);
        next = 0x78;
    }
    PCBox_SlideOutActionMenu(work->view->panels);
    PCBox_WaitForUpdateFunc(work, ov14_021E9450, next);
}

void ov14_021F1F24(PCBoxWork *work) {
    work->view->temp = Heap_Alloc(HEAP_ID_10, 0x1c);
}

void ov14_021F1F38(PCBoxWork *work) {
    Heap_Free(work->view->temp);
}

u32 ov14_021F1F44(PCBoxWork *work) {
    u32 v;
    if (PCBox_IsSummaryShortcutPressed(work) == 1) {
        PlaySE(SEQ_SE_DP_DECIDE);
        work->unk_26 = 1;
        work->unk_27 = 1;
        work->unk_28 = GridInputHandler_GetNextInput(work->view->gridInput);
        return ov14_021F2330(work, 0xf, 0x97);
    }
    v = ov14_021F7340(work);
    if (GridInputHandler_IsButtonInputMode(work->view->gridInput) == 0) {
        v = (u32)-2;
    }
    switch (v) {
    case (u32)-3:
        PlaySE(SEQ_SE_DP_SELECT);
        return PCBox_WaitForPendingUpdateFunc(work, 0x74);
    case (u32)-2:
    case 0x24:
        return ov14_021F15C8(work, 0xff);
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2a:
    default:
        return ov14_021F15C8(work, v);
    case (u32)-4:
    case (u32)-1:
        return 0x73;
    }
}

u32 ov14_021F2010(PCBoxWork *work) {
    Heap_Free(work->view->temp);
    return 0x73;
}

u32 PCBox_TakeHeldItemToBag(PCBoxWork *work) {
    int item;
    void *boxmon;
    item = PCBox_GetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, 0);
    if (Bag_AddItem(work->bag, item, 1, HEAP_ID_10) == 1) {
        boxmon = PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot);
        PCBox_PrintTookItem(work, item, 0x25);
        item = 0;
        PCBox_SetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, &item);
        if (PCBox_UpdateArceusGiratinaForm(boxmon) == 1) {
            PCBox_RefreshSlotIcon(work, work->activeBox, work->selectedSlot, work->view->slotIcons[work->selectedSlot]);
        }
        PCBox_RedrawSlotMonInfo(work, work->selectedSlot);
    } else {
        PlaySE(SEQ_SE_DP_BOX03);
        PCBox_PrintBagFull(work, 0x25);
    }
    work->nextState = 0xe;
    return 6;
}

u32 PCBox_ConfirmRelease(PCBoxWork *work) {
    PCBox_HideSelectedIconOutline(work);
    PCBox_CloseMessageWindow(work->view, 0x25);
    PCBox_StartReleaseMoveCheck(work);
    PCBox_StartReleaseScale(work->view->releaseWork);
    ov14_021E884C(work->view);
    return 0x1a;
}

u32 PCBox_PutAwayHeldItem(PCBoxWork *work) {
    u16 v;
    if (Bag_AddItem(Save_Bag_Get(work->args->saveData), work->view->heldItem, 1, HEAP_ID_10) == 0) {
        PlaySE(SEQ_SE_DP_BOX03);
        PCBox_PrintItemActionMessage(work, 6, 0x25);
        work->nextState = 0x7a;
        return 6;
    }
    v = 0;
    PCBox_SetSlotMonData(work, work->selectedSlot, MON_DATA_HELD_ITEM, &v);
    if (PCBox_UpdateArceusGiratinaForm(PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot)) == 1) {
        PCBox_RefreshSlotIcon(work, work->activeBox, work->selectedSlot, work->view->slotIcons[work->selectedSlot]);
    }
    PCBox_ShowSlotMonInfo(work, work->selectedSlot);
    PCBox_HideSelectedIconOutline(work);
    PCBox_CloseMessageWindow(work->view, 0x25);
    ov14_021F391C(work->view, 1);
    PCBox_SetSpriteAnim(work->view, 0xb, 2);
    ov14_021E88F8(work->view->panels);
    return 0x79;
}

u32 PCBox_ConfirmExit(PCBoxWork *work) {
    PlaySE(SEQ_SE_DP_PC_LOGOFF);
    work->nextState = 0xb3;
    return PCBox_StartFadeOut(work, 1);
}

u32 PCBox_CancelExit(PCBoxWork *work) {
    PCBox_CloseMessageWindow(work->view, 0x25);
    GridInputHandler_SetButtonInputMode(work->view->gridInput, 1);
    switch (work->args->unk8) {
    case 0:
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
        PCBox_SetIconsTranslucent(work, 2, 0);
        return 0x5b;
    case 1:
        PCBox_PrintMonActionMessage(work, 0, 0, 0x27);
        PCBox_SetBoxArrowsVisible(work->view, 1);
        PCBox_SetIconsTranslucent(work, 1, 0);
        return 0x51;
    case 2:
        PCBox_SetBoxArrowsVisible(work->view, 1);
        PCBox_SetIconsTranslucent(work, 1, 0);
        return 0xc;
    case 3:
        PCBox_SetBoxArrowsVisible(work->view, 1);
        PCBox_SetIconsTranslucent(work, 0x81, 0);
        return 0x75;
    }
    return 0xc;
}

u32 PCBox_FlashPanel(PCBoxWork *work, u32 a1, u32 a2) {
    s8 a;
    s8 b;
    u16 c;
    u16 d;
    BgCellManager_GetCellPosition(work->view->panels, a1, &a, &b);
    BgCellManager_GetCellSize(work->view->panels, a1, &c, &d);
    work->view->paletteFlash.isBg = 1;
    work->view->paletteFlash.index = BgCellManager_GetCellBgId(work->view->panels, a1);
    work->view->paletteFlash.palA = 0xd;
    work->view->paletteFlash.palB = 0xc;
    work->view->paletteFlash.state = 0;
    work->view->paletteFlash.timer = 0;
    work->view->paletteFlash.x = a;
    work->view->paletteFlash.y = b;
    work->view->paletteFlash.w = c;
    work->view->paletteFlash.h = d;
    work->nextState = a2;
    return 8;
}

u32 ov14_021F2330(PCBoxWork *work, u32 a1, u32 a2) {
    s8 a;
    s8 b;
    u16 c;
    u16 d;
    BgCellManager_GetCellPosition(work->view->panels, a1, &a, &b);
    BgCellManager_GetCellSize(work->view->panels, a1, &c, &d);
    work->view->paletteFlash.isBg = 1;
    work->view->paletteFlash.index = BgCellManager_GetCellBgId(work->view->panels, a1);
    work->view->paletteFlash.palA = 3;
    work->view->paletteFlash.palB = 2;
    work->view->paletteFlash.state = 0;
    work->view->paletteFlash.timer = 0;
    work->view->paletteFlash.x = a;
    work->view->paletteFlash.y = b;
    work->view->paletteFlash.w = c;
    work->view->paletteFlash.h = d;
    work->nextState = a2;
    return 8;
}

u32 ov14_021F23F0(PCBoxWork *work, u32 a1, u32 a2) {
    work->view->paletteFlash.isBg = 1;
    work->view->paletteFlash.index = BgCellManager_GetCellBgId(work->view->panels, 2);
    work->view->paletteFlash.palA = 3;
    work->view->paletteFlash.palB = 2;
    work->view->paletteFlash.state = 0;
    work->view->paletteFlash.timer = 0;
    if (a1 == 0) {
        work->view->paletteFlash.y = 0x10;
    } else {
        work->view->paletteFlash.y = 0x14;
    }
    work->view->paletteFlash.x = 0x16;
    work->view->paletteFlash.w = 9;
    work->view->paletteFlash.h = 4;
    work->nextState = a2;
    return 8;
}

u32 ov14_021F2490(PCBoxWork *work, u32 a1, u32 a2) {
    work->view->paletteFlash.isBg = 1;
    work->view->paletteFlash.index = BgCellManager_GetCellBgId(work->view->panels, 1);
    work->view->paletteFlash.palA = 3;
    work->view->paletteFlash.palB = 2;
    work->view->paletteFlash.state = 0;
    work->view->paletteFlash.timer = 0;
    if (a1 == 0) {
        work->view->paletteFlash.x = 3;
    } else {
        work->view->paletteFlash.x = 0x16;
    }
    work->view->paletteFlash.y = 0x14;
    work->view->paletteFlash.w = 9;
    work->view->paletteFlash.h = 4;
    work->nextState = a2;
    return 8;
}

void ov14_021F2534(PCBoxWork *work) {
    switch (work->args->unk8) {
    case 0:
        PCBox_SetIconsTranslucent(work, 2, 1);
        break;
    case 1:
    case 2:
    case 3:
        PCBox_SetBoxArrowsVisible(work->view, 0);
        PCBox_SetIconsTranslucent(work, 1, 1);
        break;
    }
}

u32 ov14_021F2574(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_PrintExitPrompt(work, 0x25, 1);
    ov14_021F2534(work);
    return PCBox_ShowYesNo(work, 3);
}

u32 ov14_021F259C(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_PrintExitPrompt(work, 0x25, 0);
    ov14_021F2534(work);
    return PCBox_ShowYesNo(work, 4);
}

void ov14_021F25C4(PCBoxWork *work) {
    PCBox_ClearMonInfoAndSelection(work);
    ov14_021F0AD8(work);
}

void ov14_021F25D4(PCBoxWork *work) {
    PCBox_ClearMonInfoAndSelection(work);
    ov14_021F0C88(work);
}

u32 ov14_021F25E4(PCBoxWork *work) {
    PCBox_SetSelectedIconOutlineVisible(work, 0);
    work->trayOpen = 1;
    work->unk_29 = 0;
    PCBox_SetSpriteVisible(work->view, 9, 0);
    return ov14_021F1100(work, 0x2f);
}

u32 ov14_021F2610(PCBoxWork *work) {
    return PCBox_StartSubApp(work, 0);
}

void ov14_021F261C(PCBoxWork *work) {
    PCBox_HandleItemCommand(work);
}

void ov14_021F2624(PCBoxWork *work) {
    ov14_021F0A80(work);
}

void ov14_021F262C(PCBoxWork *work) {
    ov14_021F09BC(work);
}

void ov14_021F2634(PCBoxWork *work) {
    ov14_021F0AAC(work);
}

void ov14_021F263C(PCBoxWork *work) {
    PCBox_SlideOutActionMenu(work->view->panels);
    PCBox_SlideInPartyButton(work->view->panels);
    ov14_021E82A8(work->view->panels);
    ov14_021E8328(work->view->panels);
    ov14_021E884C(work->view);
    PCBox_SetSelectedIconOutlineVisible(work, 0);
    PCBox_WaitForUpdateFunc(work, ov14_021EA180, 0x4a);
}

void ov14_021F2690(PCBoxWork *work) {
    if (work->unk_2B == 0) {
        PCBox_ClearMonInfoAndSelection(work);
    } else {
        PCBox_SetSelectedIconOutlineVisible(work, 0);
    }
    ov14_021F0C0C(work);
}

void ov14_021F26B4(PCBoxWork *work) {
    PCBox_SlideOutActionMenu(work->view->panels);
    ov14_021E8328(work->view->panels);
    ov14_021E884C(work->view);
    PCBox_SetSelectedIconOutlineVisible(work, 0);
    PCBox_WaitForUpdateFunc(work, ov14_021EA180, 0x4c);
}

void ov14_021F26F0(PCBoxWork *work) {
    PCBox_ClearMonInfoAndSelection(work);
    ov14_021F0B34(work);
}

void ov14_021F2700(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_JumpToTrayBox(work, 3);
}

#pragma force_active on

void ov14_021F2718(PCBoxWork *work) {
    PCBox_ClearMonInfoAndSelection(work);
    ov14_021F0CD8(work);
}

#pragma force_active reset

u32 ov14_021F2728(PCBoxWork *work) {
    return PCBox_JumpToTrayBox(work, 1);
}

#pragma force_active on

void ov14_021F2734(PCBoxWork *work) {
    PCBox_CloseMessageWindow(work->view, 39);
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_SetIconsTranslucent(work, 1, 1);
    PCBox_CloseBoxTray(work, 63);
}

#pragma force_active reset

u32 ov14_021F2760(PCBoxWork *work) {
    return PCBox_StartSubApp(work, 2);
}

u32 ov14_021F276C(PCBoxWork *work) {
    return PCBox_JumpToTrayBox(work, 2);
}

#pragma force_active on

int ov14_021F2778(PCBoxWork *work) {
    PCBox_CloseMessageWindow(work->view, 39);
    PCBox_SetSpriteVisible(work->view, 9, 0);
    if (work->args->unk8 == PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_SetIconsTranslucent(work, 129, 0);
    } else {
        PCBox_SetIconsTranslucent(work, 1, 0);
    }
    work->view->unk_043C = work->trayBox % 6;
    work->unk_2C = 9;
    return 67;
}

#pragma force_active reset

void ov14_021F27CC(PCBoxWork *work) {
    PCBox_TryWithdraw(work);
}

#pragma force_active on

void ov14_021F27D4(PCBoxWork *work) {
    PCBox_SlideOutActionMenu(work->view->panels);
    ov14_021E8328(work->view->panels);
    ov14_021E884C(work->view);
    PCBox_SetSelectedIconOutlineVisible(work, 0);
    PCBox_WaitForUpdateFunc(work, ov14_021EA130, 89);
}

#pragma force_active reset

void ov14_021F2810(PCBoxWork *work) {
    ov14_021F1414(work);
}

void ov14_021F2818(PCBoxWork *work) {
    PCBox_DrawReturnButton(work, 0);
    PCBox_SlideOutActionMenu(work->view->panels);
    ov14_021E8328(work->view->panels);
    ov14_021E884C(work->view);
    PCBox_SetSelectedIconOutlineVisible(work, 0);
    PCBox_WaitForUpdateFunc(work, ov14_021EA130, 0x70);
}

void ov14_021F2858(PCBoxWork *work) {
    PCBox_ClearMonInfoAndSelection(work);
    PCBox_SetIconsTranslucent(work, 0x82, 1);
    ov14_021F0AD8(work);
}

void ov14_021F2874(PCBoxWork *work) {
    PCBox_ClearMonInfoAndSelection(work);
    PCBox_SetIconsTranslucent(work, 0x82, 1);
    ov14_021F0C88(work);
}

void ov14_021F2890(PCBoxWork *work) {
    ov14_021F1EB8(work);
}

#pragma force_active on

void ov14_021F2898(PCBoxWork *work) {
    PCBox_SlideOutActionMenu(work->view->panels);
    PCBox_SlideInPartyButton(work->view->panels);
    ov14_021E82A8(work->view->panels);
    ov14_021E8328(work->view->panels);
    PCBox_HideSelectedIconOutline(work);
    if (work->view->heldItem) {
        ov14_021E88F8(work->view->panels);
    }
    PCBox_WaitForUpdateFunc(work, ov14_021EA674, 118);
}

#pragma force_active reset

void ov14_021F28FC(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
    PCBox_JumpToTrayBox(work, 4);
}

#pragma force_active on

void ov14_021F2914(PCBoxWork *work) {
    PCBox_ClearMonInfoAndSelection(work);
    PCBox_SetSpriteVisible(work->view, 11, 0);
    ov14_021F0C0C(work);
}

void ov14_021F2930(PCBoxWork *work) {
    PCBox_SlideOutActionMenu(work->view->panels);
    ov14_021E8328(work->view->panels);
    PCBox_HideSelectedIconOutline(work);
    if (work->view->heldItem) {
        ov14_021E88F8(work->view->panels);
    }
    PCBox_WaitForUpdateFunc(work, ov14_021EA674, 140);
}

void ov14_021F297C(PCBoxWork *work) {
    GfGfx_EngineATogglePlanes(16, 1);
    GfGfx_EngineBTogglePlanes(16, 1);
    PCBox_InitSpriteSystem(work->view);
    PCBox_LoadSpriteResources(work->view);
    PCBox_CreateSprites(work);
    PCBox_CreateTrayTextObjs(work->view);
}

#pragma force_active reset
