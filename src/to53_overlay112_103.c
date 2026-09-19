#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


int ov112_021ED224(void *p) {
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)p + 0x1E524)) &&
        *(u16 *)((u8 *)p + 0x1F2E0) == 256) {
        ov112_021EA5A4(p, 2);
        return 28;
    }
    return 23;
}

void ov112_021ED25C(void *p) {
    *(u16 *)((u8 *)p + 0x14) = 0;
    *(u16 *)((u8 *)p + 0x16) = 0;
}
