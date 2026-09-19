#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


int ov112_021EB1D8(void *p) {
    ov112_021EC460(p, 2, 0, 6);
    return 24;
}

int ov112_021EB1E8(void *p) {
    return PRINTER_STATE(p, 7, 5);
}
