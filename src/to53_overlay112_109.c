#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


BOOL ov112_021ED610(void *p) {
    struct Bits { u8 pad[0x10E7]; u8 x:2; u8 flag:1; };
    u16 v;
    if (((struct Bits *)p)->flag == 1) {
        v = *(u16 *)((u8 *)p + 0x9D44);
        if (v == 0 || v > 0x1ED) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL ov112_021ED640(void *p) {
    struct Bits { u8 pad[0xAABC]; u8 x:5; u8 flag:1; };
    u16 v;
    if (((struct Bits *)p)->flag == 1) {
        v = *(u16 *)((u8 *)p + 0xAD00);
        if (v == 0 || v > 0x1ED) {
            return FALSE;
        }
    }
    return TRUE;
}
