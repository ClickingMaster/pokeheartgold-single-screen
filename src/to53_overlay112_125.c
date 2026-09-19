#include "to53_overlay112_w14_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                              \
    do {                                                         \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                             \
    } while (0)


int ov112_021EE604(void) { return 2; }

int ov112_021EE608(void *p) {
    if (*(u32 *)(gSystem + 0x48) & 0x80) {
        *(u32 *)((u8 *)p + 4) = 6;
        return 3;
    }
    return 2;
}

int ov112_021EE624(void) { return 1; }
