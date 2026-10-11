#include "r40_overlay_14_residual_14_private.h"

void PCBox_SlideOutPartyButton(BgCellManager *p) {
    BgCellManager_StartCellMove(p, 8, 0, 1, 3);
}

void PCBox_SlideInPartyButton(BgCellManager *p) {
    BgCellManager_StartCellMove(p, 8, 0, -1, 3);
}

void ov14_021E825C(void *p) {
    ov14_021F5E94(p, 0);
    BgCellManager_DrawCellAt(*(BgCellManager **)((u8 *)*(void **)((u8 *)p + 0x34) + 0x2F0), 9, 12, 21);
}
