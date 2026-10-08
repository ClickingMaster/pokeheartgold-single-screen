#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct {
    u8 pad0[0xd];
    u8 cursor;
    u8 padE[0x90 - 0xe];
    u8 touched;
    u8 pad91[0x204 - 0x91];
    void *cursorSprite;
} Work;
extern int ov82_0223F558(Work *);
extern int ov82_0223F570(Work *);
extern void ov82_0223FCBC(void *, int, int);
extern void ov82_0223FCFC(void *, int);
extern int ov80_02237920(u8);
BOOL ov82_0223F488(Work *w);
// @fn ov82_0223F488
BOOL ov82_0223F488(Work *w) {
    u16 x, y;
    int i, j;
    int top, bottom, left, right;
    if (gSystem.touchNew != 0) {
        x = gSystem.touchX;
        y = gSystem.touchY;
        for (i = 0; i < 5; i++) {
            top = i * 36 + 3;
            bottom = top + 35;
            for (j = 0; j < 4; j++) {
                left = j * 64 + 1;
                right = left + 63;
                if (left <= x && x <= right && top <= y && y <= bottom) {
                    w->cursor = j + i * 4;
                    ov82_0223FCBC(w->cursorSprite, ov82_0223F558(w), ov82_0223F570(w));
                    if (ov80_02237920(w->cursor) == 0xFE) {
                        ov82_0223FCFC(w->cursorSprite, 2);
                        ov82_0223FCBC(w->cursorSprite, 128, 168);
                    } else {
                        ov82_0223FCFC(w->cursorSprite, 1);
                    }
                    w->touched = 1;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}
