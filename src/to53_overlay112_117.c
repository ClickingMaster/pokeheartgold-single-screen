#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


int ov112_021EDF08(void *p) {
    ov112_021EAAE4(p, (u8 *)p + 0x1F2C0);
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)p + 0x1E524)) &&
        !ov112_021EA838(p) && !IsFanfarePlaying()) {
        *(u32 *)((u8 *)p + 4) = 5;
        return 66;
    }
    return 36;
}
