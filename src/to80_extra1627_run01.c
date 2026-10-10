#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Bytes4 {
    u8 a, b, c, d;
} Bytes4;

void ov00_022143E4(u8 **, u32, u32 *, ...);

void ov00_022143E4(u8 **out, u32 value, u32 *offset, ...) {
    memcpy(*out, &value, 4);
    *offset += 4;
    *out += 4;
}
