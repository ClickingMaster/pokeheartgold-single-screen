#include "global.h"

#include "application/footprint_stamp.h"

#include "gf_gfx_loader.h"

// Built without -ipa file (Makefile), so it cannot share a translation unit with its neighbours.

#pragma force_active on

// Draws a footprint (the Pokemon's own, or the generic one) into the 16x16 2bpp texture of a stamp model and sets
// the stamp's ink color as palette color 1.
void FootprintStamp_DrawFootprintTexture(NNSG3dResTex *texture, NARC *narc, NARC *footprintNarc, FootprintStampMon *mon, u32 unk19E0) {
    int shift;
    u8 *src;
    NNSG2dCharacterData *charData;
    u16 *dst;
    u16 *bitmap;
    void *file;
    u32 type;
    int k;
    int x;
    int y;
    int i;
    u8 *chars;

    type = FootprintStamp_GetFootprintType(mon->species, mon->form, unk19E0);
    bitmap = (u16 *)((u8 *)texture + texture->texInfo.ofsTex);
    if (type == 1) {
        file = GfGfxLoader_LoadFromOpenNarc(footprintNarc, mon->species + 3, TRUE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
    } else {
        file = GfGfxLoader_LoadFromOpenNarc(narc, 16, FALSE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
    }
    MI_CpuClear16(bitmap, 0x40);
    chars = charData->pRawData;
    src = chars;
    dst = bitmap;
    // the four 8x8 tiles of the footprint, in the order top left, top right, bottom left, bottom right
    for (i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            switch (type) {
            case 1:
                src = chars + 0x80;
                break;
            default:
                src = chars;
                break;
            }
            dst = bitmap;
            break;
        case 1:
            switch (type) {
            case 1:
                src = chars + 0xA0;
                break;
            default:
                src = chars + 0x20;
                break;
            }
            dst = bitmap + 1;
            break;
        case 2:
            switch (type) {
            case 1:
                src = chars;
                break;
            default:
                src = chars + 0x40;
                break;
            }
            dst = bitmap + 0x10;
            break;
        case 3:
            switch (type) {
            case 1:
                src = chars + 0x20;
                break;
            default:
                src = chars + 0x60;
                break;
            }
            dst = bitmap + 0x11;
            break;
        }
        // 4bpp pixels -> 2bpp: any non-zero pixel becomes color 1
        k = 0;
        for (y = 0; y < 8; y++) {
            for (shift = 0, x = 0; x < 4; x++, shift += 4, k++) {
                if (src[k] & 0xF) {
                    *dst |= 1 << shift;
                }
                if (src[k] & 0xF0) {
                    *dst |= 1 << (shift + 2);
                }
            }
            dst += 2;
        }
    }
    *(u16 *)((u8 *)texture + texture->plttInfo.ofsPlttData + 2) = mon->color;
    Heap_Free(file);
}
