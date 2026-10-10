#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[13];
    u8 count;
    u8 q[0xf4 - 14];
    u32 ids[33];
    u8 r[0x1a8 - 0x178];
    u8 mode;
    u8 s[0x2d0 - 0x1a9];
    u8 indices[33];
} State;

extern State *ov00_021F955C(void);
extern void ov00_021F4304(int, u32, int, int, u32 *, int);
extern int ov00_021F5B30(void);
extern int ov00_021F7CEC(u32, int);
extern void ov00_021EE31C(int);
extern void ov00_021F9630(int, int, u8 *, int);
int ov00_021F7158(u32);
void ov00_021F7294(u32, int);

int ov00_021F7158(u32 mask) {
    u32 rejected[32], accepted[32];
    int nr = 0, na = nr, i = 1;
    for (; i <= ov00_021F955C()->count; i++) {
        if (mask & (1 << ov00_021F955C()->indices[i])) {
            accepted[na] = ov00_021F955C()->ids[i];
            na++;
        } else {
            rejected[nr] = ov00_021F955C()->ids[i];
            nr++;
        }
    }
    for (i = 0; i < na; i++) {
        ov00_021F4304(16, accepted[i], 0, 0, rejected, nr);
        if (ov00_021F5B30()) {
            return 0;
        }
    }
    ov00_021F955C()->mode = 2;
    for (i = 0; i < nr; i++) {
        int value = ov00_021F7CEC(rejected[i], 0);
        if (value != 255) {
            ov00_021EE31C(value);
        }
    }
    ov00_021F955C()->mode = 0;
    return 1;
}

void ov00_021F7294(u32 id, int kind) {
    u8 buffer[4];
    switch (kind) {
    case 2: {
        State *s = ov00_021F955C();
        u8 i;
        if (id == s->indices[ov00_021F955C()->count]) {
            buffer[0] = 1;
        } else {
            buffer[0] = 0;
        }
        for (i = 1; i <= ov00_021F955C()->count; i++) {
            if (id == ov00_021F955C()->indices[i]) {
                buffer[1] = i;
                buffer[2] = id;
                break;
            }
        }
        break;
    }
    case 3: {
        buffer[0] = *(u16 *)((u8 *)ov00_021F955C() + 0x1b0);
        buffer[1] = *(u16 *)((u8 *)ov00_021F955C() + 0x1b0) >> 8;
        break;
    }
    default:
        break;
    }
    ov00_021F9630(kind, id, buffer, 4);
    {
        State *s = ov00_021F955C();
        *(u64 *)((u8 *)s + 0x1e0) = OS_GetTick();
    }
}
