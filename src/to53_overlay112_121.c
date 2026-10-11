#include "to53_overlay112_w14_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                              \
    do {                                                         \
        *(void **)((u8 *)(p) + 0x1E524) = PokewalkerApp_StartMessageBox(p, a, b); \
        return next;                                             \
    } while (0)


int ov112_021EE29C(void *p) { LOAD_PRINTER(p, 0, 19, 48); }

int ov112_021EE2B4(void *p) { return PRINTER_STATE(p, 49, 48); }
