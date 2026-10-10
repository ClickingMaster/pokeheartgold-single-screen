#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x468];
    u32 values[119];
    u8 bytes[256];
} S;

extern S *ov00_0221A784;
extern u32 ov00_021EE64C(u8);
extern u32 ov00_021EE6C0(u32);
extern u32 ov00_021EE72C(u32);
u32 ov00_021EE73C(u32, int);

u32 ov00_021EE73C(u32 value, int count) {
    u8 i;
    for (i = 0; i < count; i++) {
        if (value == ov00_0221A784->values[i]) {
            break;
        }
    }
    if (i >= count) {
        return 0;
    }
    return ov00_021EE72C(ov00_021EE6C0(ov00_021EE64C(ov00_0221A784->bytes[i])));
}
