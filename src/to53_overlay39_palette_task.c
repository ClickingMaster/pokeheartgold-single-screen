#include "to53_overlay39_palette_task_private.h"

void ov39_022287F8(void *task, void *p) {
    if (U32(p, 4) != 0) {
        U8(p, 0x32B) ^= 1;
        if (!(U8(p, 0x32B) & 1)) {
            if (U32(p, 0x32C) <= 1) {
                GX_LoadBGPltt((u8 *)p + 0x88 + S16(p, 0x328) * 0x20, 0, 0x20);
            }
            if (U32(p, 0x32C) == 0 || U32(p, 0x32C) == 2) {
                GXS_LoadBGPltt((u8 *)p + 0x88 + S16(p, 0x328) * 0x20, 0, 0x20);
            }
            if (S8(p, 0x32A) == 0) {
                S16(p, 0x328)
                ++;
                if (S16(p, 0x328) >= 0x15) {
                    S16(p, 0x328) = 0x13;
                    S8(p, 0x32A) ^= 1;
                }
            } else {
                S16(p, 0x328)
                --;
                if (S16(p, 0x328) < 0) {
                    S16(p, 0x328) = 1;
                    S8(p, 0x32A) ^= 1;
                }
            }
        }
    }
}

void ov39_022288A0(void *p) {
    AddWindowParameterized(PTR(p, 4), (u8 *)p + 0x64, 0, 4, 4, 0x17, 0x10, 0xD, 0x94);
    FillWindowPixelBuffer((u8 *)p + 0x64, 0);
    AddWindowParameterized(PTR(p, 4), (u8 *)p + 0x54, 0, 4, 1, 0x18, 2, 0xD, 0x204);
    FillWindowPixelBuffer((u8 *)p + 0x54, 0);
    ov39_0222899C((u8 *)p + 0x54, PTR(p, 0x38), 0, 1, 1, 0xF0E00);
    AddWindowParameterized(PTR(p, 4), (u8 *)p + 0x44, 0, 2, 0x13, 0x1B, 4, 0xD, 0x28);
    FillWindowPixelBuffer((u8 *)p + 0x44, 0);
}
