#include "box_mon_selector.h"

#include "global.h"

#include "constants/items.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0298.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "vram_transfer_manager.h"

enum BoxMonSelectorState {
    BOX_MON_SELECTOR_STATE_SELECT,
    BOX_MON_SELECTOR_STATE_CONFIRM_INIT,
    BOX_MON_SELECTOR_STATE_CONFIRM,
    BOX_MON_SELECTOR_STATE_IDLE,
    BOX_MON_SELECTOR_STATE_REMOVE_INIT,
    BOX_MON_SELECTOR_STATE_REMOVE,
    BOX_MON_SELECTOR_STATE_FINISH,
};

// GridCursor panels (gGridCursorPanels).
enum BoxMonSelectorPanel {
    BOX_MON_SELECTOR_PANEL_CANCEL = 1,
    BOX_MON_SELECTOR_PANEL_BOX,
    BOX_MON_SELECTOR_PANEL_PICKS,
    BOX_MON_SELECTOR_PANEL_PLACE, // choosing a pick slot for the Pokemon in hand
    BOX_MON_SELECTOR_PANEL_PAGE,
    BOX_MON_SELECTOR_PANEL_PARTY,
};

// Touch hitboxes: the 30 box slots, the CANCEL button, the page arrows, the pick slots.
#define HITBOX_CANCEL    30
#define HITBOX_PREV_PAGE 31
#define HITBOX_NEXT_PAGE 32
#define HITBOX_PICKS     33

// Slot states (BoxMonSelector_GetBoxSlotState / BoxMonSelector_GetPickState).
#define SLOT_FULL       0 // no free pick slot left
#define SLOT_SELECTABLE 1
#define SLOT_PICKED     2
#define SLOT_INVALID    3 // empty, an Egg, or no pick there

#define DRAG_FROM_BOX   1
#define DRAG_FROM_PICKS 2

extern const BgTemplate gBoxMonSelectorBgTemplateMain2;
extern const BgTemplate gBoxMonSelectorBgTemplateMain1;
extern const BgTemplate gBoxMonSelectorBgTemplateMain0;
extern const BgTemplate gBoxMonSelectorBgTemplateMain3;

int BoxMonSelector_HandleInput(BoxMonSelector *selector);
void BoxMonSelector_ShowConfirmPrompt(BoxMonSelector *selector);
void BoxMonSelector_HandleConfirmPrompt(BoxMonSelector *selector);
void BoxMonSelector_ShowRemovePrompt(BoxMonSelector *selector);
void BoxMonSelector_HandleRemovePrompt(BoxMonSelector *selector);
void BoxMonSelector_InitSpriteResources(BoxMonSelector *selector);
void BoxMonSelector_InitSpriteHeader(BoxMonSelector *selector);
void BoxMonSelector_CreatePickSlots(BoxMonSelector *selector);
int BoxMonSelector_HandleDrop(BoxMonSelector *selector);
void BoxMonSelector_AddPick(BoxMonSelector *selector, u8 pick);
void BoxMonSelector_MovePick(BoxMonSelector *selector, u8 from, u8 to);
void BoxMonSelector_CreateButtons(BoxMonSelector *selector);
void BoxMonSelector_InitSpriteTemplate(SpriteTemplate *template, BoxMonSelector *selector, NNS_G2D_VRAM_TYPE vram);
Sprite *BoxMonSelector_CreateSprite(BoxMonSelector *selector, int x, int y, int animSeq, BOOL drawFlag);
void BoxMonSelector_LoadScreens(BoxMonSelector *selector, BgConfig *bgConfig, enum HeapID heapId);
void BoxMonSelector_SetHitboxRect(TouchscreenHitbox *hitbox, int x, int y, int width, int height);
void BoxMonSelector_CreateBoxIcons(BoxMonSelector *selector);
int BoxMonSelector_LoadPage(BoxMonSelector *selector);
int BoxMonSelector_LoadBoxPage(BoxMonSelector *selector);
int BoxMonSelector_LoadPartyPage(BoxMonSelector *selector);
void BoxMonSelector_SetPickIcon(BoxMonSelector *selector, u8 pick, int source, BOOL fromPick);
void BoxMonSelector_TransferIconChars(BoxMonSelectorIconChars *chars);
int BoxMonSelector_PointAtBoxSlot(BoxMonSelector *selector, int slot);
int BoxMonSelector_GetBoxSlotState(BoxMonSelector *selector, int slot);
int BoxMonSelector_PointAtPick(BoxMonSelector *selector, int pick);
int BoxMonSelector_GetPickState(BoxMonSelector *selector, int pick);
void BoxMonSelector_StartDrag(BoxMonSelector *selector, int source, int index);
void BoxMonSelector_UpdateTouchDrag(BoxMonSelector *selector);
void BoxMonSelector_Drop(BoxMonSelector *selector);
void BoxMonSelector_EndDrag(BoxMonSelector *selector, int result);
void BoxMonSelector_LoadIconChars(BoxMonSelectorMon *mon, BoxMonSelectorIconChars *chars, int index, Sprite *sprite, void *buf, NARC *narc, int baseTile);
void BoxMonSelector_ReadIconChars(u32 fileId, NNSG2dCharacterData **ppCharData, void *dest, NARC *narc);
void BoxMonSelector_PrintPageName(BoxMonSelector *selector, int page);
void BoxMonSelector_PrintCancelButton(BoxMonSelector *selector);
void BoxMonSelector_InitBgs(BgConfig *bgConfig, enum HeapID heapId);
void BoxMonSelector_InitBgLayers(BgConfig *bgConfig);
void BoxMonSelector_UpdatePickMarks(BoxMonSelector *selector);
void BoxMonSelector_GetPartyMon(BoxMonSelector *selector, int slot, BoxMonSelectorMon *mon);
void BoxMonSelector_RemovePick(BoxMonSelector *selector, int pick);
void BoxMonSelector_ShowMessage(BoxMonSelector *selector, int msgId, FontID fontId);
void BoxMonSelector_ShowTakePrompt(BoxMonSelector *selector);
void BoxMonSelector_RemoveTakePrompt(BoxMonSelector *selector);
void BoxMonSelector_ClearMessageWindow(Window *window, BOOL hidePlane);
void BoxMonSelector_WriteResults(BoxMonSelector *selector);
void BoxMonSelector_DeleteSprites(BoxMonSelector *selector);
void BoxMonSelector_UpdateCursorSprite(BoxMonSelector *selector);
int BoxMonSelector_HandleAButton(BoxMonSelector *selector);
int BoxMonSelector_HandleBButton(BoxMonSelector *selector);
void BoxMonSelector_PrevPage(BoxMonSelector *selector);
void BoxMonSelector_NextPage(BoxMonSelector *selector);
void BoxMonSelector_PressCancelButton(BoxMonSelector *selector);
void BoxMonSelector_NotifyCursorMove(BoxMonSelector *selector);
void BoxMonSelector_DimScreen(void);
void BoxMonSelector_ResetBlend(void);

#pragma force_active on

BoxMonSelector *BoxMonSelector_New(enum HeapID heapId, int maxPicks, int numPages) {
    BoxMonSelector *selector = Heap_Alloc(heapId, sizeof(BoxMonSelector));
    memset(selector, 0, sizeof(BoxMonSelector));
    GF_ASSERT(maxPicks != 0);
    selector->heapId = heapId;
    selector->maxPicks = maxPicks;
    selector->numPages = numPages;
    if (selector->maxPicks > BOX_MON_SELECTOR_MAX_PICKS) {
        GF_ASSERT(FALSE);
        selector->maxPicks = BOX_MON_SELECTOR_MAX_PICKS;
    }
    selector->marks = Heap_Alloc(selector->heapId, selector->maxPicks * sizeof(BoxMonSelectorMark));
    selector->hitboxes = Heap_Alloc(selector->heapId, (selector->maxPicks + HITBOX_PICKS + 1) * sizeof(TouchscreenHitbox));
    selector->cursor = GridCursor_New(selector->heapId, &selector->page);
    return selector;
}

void BoxMonSelector_Delete(BoxMonSelector *selector) {
    int i;

    GridCursor_Delete(selector->cursor);
    BoxMonSelector_DeleteSprites(selector);
    BoxMonSelector_ClearMessageWindow(&selector->pageNameWindow, TRUE);
    RemoveWindow(&selector->cancelWindow);
    BoxMonSelector_RemoveTakePrompt(selector);
    Heap_Free(selector->screenFiles[0]);
    Heap_Free(selector->screenFiles[1]);
    SpriteTransfer_DeleteCharTransferTask(selector->spriteResources[0]);
    SpriteTransfer_DeletePlttTransferTask(selector->spriteResources[1]);
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(selector->resManagers[i]);
    }
    SpriteList_Delete(selector->spriteList);
    Heap_Free(selector->pickDropAreas);
    YesNoPrompt_Destroy(selector->yesNoPrompt);
    Heap_Free(selector->hitboxes);
    Heap_Free(selector->marks);
    FontID_Release(4);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    reg_G2_BLDCNT = 0;
    FreeBgTilemapBuffer(selector->bgConfig, GF_BG_LYR_MAIN_0);
    FreeBgTilemapBuffer(selector->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(selector->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(selector->bgConfig, GF_BG_LYR_MAIN_3);
    Heap_Free(selector);
}

void BoxMonSelector_Init(BoxMonSelector *selector, BgConfig *bgConfig, void *storage, Party *party, int unused11, int unused12, BOOL canCancel, int initialPage, BoxMonSelectorGetMon getMon, BoxMonSelectorGetPageName getPageName, BoxMonSelectorResult *results, BoxMonSelectorCursorCallback onCursorMove, void *onCursorMoveArg) {
    selector->yesNoPrompt = YesNoPrompt_Create(selector->heapId);
    selector->storage = storage;
    selector->party = party;
    selector->bgConfig = bgConfig;
    selector->onCursorMoveArg = onCursorMoveArg;
    selector->onCursorMove = onCursorMove;
    selector->page = initialPage;
    selector->lastCursorPage = -1;
    selector->lastCursorSlot = -1;
    selector->canCancel = canCancel;
    BoxMonSelector_InitBgs(selector->bgConfig, selector->heapId);
    BoxMonSelector_InitSpriteResources(selector);
    BoxMonSelector_InitSpriteHeader(selector);
    BoxMonSelector_CreateButtons(selector);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    FontID_Alloc(4, selector->heapId);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, selector->heapId);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, selector->heapId);
    selector->unused_11 = unused11;
    selector->unused_12 = unused12;
    GridCursor_Init(selector->cursor, (u8)canCancel);
    GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_BOX, 0);
    BoxMonSelector_CreatePickSlots(selector);
    selector->getMon = getMon;
    selector->getPageName = getPageName;
    AddWindowParameterized(selector->bgConfig, &selector->pageNameWindow, GF_BG_LYR_MAIN_1, 4, 1, 13, 2, 13, 160);
    AddWindowParameterized(selector->bgConfig, &selector->cancelWindow, GF_BG_LYR_MAIN_1, 25, 21, 6, 2, 12, 186);
    BoxMonSelector_PrintCancelButton(selector);
    BoxMonSelector_LoadScreens(selector, selector->bgConfig, selector->heapId);
    BoxMonSelector_CreateBoxIcons(selector);
    BoxMonSelector_LoadPage(selector);
    BoxMonSelector_ShowTakePrompt(selector);
    BoxMonSelector_ResetBlend();
    selector->results = results;
    selector->state = BOX_MON_SELECTOR_STATE_SELECT;
    selector->keyDragging = FALSE;
    selector->touchHeld = FALSE;
    selector->touchStarted = FALSE;
}

#pragma force_active off

void BoxMonSelector_VBlank(BoxMonSelector *selector) {
    if (selector->vblankTask != NULL) {
        selector->vblankTask(selector->iconChars);
        selector->vblankTask = NULL;
        BoxMonSelector_NotifyCursorMove(selector);
    }
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates(selector->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// Returns 0 while running, 2 when the picks are confirmed, 3 when CANCEL was pressed.
int BoxMonSelector_Run(BoxMonSelector *selector) {
    switch (selector->state) {
    case BOX_MON_SELECTOR_STATE_SELECT:
        return BoxMonSelector_HandleInput(selector);
    case BOX_MON_SELECTOR_STATE_CONFIRM_INIT:
        BoxMonSelector_ShowConfirmPrompt(selector);
        return 0;
    case BOX_MON_SELECTOR_STATE_CONFIRM:
        BoxMonSelector_HandleConfirmPrompt(selector);
        return 0;
    case BOX_MON_SELECTOR_STATE_IDLE:
        break;
    case BOX_MON_SELECTOR_STATE_REMOVE_INIT:
        BoxMonSelector_ShowRemovePrompt(selector);
        return 0;
    case BOX_MON_SELECTOR_STATE_REMOVE:
        BoxMonSelector_HandleRemovePrompt(selector);
        return 0;
    case BOX_MON_SELECTOR_STATE_FINISH:
        BoxMonSelector_WriteResults(selector);
        return 2;
    }
}

#pragma force_active on

int BoxMonSelector_HandleInput(BoxMonSelector *selector) {
    int result = 0;
    int keys = 0;
    int hit;
    s8 dx, dy;
    VecFx32 pos;

    if (!System_GetTouchHeld() && !selector->touchHeld && (keys = gSystem.newKeys) != 0) {
        int panel;
        dx = 0;
        dy = 0;
        panel = GridCursor_GetPanel(selector->cursor);
        switch (keys) {
        case PAD_KEY_LEFT:
            if (panel == BOX_MON_SELECTOR_PANEL_PAGE) {
                BoxMonSelector_PrevPage(selector);
            } else {
                dx--;
            }
            break;
        case PAD_KEY_RIGHT:
            if (panel == BOX_MON_SELECTOR_PANEL_PAGE) {
                BoxMonSelector_NextPage(selector);
            } else {
                dx++;
            }
            break;
        case PAD_KEY_UP:
            dy--;
            break;
        case PAD_KEY_DOWN:
            dy++;
            break;
        case PAD_BUTTON_A:
            result = BoxMonSelector_HandleAButton(selector);
            if (result != 0 && result != 3) {
                PlaySE(SEQ_SE_DP_DECIDE);
            }
            break;
        case PAD_BUTTON_B:
            result = BoxMonSelector_HandleBButton(selector);
            if (result != 0 && result != 3) {
                PlaySE(SEQ_SE_DP_DECIDE);
            }
            break;
        }
        if (dx || dy) {
            int moved = GridCursor_Move(selector->cursor, dx, dy);
            BoxMonSelector_UpdateCursorSprite(selector);
            if (moved) {
                PlaySE(SEQ_SE_DP_SELECT);
            }
        }
    }

    if (keys == 0 && selector->keyDragging == FALSE) {
        if (System_GetTouchNew() && selector->dragging == FALSE) {
            selector->touchStarted = TRUE;
            hit = TouchscreenHitbox_FindRectAtTouchNew(selector->hitboxes);
            if (hit != -1) {
                if (hit < BOX_MON_SELECTOR_BOX_SLOTS) {
                    if (selector->page != BOX_MON_SELECTOR_PARTY_PAGE || hit < PARTY_SIZE) {
                        GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_BOX, (u8)hit);
                        if (BoxMonSelector_PointAtBoxSlot(selector, hit) == SLOT_SELECTABLE) {
                            PlaySE(SEQ_SE_DP_BOX02);
                            BoxMonSelector_StartDrag(selector, DRAG_FROM_BOX, hit);
                            result = 1;
                        }
                    }
                } else if (hit >= HITBOX_PICKS && hit < HITBOX_PICKS + selector->maxPicks) {
                    GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_PICKS, (u8)(hit - HITBOX_PICKS));
                    if (BoxMonSelector_PointAtPick(selector, hit - HITBOX_PICKS) == SLOT_SELECTABLE) {
                        PlaySE(SEQ_SE_DP_BOX02);
                        BoxMonSelector_StartDrag(selector, DRAG_FROM_PICKS, hit - HITBOX_PICKS);
                        result = 1;
                    }
                } else {
                    switch (hit) {
                    case HITBOX_PREV_PAGE:
                        BoxMonSelector_PrevPage(selector);
                        break;
                    case HITBOX_NEXT_PAGE:
                        BoxMonSelector_NextPage(selector);
                        break;
                    case HITBOX_CANCEL:
                        if (selector->canCancel) {
                            pos.x = 14 << 16;
                            pos.y = 11 << 16;
                            pos.z = 0;
                            Sprite_SetMatrix(selector->handCursor, &pos);
                            GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_CANCEL, 0);
                            BoxMonSelector_PressCancelButton(selector);
                            result = 3;
                        }
                        break;
                    }
                }
                BoxMonSelector_NotifyCursorMove(selector);
            }
            selector->touchHeld = TRUE;
        } else if (System_GetTouchHeld() && selector->touchStarted) {
            BoxMonSelector_UpdateTouchDrag(selector);
            selector->touchHeld = TRUE;
        } else {
            BoxMonSelector_Drop(selector);
            selector->touchHeld = FALSE;
            selector->touchStarted = FALSE;
        }
    }

    if (selector->numPicked == selector->maxPicks) {
        selector->state = BOX_MON_SELECTOR_STATE_CONFIRM_INIT;
    }
    return result;
}

#pragma force_active off

void BoxMonSelector_ShowConfirmPrompt(BoxMonSelector *selector) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = selector->bgConfig;
    template.bgId = 0;
    template.tileStart = 0x200;
    template.plttSlot = 8;
    template.x = 25;
    template.y = 15;
    YesNoPrompt_InitFromTemplate(selector->yesNoPrompt, &template);
    if (selector->maxPicks == 1) {
        BoxMonSelector_ClearMessageWindow(&selector->messageWindow, FALSE);
        BoxMonSelector_ShowMessage(selector, msg_0298_00003, 1);
    } else {
        BoxMonSelector_ShowMessage(selector, msg_0298_00002, 1);
    }
    BoxMonSelector_DimScreen();
    selector->state = BOX_MON_SELECTOR_STATE_CONFIRM;
}

void BoxMonSelector_HandleConfirmPrompt(BoxMonSelector *selector) {
    switch (YesNoPrompt_HandleInput(selector->yesNoPrompt)) {
    case 1:
        BoxMonSelector_ClearMessageWindow(&selector->messageWindow, TRUE);
        YesNoPrompt_Reset(selector->yesNoPrompt);
        selector->state = BOX_MON_SELECTOR_STATE_FINISH;
        break;
    case 2:
        BoxMonSelector_ClearMessageWindow(&selector->messageWindow, TRUE);
        YesNoPrompt_Reset(selector->yesNoPrompt);
        BoxMonSelector_RemovePick(selector, selector->lastPick);
        BoxMonSelector_ShowTakePrompt(selector);
        BoxMonSelector_ResetBlend();
        BoxMonSelector_NotifyCursorMove(selector);
        selector->state = BOX_MON_SELECTOR_STATE_SELECT;
        break;
    }
}

void BoxMonSelector_ShowRemovePrompt(BoxMonSelector *selector) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = selector->bgConfig;
    template.bgId = 0;
    template.tileStart = 0x200;
    template.plttSlot = 8;
    template.x = 25;
    template.y = 15;
    YesNoPrompt_InitFromTemplate(selector->yesNoPrompt, &template);
    BoxMonSelector_ShowMessage(selector, msg_0298_00004, 0);
    BoxMonSelector_DimScreen();
    selector->state = BOX_MON_SELECTOR_STATE_REMOVE;
}

void BoxMonSelector_HandleRemovePrompt(BoxMonSelector *selector) {
    switch (YesNoPrompt_HandleInput(selector->yesNoPrompt)) {
    case 1:
        BoxMonSelector_ClearMessageWindow(&selector->messageWindow, TRUE);
        YesNoPrompt_Reset(selector->yesNoPrompt);
        BoxMonSelector_RemovePick(selector, selector->dragIndex);
        Sprite_SetAnimCtrlSeq(selector->handCursor, 45);
        BoxMonSelector_EndDrag(selector, 2);
        selector->keyDragging = FALSE;
        BoxMonSelector_ResetBlend();
        BoxMonSelector_NotifyCursorMove(selector);
        selector->state = BOX_MON_SELECTOR_STATE_SELECT;
        break;
    case 2:
        BoxMonSelector_ClearMessageWindow(&selector->messageWindow, TRUE);
        YesNoPrompt_Reset(selector->yesNoPrompt);
        BoxMonSelector_EndDrag(selector, 0);
        BoxMonSelector_ResetBlend();
        selector->state = BOX_MON_SELECTOR_STATE_SELECT;
        break;
    }
}

void BoxMonSelector_RenderSprites(BoxMonSelector *selector) {
    if (selector->spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(selector->spriteList);
    }
}

#pragma force_active on

void BoxMonSelector_InitSpriteResources(BoxMonSelector *selector) {
    int i;

    selector->spriteList = G2dRenderer_Init(43, &selector->renderer, selector->heapId);
    G2dRenderer_SetSubSurfaceCoords(&selector->renderer, 0, 256 * FX32_ONE);
    for (i = 0; i < 6; i++) {
        selector->resManagers[i] = Create2DGfxResObjMan(3, (GfGfxResType)i, selector->heapId);
    }
    selector->spriteResources[0] = AddCharResObjFromNarc(selector->resManagers[0], NARC_a_1_5_1, 8, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, selector->heapId);
    selector->spriteResources[1] = AddPlttResObjFromNarc(selector->resManagers[1], NARC_a_1_5_1, 5, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 4, selector->heapId);
    selector->spriteResources[2] = AddCellOrAnimResObjFromNarc(selector->resManagers[2], NARC_a_1_5_1, 7, TRUE, 0, GF_GFX_RES_TYPE_CELL, selector->heapId);
    selector->spriteResources[3] = AddCellOrAnimResObjFromNarc(selector->resManagers[3], NARC_a_1_5_1, 6, TRUE, 0, GF_GFX_RES_TYPE_ANIM, selector->heapId);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(selector->spriteResources[0]);
    SpriteTransfer_CreateExtPlttTransferTask(selector->spriteResources[1]);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_8_OFFSET, 0, selector->heapId);
}

#pragma force_active off

void BoxMonSelector_InitSpriteHeader(BoxMonSelector *selector) {
    CreateSpriteResourcesHeader(&selector->spriteHeader, 0, 0, 0, 0, -1, -1, 0, 0, selector->resManagers[0], selector->resManagers[1], selector->resManagers[2], selector->resManagers[3], NULL, NULL);
}

void BoxMonSelector_CreatePickSlots(BoxMonSelector *selector) {
    SpriteTemplate template;
    int i;
    u8 numEmpty;

    BoxMonSelector_InitSpriteTemplate(&template, selector, NNS_G2D_VRAM_TYPE_2DMAIN);
    selector->pickDropAreas = Heap_Alloc(selector->heapId, selector->maxPicks * sizeof(BoxMonSelectorDropArea));
    for (i = 0; i < selector->maxPicks; i++) {
        selector->pickDropAreas[i].top = 32 + i * 40;
        selector->pickDropAreas[i].bottom = selector->pickDropAreas[i].top + 24;
        selector->pickDropAreas[i].left = 192;
        selector->pickDropAreas[i].right = selector->pickDropAreas[i].left + 32;
        selector->pickDropAreas[i].occupied = FALSE;
        template.position.x = 212 * FX32_ONE;
        template.position.y = (i * 40 + 40) << FX32_SHIFT;
        if (selector->pickIcons[i].sprite == NULL) {
            selector->pickIcons[i].sprite = Sprite_CreateAffine(&template);
        }
        Sprite_SetAnimActiveFlag(selector->pickIcons[i].sprite, TRUE);
        Sprite_SetAnimCtrlSeq(selector->pickIcons[i].sprite, i + 39);
        Sprite_SetPriority(selector->pickIcons[i].sprite, 1);
        Sprite_SetDrawFlag(selector->pickIcons[i].sprite, FALSE);
        Sprite_SetDrawPriority(selector->pickIcons[i].sprite, 6);
        BoxMonSelector_SetHitboxRect(&selector->hitboxes[HITBOX_PICKS + i], 212, i * 40 + 44, 24, 24);
    }

    numEmpty = 0;
    for (; i < BOX_MON_SELECTOR_MAX_PICKS; i++) {
        selector->emptyPickSlots[numEmpty++] = BoxMonSelector_CreateSprite(selector, 212, i * 40 + 44, 47, TRUE);
    }

    selector->boxDropArea.top = 32;
    selector->boxDropArea.bottom = 151;
    selector->boxDropArea.left = 8;
    selector->boxDropArea.right = 151;
    selector->boxDropArea.occupied = FALSE;
}

#pragma force_active on

// Returns 0 when nothing happened, 1 when the Pokemon in hand was placed in a free pick slot, 2 when it was dropped
// back on the box (pick removed), 3 when a pick was moved to another pick slot.
int BoxMonSelector_HandleDrop(BoxMonSelector *selector) {
    if (selector->dragSource == DRAG_FROM_BOX) {
        int i = 0, count;
        count = selector->maxPicks;
        for (; i < count; i++) {
            if (!selector->pickDropAreas[i].occupied) {
                u32 y = selector->touchY;
                u32 left = selector->pickDropAreas[i].left, top = selector->pickDropAreas[i].top, bottom = selector->pickDropAreas[i].bottom, right = selector->pickDropAreas[i].right;
                if ((u32)(selector->touchX - left) < (u32)(right - left) && (u32)(y - top) < (u32)(bottom - top)) {
                    BoxMonSelector_AddPick(selector, (u8)i);
                    return 1;
                }
            } else {
                u32 y = selector->touchY;
                u32 left = selector->pickDropAreas[i].left, top = selector->pickDropAreas[i].top, bottom = selector->pickDropAreas[i].bottom, right = selector->pickDropAreas[i].right;
                if ((u32)(selector->touchX - left) < (u32)(right - left) && (u32)(y - top) < (u32)(bottom - top)) {
                    BoxMonSelector_RemovePick(selector, i);
                    BoxMonSelector_AddPick(selector, (u8)i);
                    return 1;
                }
            }
        }
    } else if (selector->dragSource == DRAG_FROM_PICKS) {
        u32 y = selector->touchY;
        u32 left = selector->boxDropArea.left, top = selector->boxDropArea.top, bottom = selector->boxDropArea.bottom;
        u32 x = selector->touchX;
        u32 right = selector->boxDropArea.right;
        if ((u32)(x - left) < (u32)(right - left) && (u32)(y - top) < (u32)(bottom - top)) {
            BoxMonSelector_RemovePick(selector, selector->dragIndex);
            BoxMonSelector_NotifyCursorMove(selector);
            return 2;
        } else {
            int i, count = selector->maxPicks;
            for (i = 0; i < count; i++) {
                if (!selector->pickDropAreas[i].occupied) {
                    u32 left = selector->pickDropAreas[i].left, top = selector->pickDropAreas[i].top, bottom = selector->pickDropAreas[i].bottom, right = selector->pickDropAreas[i].right;
                    if ((u32)(x - left) < (u32)(right - left) && (u32)(y - top) < (u32)(bottom - top)) {
                        BoxMonSelector_MovePick(selector, (u8)selector->dragIndex, (u8)i);
                        return 3;
                    }
                }
            }
        }
    } else {
        GF_ASSERT(FALSE);
    }
    return 0;
}

#pragma force_active off

void BoxMonSelector_AddPick(BoxMonSelector *selector, u8 pick) {
    u8 i;

    PlaySE(SEQ_SE_DP_BOX01);
    selector->pickDropAreas[pick].occupied = TRUE;
    BoxMonSelector_SetPickIcon(selector, pick, (u8)selector->dragIndex, FALSE);

    for (i = 0; i < selector->maxPicks; i++) {
        if (selector->marks[i].slot == -1) {
            VecFx32 pos = selector->dragOrigin;
            pos.x -= 0x8000;
            pos.y -= 0x4000;
            Sprite_SetMatrix(selector->marks[i].sprite, &pos);
            Sprite_SetDrawFlag(selector->marks[i].sprite, TRUE);
            selector->marks[i].slot = selector->dragIndex;
            selector->marks[i].page = selector->page;
            selector->numPicked++;
            Sprite_SetOamMode(selector->boxIcons[selector->dragIndex].sprite, GX_OAM_MODE_XLU);
            selector->picks[pick].slot = selector->dragIndex;
            selector->picks[pick].page = selector->page;
            selector->picks[pick].markIndex = i;
            selector->picks[pick].active = TRUE;
            selector->lastPick = pick;
            return;
        }
    }
}

void BoxMonSelector_MovePick(BoxMonSelector *selector, u8 from, u8 to) {
    PlaySE(SEQ_SE_DP_BOX01);
    selector->picks[from].active = FALSE;
    Sprite_SetDrawFlag(selector->pickIcons[from].sprite, FALSE);
    selector->pickDropAreas[from].occupied = FALSE;
    selector->pickDropAreas[to].occupied = TRUE;
    BoxMonSelector_SetPickIcon(selector, to, from, TRUE);

    selector->picks[to].slot = selector->picks[from].slot;
    selector->picks[to].page = selector->picks[from].page;
    selector->picks[to].markIndex = selector->picks[from].markIndex;
    selector->picks[to].active = TRUE;
    selector->lastPick = to;
}

void BoxMonSelector_CreateButtons(BoxMonSelector *selector) {
    int i;

    BoxMonSelector_SetHitboxRect(&selector->hitboxes[HITBOX_CANCEL], 224, 176, 50, 32);
    selector->cancelButton = BoxMonSelector_CreateSprite(selector, 224, 176, 4, TRUE);
    Sprite_SetPriority(selector->cancelButton, 3);
    if (selector->canCancel == FALSE) {
        Sprite_SetDrawFlag(selector->cancelButton, FALSE);
    }

    BoxMonSelector_SetHitboxRect(&selector->hitboxes[HITBOX_NEXT_PAGE], 144, 16, 32, 32);
    selector->nextPageArrow = BoxMonSelector_CreateSprite(selector, 144, 16, 2, TRUE);
    BoxMonSelector_SetHitboxRect(&selector->hitboxes[HITBOX_PREV_PAGE], 16, 16, 32, 32);
    selector->prevPageArrow = BoxMonSelector_CreateSprite(selector, 16, 16, 0, TRUE);
    selector->releaseEffect = BoxMonSelector_CreateSprite(selector, 0, 0, 50, FALSE);
    selector->hitboxes[selector->maxPicks + HITBOX_PICKS].rect.top = TOUCHSCREEN_RECTLIST_END;

    for (i = 0; i < selector->maxPicks; i++) {
        selector->marks[i].sprite = BoxMonSelector_CreateSprite(selector, i * 36, 64, 6, FALSE);
        selector->marks[i].slot = -1;
        selector->marks[i].page = selector->numPages;
        Sprite_SetDrawPriority(selector->marks[i].sprite, 4);
    }

    selector->numPicked = 0;
    selector->handCursor = BoxMonSelector_CreateSprite(selector, 20, 40, 45, TRUE);
    Sprite_SetPriority(selector->handCursor, 1);
    Sprite_SetDrawPriority(selector->handCursor, 1);
}

void BoxMonSelector_InitSpriteTemplate(SpriteTemplate *template, BoxMonSelector *selector, NNS_G2D_VRAM_TYPE vram) {
    template->spriteList = selector->spriteList;
    template->header = &selector->spriteHeader;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 20;
    template->whichScreen = vram;
    template->heapID = selector->heapId;
}

Sprite *BoxMonSelector_CreateSprite(BoxMonSelector *selector, int x, int y, int animSeq, BOOL drawFlag) {
    SpriteTemplate template;
    Sprite *sprite;
    BoxMonSelector_InitSpriteTemplate(&template, selector, NNS_G2D_VRAM_TYPE_2DMAIN);
    template.position.x = x << FX32_SHIFT;
    template.position.y = y << FX32_SHIFT;
    template.drawPriority = 10;
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, animSeq);
    Sprite_SetPriority(sprite, 1);
    Sprite_SetDrawFlag(sprite, drawFlag);
    return sprite;
}

#pragma force_active on

void BoxMonSelector_LoadScreens(BoxMonSelector *selector, BgConfig *bgConfig, enum HeapID heapId) {
    GfGfxLoader_LoadScrnData(NARC_a_1_5_1, 1, bgConfig, GF_BG_LYR_MAIN_2, 0, 0x600, TRUE, heapId);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 24, 1);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_2);
    GfGfxLoader_LoadScrnData(NARC_a_1_5_1, 4, bgConfig, GF_BG_LYR_MAIN_3, 0, 0x600, TRUE, heapId);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_MAIN_3, 0, 0, 32, 24, 1);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_MAIN_3);
    selector->screenFiles[0] = GfGfxLoader_GetScrnData(NARC_a_1_5_1, 3, TRUE, &selector->screens[0], heapId);
    selector->screenFiles[1] = GfGfxLoader_GetScrnData(NARC_a_1_5_1, 1, TRUE, &selector->screens[1], heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

#pragma force_active off

void BoxMonSelector_SetHitboxRect(TouchscreenHitbox *hitbox, int x, int y, int width, int height) {
    hitbox->rect.top = y - height / 2;
    hitbox->rect.left = x - width / 2;
    hitbox->rect.bottom = y + height / 2;
    hitbox->rect.right = x + width / 2;
}

#pragma force_active on

void BoxMonSelector_CreateBoxIcons(BoxMonSelector *selector) {
    SpriteTemplate template;
    int col, row, n;

    BoxMonSelector_InitSpriteTemplate(&template, selector, NNS_G2D_VRAM_TYPE_2DMAIN);
    n = 0;
    for (row = 0; row < 5; row++) {
        for (col = 0; col < 6; col++) {
            BoxMonSelector_SetHitboxRect(&selector->hitboxes[n], 20 + col * 24, 40 + row * 24, 24, 24);
            template.position.x = (20 + col * 24) * FX32_ONE;
            template.position.y = (40 + row * 24) * FX32_ONE;
            if (selector->boxIcons[n].sprite == NULL) {
                selector->boxIcons[n].sprite = Sprite_CreateAffine(&template);
            }
            Sprite_SetAnimActiveFlag(selector->boxIcons[n].sprite, TRUE);
            Sprite_SetAnimCtrlSeq(selector->boxIcons[n].sprite, n + 7);
            Sprite_SetPriority(selector->boxIcons[n].sprite, 1);
            Sprite_SetDrawFlag(selector->boxIcons[n].sprite, FALSE);
            Sprite_SetDrawPriority(selector->boxIcons[n].sprite, 6);
            n++;
        }
    }

    template.position.x = 20 * FX32_ONE;
    template.position.y = 40 * FX32_ONE;
    if (selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite == NULL) {
        selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite = Sprite_CreateAffine(&template);
    }
    Sprite_SetAnimActiveFlag(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, TRUE);
    Sprite_SetAnimCtrlSeq(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, 48);
    Sprite_SetPriority(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, 1);
    Sprite_SetDrawFlag(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, FALSE);
    Sprite_SetDrawPriority(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, 8);
    Sprite_SetOamMode(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, GX_OAM_MODE_XLU);
}

#pragma force_active off

int BoxMonSelector_LoadPage(BoxMonSelector *selector) {
    if (selector->page == BOX_MON_SELECTOR_PARTY_PAGE) {
        return BoxMonSelector_LoadPartyPage(selector);
    }
    return BoxMonSelector_LoadBoxPage(selector);
}

int BoxMonSelector_LoadBoxPage(BoxMonSelector *selector) {
    BoxMonSelectorMon mon;
    void *buf;
    NARC *narc;
    int i;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, selector->heapId);
    buf = Heap_AllocAtEnd(selector->heapId, 0x1000);
    for (i = 0; i < BOX_MON_SELECTOR_BOX_SLOTS; i++) {
        Sprite_SetOamMode(selector->boxIcons[i].sprite, GX_OAM_MODE_NORMAL);
        if (selector->getMon(selector->storage, selector->page, i, &mon)) {
            selector->boxIcons[i].mon = mon;
            BoxMonSelector_LoadIconChars(&mon, selector->iconChars, i, selector->boxIcons[i].sprite, buf, narc, 0x40);
            if (mon.isEgg) {
                Sprite_SetOamMode(selector->boxIcons[i].sprite, GX_OAM_MODE_XLU);
            } else {
                Sprite_SetOamMode(selector->boxIcons[i].sprite, GX_OAM_MODE_NORMAL);
            }
        } else {
            BoxMonSelector_LoadIconChars(&mon, selector->iconChars, i, NULL, buf, narc, 0x40);
            Sprite_SetDrawFlag(selector->boxIcons[i].sprite, FALSE);
        }
    }
    Heap_Free(buf);
    NARC_Delete(narc);
    selector->vblankTask = BoxMonSelector_TransferIconChars;
    BoxMonSelector_PrintPageName(selector, selector->page);
    BoxMonSelector_UpdatePickMarks(selector);
    CopyRectToBgTilemapRect(selector->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 20, 20, selector->screens[1]->rawData, 0, 0, 32, 24);
    ScheduleBgTilemapBufferTransfer(selector->bgConfig, GF_BG_LYR_MAIN_2);
}

int BoxMonSelector_LoadPartyPage(BoxMonSelector *selector) {
    BoxMonSelectorMon mon;
    void *buf;
    NARC *narc;
    int count;
    int i;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, selector->heapId);
    buf = Heap_AllocAtEnd(selector->heapId, 0x1000);
    count = (u8)Party_GetCount(selector->party);
    for (i = 0; i < count; i++) {
        BoxMonSelector_GetPartyMon(selector, i, &mon);
        selector->boxIcons[i].mon = mon;
        BoxMonSelector_LoadIconChars(&mon, selector->iconChars, i, selector->boxIcons[i].sprite, buf, narc, 0x40);
        if (mon.isEgg) {
            Sprite_SetOamMode(selector->boxIcons[i].sprite, GX_OAM_MODE_XLU);
        } else {
            Sprite_SetOamMode(selector->boxIcons[i].sprite, GX_OAM_MODE_NORMAL);
        }
    }
    mon.species = SPECIES_NONE;
    mon.isEgg = FALSE;
    mon.form = 0;
    mon.heldItem = ITEM_NONE;
    mon.unk_0E = 0;
    for (; i < BOX_MON_SELECTOR_BOX_SLOTS; i++) {
        BoxMonSelector_LoadIconChars(&mon, selector->iconChars, i, NULL, buf, narc, 0x40);
        Sprite_SetDrawFlag(selector->boxIcons[i].sprite, FALSE);
    }
    Heap_Free(buf);
    NARC_Delete(narc);
    selector->vblankTask = BoxMonSelector_TransferIconChars;
    BoxMonSelector_PrintPageName(selector, selector->page);
    BoxMonSelector_UpdatePickMarks(selector);
    CopyRectToBgTilemapRect(selector->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 20, 20, selector->screens[0]->rawData, 0, 0, 20, 20);
    ScheduleBgTilemapBufferTransfer(selector->bgConfig, GF_BG_LYR_MAIN_2);
}

#pragma force_active on

void BoxMonSelector_SetPickIcon(BoxMonSelector *selector, u8 pick, int source, BOOL fromPick) {
    void *buf;
    NARC *narc;
    int i;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, selector->heapId);
    buf = Heap_AllocAtEnd(selector->heapId, 0x1000);
    for (i = 0; i < BOX_MON_SELECTOR_BOX_SLOTS; i++) {
        selector->iconChars[i].sprite = NULL;
    }
    if (fromPick) {
        selector->pickIcons[pick].mon = selector->pickIcons[source].mon;
        BoxMonSelector_LoadIconChars(&selector->pickIcons[source].mon, selector->iconChars, pick, selector->pickIcons[pick].sprite, buf, narc, 0x222);
    } else {
        selector->pickIcons[pick].mon = selector->boxIcons[source].mon;
        BoxMonSelector_LoadIconChars(&selector->boxIcons[source].mon, selector->iconChars, pick, selector->pickIcons[pick].sprite, buf, narc, 0x222);
    }
    Heap_Free(buf);
    NARC_Delete(narc);
    selector->vblankTask = BoxMonSelector_TransferIconChars;
}

#pragma force_active off

void BoxMonSelector_TransferIconChars(BoxMonSelectorIconChars *chars) {
    int i;
    for (i = 0; i < BOX_MON_SELECTOR_BOX_SLOTS; i++) {
        if (chars->sprite != NULL) {
            DC_FlushRange(chars->chars, sizeof(chars->chars));
            GX_LoadOBJ(chars->chars, chars->vramOffset, sizeof(chars->chars));
            Sprite_SetDrawFlag(chars->sprite, TRUE);
        }
        chars++;
    }
}

int BoxMonSelector_PointAtBoxSlot(BoxMonSelector *selector, int slot) {
    Sprite_SetMatrix(selector->handCursor, Sprite_GetMatrixPtr(selector->boxIcons[slot].sprite));
    return BoxMonSelector_GetBoxSlotState(selector, slot);
}

#pragma force_active on

int BoxMonSelector_GetBoxSlotState(BoxMonSelector *selector, int slot) {
    int i, found;
    if (selector->boxIcons[slot].sprite && Sprite_GetDrawFlag(selector->boxIcons[slot].sprite) == FALSE) {
        return SLOT_INVALID;
    }
    if (selector->boxIcons[slot].mon.isEgg) {
        return SLOT_INVALID;
    }
    found = -1;
    i = 0;
    for (; found == -1 && i < selector->maxPicks; i++) {
        if (selector->page == selector->marks[i].page && slot == selector->marks[i].slot) {
            found = i;
        }
    }
    if (found == -1) {
        if (selector->numPicked == selector->maxPicks) {
            return SLOT_FULL;
        }
        for (i = 0; i < selector->maxPicks; i++) {
            if (selector->marks[i].slot == -1) {
                return SLOT_SELECTABLE;
            }
        }
    }
    return SLOT_PICKED;
}

#pragma force_active off

int BoxMonSelector_PointAtPick(BoxMonSelector *selector, int pick) {
    int ret = BoxMonSelector_GetPickState(selector, pick);
    if (pick < selector->maxPicks) {
        Sprite_SetMatrix(selector->handCursor, Sprite_GetMatrixPtr(selector->pickIcons[pick].sprite));
        return ret;
    }
    return ret;
}

int BoxMonSelector_GetPickState(BoxMonSelector *selector, int pick) {
    if (pick >= selector->maxPicks) {
        return SLOT_INVALID;
    }
    if (selector->pickIcons[pick].sprite != NULL && !Sprite_GetDrawFlag(selector->pickIcons[pick].sprite)) {
        return SLOT_INVALID;
    }
    return SLOT_SELECTABLE;
}

#pragma force_active on

void BoxMonSelector_StartDrag(BoxMonSelector *selector, int source, int index) {
    BoxMonSelectorIcon *icons;

    selector->dragging = TRUE;
    selector->dragIndex = index;
    selector->dragSource = source;
    if (source == DRAG_FROM_BOX) {
        icons = selector->boxIcons;
        selector->dragIcons = icons;
    } else if (source == DRAG_FROM_PICKS) {
        icons = selector->pickIcons;
        selector->dragIcons = icons;
    } else {
        GF_ASSERT(FALSE);
    }
    selector->dragOrigin = *Sprite_GetMatrixPtr(icons[index].sprite);
    Sprite_SetAnimCtrlSeq(selector->handCursor, 46);
    Sprite_SetDrawPriority(selector->dragIcons[selector->dragIndex].sprite, 3);
    if (source == DRAG_FROM_BOX) {
        VecFx32 *pos = Sprite_GetMatrixPtr(selector->dragIcons[selector->dragIndex].sprite);
        int i;
        NARC *narc;
        void *buf;

        for (i = 0; i < BOX_MON_SELECTOR_BOX_SLOTS; i++) {
            selector->iconChars[i].sprite = NULL;
        }
        narc = NARC_New(NARC_poketool_icongra_poke_icon, selector->heapId);
        buf = Heap_AllocAtEnd(selector->heapId, 0x1000);
        BoxMonSelector_LoadIconChars(&selector->dragIcons[selector->dragIndex].mon, selector->iconChars, 0, selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, buf, narc, 0x2B1);
        Sprite_SetMatrix(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, pos);
        Sprite_SetDrawFlag(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, TRUE);
        Heap_Free(buf);
        NARC_Delete(narc);
        selector->vblankTask = BoxMonSelector_TransferIconChars;
    }
}

void BoxMonSelector_UpdateTouchDrag(BoxMonSelector *selector) {
    u32 x, y;
    VecFx32 pos, iconPos;

    if (selector->dragging != FALSE) {
        System_GetTouchHeldCoords(&x, &y);
        pos.x = x << FX32_SHIFT;
        pos.y = y << FX32_SHIFT;
        pos.z = 0;
        iconPos = pos;
        iconPos.y -= 0x4000;
        Sprite_SetMatrix(selector->dragIcons[selector->dragIndex].sprite, &iconPos);
        Sprite_SetMatrix(selector->handCursor, &pos);
        selector->touchX = x;
        selector->touchY = y;
    }
}

#pragma force_active off

void BoxMonSelector_Drop(BoxMonSelector *selector) {
    if (selector->dragging != FALSE) {
        BoxMonSelector_EndDrag(selector, BoxMonSelector_HandleDrop(selector));
    }
}

#pragma force_active on

// result: 0 puts the Pokemon in hand back, otherwise the result of BoxMonSelector_HandleDrop.
void BoxMonSelector_EndDrag(BoxMonSelector *selector, int result) {
    VecFx32 position;

    if (result) {
        u8 index;
        if (selector->dragSource == DRAG_FROM_PICKS) {
            if (result == 2) {
                position = selector->dragOrigin;
                index = (u8)selector->dragIndex;
            } else {
                u8 pick = (u8)selector->lastPick;
                position.x = 212 * FX32_ONE;
                position.y = 40 * (pick + 1) << FX32_SHIFT;
                position.z = 0;
                index = (u8)selector->lastPick;
            }
        } else {
            u8 pick = (u8)selector->lastPick;
            position.x = 212 * FX32_ONE;
            position.y = 40 * (pick + 1) << FX32_SHIFT;
            position.z = 0;
            index = (u8)selector->lastPick;
        }
        GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_PICKS, index);
    } else {
        position = selector->dragOrigin;
        if (selector->dragSource == DRAG_FROM_PICKS) {
            GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_PICKS, (u8)selector->dragIndex);
        } else {
            GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_BOX, (u8)selector->dragIndex);
        }
    }
    Sprite_SetMatrix(selector->dragIcons[selector->dragIndex].sprite, &selector->dragOrigin);
    Sprite_SetDrawPriority(selector->dragIcons[selector->dragIndex].sprite, 6);
    Sprite_SetMatrix(selector->handCursor, &position);
    Sprite_SetAnimCtrlSeq(selector->handCursor, 45);
    if (selector->keyDragging) {
        selector->keyDragging = FALSE;
    }
    selector->dragging = FALSE;
    selector->dragIndex = -1;
    selector->dragOrigin.x = 0;
    selector->dragOrigin.y = 0;
    selector->dragOrigin.z = 0;
    selector->touchX = 0;
    selector->touchY = 0;
    selector->dragIcons = NULL;
    selector->dragSource = 0;
    Sprite_SetDrawFlag(selector->boxIcons[BOX_MON_SELECTOR_BOX_SLOTS].sprite, FALSE);
}

void BoxMonSelector_LoadIconChars(BoxMonSelectorMon *mon, BoxMonSelectorIconChars *chars, int index, Sprite *sprite, void *buf, NARC *narc, int baseTile) {
    BoxMonSelectorIconChars *entry = &chars[index];
    if (sprite) {
        NNSG2dCharacterData *charData;
        BoxMonSelector_ReadIconChars(GetMonIconNaixEx(mon->species, mon->isEgg, mon->form), &charData, buf, narc);
        MIi_CpuCopyFast(charData->pRawData, (u32 *)entry->chars, sizeof(entry->chars));
        entry->vramOffset = (baseTile + (index << 4)) << 5;
        entry->sprite = sprite;
        entry->palette = GetMonIconPaletteEx(mon->species, mon->form, mon->isEgg) + 8;
        Sprite_SetPaletteOverride(sprite, entry->palette);
    } else {
        entry->sprite = NULL;
    }
}

#pragma force_active off

void BoxMonSelector_ReadIconChars(u32 fileId, NNSG2dCharacterData **ppCharData, void *dest, NARC *narc) {
    NARC_ReadWholeMember(narc, fileId, dest);
    NNS_G2dGetUnpackedBGCharacterData(dest, ppCharData);
}

#pragma force_active on

void BoxMonSelector_PrintPageName(BoxMonSelector *selector, int page) {
    String *string;
    if (page == BOX_MON_SELECTOR_PARTY_PAGE) {
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0298_bin, selector->heapId);
        string = NewString_ReadMsgData(msgData, msg_0298_00001);
        DestroyMsgData(msgData);
    } else {
        string = String_New(64, selector->heapId);
        selector->getPageName(string, selector->storage, page);
    }
    FillWindowPixelBuffer(&selector->pageNameWindow, 0);
    AddTextPrinterParameterizedWithColor(&selector->pageNameWindow, 0, string, FontID_String_GetCenterAlignmentX(0, string, 0, 96), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    CopyWindowToVram(&selector->pageNameWindow);
    String_Delete(string);
}

void BoxMonSelector_PrintCancelButton(BoxMonSelector *selector) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0298_bin, selector->heapId);
    String *string = NewString_ReadMsgData(msgData, msg_0298_00000);
    FillWindowPixelBuffer(&selector->cancelWindow, 0);
    if (selector->canCancel) {
        int width = GetWindowWidth(&selector->cancelWindow) << 3;
        AddTextPrinterParameterizedWithColor(&selector->cancelWindow, 4, string, FontID_String_GetCenterAlignmentX(4, string, 0, width), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    }
    CopyWindowToVram(&selector->cancelWindow);
    String_Delete(string);
    DestroyMsgData(msgData);
}

#pragma force_active off

void BoxMonSelector_InitBgs(BgConfig *bgConfig, enum HeapID heapId) {
    BoxMonSelector_InitBgLayers(bgConfig);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    GfGfxLoader_GXLoadPal(NARC_a_1_5_1, 0, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0, 0xC0, heapId);
    GfGfxLoader_LoadCharData(NARC_a_1_5_1, 2, bgConfig, GF_BG_LYR_MAIN_2, 0, 5 << 10, TRUE, heapId);
    BG_FillCharDataRange(bgConfig, GF_BG_LYR_MAIN_0, 0, 1, 0);
}

void BoxMonSelector_InitBgLayers(BgConfig *bgConfig) {
    {
        BgTemplate template = gBoxMonSelectorBgTemplateMain0;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_0, &template, 0);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_0);
    }
    {
        BgTemplate template = gBoxMonSelectorBgTemplateMain1;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_1, &template, 0);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_1);
    }
    {
        BgTemplate template = gBoxMonSelectorBgTemplateMain2;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_2, &template, 0);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_2);
    }
    {
        BgTemplate template = gBoxMonSelectorBgTemplateMain3;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_MAIN_3, &template, 0);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_MAIN_3);
    }
}

// Shows the marks of the picks on the page shown and makes their box icons translucent.
void BoxMonSelector_UpdatePickMarks(BoxMonSelector *selector) {
    int i;
    for (i = 0; i < selector->maxPicks; i++) {
        if (selector->marks[i].slot != -1 && selector->page == selector->marks[i].page) {
            Sprite_SetDrawFlag(selector->marks[i].sprite, TRUE);
            Sprite_SetOamMode(selector->boxIcons[selector->marks[i].slot].sprite, GX_OAM_MODE_XLU);
        } else {
            Sprite_SetDrawFlag(selector->marks[i].sprite, FALSE);
        }
    }
}

void BoxMonSelector_GetPartyMon(BoxMonSelector *selector, int slot, BoxMonSelectorMon *mon) {
    Pokemon *partyMon = Party_GetMonByIndex(selector->party, slot);
    mon->species = GetMonData(partyMon, MON_DATA_SPECIES, NULL);
    mon->isEgg = GetMonData(partyMon, MON_DATA_IS_EGG, NULL);
    mon->form = GetMonData(partyMon, MON_DATA_FORM, NULL);
    mon->heldItem = GetMonData(partyMon, MON_DATA_HELD_ITEM, NULL);
    mon->unk_0E = 0;
}

#pragma force_active on

void BoxMonSelector_RemovePick(BoxMonSelector *selector, int pick) {
    u32 markIndex = selector->picks[pick].markIndex;
    int page = selector->marks[markIndex].page;
    int slot = selector->marks[markIndex].slot;
    Sprite_SetDrawFlag(selector->marks[markIndex].sprite, FALSE);
    selector->marks[markIndex].slot = -1;
    selector->numPicked--;
    if (page == selector->page) {
        Sprite_SetOamMode(selector->boxIcons[slot].sprite, GX_OAM_MODE_NORMAL);
    } else {
        Sprite_SetMatrix(selector->releaseEffect, Sprite_GetMatrixPtr(selector->pickIcons[pick].sprite));
        Sprite_SetDrawFlag(selector->releaseEffect, TRUE);
        Sprite_SetAnimCtrlSeq(selector->releaseEffect, 50);
    }
    selector->picks[pick].active = FALSE;
    Sprite_SetDrawFlag(selector->pickIcons[pick].sprite, FALSE);
    selector->pickDropAreas[pick].occupied = FALSE;
}

void BoxMonSelector_ShowMessage(BoxMonSelector *selector, int msgId, FontID fontId) {
    MsgData *msgData;
    String *string;

    LoadUserFrameGfx2(selector->bgConfig, GF_BG_LYR_MAIN_0, 978, 11, 0, selector->heapId);
    AddWindowParameterized(selector->bgConfig, &selector->messageWindow, GF_BG_LYR_MAIN_3, 2, 21, 21, 2, 13, 198);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0298_bin, selector->heapId);
    string = NewString_ReadMsgData(msgData, msgId);
    FillWindowPixelBuffer(&selector->messageWindow, 15);
    AddTextPrinterParameterizedWithColor(&selector->messageWindow, fontId, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(&selector->messageWindow);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
}

#pragma force_active off

void BoxMonSelector_ShowTakePrompt(BoxMonSelector *selector) {
    if (selector->maxPicks == 1) {
        BoxMonSelector_ShowMessage(selector, msg_0298_00005, 1);
    }
}

void BoxMonSelector_RemoveTakePrompt(BoxMonSelector *selector) {
    if (selector->maxPicks == 1) {
        if (GetWindowBgConfig(&selector->messageWindow) != NULL) {
            RemoveWindow(&selector->messageWindow);
        }
    }
}

void BoxMonSelector_ClearMessageWindow(Window *window, BOOL hidePlane) {
    if (window->bgConfig != NULL) {
        if (hidePlane) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
        }
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
}

void BoxMonSelector_WriteResults(BoxMonSelector *selector) {
    int i;
    for (i = 0; i < selector->maxPicks; i++) {
        selector->results[i].page = selector->picks[i].page;
        selector->results[i].slot = selector->picks[i].slot;
    }
}

void BoxMonSelector_DeleteSprites(BoxMonSelector *selector) {
    int i;
    u8 numEmpty;
    for (i = 0; i < BOX_MON_SELECTOR_BOX_SLOTS; i++) {
        if (selector->boxIcons[i].sprite != NULL) {
            Sprite_Delete(selector->boxIcons[i].sprite);
        }
    }
    for (i = 0; i < selector->maxPicks; i++) {
        if (selector->pickIcons[i].sprite != NULL) {
            Sprite_Delete(selector->pickIcons[i].sprite);
        }
    }
    for (i = 0; i < selector->maxPicks; i++) {
        if (selector->marks[i].sprite != NULL) {
            Sprite_Delete(selector->marks[i].sprite);
        }
    }
    if (selector->cancelButton != NULL) {
        Sprite_Delete(selector->cancelButton);
    }
    if (selector->prevPageArrow != NULL) {
        Sprite_Delete(selector->prevPageArrow);
    }
    if (selector->nextPageArrow != NULL) {
        Sprite_Delete(selector->nextPageArrow);
    }
    if (selector->releaseEffect != NULL) {
        Sprite_Delete(selector->releaseEffect);
    }
    if (selector->handCursor != NULL) {
        Sprite_Delete(selector->handCursor);
    }
    numEmpty = BOX_MON_SELECTOR_MAX_PICKS - selector->maxPicks;
    for (i = 0; i < numEmpty; i++) {
        if (selector->emptyPickSlots[i] != NULL) {
            Sprite_Delete(selector->emptyPickSlots[i]);
        }
    }
}

void BoxMonSelector_UpdateCursorSprite(BoxMonSelector *selector) {
    VecFx32 pos;
    VecFx32 iconPos;
    int moved = FALSE;
    int panel = GridCursor_GetPanel(selector->cursor);
    int index = GridCursor_GetIndex(selector->cursor);
    pos.z = 0;
    switch (panel) {
    case BOX_MON_SELECTOR_PANEL_PAGE:
        pos.x = 5 << 16;
        pos.y = 1 << 16;
        moved = TRUE;
        break;
    case BOX_MON_SELECTOR_PANEL_BOX:
    case BOX_MON_SELECTOR_PANEL_PARTY:
        pos.x = ((u8)(index % 6) * 24 + 20) << FX32_SHIFT;
        pos.y = ((u8)(index / 6) * 24 + 40) << FX32_SHIFT;
        moved = TRUE;
        break;
    case BOX_MON_SELECTOR_PANEL_PICKS:
        pos.x = 212 * FX32_ONE;
        pos.y = (index + 1) * 40 << FX32_SHIFT;
        moved = TRUE;
        break;
    case BOX_MON_SELECTOR_PANEL_PLACE:
        pos.x = 212 * FX32_ONE;
        pos.y = (index + 1) * 40 << FX32_SHIFT;
        if (selector->keyDragging) {
            iconPos = pos;
            iconPos.y -= 4 * FX32_ONE;
            Sprite_SetMatrix(selector->dragIcons[selector->dragIndex].sprite, &iconPos);
            selector->touchX = 212;
            selector->touchY = (index + 1) * 40;
        }
        moved = TRUE;
        break;
    case BOX_MON_SELECTOR_PANEL_CANCEL:
        pos.x = 224 * FX32_ONE;
        pos.y = 176 * FX32_ONE;
        moved = TRUE;
        break;
    }
    if (moved) {
        Sprite_SetMatrix(selector->handCursor, &pos);
        BoxMonSelector_NotifyCursorMove(selector);
    }
}

#pragma force_active on

// Returns 1 when a Pokemon was taken in hand, 3 when CANCEL was pressed.
int BoxMonSelector_HandleAButton(BoxMonSelector *selector) {
    int result = 0;
    u32 panel = GridCursor_GetPanel(selector->cursor), index = GridCursor_GetIndex(selector->cursor);
    switch (panel) {
    case BOX_MON_SELECTOR_PANEL_BOX:
    case BOX_MON_SELECTOR_PANEL_PARTY:
        if (BoxMonSelector_PointAtBoxSlot(selector, index) == SLOT_SELECTABLE) {
            int i;
            BoxMonSelector_StartDrag(selector, DRAG_FROM_BOX, index);
            for (i = 0; i < selector->maxPicks; i++) {
                VecFx32 pos = { 0, 0, 0 }, iconPos;
                if (!selector->pickDropAreas[i].occupied) {
                    selector->touchX = 212;
                    selector->touchY = 40 * ((u8)i + 1);
                    pos.x = (s32)(selector->touchX * FX32_ONE);
                    pos.y = (s32)(selector->touchY * FX32_ONE);
                    iconPos = pos;
                    iconPos.y = (s32)((u32)iconPos.y - 4 * FX32_ONE);
                    Sprite_SetMatrix(selector->dragIcons[selector->dragIndex].sprite, &iconPos);
                    Sprite_SetMatrix(selector->handCursor, &pos);
                    GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_PLACE, (u8)i);
                    break;
                }
            }
            selector->keyDragging = TRUE;
            result = 1;
        }
        break;
    case BOX_MON_SELECTOR_PANEL_PICKS:
        if (BoxMonSelector_PointAtPick(selector, index) == SLOT_SELECTABLE) {
            VecFx32 iconPos;
            BoxMonSelector_StartDrag(selector, DRAG_FROM_PICKS, index);
            iconPos = *Sprite_GetMatrixPtr(selector->dragIcons[selector->dragIndex].sprite);
            iconPos.y = (s32)((u32)iconPos.y - 4 * FX32_ONE);
            Sprite_SetMatrix(selector->dragIcons[selector->dragIndex].sprite, &iconPos);
            selector->keyDragging = TRUE;
            result = 1;
            selector->state = BOX_MON_SELECTOR_STATE_REMOVE_INIT;
        }
        break;
    case BOX_MON_SELECTOR_PANEL_CANCEL:
        BoxMonSelector_PressCancelButton(selector);
        result = 3;
        break;
    case BOX_MON_SELECTOR_PANEL_PLACE:
        BoxMonSelector_Drop(selector);
        break;
    case 0:
    case BOX_MON_SELECTOR_PANEL_PAGE:
        break;
    }
    return result;
}

#pragma force_active off

int BoxMonSelector_HandleBButton(BoxMonSelector *selector) {
    VecFx32 pos;
    int result = 0;
    switch (GridCursor_GetPanel(selector->cursor)) {
    case BOX_MON_SELECTOR_PANEL_PLACE:
        PlaySE(SEQ_SE_DP_SELECT);
        BoxMonSelector_EndDrag(selector, result);
        break;
    case BOX_MON_SELECTOR_PANEL_CANCEL:
        BoxMonSelector_PressCancelButton(selector);
        result = 3;
        break;
    default:
        if (selector->canCancel) {
            GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_CANCEL, result);
            pos.x = 224 * FX32_ONE;
            pos.y = 176 * FX32_ONE;
            pos.z = 0;
            Sprite_SetMatrix(selector->handCursor, &pos);
            PlaySE(SEQ_SE_DP_SELECT);
        }
        break;
    }
    return result;
}

#pragma force_active on

void BoxMonSelector_PrevPage(BoxMonSelector *selector) {
    if (selector->page == 0) {
        selector->page = selector->numPages - 1;
    } else {
        selector->page--;
    }
    Sprite_SetAnimCtrlSeq(selector->prevPageArrow, 1);
    BoxMonSelector_LoadPage(selector);
    PlaySE(SEQ_SE_DP_SELECT);
    switch (selector->page) {
    case BOX_MON_SELECTOR_PARTY_PAGE:
        if (GridCursor_GetPanel(selector->cursor) == BOX_MON_SELECTOR_PANEL_BOX) {
            u8 column = GridCursor_GetIndex(selector->cursor) % 6;
            VecFx32 pos;
            GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_PARTY, column);
            pos.x = (column * 24 + 20) << FX32_SHIFT;
            pos.y = 40 << FX32_SHIFT;
            pos.z = 0;
            Sprite_SetMatrix(selector->handCursor, &pos);
        }
        break;
    default:
        if (GridCursor_GetPanel(selector->cursor) == BOX_MON_SELECTOR_PANEL_PARTY) {
            GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_BOX, GridCursor_GetIndex(selector->cursor));
        }
        break;
    }
}

#pragma force_active off

void BoxMonSelector_NextPage(BoxMonSelector *selector) {
    VecFx32 pos;
    u8 index;
    u8 column;

    selector->page++;
    if (selector->page == selector->numPages) {
        selector->page = 0;
    }
    Sprite_SetAnimCtrlSeq(selector->nextPageArrow, 3);
    BoxMonSelector_LoadPage(selector);
    PlaySE(SEQ_SE_DP_SELECT);
    if (selector->page == BOX_MON_SELECTOR_PARTY_PAGE) {
        if (GridCursor_GetPanel(selector->cursor) == BOX_MON_SELECTOR_PANEL_BOX) {
            column = GridCursor_GetIndex(selector->cursor) % 6;
            GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_PARTY, column);
            pos.x = (column * 24 + 20) << FX32_SHIFT;
            pos.y = 40 * FX32_ONE;
            pos.z = 0;
            Sprite_SetMatrix(selector->handCursor, &pos);
        }
    } else if (GridCursor_GetPanel(selector->cursor) == BOX_MON_SELECTOR_PANEL_PARTY) {
        index = GridCursor_GetIndex(selector->cursor);
        GridCursor_SetPosition(selector->cursor, BOX_MON_SELECTOR_PANEL_BOX, index);
    }
}

void BoxMonSelector_PressCancelButton(BoxMonSelector *selector) {
    Sprite_SetAnimCtrlSeq(selector->cancelButton, 5);
    PlaySE(SEQ_SE_DP_SELECT);
}

#pragma force_active on

// Tells the caller about the Pokemon under the cursor (page and slot, or -1, -1) when it changed.
void BoxMonSelector_NotifyCursorMove(BoxMonSelector *selector) {
    int page, slot, index;
    if (selector->onCursorMove) {
        page = slot = -1;
        switch (GridCursor_GetPanel(selector->cursor)) {
        case 0:
        case BOX_MON_SELECTOR_PANEL_CANCEL:
        case BOX_MON_SELECTOR_PANEL_PAGE:
            break;
        case BOX_MON_SELECTOR_PANEL_BOX:
        case BOX_MON_SELECTOR_PANEL_PARTY:
            page = selector->page;
            index = GridCursor_GetIndex(selector->cursor);
            if (BoxMonSelector_GetBoxSlotState(selector, index) == SLOT_SELECTABLE) {
                slot = index;
            }
            break;
        case BOX_MON_SELECTOR_PANEL_PICKS:
            index = GridCursor_GetIndex(selector->cursor);
            if (BoxMonSelector_GetPickState(selector, index) == SLOT_SELECTABLE) {
                page = selector->picks[index].page;
                slot = selector->picks[index].slot;
            }
            break;
        case BOX_MON_SELECTOR_PANEL_PLACE:
            if (selector->dragging) {
                page = selector->page;
                slot = selector->dragIndex;
            }
            break;
        }
        if (selector->lastCursorPage != page || selector->lastCursorSlot != slot) {
            selector->onCursorMove(page, slot, selector->onCursorMoveArg);
        }
        selector->lastCursorPage = page;
        selector->lastCursorSlot = slot;
    }
}

#pragma force_active off

void BoxMonSelector_DimScreen(void) {
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_OBJ, -7);
}

void BoxMonSelector_ResetBlend(void) {
    reg_G2_BLDCNT = 0;
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_PLANEMASK_BG2, 6, 10);
}
