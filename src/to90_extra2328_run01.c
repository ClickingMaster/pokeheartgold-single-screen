#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[8];
    u16 *tiles;
    u8 q[8];
    void *sprite;
} Work;

typedef struct {
    u8 p[4];
    u16 position;
    u16 q;
    Work *work;
} Control;

extern s32 FX_DivS32(s32, s32);
extern Control ov13_0224F54C;
extern u16 ov13_02245536[];
extern void WifiSetupText_Clear(void *, int);
extern void ov13_02233E8C(int, int);
extern void ov13_022392CC(void *, int, int, int);
extern void ov13_02234028(int, int);
extern void ov13_022392B4(void);
extern void WifiSetupText_MarkDirty(void *);
extern void ov13_022344F0(void);
void ov13_02233DCC(void);
#pragma force_active on

void ov13_02233DCC(void) {
    int base = FX_DivS32(ov13_0224F54C.position, 29);
    int cursor, n;
    WifiSetupText_Clear(ov13_0224F54C.work->sprite, 0);
    cursor = base;
    n = 0;
    for (; n < 5; n++, cursor++) {
        ov13_02233E8C(cursor, n);
    }
    ov13_022392CC(ov13_0224F54C.work->tiles + ov13_02245536[base], 0, 30, 19);
    for (cursor = 0; cursor < 5; cursor++, base++) {
        ov13_02234028(base, cursor);
    }
    ov13_022392B4();
    WifiSetupText_MarkDirty(ov13_0224F54C.work->sprite);
    ov13_022344F0();
}
