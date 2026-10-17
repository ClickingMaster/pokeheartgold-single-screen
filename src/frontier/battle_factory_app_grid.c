#include "global.h"

#include "frontier/battle_factory_app_internal.h"

#include "grid_input_handler.h"

static void BattleFactory_GridNop(void *data, int input, int prevInput);
static void BattleFactoryApp_OnRentalCursorMove(void *data, int input, int prevInput);
static void BattleFactoryApp_OnSwapOutCursorMove(void *data, int input, int prevInput);
static void BattleFactoryApp_OnSwapInCursorMove(void *data, int input, int prevInput);

static const GridCallbacks sBattleFactoryRentalGridCallbacks = {
    .onButton = BattleFactory_GridNop,
    .onSwitchToTouchMode = BattleFactory_GridNop,
    .onKeyMove = BattleFactoryApp_OnRentalCursorMove,
    .onTouch = BattleFactoryApp_OnRentalCursorMove,
};

static const GridCallbacks sBattleFactorySwapInGridCallbacks = {
    .onButton = BattleFactory_GridNop,
    .onSwitchToTouchMode = BattleFactory_GridNop,
    .onKeyMove = BattleFactoryApp_OnSwapInCursorMove,
    .onTouch = BattleFactoryApp_OnSwapInCursorMove,
};

static const GridCallbacks sBattleFactorySwapOutGridCallbacks = {
    .onButton = BattleFactory_GridNop,
    .onSwitchToTouchMode = BattleFactory_GridNop,
    .onKeyMove = BattleFactoryApp_OnSwapOutCursorMove,
    .onTouch = BattleFactoryApp_OnSwapOutCursorMove,
};

static const TouchscreenHitbox sBattleFactorySwapOutGridHitboxes[] = {
    { .rect = { 44, 83, 44, 83 } },
    { .rect = { 44, 83, 100, 139 } },
    { .rect = { 44, 83, 156, 195 } },
    { .rect = { 116, 139, 180, 243 } },
    { .rect = { 92, 115, 12, 115 } },
    { .rect = { 92, 115, 140, 243 } },
    { .rect = { 116, 139, 76, 179 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattleFactoryLinkSwapOutGridHitboxes[] = {
    { .rect = { 44, 83, 76, 115 } },
    { .rect = { 44, 83, 132, 171 } },
    { .rect = { 0, 0, 0, 0 } },
    { .rect = { 116, 139, 180, 243 } },
    { .rect = { 92, 115, 12, 115 } },
    { .rect = { 92, 115, 140, 243 } },
    { .rect = { 116, 139, 76, 179 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattleFactorySwapInGridHitboxesA[] = {
    { .rect = { 44, 83, 20, 59 } },
    { .rect = { 44, 83, 76, 115 } },
    { .rect = { 44, 83, 132, 171 } },
    { .rect = { 44, 83, 188, 227 } },
    { .rect = { 116, 139, 12, 75 } },
    { .rect = { 116, 139, 180, 243 } },
    { .rect = { 116, 139, 12, 115 } },
    { .rect = { 116, 140, 140, 243 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattleFactorySwapInGridHitboxesB[] = {
    { .rect = { 44, 83, 44, 83 } },
    { .rect = { 44, 83, 100, 139 } },
    { .rect = { 44, 83, 156, 195 } },
    { .rect = { 0, 0, 0, 0 } },
    { .rect = { 116, 139, 12, 75 } },
    { .rect = { 116, 139, 180, 243 } },
    { .rect = { 116, 139, 12, 115 } },
    { .rect = { 116, 140, 140, 243 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const TouchscreenHitbox sBattleFactoryRentalGridHitboxes[] = {
    { .rect = { 44, 83, 4, 43 } },
    { .rect = { 44, 83, 44, 83 } },
    { .rect = { 44, 83, 84, 123 } },
    { .rect = { 44, 83, 124, 163 } },
    { .rect = { 44, 83, 164, 203 } },
    { .rect = { 44, 83, 204, 243 } },
    { .rect = { 92, 115, 12, 115 } },
    { .rect = { 92, 115, 140, 243 } },
    { .rect = { 116, 139, 76, 179 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const DpadMenuBox sBattleFactoryLinkSwapOutGridBoxes[] = {
    { 96,  64,  0, 0, 3,   3,   3, 1 },
    { 152, 64,  0, 0, 3,   3,   0, 3 },
    { 0,   0,   0, 0, 2,   2,   2, 2 },
    { 212, 128, 0, 0, 128, 128, 1, 0 },
    { 64,  104, 0, 0, 4,   6,   4, 5 },
    { 192, 104, 0, 0, 5,   6,   4, 5 },
    { 128, 128, 0, 0, 132, 6,   6, 6 },
};

static const DpadMenuBox sBattleFactorySwapOutGridBoxes[] = {
    { 64,  64,  0, 0, 3,   3,   3, 1 },
    { 120, 64,  0, 0, 3,   3,   0, 2 },
    { 176, 64,  0, 0, 3,   3,   1, 3 },
    { 212, 128, 0, 0, 128, 128, 2, 0 },
    { 64,  104, 0, 0, 4,   6,   4, 5 },
    { 192, 104, 0, 0, 5,   6,   4, 5 },
    { 128, 128, 0, 0, 132, 6,   6, 6 },
};

static const DpadMenuBox sBattleFactorySwapInGridBoxesA[] = {
    { 40,  64,  0, 0, 4, 4, 5, 1 },
    { 96,  64,  0, 0, 4, 4, 0, 2 },
    { 152, 64,  0, 0, 4, 4, 1, 3 },
    { 208, 64,  0, 0, 4, 4, 2, 4 },
    { 44,  128, 0, 0, 0, 0, 3, 5 },
    { 212, 128, 0, 0, 0, 0, 4, 0 },
    { 64,  128, 0, 0, 6, 6, 6, 7 },
    { 192, 128, 0, 0, 7, 7, 6, 7 },
};

static const DpadMenuBox sBattleFactorySwapInGridBoxesB[] = {
    { 64,  64,  0, 0, 4, 4, 5, 1 },
    { 120, 64,  0, 0, 4, 4, 0, 2 },
    { 176, 64,  0, 0, 4, 4, 1, 4 },
    { 0,   0,   0, 0, 3, 3, 3, 3 },
    { 44,  128, 0, 0, 0, 0, 2, 5 },
    { 212, 128, 0, 0, 0, 0, 4, 0 },
    { 64,  128, 0, 0, 6, 6, 6, 7 },
    { 192, 128, 0, 0, 7, 7, 6, 7 },
};

static const DpadMenuBox sBattleFactoryRentalGridBoxes[] = {
    { 24,  64,  0, 0, 0,   0, 5, 1 },
    { 64,  64,  0, 0, 1,   1, 0, 2 },
    { 104, 64,  0, 0, 2,   2, 1, 3 },
    { 144, 64,  0, 0, 3,   3, 2, 4 },
    { 184, 64,  0, 0, 4,   4, 3, 5 },
    { 224, 64,  0, 0, 5,   5, 4, 0 },
    { 64,  104, 0, 0, 6,   8, 6, 7 },
    { 192, 104, 0, 0, 7,   8, 6, 7 },
    { 128, 128, 0, 0, 134, 8, 8, 8 },
};

void BattleFactory_FreeGrid(GridInputHandler *grid) {
    GridInputHandler_Free(grid);
}

void BattleFactory_GetGridCursorPos(GridInputHandler *grid, int target, u32 *x, u32 *y) {
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(grid, target);
    *x = box->left;
    *y = box->top;
}

static void BattleFactory_GridNop(void *data, int input, int prevInput) {
}

GridInputHandler *BattleFactoryApp_CreateRentalGrid(BattleFactoryApp *app, int cursor) {
    return GridInputHandler_Create(sBattleFactoryRentalGridHitboxes, sBattleFactoryRentalGridBoxes, &sBattleFactoryRentalGridCallbacks, app, TRUE, (u8)cursor, HEAP_ID_100);
}

void BattleFactory_SetRentalGridMenu(GridInputHandler *grid, int menuOpen) {
    if (menuOpen == TRUE) {
        GridInputHandler_SetAllEnabled(grid);
    } else {
        GridInputHandler_ClearEnabledFlag(grid, 6);
        GridInputHandler_ClearEnabledFlag(grid, 7);
        GridInputHandler_ClearEnabledFlag(grid, 8);
    }
}

static void BattleFactoryApp_OnRentalCursorMove(void *data, int input, int prevInput) {
    BattleFactoryApp *app = data;

    if (input >= 6) {
        BattleFactoryApp_MoveButtonCursor(app, input);
    } else {
        BattleFactoryApp_MoveRentalCursor(app, input);
    }
}

GridInputHandler *BattleFactoryApp_CreateSwapOutGrid(BattleFactoryApp *app, int cursor) {
    return GridInputHandler_Create(sBattleFactorySwapOutGridHitboxes, sBattleFactorySwapOutGridBoxes, &sBattleFactorySwapOutGridCallbacks, app, TRUE, (u8)cursor, HEAP_ID_100);
}

GridInputHandler *BattleFactoryApp_CreateLinkSwapOutGrid(BattleFactoryApp *app, int cursor) {
    GridInputHandler *grid = GridInputHandler_Create(sBattleFactoryLinkSwapOutGridHitboxes, sBattleFactoryLinkSwapOutGridBoxes, &sBattleFactorySwapOutGridCallbacks, app, TRUE, (u8)cursor, HEAP_ID_100);
    GridInputHandler_ClearEnabledFlag(grid, 2);
    return grid;
}

void BattleFactory_SetSwapOutGridMenu(GridInputHandler *grid, int menuOpen, int frontierType) {
    if (menuOpen == TRUE) {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 3);
    } else {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 4);
        GridInputHandler_ClearEnabledFlag(grid, 5);
        GridInputHandler_ClearEnabledFlag(grid, 6);
    }
    if (BattleFactory_IsLinkMode(frontierType) == TRUE) {
        GridInputHandler_ClearEnabledFlag(grid, 2);
    }
}

static void BattleFactoryApp_OnSwapOutCursorMove(void *data, int input, int prevInput) {
    BattleFactoryApp *app = data;

    if (input >= 4) {
        BattleFactoryApp_MoveButtonCursor(app, input);
    } else {
        BattleFactoryApp_MoveSwapOutCursor(app, input);
    }
}

GridInputHandler *BattleFactoryApp_RecreateSwapOutGrid(BattleFactoryApp *app, GridInputHandler *grid) {
    BattleFactory_FreeGrid(grid);
    if (BattleFactoryApp_IsLinkMode(app) == TRUE) {
        return BattleFactoryApp_CreateLinkSwapOutGrid(app, 0);
    } else {
        return BattleFactoryApp_CreateSwapOutGrid(app, 0);
    }
}

GridInputHandler *BattleFactoryApp_CreateSwapInGrid(BattleFactoryApp *app, GridInputHandler *grid) {
    BattleFactory_FreeGrid(grid);
    if (BattleFactoryApp_IsLinkMode(app) == TRUE) {
        return GridInputHandler_Create(sBattleFactorySwapInGridHitboxesA, sBattleFactorySwapInGridBoxesA, &sBattleFactorySwapInGridCallbacks, app, TRUE, 0, HEAP_ID_100);
    }
    return GridInputHandler_Create(sBattleFactorySwapInGridHitboxesB, sBattleFactorySwapInGridBoxesB, &sBattleFactorySwapInGridCallbacks, app, TRUE, 0, HEAP_ID_100);
}

void BattleFactory_SetSwapInGridMenu(GridInputHandler *grid, int menuOpen, int frontierType) {
    if (menuOpen == TRUE) {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 4);
        GridInputHandler_ClearEnabledFlag(grid, 5);
    } else {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 6);
        GridInputHandler_ClearEnabledFlag(grid, 7);
    }
    if (BattleFactory_IsLinkMode(frontierType) == FALSE) {
        GridInputHandler_ClearEnabledFlag(grid, 3);
    }
}

static void BattleFactoryApp_OnSwapInCursorMove(void *data, int input, int prevInput) {
    BattleFactoryApp *app = data;

    if (input >= 6) {
        BattleFactoryApp_MoveButtonCursor(app, input);
    } else {
        BattleFactoryApp_MoveSwapInCursor(app, input);
    }
}
