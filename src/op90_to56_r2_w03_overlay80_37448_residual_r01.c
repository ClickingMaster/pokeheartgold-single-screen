#pragma force_active on
#include "global.h"
#include "math_util.h"
typedef struct {
    u16 min;
    u16 max;
} IdRange;
typedef struct {
    u16 a;
    u16 b;
} IdPair;
extern const u16 ov80_0223C990[];
extern const IdRange ov80_0223C5A8[];
extern const IdRange ov80_0223C5B4;
extern const IdRange ov80_0223C5E0[];
extern const IdPair ov80_0223CD4A[];
extern int ov80_022379C0(int);
void ov80_02237448(u16 n, u16 key, int arg2, u8 slot, u16 exclude, u16 *list, int mode);
// @fn ov80_02237448
void ov80_02237448(u16 n, u16 key, int arg2, u8 slot, u16 exclude, u16 *list, int mode) {
    u16 i, j;
    u16 found;
    u16 count;
    u16 idx;
    const IdRange *range;
    BOOL wrapped;
    u16 start;
    u8 base;
    u8 tier;
    int num;

    num = 0;
    base = slot * 2;
    wrapped = FALSE;
    tier = ov80_022379C0(arg2);
    if (mode != 0) {
        for (i = 0; i < 0x1DD; i++) {
            if (exclude == ov80_0223C990[i]) {
                found = i;
                break;
            }
        }
        if (i == 0x1DD) {
            found = 0x178;
        }
        for (i = 0; i < 4; i++) {
            if (found < ov80_0223C5A8[i].max) {
                break;
            }
        }
        if (i == 4) {
            i = 3;
        }
        if (mode == 2) {
            range = &ov80_0223C5B4;
        } else {
            range = &ov80_0223C5A8[i];
        }
    } else {
        range = &ov80_0223C5E0[tier];
    }
    count = range->max - range->min + 1;
    idx = range->min + LCRandom() % count;
    idx--;
    start = idx;
    while (num < n) {
        if (wrapped == FALSE) {
            for (j = 0; j < base; j++) {
                if (idx + 1 == list[j]) {
                    break;
                }
            }
        } else if (idx + 1 == list[base - 2]) {
            j = 0;
        } else {
            j = base;
        }
        if (j == base) {
            if (mode != 0) {
                if (exclude != ov80_0223C990[idx]) {
                    list[base + num] = idx + 1;
                    num++;
                }
            } else if (key == ov80_0223CD4A[idx].a || key == ov80_0223CD4A[idx].b) {
                if (exclude != ov80_0223C990[idx]) {
                    list[base + num] = idx + 1;
                    num++;
                }
            }
        }
        idx++;
        if (idx + 1 >= range->max) {
            idx = range->min - 1;
        }
        if (idx == start) {
            wrapped = TRUE;
        }
    }
}
