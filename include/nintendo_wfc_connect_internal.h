#ifndef POKEHEARTGOLD_NINTENDO_WFC_CONNECT_INTERNAL_H
#define POKEHEARTGOLD_NINTENDO_WFC_CONNECT_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "sys_task_api.h"

enum NintendoWfcPaletteScreens {
    NINTENDO_WFC_PALETTE_BOTH_SCREENS,
    NINTENDO_WFC_PALETTE_MAIN_SCREEN,
    NINTENDO_WFC_PALETTE_SUB_SCREEN,
};

// Background palette cycling (VBlank task).
typedef struct NintendoWfcPaletteCycle {
    SysTask *task;
    int active;
    u16 basePltt[64];
    u16 cyclePltt[21 * 16];
    s16 index;
    s8 direction;
    u8 tick;
    u32 screens; // 0: both, 1: main only, 2: sub only
} NintendoWfcPaletteCycle;

// Touch YES/NO menu on the sub screen (texts from message bank 191).
typedef struct NintendoWfcYesNoMenu {
    BgConfig *bgConfig;
    Sprite *cursorSprite;
    Window yesWindow;
    Window noWindow;
    enum HeapID heapID;
    u8 initialChoice;
    u8 state; // index into sNintendoWfcYesNoStates
    u8 bgPltt;
    u8 bgLayer;
    u8 fontPltt;
    u8 baseTile;
    u8 textBgLayer;
    u8 choice; // 0: yes, 1: no
    int cursorOffsetY;
} NintendoWfcYesNoMenu;

void NintendoWfcYesNo_MoveCursor(NintendoWfcYesNoMenu *, int);

#endif // POKEHEARTGOLD_NINTENDO_WFC_CONNECT_INTERNAL_H
