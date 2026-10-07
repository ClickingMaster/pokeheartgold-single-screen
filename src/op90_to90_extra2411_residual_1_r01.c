#pragma force_active on
#include "global.h"
int ov112_021E935C(u8 *data, int width, int x, int y);
int ov112_021E93BC(u8 *src, int width, int unused, u8 *dst, int x, int y, int w, int h);

// @fn ov112_021E93BC
int ov112_021E93BC(u8 *src, int width, int unused, u8 *dst, int x, int y, int w, int h) {
    int i, j, k;
    int pos = 0;

    MIi_CpuClearFast(0, (u32 *)dst, (w * h) / 8 * 2);

    for (i = 0; i < h / 8; i++) {
        for (j = 0; j < w; j++) {
            u8 *out = dst + pos;
            for (k = 0; k < 8; k++) {
                int v = ov112_021E935C(src, width, j + x, y + k + i * 8);
                out[0] |= (v >> 1) << k;
                out[1] |= (v & 1) << k;
            }
            pos += 2;
        }
    }
    return pos;
}
