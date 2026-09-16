#include "slop_r1_overlay98_module11_internal.h"

void ov98_0221EA4C(ManagedSprite *sprite, NarcId narcId, s32 memberNo, BOOL isMain, BOOL grayscale, enum HeapID heapId) {
    NNS_G2D_VRAM_TYPE vramType;
    enum GFPalLoadLocation location;
    u32 palLocation;

    if (isMain) {
        vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        location = GF_PAL_LOCATION_MAIN_OBJ;
    } else {
        vramType = NNS_G2D_VRAM_TYPE_2DSUB;
        location = GF_PAL_LOCATION_SUB_OBJ;
    }
    palLocation = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite->sprite), vramType);
    GfGfxLoader_GXLoadPal(narcId, memberNo, location, (enum GFPalSlotOffset)palLocation, 0x20, heapId);
    if (grayscale) {
        u16 *palette;
        if (isMain) {
            palette = (u16 *)(HW_OBJ_PLTT + palLocation);
        } else {
            palette = (u16 *)(HW_DB_OBJ_PLTT + palLocation);
        }
        TintPalette_GrayScale(palette, 0x20);
    }
}
