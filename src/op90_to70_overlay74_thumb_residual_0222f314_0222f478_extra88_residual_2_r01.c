#pragma force_active on
#include "global.h"
#include "gf_gfx_loader.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
typedef struct Party74 {
    u8 unk000[0x34A];
    u16 species[3];
} Party74;
typedef struct Work74 {
    enum HeapID heapId;
    u8 unk0004[0x2BB0];
    Party74 *party;
    u8 unk2BB8[0x1CC];
    Sprite *sprites[3];
} Work74;
u32 ov74_0223567C(void);
void ov74_0222EC60(Work74 *work);
void ov74_0223563C(void);
void ov74_02235690(void);
void ov74_02235728(int, int, int, int, int, int);

Sprite *ov74_02235930(int, Sprite *, int, int, int);
void ov74_0222F478(Work74 *work);
// @fn ov74_0222F478
void ov74_0222F478(Work74 *work) {
    int y;
    int i;
    int species;
    void *raw;
    NNSG2dCharacterData *charData;
    if (work->sprites[0] == NULL && work->sprites[1] == NULL && work->sprites[2] == NULL) {
        if (ov74_0223567C() == 1) {
            ov74_0222EC60(work);
        }
        ov74_0223563C();
        ov74_02235690();
        ov74_02235728(113, 30, 27, 29, 28, 0);
        GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_MAIN_OBJ, (enum GFPalSlotOffset)0x60, 0, work->heapId);
    }
    y = 178;
    for (i = 0; i < 3; i++, y += 25) {
        species = work->party->species[i];
        if (species == 0) {
            if (work->sprites[i] != NULL) {
                Sprite_SetDrawFlag(work->sprites[i], FALSE);
            }
        } else {
            work->sprites[i] = ov74_02235930(0, work->sprites[i], y, 16, i + 10);
            raw = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(species, FALSE, 0), FALSE, &charData, work->heapId);
            DC_FlushRange(charData->pRawData, 0x200);
            GX_LoadOBJ(charData->pRawData, (100 + i * 16) * 32, 0x200);
            Sprite_SetPaletteOverride(work->sprites[i], GetMonIconPaletteEx(species, 0, FALSE) + 3);
            Heap_Free(raw);
        }
    }
}
