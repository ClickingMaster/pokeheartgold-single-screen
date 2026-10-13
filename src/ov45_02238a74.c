#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Tag {
    u8 value;
} Tag;

void ov45_02238ACC(u32 *, u32, Tag, u32);
void DWC_Free(u32, u32, u32);
u32 *ov45_02238A74(u32 *);

static inline Tag tag(void) {
    Tag t;
    t.value = 0;
    return t;
}

u32 *ov45_02238A74(u32 *p) {
    if (p[0]) {
        u32 current = p[1];
        Tag t = { 0 };
        ov45_02238ACC(p, current, t, 0);
        if (p[0]) {
            DWC_Free(0, p[0], 0);
        }
    }
    return p;
}
