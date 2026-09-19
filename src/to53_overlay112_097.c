#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


int ov112_021ECECC(void *p) {
    return PRINTER_STATE(p, 14, 13);
}

int ov112_021ECEEC(void *p) {
    ov112_021E966C(p, 0);
    return 15;
}
