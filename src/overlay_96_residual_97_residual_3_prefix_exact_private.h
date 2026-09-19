#ifndef POKEHEARTGOLD_OVERLAY_96_R97_PREFIX_EXACT_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY_96_R97_PREFIX_EXACT_PRIVATE_H
#include "global.h"
typedef struct R97PrefixItem {
    u32 w[5];
    union {
        u32 raw;
        struct { u32 lo:8; u32 field8:8; u32 count:8; u32 mid:6; u32 state:2; } b;
    } v5;
    union { u32 raw; struct { u32 low:11; u32 counter:11; u32 high:10; } b; } w6;
    u32 w7;
    u32 w8;
} R97PrefixItem;
void ov96_0220CD84(R97PrefixItem *p, int mode);
#endif
