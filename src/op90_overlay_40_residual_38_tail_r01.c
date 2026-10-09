#pragma force_active on
#include "global.h"

typedef struct {
    int unk0;
    int group;
    u8 unk8[0x14];
} Ov40Entry;

typedef struct {
    Ov40Entry entries[12];
    Ov40Entry extra[2];
    u8 unk188[0x1DC - 0x188];
    u16 layout[4];
    u16 bounds[6];
    u8 unk1F0[0x204 - 0x1F0];
    int total;
    u8 unk208[0x298 - 0x208];
    int unk298;
    u8 unk29C[0x2AC - 0x29C];
    int counts[3];
} Ov40Work;

typedef struct {
    int v[2][3];
} Ov40Layout;

extern const Ov40Layout ov40_02245C28;
void ov40_0224301C(Ov40Work *work, u32 value);
void ov40_022430A0(Ov40Work *work);

// @fn ov40_022430A0
void ov40_022430A0(Ov40Work *work) {
    Ov40Layout layout;
    int i;
    int sel;
    int sum;
    int group;
    int j;

    work->unk298 = 1;
    work->bounds[0] = 0;
    work->bounds[1] = work->counts[0];
    work->bounds[2] = work->counts[0];
    work->bounds[3] = work->counts[0] + work->counts[1];
    work->bounds[4] = work->counts[0] + work->counts[1];
    work->bounds[5] = work->counts[2] + (work->counts[0] + work->counts[1]);
    ov40_0224301C(work, 1);
    for (i = 0; i < 3; i++) {
        work->total += work->counts[i];
    }
    sel = 0;
    layout = ov40_02245C28;
    if (work->counts[0] != 4) {
        sel = 1;
    }
    for (i = 0; i < 3; i++) {
        work->layout[i] = layout.v[sel][i];
    }
    work->layout[i] = layout.v[sel][i - 1];
    sum = 0;
    for (i = 0; i < 2; i++) {
        sum += work->counts[i];
        work->extra[i].unk0 = sum - 1;
    }
    i = 0;
    group = 0;
    do {
        for (j = 0; j < work->counts[group]; j++) {
            work->entries[i].group = group + 1;
            i++;
        }
        group++;
    } while (i < work->total);
}
