#pragma force_active on
#include "global.h"

typedef struct Row {
    u32 a;
    u32 b;
    u32 c;
} Row;

typedef struct W {
    void *base;
    Row row[2];
    u32 timer : 8;
    u32 rest : 24;
} W;

u8 *ov96_021E8A20(void *ptr);
void ov96_0220FF88(W *w, void *y);
void ov96_0220FE38(Row *row, u16 *p, void *base);
void ov96_02210030(W *w, u8 *x, void *y);

// @fn ov96_02210030
void ov96_02210030(W *w, u8 *x, void *y) {
    int i;
    u16 *p = (u16 *)ov96_021E8A20(x + 0x28);

    if (w->timer == 0) {
        ov96_0220FF88(w, y);
    } else {
        w->timer--;
    }
    for (i = 0; i < 2; i++) {
        ov96_0220FE38(&w->row[i], &p[i], w->base);
    }
}
