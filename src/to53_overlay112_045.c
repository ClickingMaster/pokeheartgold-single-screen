#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


void ov112_021EA17C(void *p) {
    *(void **)((u8 *)p + 0x1EC4C) = WaitingIcon_New((u8 *)p + 0x1EA88, 0x3E2);
}

void ov112_021EA19C(void *p) {
    sub_0200F450(*(void **)((u8 *)p + 0x1EC4C));
}
