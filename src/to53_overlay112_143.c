#include "to53_overlay112_w14_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                              \
    do {                                                         \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                             \
    } while (0)


int ov112_021EF62C(void *p) {
    PokewalkerApp_Setup(p);
    *(u32 *)((u8 *)p + 8) = 0;
    *(u32 *)((u8 *)p + 4) = 10;
    return 1;
}

int ov112_021EF640(void) { return 2; }

int ov112_021EF644(void *p) {
    if (*(u32 *)(gSystem + 0x48) & 0x80) {
        *(u32 *)((u8 *)p + 4) = 3;
        return 3;
    }
    return 2;
}

int ov112_021EF660(void) { return 1; }

int ov112_021EF664(void *p) {
    ov112_021E9C10(p, 1);
    ov112_021E9A78(p, 4);
    return 2;
}
