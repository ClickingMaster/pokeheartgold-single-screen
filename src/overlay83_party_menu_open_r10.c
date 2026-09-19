#include "overlay83_party_menu_open_r10_private.h"

void ov83_022403C0(void *work, void *arg) {
    U8_AT(work, 0xE) |= 8;
    ov83_02241354(work + 0xB0);
    ClearWindowTilemapAndScheduleTransfer(work + 0xB0);
    ClearWindowTilemapAndScheduleTransfer(work + 0xE0);
    ov83_0223FB6C(work);
    ov83_022477EC(2, 1, work + 0x868);
    ov83_022477EC(6, 1, work + 0x868);
    ov83_022477EC(7, 1, work + 0x868);
    U8_AT(work, 0x860) = 0;
    U16_AT(work, 0x862) = 0;
    ov83_02240B54(work, arg);
}
