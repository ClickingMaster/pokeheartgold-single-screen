#pragma force_active on
#include "global.h"
typedef struct { u8 x, y; } Point;
extern const Point *ov112_021FF504[];
u8 ov112_021E98F8(const u8 *, int, int);
void ov112_021E9934(u8 *, int, int, int);
void ov112_021E9998(u8 *dst, const u8 *src, u32 shape, int x, int y);

// @fn ov112_021E9998
void ov112_021E9998(u8 *dst, const u8 *src, u32 shape, int x, int y) {
    int i;
    for (i = 0; i < 4; i++) {
        const Point *pts = ov112_021FF504[i];
        u8 j = 0;
        if (pts[0].x != 0xFF) {
            do {
                int px = (shape & 0xF) - 8 + (x + pts[j].x);
                int py = y + pts[j].y + ((shape & 0xF0) >> 4) - 8;
                if (ov112_021E98F8(src, px, py) == 2) {
                    ov112_021E9934(dst, px, py, 1);
                }
                j++;
            } while (pts[j].x != 0xFF);
        }
        shape >>= 8;
    }
}
