#include "global.h"

#include "filesystem.h"
#include "pokemon.h"
#include "unk_02013FDC.h"

typedef struct GraphicsBanks {
    u32 bank[10];
} GraphicsBanks;

typedef struct TouchBox {
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
} TouchBox;

extern u8 gSystem[];
extern const u8 ov112_021FF2DC[];
extern const GraphicsBanks ov112_021FF370;
extern const void *ov112_021FF244[];
extern const u8 ov112_021FF2CC[];

NNSG2dImageProxy *SpriteTransfer_GetCharProxy(void *);
NNSG2dImagePaletteProxy *SpriteTransfer_GetPaletteProxy(void *, NNSG2dImageProxy *);
void GXS_LoadOBJ(const void *, u32, u32);
void GXS_LoadOBJPltt(const void *, u32, u32);
void Main_SetVBlankIntrCB(void *, void *);
void HBlankInterruptDisable(void);
void GfGfx_DisableEngineAPlanes(void);
void GfGfx_DisableEngineBPlanes(void);
void GfGfx_SetBanks(const GraphicsBanks *);
int SpeciesToOverworldModelIndexOffset(int);
BOOL OverworldModelLookupHasFemaleForm(int);
int OverworldModelLookupFormCount(int);
BOOL TouchscreenHitbox_TouchNewIsIn(const TouchBox *);
void PlaySE(int);
void MI_CpuCopy8(const void *, void *, u32);
void Heap_Free(void *);

void PokewalkerBoxDisplay_GetMonPictureLocation(void *);
void PokewalkerBoxDisplay_LoadMonPicture(void *, const void *);
void PokewalkerBoxDisplay_TransferMonPicture(void *);
void ov112_021F0D48(void *);
void ov112_021F0DC0(void);
void ov112_021F0DF4(void);
int ov112_021F0E14(int, int, int);
BOOL ov112_021F0E60(void *);
BOOL ov112_021F0EB4(void *);
BOOL ov112_021F0EFC(void *);

void PokewalkerBoxDisplay_GetMonPictureLocation(void *vp) {
    u8 *p = vp;
    void *paletteResource = *(void **)(p + 0x164);
    NNSG2dImageProxy *image = SpriteTransfer_GetCharProxy(*(void **)(p + 0x160));
    NNSG2dImagePaletteProxy *palette = SpriteTransfer_GetPaletteProxy(paletteResource, image);
    *(u32 *)(p + 0x1dc) = NNS_G2dGetImageLocation(image, NNS_G2D_VRAM_TYPE_2DSUB);
    *(u32 *)(p + 0x1e0) = NNS_G2dGetImagePaletteLocation(palette, NNS_G2D_VRAM_TYPE_2DSUB);
}

typedef struct MonImageDesc {
    u32 personality;
    u16 species;
    u8 shiny;
    u8 form;
    u8 gender;
} MonImageDesc;

void PokewalkerBoxDisplay_LoadMonPicture(void *vp, const void *descv) {
    u8 *p = vp;
    const MonImageDesc *desc = descv;
    PokepicTemplate tmpl;
    NNSG2dPaletteData *palette;
    void *raw;
    GetMonSpriteCharAndPlttNarcIdsEx(&tmpl, desc->species, desc->gender, 2, desc->shiny, desc->form, desc->personality);
    sub_02014540((NarcId)tmpl.narcID, tmpl.charDataID, *(enum HeapID *)p, *(void **)(p + 0x1b8), desc->personality, FALSE, 2, desc->species);
    raw = AllocAndReadWholeNarcMemberByIdPair((NarcId)tmpl.narcID, tmpl.palDataID, *(enum HeapID *)p);
    if (NNS_G2dGetUnpackedPaletteData(raw, &palette) != TRUE) {
        GF_AssertFail();
    }
    MIi_CpuCopy16(palette->pRawData, (u16 *)(p + 0x1bc), 0x20);
    Heap_Free(raw);
}

void PokewalkerBoxDisplay_TransferMonPicture(void *vp) {
    u8 *p = vp;
    DC_FlushRange(*(void **)(p + 0x1b8), 0xc80);
    GXS_LoadOBJ(*(void **)(p + 0x1b8), *(u32 *)(p + 0x1dc), 0xc80);
    DC_FlushRange(p + 0x1bc, 0x20);
    GXS_LoadOBJPltt(p + 0x1bc, *(u32 *)(p + 0x1e0), 0x20);
}

typedef struct RowInfo {
    u16 offset;
    u8 length;
    u8 checksum;
} RowInfo;
