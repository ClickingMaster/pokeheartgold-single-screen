#pragma force_active on
#include "global.h"
#include "bg_window.h"

typedef struct Root {
    u8 pad[0x27];
    u8 col[5];
    u8 row[5];
} Root;

typedef struct Work {
    Root *root;
    u8 pad[0x28];
    Window *windows;
    u8 pad2[0x114D - 0x30];
    u8 sel;
} Work;

void BattleBagMenu_PrintUseItemName(Work *, u32);
void BattleBagMenu_PrintItemQuantity(Work *, u32, u32, u32, u32, u32, u32);
void BattleBagMenu_PrintItemDescription(Work *, u32);
void BattleBagMenu_PrintCenteredText(Work *, u32, u32, u32, u32, u32);
void BattleBagMenu_PrintUseItemPage(Work *work);

// @fn BattleBagMenu_PrintUseItemPage
void BattleBagMenu_PrintUseItemPage(Work *work) {
    u32 i;
    u8 col, row;
    for (i = 0; i < 4; i++) {
        FillWindowPixelBuffer(&work->windows[i], 0);
    }
    col = work->root->col[work->sel];
    row = work->root->row[work->sel];
    BattleBagMenu_PrintUseItemName(work, col + row * 6);
    BattleBagMenu_PrintItemQuantity(work, col + row * 6, 0, 1, 0, 0, 0x10200);
    BattleBagMenu_PrintItemDescription(work, col + row * 6);
    BattleBagMenu_PrintCenteredText(work, 3, 31, 4, 5, 0x30201);
}
