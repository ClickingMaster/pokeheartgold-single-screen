#pragma force_active on
#include "global.h"

#include "wifi_setup_keyboard.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Software keyboard: key sizes and row placement. Compiled without -ipa file (see Makefile).

typedef struct RowCounts {
    u8 counts[5];
} RowCounts;

const WifiSetupPoint ov13_02244F60 = { 0x1C, 0x02 };
const WifiSetupPoint ov13_02244F64 = { 120, 18 }; // button size
const WifiSetupPoint ov13_02244F68 = { 17, 18 };  // character key size

// number of buttons, character keys and extra keys in each of the five rows (row 4: buttons)
static const RowCounts sButtonsPerRow = {
    { 0, 0, 0, 0, 2 }
};
static const RowCounts sCharKeysPerRow = {
    { 12, 12, 12, 11, 0 }
};
static const RowCounts sExtraKeysPerRow = {
    { 1, 1, 1, 1, 0 }
};

static inline u32 OamPos(u32 attr, int x, int y) {
    return (attr & 0xFE00FF00) | (y & 0xff) | ((x & 0x1ff) << 16);
}

// Moves every object of row `row` (0-3: key rows, 4: buttons) to height y; the row's text is
// taken from page `page`.
void WifiSetupKeyboard_SetRowY(int page, int row, int y) {
    RowCounts charKeys = sCharKeysPerRow;
    RowCounts extraKeys = sExtraKeysPerRow;
    RowCounts buttons = sButtonsPerRow;
    int key;
    int i;
    int start = row * 12;
    int extra;
    key = start;
    for (i = 0; i < charKeys.counts[row]; i++) {
        int x = gWifiSetupKeyboardKeyPositions[key].x;
        gWifiSetupKeyboard->keys[key]->attr01 &= 0xC1FFFCFF;
        gWifiSetupKeyboard->keys[key]->attr01 = OamPos(gWifiSetupKeyboard->keys[key]->attr01, x, y);
        key++;
    }
    if (row < 4) {
        WifiSetupText_ShowObjCanvas(gWifiSetupKeyboard->rows[page][row], gWifiSetupKeyboardKeyPositions[start].x, y, gWifiSetupKeyboard->rowBlocks[row], 2);
    }
    extra = FX_ModS32(row + 3, 4);
    for (i = 0; i < extraKeys.counts[row]; i++) {
        int x = gWifiSetupKeyboardExtraKeyPositions[extra].x;
        gWifiSetupKeyboard->extraKeys[extra]->attr01 &= 0xC1FFFCFF;
        gWifiSetupKeyboard->extraKeys[extra]->attr01 = OamPos(gWifiSetupKeyboard->extraKeys[extra]->attr01, x, y);
    }
    for (i = 0; i < buttons.counts[row]; i++) {
        WifiSetupOamBlock_SetAffineMode(gWifiSetupKeyboard->buttons[i], -1, 0, 0);
        WifiSetupOamBlock_SetPosition(gWifiSetupKeyboard->buttons[i], -1, gWifiSetupKeyboardButtonPositions[i].x, y);
    }
}
