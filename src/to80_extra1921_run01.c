#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void long_from_char__sha1(unsigned long *, const u8 *, u32);
#pragma force_active on

void long_from_char__sha1(unsigned long *out, const u8 *in, u32 size) {
    u32 i;
    for (i = 0; i < size; i += 4) {
        *out++ = in[i + 3] | (((u32)in[i + 2] << 8) | (((u32)in[i] << 24) | ((u32)in[i + 1] << 16)));
    }
}
