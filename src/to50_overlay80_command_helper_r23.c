#include "to50_overlay80_command_helper_r23_private.h"

BOOL ov80_0222D2EC(void *c) {
    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew()) {
        return TRUE;
    }
    U16(c, 0x78)
    --;
    if (U16(c, 0x78) == 0) {
        return TRUE;
    }
    return FALSE;
}
