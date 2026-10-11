#include "to45_overlay96_r82_private.h"

void ov96_02201E10(void *work, u32 index) {
    GfGfxLoader_LoadCharData(0xdb, 1, PTR_AT(work, 0), 1, 0, 0x4000, 0, U32_AT(work, 0x44));
    GfGfxLoader_LoadScrnData(0xdb, 2, PTR_AT(work, 0), 1, 0, 0, 0, U32_AT(work, 0x44));
    GfGfxLoader_GXLoadPal(0xdb, 0, 0, 0, 0x20, U32_AT(work, 0x44));
    ov96_02203A64(PTR_AT(work, 0x5e0), index);
}

void ov96_02201E70(void *p) {
    PokeathlonCourseSpritePool_LoadChar(p, 0xdb, 0x15, 0x6a, 3);
    PokeathlonCourseSpritePool_LoadPltt(p, 0xdb, 0x12, 0x6a, 3, 2);
    PokeathlonCourseSpritePool_LoadCell(p, 0xdb, 0x14, 0x6a);
    PokeathlonCourseSpritePool_LoadAnim(p, 0xdb, 0x13, 0x6a);
    PokeathlonCourseSpritePool_LoadChar(p, 0xdb, 0x11, 0x69, 1);
    PokeathlonCourseSpritePool_LoadPltt(p, 0xdb, 0xe, 0x69, 1, 3);
    PokeathlonCourseSpritePool_LoadCell(p, 0xdb, 0x10, 0x69);
    PokeathlonCourseSpritePool_LoadAnim(p, 0xdb, 0xf, 0x69);
}
