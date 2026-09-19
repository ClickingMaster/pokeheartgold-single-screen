#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


int ov112_021EB3A8(void *p) {
    ov112_021EC460(p, 0, 2, 12);
    return 24;
}

int ov112_021EB3B8(void *p) {
    return PRINTER_STATE(p, 12, 11);
}
