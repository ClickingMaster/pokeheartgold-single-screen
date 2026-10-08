#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct {
    u8 pad0[0x10];
    BgConfig *bgConfig;
    u8 pad14[0x214 - 0x14];
    u32 unk214;
    u32 unk218;
    u8 pad21C[0x250 - 0x21C];
    u8 cells[15][15];
} Dowsing;
extern const u16 ov28_0225EB14[];
extern const u16 ov28_0225EB32[];
u8 ov28_0225E51C(Dowsing *w, u16 x, u16 y);
void ov28_0225E578(Dowsing *w, int mode);
// @fn ov28_0225E578
void ov28_0225E578(Dowsing *w, int mode) {
    s16 x, y;
    s16 cx, cy;
    u8 target;
    if (mode == 1) {
        cx = (w->unk214 >> 3) - 3;
        cy = (w->unk218 >> 3) - 5;
    }
    for (y = 0; y < 15; y++) {
        for (x = 0; x < 15; x++) {
            if (ov28_0225EB14[y] & (1 << x)) {
                continue;
            }
            if (mode == 1) {
                if (x >= cx - 3 && x <= cx + 3 && y >= cy - 3 && y <= cy + 3 && abs(cx - x) + abs(cy - y) <= 4) {
                    target = ov28_0225E51C(w, x, y);
                    if (w->cells[y][x] < target) {
                        w->cells[y][x] += 10;
                        if (w->cells[y][x] > target) {
                            w->cells[y][x] = target;
                        }
                    } else {
                        w->cells[y][x] = target;
                    }
                } else {
                    if (w->cells[y][x] >= 2) {
                        w->cells[y][x] -= 2;
                    } else {
                        w->cells[y][x] = 0;
                    }
                }
            } else {
                if (w->cells[y][x] >= 2) {
                    w->cells[y][x] -= 2;
                } else {
                    w->cells[y][x] = 0;
                }
            }
            if (w->cells[y][x] == 0) {
                FillBgTilemapRect(w->bgConfig, 6, 0x1001, x + 3, y + 5, 1, 1, 17);
            } else {
                FillBgTilemapRect(w->bgConfig, 6, ov28_0225EB32[w->cells[y][x] / 10], x + 3, y + 5, 1, 1, 17);
            }
        }
    }
    ScheduleBgTilemapBufferTransfer(w->bgConfig, 6);
}
