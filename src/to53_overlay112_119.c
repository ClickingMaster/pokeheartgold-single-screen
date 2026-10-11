#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = PokewalkerApp_StartMessageBox(p, a, b); \
        return next;                                               \
    } while (0)


int ov112_021EE0F4(void *p) {
    LOAD_PRINTER(p, 2, 20, 40);
}

int ov112_021EE10C(void *p) {
    return PRINTER_STATE(p, 62, 40);
}

int ov112_021EE12C(void *p) {
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 2, 29);
    FillBgTilemapRect(*(void **)((u8 *)p + 0x18), 2, 0, 0, 0, 32, 24, 16);
    BgCommitTilemapBufferToVram(*(void **)((u8 *)p + 0x18), 2);
    PokewalkerIr_Stop();
    PlayFanfare(0x4A1);
    return 42;
}

int ov112_021EE178(void *p) {
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)p + 0x1E524)) && !IsFanfarePlaying()) {
        *(u32 *)((u8 *)p + 4) = 6;
        return 66;
    }
    return 42;
}

int ov112_021EE1A4(void *p) {
    LOAD_PRINTER(p, 2, 18, 44);
}

int ov112_021EE1BC(void *p) {
    return PRINTER_STATE(p, 45, 44);
}

int ov112_021EE1DC(void *p) {
    PokewalkerApp_ShowYesNoMenu(p, 1);
    return 46;
}
