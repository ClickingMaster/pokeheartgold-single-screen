#include "global.h"

#include "constants/flags.h"
#include "constants/sndseq.h"

#include "application/palpad/palpad_internal.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "sound_playback.h"
#include "string_util.h"
#include "sys_flags.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02013FDC.h"
#include "frontier_save_access.h"
#include "union_room_avatar.h"

// input given to PalPadDetails_HandleAction
enum PalPadDetailsInput {
    DETAILS_INPUT_SELECT,
    DETAILS_INPUT_BACK,
    DETAILS_INPUT_PAGE_LEFT,
    DETAILS_INPUT_PAGE_RIGHT,
    DETAILS_INPUT_PREV_FRIEND,
    DETAILS_INPUT_NEXT_FRIEND,
};

typedef void (*PalPadDetailsPagePrinter)(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

extern const TouchscreenHitbox sPalPadDetailsHitboxes[];
extern const TouchscreenHitbox sPalPadDetailsFrontierHitboxes[];
extern const u32 sPalPadDetailsSlotBaseTiles[3];
extern const PalPadPanelLayout sPalPadPanelLayouts[3];
extern const PalPadWindowSpec *const sPalPadDetailsPageWindowSpecs[8];
extern const u8 sPalPadDetailsPageWindowCounts[8];
extern const WindowTemplate sPalPadDetailsProfileWindowTemplates[8];
extern const u8 sPalPadDetailsTabX[2][8];
extern const u8 sPalPadDetailsTabWidths[2][8];
extern const u8 sPalPadDetailsTabSrcY[2];
// u16 view of the start of the rodata; [23..26] are the file ids of the details screens
extern const u16 _0222ED5C[];
extern const PalPadDetailsPagePrinter sPalPadDetailsPagePrinters[3];
extern const PalPadDetailsPagePrinter sPalPadDetailsFrontierPagePrinters[8];

BOOL PalPadDetails_IsLastPage(PalPadDetails *details, int page);
int PalPadDetails_HandleAction(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, int input);
int PalPadDetails_HandleKeys(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
int PalPadDetails_HandleTouch(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, BOOL *touched);
int PalPadDetails_HandleInput(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx);
void PalPadDetails_StartPageScroll(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
BOOL PalPadDetails_UpdatePageScroll(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_Show(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_Hide(PalPadDetails *details, PalPadGfx *gfx);
void PalPadDetails_HideForFriendChange(PalPadDetails *details, PalPadGfx *gfx);
void PalPadDetails_DrawProfile(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_ClearProfile(PalPadDetails *details);
void PalPadDetails_DrawPage(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, int panel, int page, enum HeapID heapId);
void PalPadDetails_SetCursor(PalPadDetails *details, PalPadGfx *gfx, int page);
void PalPadDetails_HideCursor(PalPadDetails *details, PalPadGfx *gfx);
void PalPadDetails_DrawTabs(PalPadDetails *details, PalPadGfx *gfx, int selected);
void PalPadDetails_ClearTabs(PalPadDetails *details, PalPadGfx *gfx);
BOOL PalPadDetails_SelectNextFriend(PalPadShared *shared, int direction);
void PalPadDetails_LoadScreens(PalPadDetailsScreen *screens, PalPadGfx *gfx, enum HeapID heapId);
void PalPadDetails_FreeScreens(PalPadDetailsScreen *screens);
void PalPadDetails_AddProfileWindows(PalPadDetails *details, PalPadGfx *gfx);
void PalPadDetails_RemoveProfileWindows(PalPadDetails *details, PalPadGfx *gfx);
void PalPadDetailsSlot_Init(PalPadDetailsSlot *slot, PalPadGfx *gfx, const PalPadPanelLayout *layout, u32 baseTile, enum HeapID heapId);
void PalPadDetailsSlot_Free(PalPadDetailsSlot *slot);
void PalPadDetailsSlot_AddWindows(PalPadDetailsSlot *slot, PalPadGfx *gfx, u32 page, u32 count, const PalPadWindowSpec *specs, enum HeapID heapId);
void PalPadDetailsSlot_RemoveWindows(PalPadDetailsSlot *slot, u32 page);
void PalPadDetailsSlot_DrawPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, u32 page, void *unused, BOOL frontier, enum HeapID heapId);
void PalPadDetailsSlot_Clear(PalPadDetailsSlot *slot, PalPadGfx *gfx);

static inline int Abs16(s16 value) {
    return value < 0 ? -value : value;
}

void PalPadDetails_Init(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    // the Battle Frontier pages need the Frontier fly point
    details->hasFrontierPages = Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(shared->saveData), FLAG_ACTION_CHECK, 27);
    PalPadDetails_Setup(details, shared, gfx, heapId);
}

#pragma force_active on

int PalPadDetails_Main(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    // the friend change delay and held flag are accessed as volatile here (the original reloads them)
    switch (shared->subState) {
    case 0:
        *(vu16 *)&details->friendChangeHeld = FALSE;
        details->page = 0;
        // fallthrough
    case 1:
        PalPadDetails_Show(details, shared, gfx, heapId);
        shared->subState = 2;
        break;
    case 2: // returns the next state
        shared->subState = PalPadDetails_HandleInput(details, shared, gfx);
        break;
    case 3:
        PalPadDetails_StartPageScroll(details, shared, gfx, heapId);
        PalPadDetails_UpdatePageScroll(details, shared, gfx, heapId);
        shared->subState = 4;
        break;
    case 4:
        if (PalPadDetails_UpdatePageScroll(details, shared, gfx, heapId) == TRUE) {
            shared->subState = 2;
        }
        break;
    case 5: // previous / next friend; repeats while the key is held
        if (*(vu16 *)&details->friendChangeHeld) {
            (*(vu16 *)&details->friendChangeDelay)--;
            if (*(vu16 *)&details->friendChangeDelay) {
                break;
            }
        }
        if (PalPadDetails_SelectNextFriend(shared, details->direction)) {
            PlaySE(SEQ_SE_DP_BUTTON3);
            PalPadDetails_HideForFriendChange(details, gfx);
            PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_DETAILS, 1);
            *(vu16 *)&details->friendChangeHeld = TRUE;
            return 1;
        }
        shared->subState = 2;
        break;
    case 6:
        PalPadDetails_Hide(details, gfx);
        PalPadShared_SetNextScreen(shared, PALPAD_SCREEN_ROSTER, 9);
        return 1;
    }
    return 0;
}

#pragma force_active reset

void PalPadDetails_Exit(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx) {
    PalPadDetails_Teardown(details, shared, gfx);
}

// the last tab is the return button
BOOL PalPadDetails_IsLastPage(PalPadDetails *details, int page) {
    int last = (details->hasFrontierPages == TRUE ? 8 : 3) - 1;
    return page == last;
}

#pragma force_active on

int PalPadDetails_HandleAction(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, int input) {
    switch (input) {
    case DETAILS_INPUT_SELECT:
        if (!PalPadDetails_IsLastPage(details, details->page)) {
            break;
        }
        PlaySE(SEQ_SE_DP_DECIDE);
        return 6;
    case DETAILS_INPUT_BACK:
        PlaySE(SEQ_SE_DP_DECIDE);
        return 6;
    case DETAILS_INPUT_PAGE_LEFT:
        PalPadDetails_SetCursor(details, gfx, details->page);
        PalPadDetails_DrawTabs(details, gfx, details->page);
        details->direction = 2;
        return 3;
    case DETAILS_INPUT_PAGE_RIGHT:
        PalPadDetails_SetCursor(details, gfx, details->page);
        PalPadDetails_DrawTabs(details, gfx, details->page);
        details->direction = 3;
        return 3;
    case DETAILS_INPUT_PREV_FRIEND:
        details->direction = 0;
        details->friendChangeDelay = 4;
        PalPadGfx_AnimateArrow(gfx, 2, 1);
        return 5;
    case DETAILS_INPUT_NEXT_FRIEND:
        details->direction = 1;
        details->friendChangeDelay = 4;
        PalPadGfx_AnimateArrow(gfx, 3, 1);
        return 5;
    }
    return 2;
}

int PalPadDetails_HandleKeys(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx) {
    u32 keys;

    if (gSystem.newKeys & PALPAD_KEYS_ANY) {
        shared->inputState = 0;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_BACK);
    }
    keys = gSystem.newAndRepeatedKeys;
    if ((keys & PAD_KEY_LEFT) || (shared->lrPaging && (keys & PAD_BUTTON_L))) {
        PalPadDetails_MovePage(details, -1);
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_PAGE_LEFT);
    }
    if ((keys & PAD_KEY_RIGHT) || (shared->lrPaging && (keys & PAD_BUTTON_R))) {
        PalPadDetails_MovePage(details, 1);
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_PAGE_RIGHT);
    }
    if (gSystem.heldKeys & PAD_KEY_UP) {
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_PREV_FRIEND);
    }
    if (gSystem.heldKeys & PAD_KEY_DOWN) {
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_NEXT_FRIEND);
    }
    return 2;
}

// hitboxes: 0/1 arrows (previous / next friend), 2.. page tabs
int PalPadDetails_HandleTouch(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, BOOL *touched) {
    int hit;
    s16 delta;

    if (details->hasFrontierPages) {
        hit = TouchscreenHitbox_FindHitboxAtTouchNew(sPalPadDetailsFrontierHitboxes);
    } else {
        hit = TouchscreenHitbox_FindHitboxAtTouchNew(sPalPadDetailsHitboxes);
    }
    if (hit == -1) {
        return 2;
    }
    if (hit == 0) {
        *touched = TRUE;
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_PREV_FRIEND);
    }
    if (hit == 1) {
        *touched = TRUE;
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_NEXT_FRIEND);
    }
    if (PalPadDetails_IsLastPage(details, (s16)(hit - 2))) {
        details->page = hit - 2;
        *touched = TRUE;
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_BACK);
    }
    delta = (s16)((hit - 2) - details->page);
    if (delta == 0) {
        return 2;
    }
    *touched = TRUE;
    PalPadDetails_MovePage(details, delta);
    if (delta >= 0) {
        return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_PAGE_RIGHT);
    }
    return PalPadDetails_HandleAction(details, shared, gfx, DETAILS_INPUT_PAGE_LEFT);
}

#pragma force_active reset

int PalPadDetails_HandleInput(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx) {
    BOOL touched = FALSE;
    int result;

    if ((gSystem.heldKeys & (PAD_KEY_UP | PAD_KEY_DOWN)) == 0) {
        details->friendChangeHeld = FALSE;
    }
    result = PalPadDetails_HandleTouch(details, shared, gfx, &touched);
    if (touched != FALSE) {
        shared->inputState = TRUE;
        return result;
    }
    return PalPadDetails_HandleKeys(details, shared, gfx);
}

void PalPadDetails_StartPageScroll(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    int panel;

    details->scrollStep = 0;
    PalPadGfx_SetArrowsAnimActive(gfx, FALSE);
    if (details->direction == 2) {
        panel = 2;
    } else {
        panel = 1;
    }
    PalPadDetails_DrawPage(details, shared, gfx, panel, details->page, heapId);
    PlaySE(SEQ_SE_DP_SELECT5);
}

#pragma force_active on

BOOL PalPadDetails_UpdatePageScroll(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    int offset;

    if (details->scrollStep > 4) {
        ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        PalPadDetails_DrawPage(details, shared, gfx, 0, details->page, heapId);
        PalPadGfx_SetArrowsAnimActive(gfx, TRUE);
        PalPadDetails_DrawTabs(details, gfx, 0xFF);
        return TRUE;
    }
    offset = (details->scrollStep << 8) / 4;
    if (details->direction == 2) {
        offset = -offset;
    }
    ScheduleSetBgPosText(gfx->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, offset);
    details->scrollStep++;
    return FALSE;
}

void PalPadDetails_Setup(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    const PalPadWindowSpec *const *specs;
    const u8 *count;
    int j;
    int i;
    const u32 *baseTile;
    const PalPadPanelLayout *layout;
    PalPadDetailsSlot *slot;

    PalPadDetails_LoadScreens(details->screens, gfx, heapId);
    PalPadDetails_AddProfileWindows(details, gfx);
    i = 0;
    baseTile = sPalPadDetailsSlotBaseTiles;
    layout = sPalPadPanelLayouts;
    slot = details->slots;
    do {
        PalPadDetailsSlot_Init(slot, gfx, layout, *baseTile, heapId);
        specs = sPalPadDetailsPageWindowSpecs;
        count = sPalPadDetailsPageWindowCounts;
        j = 0;
        do {
            PalPadDetailsSlot_AddWindows(slot, gfx, j, *count, *specs, heapId);
            specs++;
            count++;
            j++;
        } while (j < 8);
        slot++;
        baseTile++;
        layout++;
        i++;
    } while (i < 3);
}

#pragma force_active reset

void PalPadDetails_Teardown(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx) {
    int i;
    int j;
    PalPadDetailsSlot *slot;

    for (i = 0, slot = details->slots; i < 3; i++, slot++) {
        for (j = 0; j < 8; j++) {
            PalPadDetailsSlot_RemoveWindows(slot, j);
        }
        PalPadDetailsSlot_Free(slot);
    }
    PalPadDetails_RemoveProfileWindows(details, gfx);
    PalPadDetails_FreeScreens(details->screens);
}

#pragma force_active on

void PalPadDetails_Show(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    NNSG2dScreenData *screen;

    PalPadGfx_BufferFriendName(gfx, shared->saveData, shared->friends.slots[shared->selectedFriend], heapId);
    PalPadGfx_PrintTitle(gfx, &details->profileWindows[1], 13);
    screen = details->screens[3].data;
    LoadRectToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_SUB_1, screen->rawData, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_SUB_1);
    screen = details->screens[0].data;
    LoadRectToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, screen->rawData, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_0);
    screen = details->screens[1].data;
    LoadRectToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, screen->rawData, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    PalPadDetails_DrawProfile(details, shared, gfx, heapId);
    PalPadGfx_SetCursorPriority(gfx, 0, 0);
    PalPadDetails_DrawPage(details, shared, gfx, 0, details->page, heapId);
    PalPadDetails_DrawTabs(details, gfx, 0xFF);
    Sprite_SetPositionXY(gfx->arrows[0], 244, 24);
    PalPadGfx_AnimateArrow(gfx, 2, 0);
    Sprite_SetPositionXY(gfx->arrows[1], 244, 136);
    PalPadGfx_AnimateArrow(gfx, 3, 0);
    PalPadGfx_SetArrowsVisible(gfx, TRUE);
    SetBgPriority(GF_BG_LYR_MAIN_2, 0);
}

void PalPadDetails_Hide(PalPadDetails *details, PalPadGfx *gfx) {
    int i;
    PalPadDetailsSlot *slot;
    NNSG2dScreenData *screen;

    PalPadGfx_SetArrowsVisible(gfx, FALSE);
    Sprite_SetPositionXY(gfx->arrows[0], 8, 98);
    PalPadGfx_AnimateArrow(gfx, 0, 0);
    Sprite_SetPositionXY(gfx->arrows[1], 248, 98);
    PalPadGfx_AnimateArrow(gfx, 1, 0);
    for (i = 0, slot = details->slots; i < 3; i++, slot++) {
        PalPadDetailsSlot_Clear(slot, gfx);
    }
    PalPadDetails_ClearTabs(details, gfx);
    PalPadDetails_HideCursor(details, gfx);
    PalPadDetails_ClearProfile(details);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_SUB_1, 0, 0, 0, 32, 32, 0);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, 32, 32, 0);
    FillBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 0, 32, 32, 0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_SUB_1);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_SUB_0);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    screen = details->screens[2].data;
    LoadRectToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_0, screen->rawData, 0, 0, screen->screenWidth >> 3, screen->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_0);
    PalPadGfx_ClearTitle(gfx);
    SetBgPriority(GF_BG_LYR_MAIN_2, 2);
}

#pragma force_active reset

void PalPadDetails_HideForFriendChange(PalPadDetails *details, PalPadGfx *gfx) {
    int i;
    PalPadDetailsSlot *slot;
    Window *window;

    i = 0;
    slot = details->slots;
    for (; i < 3; i++, slot++) {
        PalPadDetailsSlot_Clear(slot, gfx);
    }
    for (i = 0, window = details->profileWindows; i <= 5; i++, window++) {
        ClearWindowTilemapAndScheduleTransfer(window);
    }
    PalPadGfx_ClearTitle(gfx);
}

#pragma force_active on

// Top screen: number, name (in the gender colour), group, last meeting and the Union Room avatar.
void PalPadDetails_DrawProfile(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    UnkStruct_02070D3C avatar;
    FriendRoster *wifiList = Save_FriendRoster_Get(shared->saveData);
    int gender = FriendRoster_GetEntryField(wifiList, shared->friends.slots[shared->selectedFriend], 8);
    u32 i;
    u32 color;
    void *bitmap;

    for (i = 0; i < 8; i++) {
        FillWindowPixelBuffer(&details->profileWindows[i], 0);
    }
    BufferIntegerAsString(gfx->msgFormat, 0, shared->friends.numbers[shared->selectedFriend] + 1, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    BufferIntegerAsString(gfx->msgFormat, 1, shared->friends.numberedCount, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
    PalPadDetails_PrintProfileText(&details->profileWindows[0], shared, gfx, 18, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    if (gender == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else if (gender == 1) {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    } else {
        color = MAKE_TEXT_COLOR(1, 2, 0);
    }
    PalPadGfx_BufferFriendName(gfx, shared->saveData, shared->friends.slots[shared->selectedFriend], heapId);
    PalPadDetails_PrintProfileText(&details->profileWindows[2], shared, gfx, 17, 0, 0, color);
    PalPadDetails_PrintProfileText(&details->profileWindows[6], shared, gfx, 15, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    ov43_0222AB5C(gfx, shared->saveData, shared->friends.slots[shared->selectedFriend], heapId);
    PalPadDetails_PrintProfileText(&details->profileWindows[3], shared, gfx, 16, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    PalPadDetails_PrintProfileText(&details->profileWindows[7], shared, gfx, 27, 0, 0, MAKE_TEXT_COLOR(15, 2, 0));
    if (PalPadGfx_BufferLastDate(gfx, shared->saveData, shared->friends.slots[shared->selectedFriend])) {
        PalPadDetails_PrintProfileText(&details->profileWindows[4], shared, gfx, 28, 0, 0, MAKE_TEXT_COLOR(1, 2, 0));
    }
    sub_02070D84(GetUnionRoomAvatarAttrBySprite(gender, FriendRoster_GetEntryField(wifiList, shared->friends.slots[shared->selectedFriend], 7), 1), 2, &avatar);
    bitmap = Heap_Alloc(heapId, 80 * 80 / 2);
    sub_020141C4(avatar.narcId, avatar.ncbr_id, heapId, 0, 0, 10, 10, bitmap);
    BlitBitmapRectToWindow(&details->profileWindows[5], bitmap, 0, 0, 80, 80, 0, 0, 80, 80);
    Heap_Free(bitmap);
    GfGfxLoader_GXLoadPal(avatar.narcId, avatar.nclr_id, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, 32, heapId);
    BgTilemapRectChangePalette(gfx->bgConfig, GF_BG_LYR_SUB_0, 4, 4, 10, 10, 15);
    for (i = 0; i < 8; i++) {
        ScheduleWindowCopyToVram(&details->profileWindows[i]);
    }
}

#pragma force_active reset

void PalPadDetails_ClearProfile(PalPadDetails *details) {
    int i;
    Window *window;

    for (i = 0, window = details->profileWindows; i < 8; i++, window++) {
        ClearWindowTilemapAndScheduleTransfer(window);
    }
}

static inline void PalPadDetails_SetPage(PalPadDetails *details, PalPadGfx *gfx, int page) {
    details->page = page;
    PalPadDetails_SetCursor(details, gfx, page);
}

#pragma force_active on

// panel 0 is the visible page; 1 / 2 are drawn off screen for a scroll to the right / left
void PalPadDetails_DrawPage(PalPadDetails *details, PalPadShared *shared, PalPadGfx *gfx, int panel, int page, enum HeapID heapId) {
    if (panel == 0) {
        PalPadDetails_SetPage(details, gfx, page);
    }
    PalPadDetailsSlot_DrawPage(&details->slots[panel], shared, gfx, page, &details->screens[1], details->hasFrontierPages, heapId);
}

#pragma force_active reset

void PalPadDetails_SetCursor(PalPadDetails *details, PalPadGfx *gfx, int page) {
    int animSeq;

    if (details->hasFrontierPages == TRUE) {
        animSeq = 2;
        Sprite_SetPositionXY(gfx->cursor, page * 32 + 16, 176);
    } else if (page == 2) {
        animSeq = 2;
        Sprite_SetPositionXY(gfx->cursor, 240, 176);
    } else {
        animSeq = 1;
        Sprite_SetPositionXY(gfx->cursor, page * 112 + 56, 176);
    }
    PalPadGfx_SetCursor(gfx, TRUE, animSeq);
}

void PalPadDetails_HideCursor(PalPadDetails *details, PalPadGfx *gfx) {
    PalPadGfx_SetCursor(gfx, FALSE, 0xFF);
}

#pragma force_active on

void PalPadDetails_DrawTabs(PalPadDetails *details, PalPadGfx *gfx, int selected) {
    if (details->hasFrontierPages == TRUE) {
        CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 20, 32, 4, gfx->screens.data[1]->rawData, 0, 12, gfx->screens.data[1]->screenWidth >> 3, gfx->screens.data[1]->screenHeight >> 3);
    } else {
        CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 20, 32, 4, gfx->screens.data[1]->rawData, 0, 20, gfx->screens.data[1]->screenWidth >> 3, gfx->screens.data[1]->screenHeight >> 3);
    }
    if (selected == 0xFF) {
        ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
        return;
    }
    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, sPalPadDetailsTabX[details->hasFrontierPages][selected], 20, sPalPadDetailsTabWidths[details->hasFrontierPages][selected], 4, gfx->screens.data[1]->rawData, sPalPadDetailsTabX[details->hasFrontierPages][selected], sPalPadDetailsTabSrcY[details->hasFrontierPages], gfx->screens.data[1]->screenWidth >> 3, gfx->screens.data[1]->screenHeight >> 3);
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

#pragma force_active reset

void PalPadDetails_ClearTabs(PalPadDetails *details, PalPadGfx *gfx) {
    NNSG2dScreenData *screen = gfx->screens.data[1];

    CopyToBgTilemapRect(gfx->bgConfig, GF_BG_LYR_MAIN_2, 0, 20, 32, 4, screen->rawData, 0, 28, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

// direction 0: previous, 1: next; skips friends that were never met
BOOL PalPadDetails_SelectNextFriend(PalPadShared *shared, int direction) {
    FriendRoster *wifiList = Save_FriendRoster_Get(shared->saveData);
    int current = shared->selectedFriend;
    u32 i;

    if (direction == 0) {
        for (i = 0; i < shared->friends.count - 1; i++) {
            current--;
            if (current < 0) {
                current += shared->friends.count;
            }
            if (FriendRoster_GetEntryField(wifiList, shared->friends.slots[current], 8) != 2) {
                shared->selectedFriend = current;
                return TRUE;
            }
        }
    } else {
        for (i = 0; i < shared->friends.count - 1; i++) {
            current = (current + 1) % shared->friends.count;
            if (FriendRoster_GetEntryField(wifiList, shared->friends.slots[current], 8) != 2) {
                shared->selectedFriend = current;
                return TRUE;
            }
        }
    }
    return FALSE;
}

void PalPadDetails_LoadScreens(PalPadDetailsScreen *screens, PalPadGfx *gfx, enum HeapID heapId) {
    u16 fileIds[4];
    u16 *fileId;
    int i;

    fileIds[0] = _0222ED5C[23];
    fileIds[1] = _0222ED5C[24];
    fileId = fileIds;
    i = 0;
    fileIds[2] = _0222ED5C[25];
    fileIds[3] = _0222ED5C[26];
    for (; i < 4; i++) {
        screens->file = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, *fileId, TRUE, &screens->data, heapId);
        screens++;
        fileId++;
    }
}

void PalPadDetails_FreeScreens(PalPadDetailsScreen *screens) {
    int i;

    for (i = 0; i < 4; i++, screens++) {
        Heap_Free(screens->file);
    }
}

void PalPadDetails_AddProfileWindows(PalPadDetails *details, PalPadGfx *gfx) {
    const WindowTemplate *template = sPalPadDetailsProfileWindowTemplates;
    int i = 0;
    Window *window = details->profileWindows;

    for (; i < 8; i++) {
        AddWindow(gfx->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        template ++;
        window++;
    }
}

void PalPadDetails_RemoveProfileWindows(PalPadDetails *details, PalPadGfx *gfx) {
    int i = 0;
    Window *window;

    window = details->profileWindows;
    for (; i < 8; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
}

void PalPadDetailsSlot_Init(PalPadDetailsSlot *slot, PalPadGfx *gfx, const PalPadPanelLayout *layout, u32 baseTile, enum HeapID heapId) {
    int i;

    slot->layout = *layout;
    slot->baseTile = baseTile;
    for (i = 0; i < 8; i++) {
        slot->windows[i] = NULL;
        slot->windowCounts[i] = 0;
    }
}

void PalPadDetailsSlot_Free(PalPadDetailsSlot *slot) {
}

void PalPadDetailsSlot_AddWindows(PalPadDetailsSlot *slot, PalPadGfx *gfx, u32 page, u32 count, const PalPadWindowSpec *specs, enum HeapID heapId) {
    u32 i;
    u16 baseTile;

    slot->windows[page] = AllocWindows(heapId, (u8)count);
    slot->windowCounts[page] = count;
    baseTile = (u16)slot->baseTile;
    for (i = 0; i < count; i++) {
        InitWindow(&slot->windows[page][i]);
        AddWindowParameterized(gfx->bgConfig, &slot->windows[page][i], GF_BG_LYR_MAIN_3, specs[i].x + Abs16(slot->layout.x), specs[i].y + Abs16(slot->layout.y), specs[i].width, specs[i].height, 11, baseTile);
        FillWindowPixelBuffer(&slot->windows[page][i], 0);
        baseTile = (u16)(baseTile + specs[i].width * specs[i].height);
    }
}

void PalPadDetailsSlot_RemoveWindows(PalPadDetailsSlot *slot, u32 page) {
    if (slot->windows[page] != NULL) {
        WindowArray_Delete(slot->windows[page], (u8)slot->windowCounts[page]);
        slot->windows[page] = NULL;
        slot->windowCounts[page] = 0;
    }
}

void PalPadDetailsSlot_CopyWindowsToVram(PalPadDetailsSlot *slot, u32 page) {
    u32 i;

    for (i = 0; i < slot->windowCounts[page]; i++) {
        ScheduleWindowCopyToVram(&slot->windows[page][i]);
    }
}

void PalPadDetailsSlot_ClearWindows(PalPadDetailsSlot *slot, u32 page) {
    u32 i;

    for (i = 0; i < slot->windowCounts[page]; i++) {
        ClearWindowTilemapAndScheduleTransfer(&slot->windows[page][i]);
        FillWindowPixelBuffer(&slot->windows[page][i], 0);
    }
}

void PalPadDetailsSlot_DrawPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, u32 page, void *unused, BOOL frontier, enum HeapID heapId) {
    PalPadDetailsSlot_Clear(slot, gfx);
    if (frontier) {
        sPalPadDetailsFrontierPagePrinters[page](slot, shared, gfx, heapId);
    } else {
        sPalPadDetailsPagePrinters[page](slot, shared, gfx, heapId);
    }
}

void PalPadDetailsSlot_Clear(PalPadDetailsSlot *slot, PalPadGfx *gfx) {
    int i;

    for (i = 0; i < 8; i++) {
        PalPadDetailsSlot_ClearWindows(slot, i);
    }
    PalPadDetailsSlot_ClearBg(gfx, slot);
}

// Record pages: Union Room records, Wi-Fi Plaza, Battle Frontier facilities, return tab
void ov43_0222DFB0(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx) {
    FriendRoster *wifiList = Save_FriendRoster_Get(shared->saveData);
    u32 i;
    u32 y;

    PalPadDetailsSlot_Print(slot, 0, 0, shared, gfx, 0x13, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    i = 0;
    y = 0;
    do {
        PalPadDetailsSlot_Print(slot, 0, 2, shared, gfx, 0x14 + i, 0, (u8)y, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
        PalPadGfx_BufferNumber(gfx, FriendRoster_GetEntryField(wifiList, shared->friends.slots[shared->selectedFriend], i + 1));
        PalPadDetailsSlot_Print(slot, 0, 2, shared, gfx, 0x16, 0, (u8)y, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
        y += 0x18;
        i++;
    } while (i < 2);
    PalPadDetailsSlot_Print(slot, 0, 1, shared, gfx, 0x17, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    PalPadGfx_BufferNumber(gfx, FriendRoster_GetEntryField(wifiList, shared->friends.slots[shared->selectedFriend], 3));
    PalPadDetailsSlot_Print(slot, 0, 3, shared, gfx, 0x18, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    PalPadDetailsSlot_CopyWindowsToVram(slot, 0);
}

void ov43_0222E0E8(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx) {
    FrontierSave *frontier = Save_Frontier_GetStatic(shared->saveData);

    PalPadDetailsSlot_Print(slot, 2, 0, shared, gfx, 0x1D, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    PalPadDetailsSlot_Print(slot, 2, 1, shared, gfx, FrontierSave_GetStat(frontier, 0x64, shared->friends.slots[shared->selectedFriend]) + 0x22, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x71, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 2, 1, shared, gfx, 0x25, 0x70, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 1);
    PalPadDetailsSlot_Print(slot, 2, 1, shared, gfx, 0x24, 0, 0x18, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x70, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 2, 1, shared, gfx, 0x25, 0x70, 0x18, 0x10200, gfx->pageText, gfx->pageBuffer, 1);
    PalPadDetailsSlot_CopyWindowsToVram(slot, 2);
}

void ov43_0222E220(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    FrontierSave *frontier = Save_Frontier_GetStatic(shared->saveData);
    String *raw = String_New(0x80, heapId);
    String *expanded = String_New(0x80, heapId);
    u32 message;

    PalPadDetailsSlot_Print(slot, 3, 0, shared, gfx, 0x1E, 0, 0, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_Print(slot, 3, 1, shared, gfx, 0x26, 0, 0, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_Print(slot, 3, 1, shared, gfx, 0x28, 0xD0, 0, 0x10200, raw, expanded, 2);

    message = FrontierSave_GetStat(frontier, 0x66, shared->friends.slots[shared->selectedFriend]) == 0 ? 0x22 : 0x23;
    PalPadDetailsSlot_Print(slot, 3, 2, shared, gfx, message, 0, 0, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x73, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 3, 2, shared, gfx, 0x25, 0x70, 0, 0x10200, raw, expanded, 1);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x75, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 3, 2, shared, gfx, 0x2B, 0, 0, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_Print(slot, 3, 2, shared, gfx, 0x24, 0, 0x10, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x72, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 3, 2, shared, gfx, 0x25, 0x70, 0x10, 0x10200, raw, expanded, 1);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x74, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 3, 2, shared, gfx, 0x2B, 0, 0x10, 0x10200, raw, expanded, 0);

    PalPadDetailsSlot_Print(slot, 3, 3, shared, gfx, 0x27, 0, 0, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_Print(slot, 3, 3, shared, gfx, 0x28, 0xD0, 0, 0x10200, raw, expanded, 2);

    message = FrontierSave_GetStat(frontier, 0x68, shared->friends.slots[shared->selectedFriend]) == 0 ? 0x22 : 0x23;
    PalPadDetailsSlot_Print(slot, 3, 4, shared, gfx, message, 0, 0, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x77, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 3, 4, shared, gfx, 0x25, 0x70, 0, 0x10200, raw, expanded, 1);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x79, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 3, 4, shared, gfx, 0x2B, 0, 0, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_Print(slot, 3, 4, shared, gfx, 0x24, 0, 0x10, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x76, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 3, 4, shared, gfx, 0x25, 0x70, 0x10, 0x10200, raw, expanded, 1);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x78, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 3, 4, shared, gfx, 0x2B, 0, 0x10, 0x10200, raw, expanded, 0);

    PalPadDetailsSlot_CopyWindowsToVram(slot, 3);
    String_Delete(raw);
    String_Delete(expanded);
}

void ov43_0222E5B4(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    FrontierSave *frontier = Save_Frontier_GetStatic(shared->saveData);
    String *raw = String_New(0x80, heapId);
    String *expanded = String_New(0x80, heapId);
    u32 value;

    PalPadDetailsSlot_Print(slot, 4, 0, shared, gfx, 0x1f, 0, 0, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_Print(slot, 4, 0, shared, gfx, 0x29, 0x68, 0x14, 0x10200, raw, expanded, 0);
    value = FrontierSave_GetStat(frontier, 0x6c, shared->friends.slots[shared->selectedFriend]);
    PalPadDetailsSlot_Print(slot, 4, 0, shared, gfx, value == 0 ? 0x22 : 0x23, 0, 0x30, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x87, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 4, 0, shared, gfx, 0x2d, 0x70, 0x30, 0x10200, raw, expanded, 1);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x88, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 4, 0, shared, gfx, 0x2c, 0xa8, 0x30, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_Print(slot, 4, 0, shared, gfx, 0x24, 0, 0x48, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x86, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 4, 0, shared, gfx, 0x2d, 0x70, 0x48, 0x10200, raw, expanded, 1);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x8a, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 4, 0, shared, gfx, 0x2c, 0xa8, 0x48, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_CopyWindowsToVram(slot, 4);
    String_Delete(raw);
    String_Delete(expanded);
}

void ov43_0222E78C(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    FrontierSave *frontier = Save_Frontier_GetStatic(shared->saveData);
    String *raw = String_New(0x80, heapId);
    String *expanded = String_New(0x80, heapId);
    String *species;
    u32 value;

    PalPadDetailsSlot_Print(slot, 5, 0, shared, gfx, 0x20, 0, 0, 0x10200, raw, expanded, 0);
    species = GetSpeciesName(FrontierSave_GetStat(frontier, 0x7c, shared->friends.slots[shared->selectedFriend]), heapId);
    AddTextPrinterParameterizedWithColor(slot->windows[5],
        0,
        species,
        0,
        0x18,
        0xff,
        0x10200,
        NULL);
    String_Delete(species);
    value = FrontierSave_GetStat(frontier, 0x6a, shared->friends.slots[shared->selectedFriend]);
    PalPadDetailsSlot_Print(slot, 5, 0, shared, gfx, value == 0 ? 0x22 : 0x23, 0, 0x30, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x7b, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 5, 0, shared, gfx, 0x25, 0x70, 0x30, 0x10200, raw, expanded, 1);
    PalPadDetailsSlot_Print(slot, 5, 0, shared, gfx, 0x24, 0, 0x48, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x7a, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 5, 0, shared, gfx, 0x25, 0x70, 0x48, 0x10200, raw, expanded, 1);
    PalPadDetailsSlot_CopyWindowsToVram(slot, 5);
    String_Delete(raw);
    String_Delete(expanded);
}

void ov43_0222E904(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    FrontierSave *frontier = Save_Frontier_GetStatic(shared->saveData);
    String *raw = String_New(0x80, heapId);
    String *expanded = String_New(0x80, heapId);
    u32 value;

    PalPadDetailsSlot_Print(slot, 6, 0, shared, gfx, 0x21, 0, 0, 0x10200, raw, expanded, 0);
    value = FrontierSave_GetStat(frontier, 0x6e, shared->friends.slots[shared->selectedFriend]);
    PalPadDetailsSlot_Print(slot, 6, 0, shared, gfx, value == 0 ? 0x22 : 0x23, 0, 0x18, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x8f, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 6, 0, shared, gfx, 0x2e, 0x70, 0x18, 0x10200, raw, expanded, 1);
    PalPadDetailsSlot_Print(slot, 6, 0, shared, gfx, 0x24, 0, 0x30, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FrontierSave_GetStat(frontier, 0x8e, shared->friends.slots[shared->selectedFriend]));
    PalPadDetailsSlot_Print(slot, 6, 0, shared, gfx, 0x2e, 0x70, 0x30, 0x10200, raw, expanded, 1);
    PalPadDetailsSlot_CopyWindowsToVram(slot, 6);
    String_Delete(raw);
    String_Delete(expanded);
}

void PalPadDetails_PrintWiFiPlazaPage(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    FriendRoster *wifiList;
    String *raw;
    String *expanded;

    Save_Frontier_GetStatic(shared->saveData);
    wifiList = Save_FriendRoster_Get(shared->saveData);
    raw = String_New(0x80, heapId);
    expanded = String_New(0x80, heapId);
    PalPadDetailsSlot_Print(slot, 1, 0, shared, gfx, 0x2f, 0, 0, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferActivityName(gfx, 0);
    PalPadDetailsSlot_Print(slot, 1, 0, shared, gfx, 0x30, 0, 0x18, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FriendRoster_GetEntryField(wifiList, shared->friends.slots[shared->selectedFriend], 10));
    PalPadDetailsSlot_Print(slot, 1, 0, shared, gfx, 0x31, 0x78, 0x18, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferActivityName(gfx, 1);
    PalPadDetailsSlot_Print(slot, 1, 0, shared, gfx, 0x30, 0, 0x30, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FriendRoster_GetEntryField(wifiList, shared->friends.slots[shared->selectedFriend], 11));
    PalPadDetailsSlot_Print(slot, 1, 0, shared, gfx, 0x31, 0x78, 0x30, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferActivityName(gfx, 2);
    PalPadDetailsSlot_Print(slot, 1, 0, shared, gfx, 0x30, 0, 0x48, 0x10200, raw, expanded, 0);
    PalPadGfx_BufferNumber(gfx, FriendRoster_GetEntryField(wifiList, shared->friends.slots[shared->selectedFriend], 12));
    PalPadDetailsSlot_Print(slot, 1, 0, shared, gfx, 0x31, 0x78, 0x48, 0x10200, raw, expanded, 0);
    PalPadDetailsSlot_CopyWindowsToVram(slot, 1);
    String_Delete(raw);
    String_Delete(expanded);
}

void ov43_0222EBD8(PalPadDetailsSlot *slot, PalPadShared *shared, PalPadGfx *gfx) {
    PalPadDetailsSlot_Print(slot, 7, 0, shared, gfx, 0xe, 0, 0, 0x10200, gfx->pageText, gfx->pageBuffer, 0);
    PalPadDetailsSlot_CopyWindowsToVram(slot, 7);
}
