#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
u32 SearchLZ(const u8 *, const u8 *, u32, u16 *, u32);
u32 MI_CompressLZImpl(const u8 *, u32, u8 *, int);

u32 MI_CompressLZImpl(const u8 *source, u32 length, u8 *out, int extended) {
    const u8 *start = source;
    u32 maximum = extended ? 0x10110 : 18;
    u32 size;
    u8 flags, i;
    u32 original = length;
    *(u32 *)out = (length << 8) | 16 | (extended ? 1 : 0);
    out += 4;
    size = 4;
    while (length) {
        flags = 0;
        i = 0;
        u8 *flag = out++;
        size++;
        do {
            u32 match;
            u16 distance;
            flags <<= 1;
            if (length) {
                match = SearchLZ(start, source, length, &distance, maximum);
                if (match) {
                    u32 value;
                    flags |= 1;
                    if (size + 2 >= original) {
                        return 0;
                    }
                    if (extended) {
                        if (match >= 273) {
                            size += 2;
                            value = match - 273;
                            out[0] = (value >> 12) | 16;
                            out[1] = value >> 4;
                            out += 2;
                        } else if (match < 17) {
                            value = match - 1;
                        } else {
                            value = match - 17;
                            *out++ = value >> 4;
                            size++;
                        }
                    } else {
                        value = match - 3;
                    }
                    out[0] = (value << 4) | ((distance - 1) >> 8);
                    out[1] = distance - 1;
                    out += 2;
                    size += 2;
                    source += match;
                    length -= match;
                } else {
                    size++;
                    if (size >= original) {
                        return 0;
                    }
                    *out++ = *source++;
                    length--;
                }
            }
            i++;
        } while (i < 8);
        *flag = flags;
    }
    {
        u8 pad = 0;
        if (size & 3) {
            do {
                *out++ = 0;
                pad++;
            } while ((size + pad) & 3);
        }
    }
    return size;
}
