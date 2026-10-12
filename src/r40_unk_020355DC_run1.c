#include "global.h"

#include "r40_unk_020355DC_private.h"

BOOL sub_020355DC(u16 index) {
    if (_021D4134.work == NULL) {
        return FALSE;
    }
    if (Wireless_GetState() != 4) {
        return FALSE;
    }
    if (Wireless_GetConnectedBitmap() & (1 << index)) {
        return TRUE;
    }
    return FALSE;
}
