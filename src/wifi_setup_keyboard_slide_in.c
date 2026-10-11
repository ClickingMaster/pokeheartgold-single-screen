#pragma force_active on
#include "global.h"

#include "wifi_setup_keyboard.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Software keyboard, first part of the slide-in animation (compiled with MWCC 2.0, see Makefile):
// each task raises one key row by 12 pixels per frame until it reaches its place, then starts the
// next row from the bottom of the screen.

typedef struct RowPosition {
    int y;
    int x;
} RowPosition;

void WifiSetupKeyboard_SlideInRow0Task(void *task) {
    RowPosition pos;
    RowPosition *p = &pos;
    u32 word = gWifiSetupKeyboard->keys[0]->attr01;
    p->x = (word & 0x1ff0000) >> 16;
    p->y = (word & 0xff) - 12;
    if (p->y > 81) {
        WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 0, p->y);
        return;
    }
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 0, 81);
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 1, 192);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideInRow1Task);
}

void WifiSetupKeyboard_SlideInRow1Task(void *task) {
    RowPosition pos;
    RowPosition *p = &pos;
    u32 word = gWifiSetupKeyboard->keys[12]->attr01;
    p->x = (word & 0x1ff0000) >> 16;
    p->y = (word & 0xff) - 12;
    if (p->y > 103) {
        WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 1, p->y);
        return;
    }
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 1, 103);
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 2, 192);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideInRow2Task);
}

void WifiSetupKeyboard_SlideInRow2Task(void *task) {
    RowPosition pos;
    RowPosition *p = &pos;
    u32 word = gWifiSetupKeyboard->keys[24]->attr01;
    p->x = (word & 0x1ff0000) >> 16;
    p->y = (word & 0xff) - 12;
    if (p->y > 125) {
        WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 2, p->y);
        return;
    }
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 2, 125);
    WifiSetupKeyboard_SetRowY(gWifiSetupKeyboard->page, 3, 192);
    WifiSetupTask_SetFunc(task, (WifiSetupTaskFunc)WifiSetupKeyboard_SlideInRow3Task);
}
