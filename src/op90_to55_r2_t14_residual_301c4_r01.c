#pragma force_active on
#include "global.h"
typedef struct W {
    u8 pad[4];
    void *ctx;
    u8 gap[8];
    int top;
    int center;
    u32 phase;
    fx32 amplitude;
    int alternating;
    int slope;
    u8 gap2[8];
    u32 xy;
} W;
extern u32 *ov07_02222C84(void *);
extern u32 ov07_02222D88(u16, u16);
void ov07_022301C4(W *w);
// @fn ov07_022301C4
void ov07_022301C4(W *w) {
    u32 *buffer;
    int start;
    int end;
    int row;
    s16 wave;
    int dest;
    fx32 amp;
    s16 x;
    s16 y;
    u32 xy;
    buffer = ov07_02222C84(w->ctx);
    end = start = w->top;
    start -= 8;
    end += 88;
    if (start < 0) {
        start = 0;
    }
    if (end > 192) {
        end = 192;
    }
    w->alternating *= -1;
    for (row = start; row < end; row++) {
        if (row & 2) {
            amp = w->amplitude + (w->alternating << 12);
        } else {
            amp = w->amplitude - (w->alternating << 12);
        }
        wave = FX_Mul(FX_SinIdx((u16)(w->phase + (row - start) * 409)), amp) >> 12;
        wave += (row - w->center) * w->slope / 10;
        xy = w->xy;
        x = xy;
        y = xy >> 16;
        dest = row - 1;
        if (dest < 0) {
            dest += 192;
        }
        buffer[dest] = ov07_02222D88(x + wave, y);
    }
}
