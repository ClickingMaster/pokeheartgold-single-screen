#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


void ov112_021EDAF4(void *p, int frame) {
    if (frame == 0) {
        ManagedSprite_SetAnimationFrame(*(void **)((u8 *)p + 0x1E550), 0);
    } else if (frame > 30) {
        ManagedSprite_SetAnimationFrame(*(void **)((u8 *)p + 0x1E550), (frame - 30) / 4);
    }
}
