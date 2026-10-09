#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct OamCell {
    u32 w0;
    u32 w1;
} OamCell;
typedef struct CellPos {
    int x;
    int y;
} CellPos;
typedef struct CellSprite {
    u8 pad00[8];
    OamCell *cells;
    u8 count;
} CellSprite;
void ov13_0223FF18(CellSprite *s, int idx, int y, int x);
static inline void GetCellPos(const OamCell *c, s32 *x, s32 *y) {
    *y = (c->w0 & 0x01FF0000) >> 16;
    *x = c->w0 & 0xFF;
}
static inline void SetCellPos(OamCell *c, int x, int y) {
    c->w0 = (c->w0 & 0xFE00FF00) | (x & 0xFF) | ((y & 0x1FF) << 16);
}
// @fn ov13_0223FF18
void ov13_0223FF18(CellSprite *s, int idx, int y, int x) {
    OamCell *cells = s->cells;
    if (idx >= 0) {
        SetCellPos(&cells[idx], x, y);
    } else {
        int oy;
        int ox;
        int cy;
        int cx;
        int i;
        int dy;
        int dx;
        GetCellPos(&cells[0], (s32 *)&ox, (s32 *)&oy);
        SetCellPos(&cells[0], x, y);
        dy = y - oy;
        dx = x - ox;
        for (i = 1; i < s->count; i++) {
            GetCellPos(&cells[i], (s32 *)&cx, (s32 *)&cy);
            SetCellPos(&cells[i], cx + dx, cy + dy);
        }
    }
}
