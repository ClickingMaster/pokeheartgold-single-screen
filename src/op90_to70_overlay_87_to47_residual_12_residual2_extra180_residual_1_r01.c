#pragma force_active on
#include "global.h"
typedef struct State {
    u8 pad0[0x3A1];
    u8 count;
    u8 list[3];
    u8 flags[0x55];
    union { u8 grid[160][240]; u8 g[160 * 240]; };
} State;
extern const s16 ov87_021E82C0[];
extern const s16 ov87_021E82C2[];
void PlaySE(int);
void ov87_021E7590(State *, int, int);
int ov87_021E75E0(State *, u8);
// @fn ov87_021E7590
void ov87_021E7590(State *s, int x, int y) {
    int dy, dx;
    int yy, xx;
    for (dy = -3; dy < 3; dy++) {
        for (dx = -3; dx < 3; dx++) {
            xx = x + dx;
            yy = y + dy;
            if (xx > 0 && xx < 240 && yy > 0 && yy < 160) {
                s->grid[yy][x + dx] = 1;
            }
        }
    }
}
// @fn ov87_021E75E0
int ov87_021E75E0(State *s, u8 idx) {
    int x;
    int y;
    int count;
    int x0;
    int y0;
    count = 0;
    x0 = ov87_021E82C0[idx * 2];
    y0 = ov87_021E82C2[idx * 2];
    for (y = y0; y < y0 + 20; y++) {
        for (x = x0; x < x0 + 25; x++) {
            if (s->g[y * 240 + x] == 1) {
                count++;
            }
        }
    }
    if (count >= 380) {
        if (s->flags[idx] == 0) {
            PlaySE(1506);
            s->list[s->count] = idx;
            s->count++;
        }
        s->flags[idx] = 1;
        return 1;
    }
    return 0;
}
