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

void ov112_021F0C50(void *);
void ov112_021F0C8C(void *, const void *);
void ov112_021F0D04(void *);
void ov112_021F0D48(void *);
void ov112_021F0DC0(void);
void ov112_021F0DF4(void);
int ov112_021F0E14(int, int, int);
BOOL ov112_021F0E60(void *);
BOOL ov112_021F0EB4(void *);
BOOL ov112_021F0EFC(void *);

typedef struct MonImageDesc {
    u32 personality;
    u16 species;
    u8 shiny;
    u8 form;
    u8 gender;
} MonImageDesc;

typedef struct RowInfo {
    u16 offset;
    u8 length;
    u8 checksum;
} RowInfo;

void ov112_021F0DC0(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(volatile u32 *)0x04000000 &= 0xffffe0ff;
    *(volatile u32 *)0x04001000 &= 0xffffe0ff;
}

void ov112_021F0DF4(void) {
    GraphicsBanks banks = ov112_021FF370;
    GfGfx_SetBanks(&banks);
}

int ov112_021F0E14(int species, int form, int gender) {
    int result;
    if (species <= 0 || species > 493) {
        result = 1;
    } else {
        result = SpeciesToOverworldModelIndexOffset(species) + 297;
        if (OverworldModelLookupHasFemaleForm(species)) {
            if (gender == 1) {
                result++;
            }
        } else {
            if (form > OverworldModelLookupFormCount(species)) {
                form = 0;
            }
            result += form;
        }
    }
    return result;
}

BOOL ov112_021F0E60(void *vp) {
    u8 *p = vp;
    TouchBox box = *(const TouchBox *)(ov112_021FF2DC + 11);
    if (TouchscreenHitbox_TouchNewIsIn(&box) || (*(u32 *)(gSystem + 0x48) & 2)) {
        PlaySE(1500);
        ManagedSprite_ResetSpriteAnimCtrlState(*(void **)(p + 0x78));
        return TRUE;
    }
    return FALSE;
}

BOOL ov112_021F0EB4(void *vp) {
    u8 *p = vp;
    TouchBox box;
    if (p[0x13d] != 0) {
        box = *(const TouchBox *)(ov112_021FF2DC + 3);
        if (TouchscreenHitbox_TouchNewIsIn(&box) || (*(u32 *)(gSystem + 0x48) & 0x20)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ov112_021F0EFC(void *vp) {
    u8 *p = vp;
    TouchBox box;
    if (p[0x13d] < p[0x13e] - 1) {
        box = *(const TouchBox *)(ov112_021FF2DC + 7);
        if (TouchscreenHitbox_TouchNewIsIn(&box) || (*(u32 *)(gSystem + 0x48) & 0x10)) {
            return TRUE;
        }
    }
    return FALSE;
}
