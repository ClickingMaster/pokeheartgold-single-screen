#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Tag {
    u8 value;
} Tag;

void LobbyStringVectorBuffer_EraseAtEnd(u32 *, u32, Tag, u32);
void DWC_Free(u32, u32, u32);
u32 *LobbyStringVectorBuffer_Deallocate(u32 *);

static inline Tag tag(void) {
    Tag t;
    t.value = 0;
    return t;
}

u32 *LobbyStringVectorBuffer_Deallocate(u32 *p) {
    if (p[0]) {
        u32 current = p[1];
        Tag t = { 0 };
        LobbyStringVectorBuffer_EraseAtEnd(p, current, t, 0);
        if (p[0]) {
            DWC_Free(0, p[0], 0);
        }
    }
    return p;
}
