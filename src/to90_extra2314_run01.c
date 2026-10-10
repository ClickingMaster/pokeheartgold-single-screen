#pragma force_active on
#include "global.h"
#pragma force_active on

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 *offsets;
    u8 *base;
} S;

u16 *ov13_0222BAF4(S *, u32, int, u32);

static inline void store(u16 *p, u32 index, int pos, u32 value) {
    if (pos >= 0) {
        value += 48;
        p[pos] = value;
    }
}

u16 *ov13_0222BAF4(S *s, u32 index, int pos, u32 value) {
    u16 *p = (u16 *)(s->base + s->offsets[(u16)index]);
    if (pos >= 0) {
        value += 48;
        p[pos] = (u16)value;
    }
    return p;
}
