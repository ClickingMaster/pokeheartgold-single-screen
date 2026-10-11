#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = PokewalkerApp_StartMessageBox(p, a, b); \
        return next;                                               \
    } while (0)


int ov112_021EDD08(void *p) {
    PokewalkerApp_HideCursor(p);
    *(u16 *)((u8 *)p + 0x14) = ov112_021EDCB0(*(u16 *)((u8 *)p + 0x14));
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 2, *(u16 *)((u8 *)p + 0x14));
    return 34;
}

int ov112_021EDD30(void *p) {
    PokewalkerApp_HideCursor(p);
    *(u16 *)((u8 *)p + 0x14) = ov112_021EDCB0(*(u16 *)((u8 *)p + 0x14));
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 2, *(u16 *)((u8 *)p + 0x14));
    *(u32 *)((u8 *)p + 0xC) = 66;
    return 34;
}

int ov112_021EDD5C(void *p) {
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 1);
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 0, 7);
    return 17;
}

int ov112_021EDD80(void *p) {
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 1);
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 0, 5);
    PokewalkerApp_LoadTilemap(p, 2, 14);
    PokewalkerApp_PrintExit(p, 1);
    return 21;
}

int ov112_021EDDB4(void *p) {
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 1);
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 0, 5);
    PokewalkerApp_LoadTilemap(p, 2, 14);
    PokewalkerApp_PrintExit(p, 1);
    return 2;
}

int ov112_021EDDE8(void *p) {
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 1);
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 0, 5);
    PokewalkerApp_LoadTilemap(p, 2, 14);
    PokewalkerApp_PrintExit(p, 1);
    return 23;
}
