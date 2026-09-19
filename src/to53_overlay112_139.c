#include "to53_overlay112_w14_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                              \
    do {                                                         \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                             \
    } while (0)


int ov112_021EEF08(void *p) {
    SaveGameNormal(*(void **)((u8 *)p + 0x20));
    Save_ClearStatusFlags(*(void **)((u8 *)p + 0x20));
    return 8;
}

int ov112_021EEF1C(void *p) {
    ov112_021EA19C(p);
    return 9;
}

int ov112_021EEF28(void *p) {
    *(u32 *)((u8 *)p + 4) = 10;
    return 10;
}
