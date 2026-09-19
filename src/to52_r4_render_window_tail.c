#include "to52_r4_render_window_tail_private.h"

void sub_0200F748(void *owner, W16PicTemplate *t) {
    W16Rect rect1;
    W16Rect rect2;
    void *buf;
    void *res;
    void *image;
    u32 location;

    buf = Heap_Alloc(*(u16 *)((u8 *)owner + 0x162), 0x1900);
    rect1 = _020F5C50;
    sub_020143E0(t->narcID, t->charDataID, *(u16 *)((u8 *)owner + 0x162), &rect1, buf);
    rect2 = _020F5C40;
    sub_020143E0(t->narcID, t->charDataID, *(u16 *)((u8 *)owner + 0x162), &rect2, (u8 *)buf + 0xc80);
    res = SpriteResourceCollection_Find(*(void **)((u8 *)owner + 0x130), 0x15CD5);
    image = SpriteTransfer_GetCharProxy(res);
    location = NNS_G2dGetImageLocation(image, NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(buf, 0x1900);
    GX_LoadOBJ(buf, location, 0x1900);
    Heap_Free(buf);
    buf = sub_02014450(t->narcID, t->palDataID, *(u16 *)((u8 *)owner + 0x162));
    res = SpriteResourceCollection_Find(*(void **)((u8 *)owner + 0x134), 0x15CD5);
    image = SpriteTransfer_GetPaletteProxy(res, image);
    location = NNS_G2dGetImagePaletteLocation(image, NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(buf, 0x20);
    GX_LoadOBJPltt(buf, location, 0x20);
    Heap_Free(buf);
}

void sub_0200F82C(RenderPokemonPicState *s, u8 palette, u16 tile) {
#define BG    (*(void **)((u8 *)s + 0x168))
#define LAYER (*(u8 *)((u8 *)s + 0x16c))
#define X     (*(u8 *)((u8 *)s + 0x16d))
#define Y     (*(u8 *)((u8 *)s + 0x16e))
    FillBgTilemapRect(BG, LAYER, tile, (u8)(X - 1), (u8)(Y - 1), 1, 1, palette);
    FillBgTilemapRect(BG, LAYER, tile + 1, X, (u8)(Y - 1), 10, 1, palette);
    FillBgTilemapRect(BG, LAYER, tile + 2, (u8)(X + 10), (u8)(Y - 1), 1, 1, palette);
    FillBgTilemapRect(BG, LAYER, tile + 4, X, Y, 10, 10, palette);
    FillBgTilemapRect(BG, LAYER, tile + 3, (u8)(X - 1), Y, 1, 10, palette);
    FillBgTilemapRect(BG, LAYER, tile + 5, (u8)(X + 10), Y, 1, 10, palette);
    FillBgTilemapRect(BG, LAYER, tile + 6, (u8)(X - 1), (u8)(Y + 10), 1, 1, palette);
    FillBgTilemapRect(BG, LAYER, tile + 7, X, (u8)(Y + 10), 10, 1, palette);
    FillBgTilemapRect(BG, LAYER, tile + 8, (u8)(X + 10), (u8)(Y + 10), 1, 1, palette);
    ScheduleBgTilemapBufferTransfer(BG, LAYER);
}

void sub_0200F9DC(RenderPokemonPicState *s) {
#define BG2    (*(void **)((u8 *)s + 0x168))
#define LAYER2 (*(u8 *)((u8 *)s + 0x16c))
#define X2     (*(u8 *)((u8 *)s + 0x16d))
#define Y2     (*(u8 *)((u8 *)s + 0x16e))
    FillBgTilemapRect(BG2, LAYER2, 0, (u8)(X2 - 1), (u8)(Y2 - 1), 12, 12, 0);
    ScheduleBgTilemapBufferTransfer(BG2, LAYER2);
}
