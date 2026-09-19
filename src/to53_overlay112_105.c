#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


void ov112_021ED2F4(void *p) {
    ov112_021E98A4(p);
    *(void **)((u8 *)p + 0x1F2DC) = SysTask_CreateOnMainQueue(ov112_021ED180, p, 2);
}

void ov112_021ED314(int play) {
    if (play != 0 && !IsSEPlaying(0x934)) {
        PlaySE(0x934);
    }
}

int ov112_021ED330(void *p, int value) {
    if (ov112_021E7668()) {
        u16 old = *(u16 *)((u8 *)p + 0x1D760);
        if (old < value) {
            *(u16 *)((u8 *)p + 0x1D760) = value;
        } else {
            value = old;
        }
    } else {
        *(u16 *)((u8 *)p + 0x1D760) = 0;
    }
    return value;
}
