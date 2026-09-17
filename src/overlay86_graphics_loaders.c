#include "global.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay86_graphics_loaders_private.h"

u32 ov86_021E5E98(u32 kind) {
    u32 result = 0;
    switch (kind) {
    case 2:
    case 3:
        result = 0;
        break;
    case 5:
        result = 1;
        break;
    case 4:
        result = 2;
        break;
    case 6:
        result = 3;
        break;
    case 1:
        result = 4;
        break;
    }
    return result;
}

void ov86_021E5ECC(Overlay86GfxState *state, GFBgLayer layer) {
    NNSG2dScreenData *screenData;
    NARC *narc = NARC_New(NARC_a_1_8_3, HEAP_ID_121);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x8C, state->bgConfig, layer, 0, 0, TRUE, HEAP_ID_121);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x60, HEAP_ID_121);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x8B, state->bgConfig, layer, 0, 0, TRUE, HEAP_ID_121);
    void *screen = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 0x8F, TRUE, &screenData, HEAP_ID_121);
    MIi_CpuCopy16((const u16 *)screenData->rawData, (u16 *)state->screenData, 0x60);
    Heap_Free(screen);
    NARC_Delete(narc);
}

void ov86_021E5F58(Overlay86GfxState *state, GFBgLayer layer) {
    NARC *narc = NARC_New(NARC_a_1_8_3, HEAP_ID_121);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x8E, state->bgConfig, layer, 0, 0, TRUE, HEAP_ID_121);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x8D, state->bgConfig, layer, 0, 0, TRUE, HEAP_ID_121);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_121);
    NARC_Delete(narc);
}
