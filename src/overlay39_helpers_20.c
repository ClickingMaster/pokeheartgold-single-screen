#include "overlay39_helpers_internal.h"

void ov39_02228B6C(void *owner, u32 current, u32 next) {
    *(u32 *)((u8 *)owner + 8) = current;
    *(u32 *)((u8 *)owner + 12) = next;
}

BOOL ov39_02228B74(int a0, int a1) {
    switch (ov00_021E6A70(a0, a1)) {
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        return TRUE;
    case 0:
    case 2:
    case 3:
    default:
        return FALSE;
    }
}
