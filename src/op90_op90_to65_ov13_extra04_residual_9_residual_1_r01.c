#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct Rect16 { u16 x1, y1, x2, y2; } Rect16;
typedef struct { u32 header; u16 scroll; } Global;
extern Rect16 ov13_02245958;
extern u8 ov13_022455DC[];
extern Global ov13_0224F54C;
typedef struct { u8 v[4]; } RetCodes;
const RetCodes ov13_022454FC = {{2, 3, 7, 8}};
static const Rect16 sRect0 = {0xCC, 0x34, 0x1C, 0x18};
static const Rect16 sRect1 = {0x8F, 0x34, 0x2C, 0x18};
static const Rect16 sRect2 = {0xC0, 0x34, 0x2C, 0x18};
extern Rect16 ov13_0224558A[];
extern int FX_DivS32(int, int);
extern BOOL ov13_022417DC(Rect16 *), ov13_02241974(Rect16 *);
int ov13_02234700(void);
// @fn ov13_02234700
int ov13_02234700(void) {
    Rect16 r;
    int row, i;
    if (!ov13_022417DC(&ov13_02245958)) {
        return 14;
    }
    row = FX_DivS32(ov13_0224F54C.scroll, 29);
    r = sRect0;
    for (i = 0; i < 4; i++) {
        if (row != 2 && row != 6) {
            if (ov13_02241974(&r)) {
                return ov13_022455DC[row];
            }
        }
        r.y1 += 29;
        row++;
    }
    row = FX_DivS32(ov13_0224F54C.scroll, 29);
    for (i = 0; i < 4; i++) {
        if (row == 2) {
            r = sRect2;
            r.y1 += i * 29;
            if (ov13_02241974(&r)) {
                return ov13_022454FC.v[0];
            }
            r = sRect1;
            r.y1 += i * 29;
            if (ov13_02241974(&r)) {
                return ov13_022454FC.v[1];
            }
            break;
        }
        row++;
    }
    row = FX_DivS32(ov13_0224F54C.scroll, 29);
    for (i = 0; i < 4; i++) {
        if (row == 6) {
            r = sRect2;
            r.y1 += i * 29;
            if (ov13_02241974(&r)) {
                return ov13_022454FC.v[2];
            }
            r = sRect1;
            r.y1 += i * 29;
            if (ov13_02241974(&r)) {
                return ov13_022454FC.v[3];
            }
            break;
        }
        row++;
    }
    for (i = 0; i < 3; i++) {
        if (ov13_022417DC(&ov13_0224558A[i])) {
            return i + 11;
        }
    }
    return 14;
}
