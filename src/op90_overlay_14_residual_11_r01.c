#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct { u8 prefix[0x2f0]; BgConfig *bg; u8 gap[0x40C0 - 0x2f4]; u32 bits; } Inner;
typedef struct { u8 prefix[0x34]; Inner *inner; } Work;
u16 *sub_02019B08(BgConfig *, int);
void ov14_021E7D9C(Work *w, u32 index);
// @fn ov14_021E7D9C
void ov14_021E7D9C(Work *w, u32 index) {
    u16 *map = sub_02019B08(w->inner->bg, 2);
    u8 row = (index / 2) * 3 + 2;
    u8 col = (index % 2) * 4 + 3;
    u16 *p = map + row * 11;
    u16 *q = p + col;
    if (!(w->inner->bits & (1 << index))) {
        index += 11;
    } else {
        index += 43;
    }
    index = (u16)index;
    *q = (*q & 0xF000) + index;
}
