#pragma force_active on
#include "global.h"
#include "heap.h"
#include "gf_gfx_loader.h"
#include "nnsys/g2d/load/g2d_NCG_load.h"

typedef struct Ov89Tex {
    u8 pad0[0x14];
    u32 bitmapOffset;
    u8 pad18[0x38 - 0x18];
    u32 plttOffset;
} Ov89Tex;

typedef struct Ov89Item {
    u16 unk0;
    u16 unk2;
    u8 pad4[4];
    u8 unk8;
} Ov89Item;

u32 ov89_0225C88C(u32 value, int a, u32 b);
void ov89_0225AA24(Ov89Tex *tex, NARC *narc, NARC *itemNarc, Ov89Item *item, u32 mode);

// @fn ov89_0225AA24
void ov89_0225AA24(Ov89Tex *tex, NARC *narc, NARC *itemNarc, Ov89Item *item, u32 mode) {
    int shift;
    u8 *p;
    NNSG2dCharacterData *charData;
    u16 *d;
    u16 *dst;
    void *file;
    u32 kind;
    int k;
    int x;
    int y;
    int i;
    u8 *src;

    kind = ov89_0225C88C(item->unk2, item->unk8, mode);
    dst = (u16 *)((u8 *)tex + tex->bitmapOffset);
    if (kind == 1) {
        file = GfGfxLoader_LoadFromOpenNarc(itemNarc, item->unk2 + 3, TRUE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
    } else {
        file = GfGfxLoader_LoadFromOpenNarc(narc, 16, FALSE, HEAP_ID_125, TRUE);
        NNS_G2dGetUnpackedCharacterData(file, &charData);
    }
    MI_CpuClear16(dst, 0x40);
    src = charData->pRawData;
    p = src;
    d = dst;
    for (i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            switch (kind) {
            case 1:
                p = src + 0x80;
                break;
            default:
                p = src;
                break;
            }
            d = dst;
            break;
        case 1:
            switch (kind) {
            case 1:
                p = src + 0xA0;
                break;
            default:
                p = src + 0x20;
                break;
            }
            d = dst + 1;
            break;
        case 2:
            switch (kind) {
            case 1:
                p = src;
                break;
            default:
                p = src + 0x40;
                break;
            }
            d = dst + 0x10;
            break;
        case 3:
            switch (kind) {
            case 1:
                p = src + 0x20;
                break;
            default:
                p = src + 0x60;
                break;
            }
            d = dst + 0x11;
            break;
        }
        k = 0;
        for (y = 0; y < 8; y++) {
            for (shift = 0, x = 0; x < 4; x++, shift += 4, k++) {
                if (p[k] & 0xF) {
                    *d |= 1 << shift;
                }
                if (p[k] & 0xF0) {
                    *d |= 1 << (shift + 2);
                }
            }
            d += 2;
        }
    }
    *(u16 *)((u8 *)tex + tex->plttOffset + 2) = item->unk0;
    Heap_Free(file);
}
