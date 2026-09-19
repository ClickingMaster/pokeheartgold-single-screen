#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


int ov112_021ED4D4(void *p) {
    MIi_CpuClearFast(0, (u32 *)((u8 *)p + 0x9DFC), 0xCC0);
    ov112_021E7548((u8 *)p + 0x9D44, (u8 *)p + 0x9D70,
        (u8 *)p + 0xAABC, (u8 *)p + 0xB184);
    ov112_021E7CC8(p);
    ov112_021ED25C(p);
    ov112_021EA5A4(p, 2);
    ManagedSprite_SetAnimationFrame(*(void **)((u8 *)p + 0x1E550), 0);
    return 27;
}
