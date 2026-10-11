#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = PokewalkerApp_StartMessageBox(p, a, b); \
        return next;                                               \
    } while (0)


int ov112_021ECF38(void *p) {
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 4);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 5);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 1);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 2);
    PokewalkerApp_HideSprite(p, 3);
    PokewalkerApp_HideSprite(p, 4);
    ov112_021ED2F4(p);
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 0, 7);
    return 17;
}

int ov112_021ECF88(void *p) {
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)p + 0x1E524)) &&
        *(u16 *)((u8 *)p + 0x1F2D6) == 31 && *(u16 *)((u8 *)p + 0x1F2E0) == 256) {
        PokewalkerApp_LoadTilemap(p, 2, 14);
        PokewalkerApp_PrintExit(p, 1);
        return 24;
    }
    return 17;
}

int ov112_021ECFD4(void *p) {
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 4);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 5);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 1);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 2);
    PokewalkerApp_HideSprite(p, 3);
    PokewalkerApp_HideSprite(p, 4);
    ov112_021ED0D0(p);
    ov112_021EC460(p, 2, 16, *(u32 *)((u8 *)p + 0xC));
    return 64;
}

int ov112_021ED020(void *p) {
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 4);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 5);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 1);
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 2);
    ov112_021EC460(p, 2, 16, *(u32 *)((u8 *)p + 0xC));
    return 64;
}

int ov112_021ED054(void *p) {
    BgClearTilemapBufferAndCommit(*(void **)((u8 *)p + 0x18), 1);
    PokewalkerApp_LoadTilemap(p, 2, 14);
    PokewalkerApp_PrintExit(p, 1);
    *(void **)((u8 *)p + 0x1E524) = PokewalkerApp_StartMessageBox(p, 0, 5);
    ov112_021ED2F4(p);
    return 21;
}

int ov112_021ED090(void *p) {
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)p + 0x1E524)) &&
        *(u16 *)((u8 *)p + 0x1F2D6) == 31 && *(u16 *)((u8 *)p + 0x1F2E0) == 256) {
        return 30;
    }
    return 21;
}
