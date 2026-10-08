#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    u8 pad0[0x18];
    GXOamAttr *rows[5];
    GXOamAttr *upper;
    GXOamAttr *lower;
    GXOamAttr *cursor;
    u8 pad38[0x44 - 0x38];
    u8 dirty;
} ListView;
typedef struct {
    u32 header;
    u16 scroll;
    ListView *view;
} ListGlobal;
extern ListGlobal ov13_0224F54C;
#include <nitro/fx/fx_cp.h>
s32 FX_DivS32(s32 a, s32 b);
void ov13_022344F0(void);
static inline void GetObjPos(const GXOamAttr *oam, u32 *x, u32 *y) {
    *x = (oam->attr01 & 0x01FF0000) >> 16;
    *y = oam->attr01 & 0xff;
}
static inline void SetObjPos(GXOamAttr *oam, u32 x, u32 y) {
    oam->attr01 = (oam->attr01 & 0xFE00FF00) | (y & 0xff) | (((u32)x << 23) >> 7);
}
// @fn ov13_022344F0
void ov13_022344F0(void) {
    int x;
    int y;
    int row = FX_DivS32(ov13_0224F54C.scroll, 29);
    int base = 52 - FX_ModS32(ov13_0224F54C.scroll, 29);
    int pos;
    int i;
    if (ov13_0224F54C.view->cursor != NULL) {
        if (row == 0) {
            x = 0x26;
        } else {
            x = 0x100;
        }
        SetObjPos(ov13_0224F54C.view->cursor, x, base);
    }
    pos = base;
    for (i = 0; i < 5; i++) {
        GXOamAttr *oam = ov13_0224F54C.view->rows[i];
        GetObjPos(oam, (u32 *)&x, (u32 *)&y);
        SetObjPos(oam, x, pos);
        pos += 29;
    }
    if (row <= 2) {
        GXOamAttr *oam = ov13_0224F54C.view->upper;
        GetObjPos(oam, (u32 *)&x, (u32 *)&y);
        SetObjPos(oam, x, (2 - row) * 29 + base);
    } else {
        SetObjPos(ov13_0224F54C.view->upper, 256, 0);
    }
    if (row >= 2 && row <= 6) {
        GXOamAttr *oam = ov13_0224F54C.view->lower;
        GetObjPos(oam, (u32 *)&x, (u32 *)&y);
        SetObjPos(oam, x, (6 - row) * 29 + base);
    } else {
        SetObjPos(ov13_0224F54C.view->lower, 256, 0);
    }
    ov13_0224F54C.view->dirty = 1;
}
