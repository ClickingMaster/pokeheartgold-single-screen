#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


int ov112_021ED8DC(void *p) {
    MIi_CpuClearFast(0, (u32 *)((u8 *)p + 0xAABC), 0x6C8);
    ov112_021E7620((u8 *)p + 0x9D70, (u8 *)p + 0xB184);
    PokewalkerApp_StartLink(p);
    ov112_021ED25C(p);
    return 29;
}
