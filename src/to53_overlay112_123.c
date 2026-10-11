#include "to53_overlay112_w14_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                              \
    do {                                                         \
        *(void **)((u8 *)(p) + 0x1E524) = PokewalkerApp_StartMessageBox(p, a, b); \
        return next;                                             \
    } while (0)


int ov112_021EE510(void) { return 56; }

int ov112_021EE514(void *p) {
    PokewalkerApp_StartMessage(p, 2, 27, 57);
    return 64;
}

int ov112_021EE524(void *p) {
    *(u32 *)((u8 *)p + 4) = 6;
    return 66;
}
