#include "to52_overlay96_palette_index_private.h"

u32 ov96_02214394(u32 a, u8 b) {
    u32 result;
    switch (a) {
    case 0:
        result = 0xd8;
        break;
    case 1:
        result = 0xdc;
        break;
    case 2:
        result = 0xe0;
        break;
    case 3:
        result = 0xe4;
        break;
    default:
        GF_ASSERT(FALSE);
        return 0;
    }
    if (b >= 4) {
        GF_ASSERT(FALSE);
        return 0;
    }
    return (u8)(result + b);
}
