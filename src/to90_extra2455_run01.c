#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u16 x, y;
} Pos16;

typedef struct {
    u16 x1, y1, x2, y2;
} Rect16;

typedef struct {
    u8 p[0x61];
    s8 selected;
    u8 pad[4];
    u8 a, b, c;
} w31693_State;

extern w31693_State *ov13_0224F4C0;
extern Rect16 ov13_02245958;
extern Pos16 ov13_0224527A[];
extern Pos16 ov13_02245228;
extern Pos16 ov13_02245250[];
extern Pos16 ov13_02245230;
extern Pos16 ov13_02245258[];
extern Pos16 ov13_02245234;
extern int WifiSetupInput_IsTouchPressed(const void *);
extern void WifiSetup_RectFromPosSize(const void *, const void *, void *);
extern void ov13_0223ABB8(int);
void ov13_0222A378(void);

void ov13_0222A378(void) {
    Rect16 record;
    int i;
    Pos16 *cursor;
    if (!WifiSetupInput_IsTouchPressed(&ov13_02245958)) {
        return;
    }
    ov13_0224F4C0->selected = -1;
    cursor = ov13_0224527A;
    for (i = 0; i < 10; i++, cursor++) {
        WifiSetup_RectFromPosSize(cursor, &ov13_02245228, &record);
        if (WifiSetupInput_IsTouchPressed(&record)) {
            if (!ov13_0224F4C0->b) {
                ov13_0223ABB8(9);
                return;
            }
            ov13_0223ABB8(0);
            ov13_0224F4C0->selected = i;
            return;
        }
    }
    cursor = ov13_02245250;
    for (i = 0; i < 2; i++, cursor++) {
        WifiSetup_RectFromPosSize(cursor, &ov13_02245230, &record);
        if (WifiSetupInput_IsTouchPressed(&record)) {
            if ((i == 0 && !ov13_0224F4C0->a) || (i == 1 && !ov13_0224F4C0->c)) {
                ov13_0223ABB8(9);
                return;
            }
            ov13_0223ABB8(0);
            ov13_0224F4C0->selected = i + 10;
            return;
        }
    }
    cursor = ov13_02245258;
    for (i = 0; i < 2; i++, cursor++) {
        WifiSetup_RectFromPosSize(cursor, &ov13_02245234, &record);
        if (WifiSetupInput_IsTouchPressed(&record)) {
            ov13_0223ABB8(0);
            ov13_0224F4C0->selected = i + 12;
            return;
        }
    }
}
