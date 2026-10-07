#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad[0x200];
    s32 table[3][128];
    s32 lines[3][128];
    s32 pos[8];
} Work;
void ov96_021F6524(Work *work, int line, u8 a, u8 b);
// @fn ov96_021F6524
void ov96_021F6524(Work *work, int line, u8 a, u8 b) {
    s32 *from;
    s32 *to;
    u8 *fromIdx;
    u8 *toIdx;
    int i;
    s32 start;
    s32 delta;
    s32 span;
    if (work->pos[a] <= work->pos[b]) {
        from = &work->pos[a];
        to = &work->pos[b];
        fromIdx = &a;
        toIdx = &b;
    } else {
        from = &work->pos[b];
        to = &work->pos[a];
        fromIdx = &b;
        toIdx = &a;
    }
    for (i = 0; i < 128; i++) {
        work->lines[line][i] = 0;
    }
    start = work->table[*fromIdx][*from];
    delta = work->table[*toIdx][*to] - start;
    span = *to - *from;
    for (i = 0; i < span - 1; i++) {
        work->lines[line][*from + i + 1] = start + delta * (i + 1) / span;
    }
}
