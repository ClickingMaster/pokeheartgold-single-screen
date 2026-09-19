#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


int ov112_021EAC18(void) {
    Sound_SetSceneAndPlayBGM(73, 0x497, 0);
    PlayBGM(0x497);
    return 2;
}
