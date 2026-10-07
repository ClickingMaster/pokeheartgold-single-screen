#pragma force_active on
#include "global.h"

typedef struct Descriptor {
    u8 z;
    u8 flags;
    u8 unused;
    u8 attr;
    u8 x;
} Descriptor;

typedef struct Values {
    int z[5];
    int x[5];
    int y[5];
    int attr[5];
    int flags[5];
} Values;

typedef struct Entry {
    f32 x;
    f32 y;
    f32 z;
    u32 attr;
    u8 flags;
} Entry;

typedef struct Output {
    u8 prefix[12];
    Entry entries[4];
} Output;

Descriptor *ov96_021E60D8(void *arg, int group, u8 index);
void ov96_02206E88(void *arg, const Values *values, int group, u8 index, Output *out);

// @fn ov96_02206E88
void ov96_02206E88(void *arg, const Values *values, int group, u8 index, Output *out) {
    Descriptor *d = ov96_021E60D8(arg, group, index);

    out->entries[index].z = (f32)values->z[d->z] / 10.0;
    out->entries[index].flags = values->flags[d->flags];
    out->entries[index].x = (f32)values->x[d->x] / 10.0;
    out->entries[index].y = (f32)values->y[d->flags] / 10.0;
    out->entries[index].attr = values->attr[d->attr];
}
