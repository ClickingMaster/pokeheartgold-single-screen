#include "global.h"

#include "frontier/battle_castle.h"

#include "battle_frontier_communication.h"
#include "palette.h"
#include "sprite.h"
#include "system.h"

static void BattleCastle_GridCallbackNoOp(void *data, int newTarget, int prevTarget);
static void BattleCastlePartyApp_OnPartyGridKeyMove(void *data, int newTarget, int prevTarget);
static void BattleCastlePartyApp_OnPartyGridTouch(void *data, int newTarget, int prevTarget);
static void BattleCastleOpponentApp_OnPartyGridKeyMove(void *data, int newTarget, int prevTarget);
static void BattleCastleOpponentApp_OnPartyGridTouch(void *data, int newTarget, int prevTarget);
static void BattleCastlePartyApp_OnItemGridKeyMove(void *data, int newTarget, int prevTarget);
static void BattleCastlePartyApp_OnItemGridTouch(void *data, int newTarget, int prevTarget);

static const TouchscreenHitbox sBattleCastleItemGridArrowHitboxes[] = {
    { .rect = { 160, 191, 0, 39 } },
    { .rect = { 160, 191, 40, 79 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const GridCallbacks sBattleCastleItemGridCallbacks = {
    .onButton = BattleCastle_GridCallbackNoOp,
    .onSwitchToTouchMode = BattleCastle_GridCallbackNoOp,
    .onKeyMove = BattleCastlePartyApp_OnItemGridKeyMove,
    .onTouch = BattleCastlePartyApp_OnItemGridTouch,
};

static const GridCallbacks sBattleCastlePartyAppPartyGridCallbacks = {
    .onButton = BattleCastle_GridCallbackNoOp,
    .onSwitchToTouchMode = BattleCastle_GridCallbackNoOp,
    .onKeyMove = BattleCastlePartyApp_OnPartyGridKeyMove,
    .onTouch = BattleCastlePartyApp_OnPartyGridTouch,
};

static const GridCallbacks sBattleCastleOpponentAppPartyGridCallbacks = {
    .onButton = BattleCastle_GridCallbackNoOp,
    .onSwitchToTouchMode = BattleCastle_GridCallbackNoOp,
    .onKeyMove = BattleCastleOpponentApp_OnPartyGridKeyMove,
    .onTouch = BattleCastleOpponentApp_OnPartyGridTouch,
};

// The EXIT button, then the three party slots
static const TouchscreenHitbox sBattleCastlePartyGrid3Hitboxes[] = {
    { .rect = { 152, 175, 200, 247 } },
    { .rect = { 32, 79, 32, 95 } },
    { .rect = { 32, 79, 96, 159 } },
    { .rect = { 32, 79, 160, 223 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

// Party grid position -> party slot (4: the EXIT button)
static const int sBattleCastlePartyGridSlots[] = { 4, 0, 1, 2, 3 };

// The EXIT button, then the four party slots
static const TouchscreenHitbox sBattleCastlePartyGrid4Hitboxes[] = {
    { .rect = { 152, 175, 200, 247 } },
    { .rect = { 32, 79, 0, 63 } },
    { .rect = { 32, 79, 64, 127 } },
    { .rect = { 32, 79, 128, 191 } },
    { .rect = { 32, 79, 192, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const u32 sBattleCastleItemGridCursorAnims[] = { 3, 3, 3, 3, 3, 3, 19 };

// Rental item grid position -> button
static const int sBattleCastleItemGridButtons[] = { 0, 1, 2, 3, 4, 5, 8 };

static const TouchscreenHitbox sBattleCastleItemGridHitboxes[] = {
    { .rect = { 32, 71, 0, 127 } },
    { .rect = { 32, 71, 128, 255 } },
    { .rect = { 72, 111, 0, 127 } },
    { .rect = { 72, 111, 128, 255 } },
    { .rect = { 112, 151, 0, 127 } },
    { .rect = { 112, 151, 128, 255 } },
    { .rect = { 160, 191, 200, 255 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const DpadMenuBox sBattleCastlePartyGrid3Boxes[] = {
    { 0, 0, 0, 0, 0x81, 0, 0, 0 },
    { 0, 0, 0, 0, 1,    0, 3, 2 },
    { 0, 0, 0, 0, 2,    0, 1, 3 },
    { 0, 0, 0, 0, 3,    0, 2, 1 },
};

static const DpadMenuBox sBattleCastlePartyGrid4Boxes[] = {
    { 0, 0, 0, 0, 0x81, 0, 0, 0 },
    { 0, 0, 0, 0, 1,    0, 4, 2 },
    { 0, 0, 0, 0, 2,    0, 1, 3 },
    { 0, 0, 0, 0, 3,    0, 2, 4 },
    { 0, 0, 0, 0, 4,    0, 3, 1 },
};

static const DpadMenuBox sBattleCastleItemGridBoxes[] = {
    { 64,  52,  0, 0, 0,    2, 0, 1 },
    { 192, 52,  0, 0, 1,    3, 0, 1 },
    { 64,  92,  0, 0, 0,    4, 2, 3 },
    { 192, 92,  0, 0, 1,    5, 2, 3 },
    { 64,  132, 0, 0, 2,    6, 4, 5 },
    { 192, 132, 0, 0, 3,    6, 4, 5 },
    { 228, 176, 0, 0, 0x85, 6, 6, 6 },
};

void BattleCastle_FreeGrid(GridInputHandler *grid) {
    GridInputHandler_Free(grid);
}

// Grid callback that does nothing
static void BattleCastle_GridCallbackNoOp(void *data, int newTarget, int prevTarget) {
}

GridInputHandler *BattleCastlePartyApp_CreatePartyGrid(BattleCastlePartyApp *app, int initialPos, int numMons) {
    if (numMons == 3) {
        return GridInputHandler_Create(sBattleCastlePartyGrid3Hitboxes, sBattleCastlePartyGrid3Boxes, &sBattleCastlePartyAppPartyGridCallbacks, app, TRUE, initialPos, HEAP_ID_107);
    }
    return GridInputHandler_Create(sBattleCastlePartyGrid4Hitboxes, sBattleCastlePartyGrid4Boxes, &sBattleCastlePartyAppPartyGridCallbacks, app, TRUE, initialPos, HEAP_ID_107);
}

GridInputHandler *BattleCastleOpponentApp_CreatePartyGrid(BattleCastleOpponentApp *app, int initialPos, int numMons) {
    if (numMons == 3) {
        return GridInputHandler_Create(sBattleCastlePartyGrid3Hitboxes, sBattleCastlePartyGrid3Boxes, &sBattleCastleOpponentAppPartyGridCallbacks, app, TRUE, initialPos, HEAP_ID_107);
    }
    return GridInputHandler_Create(sBattleCastlePartyGrid4Hitboxes, sBattleCastlePartyGrid4Boxes, &sBattleCastleOpponentAppPartyGridCallbacks, app, TRUE, initialPos, HEAP_ID_107);
}

// Party grid input: the selected slot (CASTLE_*_GRID_EXIT for EXIT) or a GRID_MENU_* code
int BattleCastle_HandlePartyGridInput(GridInputHandler *grid) {
    int input = GridInputHandler_HandleInput_NoHold(grid);

    switch (input) {
    case GRID_MENU_BUTTON_MODE:
    case GRID_MENU_CURSOR_MOVE:
    case GRID_MENU_CANCEL:
    case GRID_MENU_NOTHING_CHOSEN:
        return input;
    }
    return sBattleCastlePartyGridSlots[input];
}

void BattleCastle_SetGridInputsToNext(GridInputHandler *grid) {
    int input = GridInputHandler_GetNextInput(grid);
    GridInputHandler_SetNextLastReturnInputs(grid, 0, input, input);
}

static void BattleCastlePartyApp_OnPartyGridKeyMove(void *data, int newTarget, int prevTarget) {
    BattleCastlePartyApp_SetCursorPos(data, sBattleCastlePartyGridSlots[newTarget], sBattleCastlePartyGridSlots[prevTarget]);
}

static void BattleCastlePartyApp_OnPartyGridTouch(void *data, int newTarget, int prevTarget) {
    BattleCastlePartyApp_SetCursorPos(data, sBattleCastlePartyGridSlots[newTarget], sBattleCastlePartyGridSlots[prevTarget]);
}

static void BattleCastleOpponentApp_OnPartyGridKeyMove(void *data, int newTarget, int prevTarget) {
    BattleCastleOpponentApp_SetCursorPos(data, sBattleCastlePartyGridSlots[newTarget], sBattleCastlePartyGridSlots[prevTarget]);
}

static void BattleCastleOpponentApp_OnPartyGridTouch(void *data, int newTarget, int prevTarget) {
    BattleCastleOpponentApp_SetCursorPos(data, sBattleCastlePartyGridSlots[newTarget], sBattleCastlePartyGridSlots[prevTarget]);
}

GridInputHandler *BattleCastlePartyApp_CreateItemGrid(BattleCastlePartyApp *app) {
    GridInputHandler *grid = GridInputHandler_Create(sBattleCastleItemGridHitboxes, sBattleCastleItemGridBoxes, &sBattleCastleItemGridCallbacks, app, TRUE, 0, HEAP_ID_107);

    BattleCastlePartyApp_SetItemCursor(app, 64, 52, 3);
    BattleCastlePartyApp_ShowItemDescription(app, 0);
    return grid;
}

// Rental item grid input: an item button, 6/7 for the page arrows (left/right key or touch), or a GRID_MENU_* code
int BattleCastle_HandleItemGridInput(GridInputHandler *grid) {
    int touched;
    int input = GridInputHandler_HandleInput_NoHold(grid);

    switch (input) {
    case GRID_MENU_BUTTON_MODE:
    case GRID_MENU_CURSOR_MOVE:
    case GRID_MENU_CANCEL:
        return input;
    case GRID_MENU_NOTHING_CHOSEN:
        if (gSystem.newKeys & PAD_KEY_LEFT) {
            int next = GridInputHandler_GetNextInput(grid);
            if (next == 0 || next == 2 || next == 4) {
                return 6;
            }
        }
        if (gSystem.newKeys & PAD_KEY_RIGHT) {
            int next = GridInputHandler_GetNextInput(grid);
            if (next == 1 || next == 3 || next == 5) {
                return 7;
            }
        }
        break;
    default:
        return sBattleCastleItemGridButtons[input];
    }
    touched = TouchscreenHitbox_FindRectAtTouchNew(sBattleCastleItemGridArrowHitboxes);
    if (touched == 0) {
        return 6;
    }
    if (touched == 1) {
        return 7;
    }
    return -1;
}

static void BattleCastlePartyApp_OnItemGridKeyMove(void *data, int newTarget, int prevTarget) {
    BattleCastlePartyApp_SetItemCursor(data, sBattleCastleItemGridBoxes[newTarget].left, sBattleCastleItemGridBoxes[newTarget].top, sBattleCastleItemGridCursorAnims[newTarget]);
    BattleCastlePartyApp_ShowItemDescription(data, newTarget);
}

static void BattleCastlePartyApp_OnItemGridTouch(void *data, int newTarget, int prevTarget) {
    BattleCastlePartyApp_SetItemCursor(data, sBattleCastleItemGridBoxes[newTarget].left, sBattleCastleItemGridBoxes[newTarget].top, sBattleCastleItemGridCursorAnims[newTarget]);
    BattleCastlePartyApp_ShowItemDescription(data, newTarget);
}

TouchscreenListMenuSpawner *BattleCastle_CreateListMenuSpawner(SpriteList *spriteList, PaletteData *paletteData) {
    return TouchscreenListMenuSpawner_Create(HEAP_ID_107, paletteData);
}

void BattleCastle_DestroyListMenuSpawner(TouchscreenListMenuSpawner *spawner) {
    TouchscreenListMenuSpawner_Destroy(spawner);
}

TouchscreenListMenu *BattleCastle_CreateListMenu(TouchscreenListMenuSpawner *spawner, TouchscreenListMenuHeader *header, u8 x, u8 y, u8 width) {
    return TouchscreenListMenu_Create(spawner, header, FALSE, x, y, width, 0);
}

void BattleCastle_DestroyListMenu(TouchscreenListMenu *menu) {
    TouchscreenListMenu_Destroy(menu);
}

// A or B pressed, or the touch screen tapped
BOOL BattleCastle_IsAdvancePressed(void) {
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return TRUE;
    }
    return System_GetTouchNew();
}
