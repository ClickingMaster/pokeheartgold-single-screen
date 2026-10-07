#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u32 unk0;
    u32 *oam;
    u8 pad8[0xc];
    u8 sel;
} Work2440;
extern Work2440 *ov13_0224F544;
extern u8 ov13_022454C0[];
void ov13_02232440(void);
static inline void SetOamPos(u32 *oam, int x, int y) {
    *oam = (*oam & 0xFE00FF00) | ((u32)(y & 0xff)) | ((u32)(x & 0x1ff) << 16);
}

// @fn ov13_02232440
void ov13_02232440(void) {
    int n = ov13_0224F544->sel;
    if (n > 3) {
        n = 3;
    }
    SetOamPos(ov13_0224F544->oam, ov13_022454C0[n * 3 + 2], 0x28);
}
