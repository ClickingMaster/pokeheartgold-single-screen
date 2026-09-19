#include "to53_overlay112_w14_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                              \
    do {                                                         \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                             \
    } while (0)


void ov112_021EECF0(void *p) {
    ov112_021EEAF0(p, 1);
    ov112_021EEA7C(p);
    ov112_021EE920(p);
}
