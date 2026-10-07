#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

extern const unsigned char __lower_mapC[];
u32 ov00_022141EC(const char *s, u32 mod);
// @fn ov00_022141EC
u32 ov00_022141EC(const char *s, u32 mod) {
    u32 hash = 0;
    for (; *s != 0; s++) {
        int c = *s;
        hash = 0x9CCF9319 * hash + ((c < 0 || c >= 0x80) ? c : __lower_mapC[c]);
    }
    return hash % mod;
}
