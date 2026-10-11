#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern u8 *ov13_0224F4C0;
extern u8 ov13_02245958[];
extern u8 ov13_0224527A[];
extern u8 ov13_02245228[];
extern u8 ov13_02245314[];
extern u8 ov13_02245250[];
extern u8 ov13_02245230[];
extern u8 ov13_02245224[];
extern u8 ov13_02245258[];
extern u8 ov13_02245234[];
extern u8 ov13_02245220[];
int WifiSetupInput_IsTouchReleased(const void *);
void WifiSetup_RectFromPosSize(const void *, const void *, void *);
void ov13_0222AE24(void);
void ov13_0222A554(void);

void ov13_0222A554(void) {
    u8 text[8];
    int i;
    u8 *entry;
    ov13_0224F4C0[0x60] = 0;
    if (!WifiSetupInput_IsTouchReleased(ov13_02245958)) {
        return;
    }
    entry = ov13_0224527A;
    for (i = 0; i < 10; i++, entry += 4) {
        WifiSetup_RectFromPosSize(entry, ov13_02245228, text);
        if (WifiSetupInput_IsTouchReleased(text)) {
            if (*(s8 *)(ov13_0224F4C0 + 0x61) != i) {
                return;
            }
            ov13_0224F4C0[0x60] = ov13_02245314[i];
            ov13_0224F4C0[0x63] = i;
            ov13_0222AE24();
            return;
        }
    }
    entry = ov13_02245250;
    for (i = 0; i < 2; i++, entry += 4) {
        WifiSetup_RectFromPosSize(entry, ov13_02245230, text);
        if (WifiSetupInput_IsTouchReleased(text)) {
            if (*(s8 *)(ov13_0224F4C0 + 0x61) != i + 10) {
                return;
            }
            ov13_0224F4C0[0x60] = ov13_02245224[i];
            ov13_0224F4C0[0x63] = i + 10;
            ov13_0222AE24();
            return;
        }
    }
    entry = ov13_02245258;
    for (i = 0; i < 2; i++, entry += 4) {
        WifiSetup_RectFromPosSize(entry, ov13_02245234, text);
        if (WifiSetupInput_IsTouchReleased(text)) {
            if (*(s8 *)(ov13_0224F4C0 + 0x61) != i + 12) {
                return;
            }
            ov13_0224F4C0[0x60] = ov13_02245220[i];
            ov13_0224F4C0[0x63] = i + 12;
            ov13_0222AE24();
            return;
        }
    }
}
