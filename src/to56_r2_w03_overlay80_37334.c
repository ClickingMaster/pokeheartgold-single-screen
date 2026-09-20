#include "to56_r2_w03_overlay80_private.h"

void ov80_02237334(int a, int b, int c, u32 d, u8 e, u16 *f) {
    u16 start;
    int count;
    u16 *dst;
    u32 idx;
    u8 cl;

    count = 0;
    idx = (e & 0x7f) * 2;
    cl = ov80_022379C0(d);
    if (a == 0) {
        if (e + 1 + c * 10 == 50) {
            f[idx] = 0x133;
            return;
        }
        if (e + 1 + c * 10 == 170) {
            f[idx] = 0x134;
            return;
        }
    }
    dst = &f[idx];
    do {
        u16 r = (u32)LCRandom() % 300;
        start = r;
        int pos = idx + count;
        for (;;) {
            u16 cur = f[pos];
            if (cur == ov80_0223C738[r]) {
                int j;
                for (j = 0; j < pos; j++) {
                    if (r == f[j]) {
                        break;
                    }
                }
                if (j == pos) {
                    *dst = r;
                    dst++;
                    count++;
                    break;
                }
            }
            r = (u16)(r + 1);
            if (r >= 300) {
                r = 0;
            }
            if (r == start) {
                const u16 *tbl = ov80_0223C698;
                tbl += cl << 3;
                u16 v;
                do {
                    v = tbl[(u16)((s32)LCRandom() % 8)];
                } while (cur == v);
                f[pos] = v;
            }
        }
    } while (count < b);
}
