#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 a, b;
} Rect;

typedef struct {
    u8 p[0x11c];
    u8 result, mode;
    s8 selected;
    u8 gap[2];
    u8 index, unused, flagA, flagB;
} Work;

extern Work *ov13_0224F4BC;
extern Rect ov13_02245958;
extern u32 ov13_02244FA4[];
extern u32 ov13_02244F68[];
extern u32 ov13_02244F94[];
extern u32 ov13_02244F84[];
extern u32 ov13_02244F7C[];
extern u32 ov13_02244F64[];
extern int ov13_02245D80[];
extern int WifiSetupInput_IsTouchPressed(Rect *);
extern void WifiSetup_RectFromPosSize(u32 *, u32 *, Rect *);
extern void ov13_0223ABB8(int);
void ov13_02228738(void);

void ov13_02228738(void) {
    Rect temp;
    int i;
    if (!WifiSetupInput_IsTouchPressed(&ov13_02245958)) {
        return;
    }
    ov13_0224F4BC->selected = -1;
    for (i = 0; i < 47; i++) {
        WifiSetup_RectFromPosSize(ov13_02244FA4 + i, ov13_02244F68, &temp);
        if (WifiSetupInput_IsTouchPressed(&temp)) {
            if (!ov13_0224F4BC->flagB) {
                ov13_0223ABB8(9);
                return;
            }
            ov13_0223ABB8(0);
            ov13_0224F4BC->selected = i;
            return;
        }
    }
    for (i = 0; i < 4; i++) {
        WifiSetup_RectFromPosSize(ov13_02244F84 + i, ov13_02244F94 + i, &temp);
        if (WifiSetupInput_IsTouchPressed(&temp)) {
            if ((i == 3 && !ov13_0224F4BC->flagA) || (i == 2 && !ov13_0224F4BC->flagB)) {
                ov13_0223ABB8(9);
                return;
            }
            ov13_0223ABB8(ov13_02245D80[i]);
            ov13_0224F4BC->selected = i + 47;
            return;
        }
    }
    {
        u32 *y, *x;
        int j;
        x = ov13_02244F7C;
        y = ov13_02244F64;
        for (j = 0; j < 2; j++, x++) {
            WifiSetup_RectFromPosSize(x, y, &temp);
            if (WifiSetupInput_IsTouchPressed(&temp)) {
                ov13_0223ABB8(0);
                ov13_0224F4BC->selected = j + 51;
                return;
            }
        }
    }
}
