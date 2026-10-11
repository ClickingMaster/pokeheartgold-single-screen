#pragma force_active on
#include "global.h"

#include "wifi_setup_keypad.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Numeric keypad: key sizes and row placement. Compiled without -ipa file (see Makefile).

typedef struct RowCounts {
    u8 counts[5];
} RowCounts;

const WifiSetupPoint ov13_02245230 = { 0x1C, 0x14 }; // extra key size
const WifiSetupPoint ov13_02245228 = { 0x1C, 0x14 }; // digit key size
static const WifiSetupPoint sUnused = { 0x0C, 0x04 };
const WifiSetupPoint ov13_02245234 = { 0x78, 0x12 }; // button size

// number of digit keys, extra keys and buttons in each of the five rows (row 4: buttons)
static const RowCounts sDigitKeysPerRow = {
    { 3, 3, 3, 1, 0 }
};
static const RowCounts sButtonsPerRow = {
    { 0, 0, 0, 0, 2 }
};
static const RowCounts sExtraKeysPerRow = {
    { 0, 0, 0, 2, 0 }
};
// position of the key labels in their row canvas (read by WifiSetupKeypad_Create)
static const u16 sLabelBoxPos[4] = { 0, 1, 0, 0 };

static inline u32 OamPos(u32 attr, int x, int y) {
    return (attr & 0xFE00FF00) | (y & 0xff) | ((x & 0x1ff) << 16);
}

// Moves every object of row `row` (0-3: key rows, 4: buttons) to height y.
void WifiSetupKeypad_SetRowY(int row, int y) {
    RowCounts digitKeys = sDigitKeysPerRow;
    RowCounts extraKeys = sExtraKeysPerRow;
    RowCounts buttons = sButtonsPerRow;
    int key;
    int i;
    int start = row * 3;
    key = start;
    for (i = 0; i < digitKeys.counts[row]; i++) {
        int x = ov13_0224527A[key].x;
        gWifiSetupKeypad->keys[key]->attr01 &= 0xC1FFFCFF;
        gWifiSetupKeypad->keys[key]->attr01 = OamPos(gWifiSetupKeypad->keys[key]->attr01, x, y);
        key++;
    }
    if (row < 4) {
        WifiSetupText_ShowObjCanvas(gWifiSetupKeypad->rows[row], ov13_0224527A[start].x, y, gWifiSetupKeypad->rowBlocks[row], 2);
    }
    for (i = 0; i < extraKeys.counts[row]; i++) {
        int x = ov13_02245250[i].x;
        gWifiSetupKeypad->extraKeys[i]->attr01 &= 0xC1FFFCFF;
        gWifiSetupKeypad->extraKeys[i]->attr01 = OamPos(gWifiSetupKeypad->extraKeys[i]->attr01, x, y);
    }
    for (i = 0; i < buttons.counts[row]; i++) {
        WifiSetupOamBlock_SetAffineMode(gWifiSetupKeypad->buttons[i], -1, 0, 0);
        WifiSetupOamBlock_SetPosition(gWifiSetupKeypad->buttons[i], -1, ov13_02245258[i].x, y);
    }
}
