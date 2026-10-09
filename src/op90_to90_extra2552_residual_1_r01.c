#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 v[5]; } Five;
typedef struct { u16 x, y; } Pos16;
const Pos16 ov13_02245230 = {0x1C, 0x14};
const Pos16 ov13_02245228 = {0x1C, 0x14};
static const Pos16 s522C = {0x0C, 0x04};
const Pos16 ov13_02245234 = {0x78, 0x12};
static const Five sB = {{3,3,3,1,0}};
static const Five sA = {{0,0,0,0,2}};
static const Five sC = {{0,0,0,2,0}};
static const u16 sD[4] = {0, 1, 0, 0};
typedef struct { u16 x; u16 pad; } Entry;
extern const Entry ov13_0224527A[], ov13_02245250[], ov13_02245258[];
typedef struct { void *res[4]; u32 *first[10]; u32 *second[2]; void *third[2]; void *data[4]; } Work;
extern Work *ov13_0224F4C0;
extern void ov13_02240D94(void *, int, int, void *, int);
extern void ov13_0223FDF4(void *, int, int, int);
extern void ov13_0223FF18(void *, int, int, int);
void ov13_0222AAFC(int group, int y);
static inline u32 OamPos(u32 attr, int x, int y) {
    return (attr & 0xFE00FF00) | (y & 0xff) | ((x & 0x1ff) << 16);
}
// @fn ov13_0222AAFC
void ov13_0222AAFC(int group, int y) {
    Five n1 = sB;
    Five n2 = sC;
    Five n3 = sA;
    int pos;
    int i;
    int start = group * 3;
    pos = start;
    for (i = 0; i < n1.v[group]; i++) {
        int x = ov13_0224527A[pos].x;
        *ov13_0224F4C0->first[pos] &= 0xC1FFFCFF;
        *ov13_0224F4C0->first[pos] = OamPos(*ov13_0224F4C0->first[pos], x, y);
        pos++;
    }
    if (group < 4) {
        ov13_02240D94(ov13_0224F4C0->res[group], ov13_0224527A[start].x, y, ov13_0224F4C0->data[group], 2);
    }
    for (i = 0; i < n2.v[group]; i++) {
        int x = ov13_02245250[i].x;
        *ov13_0224F4C0->second[i] &= 0xC1FFFCFF;
        *ov13_0224F4C0->second[i] = OamPos(*ov13_0224F4C0->second[i], x, y);
    }
    for (i = 0; i < n3.v[group]; i++) {
        ov13_0223FDF4(ov13_0224F4C0->third[i], -1, 0, 0);
        ov13_0223FF18(ov13_0224F4C0->third[i], -1, ov13_02245258[i].x, y);
    }
}
