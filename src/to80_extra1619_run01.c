#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Entry {
    u32 flags, other;
} Entry;

typedef struct Object {
    char pad[8];
    Entry *entries;
    u8 count;
    char end[3];
} Object;

void ov13_0223FDF4(Object *, int, u32, u32);

void ov13_0223FDF4(Object *p, int index, u32 kind, u32 option) {
    Entry *entries = p->entries;
    if (index >= 0) {
        if (kind != 256 && kind != 768) {
            u32 value = entries[index].flags & 0xc1fffcff;
            value |= kind;
            entries[index].flags = value;
        } else {
            u32 value = entries[index].flags & 0xc1fffcff;
            value |= kind;
            value |= option << 25;
            entries[index].flags = value;
        }
        return;
    }
    for (index = 0; index < p->count; index++) {
        u32 value;
        if (kind != 256 && kind != 768) {
            value = entries[index].flags & 0xc1fffcff;
            value |= kind;
        } else {
            {
                value = entries[index].flags & 0xc1fffcff;
                value |= kind;
            }
            value |= option << 25;
        }
        entries[index].flags = value;
    }
}
