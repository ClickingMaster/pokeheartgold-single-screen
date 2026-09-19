#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


int ov112_021EB5AC(void *p) {
    ov112_021EC460(p, 0, 8, 17);
    return 24;
}

int ov112_021EB5BC(void *p) {
    return PRINTER_STATE(p, 17, 16);
}
