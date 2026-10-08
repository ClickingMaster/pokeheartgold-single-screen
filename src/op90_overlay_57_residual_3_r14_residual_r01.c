#pragma force_active on
#include "global.h"
typedef struct { int active; u8 color, x, y, pad[9]; } Row;
typedef struct { u8 pad[0x34c]; Row rows[8]; u8 rest[0x450 - 0x3cc]; u8 colors[8]; } Work;
extern void System_GetTouchHeldCoords(int *, int *);
extern void ov57_02238260(Work *, u8);
extern void ov57_02238044(Work *, u8);
int ov57_02237FAC(Work *w, int n);
// @fn ov57_02237FAC
int ov57_02237FAC(Work *w, int n) {
    int i;
    for (i = 0; i < 8; i++) {
        if (w->rows[i].active != 1) {
            int x, y;
            w->rows[i].color = w->colors[n];
            w->rows[i].x = 190;
            w->rows[i].y = 70;
            System_GetTouchHeldCoords(&x, &y);
            w->rows[i].x = x;
            w->rows[i].y = y;
            ov57_02238260(w, i);
            ov57_02238044(w, i);
            w->rows[i].active = 1;
            return i;
        }
    }
    return 255;
}
