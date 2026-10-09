#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 v[5]; } Five;
typedef struct { u16 x; u16 pad; } Entry;
typedef struct { u16 x, y; } Pos16;
const Pos16 ov13_02244F60 = {0x1C, 0x02};
const Pos16 ov13_02244F64 = {0x78, 0x12};
const Pos16 ov13_02244F68 = {0x11, 0x12};
static const Five sA = {{0, 0, 0, 0, 2}};
static const Five sB = {{0x0C, 0x0C, 0x0C, 0x0B, 0}};
static const Five sC = {{1, 1, 1, 1, 0}};
extern const Entry ov13_02244FA4[], ov13_02244F84[], ov13_02244F7C[];
typedef struct {
    void *res[3][4];
    u32 *first[47];
    u32 *second[4];
    void *third[2];
    void *data[4];
} Work;
extern Work *ov13_0224F4BC;
extern void ov13_02240D94(void *, int, int, void *, int);
extern void ov13_0223FDF4(void *, int, int, int);
extern void ov13_0223FF18(void *, int, int, int);
extern s32 FX_ModS32(s32, s32);
void ov13_022290F0(int mode, int idx, int y);
static inline u32 OamPos(u32 attr, int x, int y) {
    return (attr & 0xFE00FF00) | (y & 0xff) | ((x & 0x1ff) << 16);
}
// @fn ov13_022290F0
void ov13_022290F0(int mode, int idx, int y) {
    Five n1 = sB;
    Five n2 = sC;
    Five n3 = sA;
    int pos;
    int i;
    int start = idx * 12;
    int m;
    pos = start;
    for (i = 0; i < n1.v[idx]; i++) {
        int x = ov13_02244FA4[pos].x;
        *ov13_0224F4BC->first[pos] &= 0xC1FFFCFF;
        *ov13_0224F4BC->first[pos] = OamPos(*ov13_0224F4BC->first[pos], x, y);
        pos++;
    }
    if (idx < 4) {
        ov13_02240D94(ov13_0224F4BC->res[mode][idx], ov13_02244FA4[start].x, y, ov13_0224F4BC->data[idx], 2);
    }
    m = FX_ModS32(idx + 3, 4);
    for (i = 0; i < n2.v[idx]; i++) {
        int x = ov13_02244F84[m].x;
        *ov13_0224F4BC->second[m] &= 0xC1FFFCFF;
        *ov13_0224F4BC->second[m] = OamPos(*ov13_0224F4BC->second[m], x, y);
    }
    for (i = 0; i < n3.v[idx]; i++) {
        ov13_0223FDF4(ov13_0224F4BC->third[i], -1, 0, 0);
        ov13_0223FF18(ov13_0224F4BC->third[i], -1, ov13_02244F7C[i].x, y);
    }
}
