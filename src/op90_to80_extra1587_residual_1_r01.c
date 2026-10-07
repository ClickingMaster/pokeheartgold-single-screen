#pragma force_active on
#include "global.h"
typedef struct {
    int a[8][5];
} Params;
typedef struct {
    f32 x0, x4, x8, xc, x10;
    u8 x14, x15, x16, x17;
    u8 pad[4];
} Entry;
typedef struct {
    u8 pad[0x18];
    Entry entries[4];
} Out;
u8 *ov96_021E60D8(void *course, int a, int b);
void ov96_021FEFE8(void *course, Params *params, int a2, u8 idx, Out *out);
// @fn ov96_021FEFE8
void ov96_021FEFE8(void *course, Params *params, int a2, u8 idx, Out *out) {
    u8 *info = ov96_021E60D8(course, a2, idx);
    out->entries[idx].x8 = (f32)params->a[0][info[0]] / 10.0;
    out->entries[idx].x10 = params->a[1][info[3]];
    out->entries[idx].xc = params->a[1][info[3]];
    out->entries[idx].x16 = params->a[5][info[1]];
    out->entries[idx].x14 = params->a[7][info[1]];
    out->entries[idx].x15 = params->a[6][info[1]];
    out->entries[idx].x0 = (f32)params->a[2][info[4]] / 10.0;
    out->entries[idx].x4 = (f32)params->a[3][info[1]] / 10.0;
    out->entries[idx].x17 = params->a[4][info[3]];
}
