#include "to50_overlay102_graphics_teardown_r14_internal.h"
#define PTR(base, off) (*(void **)((u8 *)(base) + (off)))
typedef struct Ov102GraphicsWork {
    u8 pad_000[0x150];
    NNSG2dImageProxy imageProxy[2];
    NNSG2dImagePaletteProxy paletteProxy[2];
    void *cellLoad[2];
    void *animLoad[2];
    void *cellBank[2];
    void *animBank[2];
} Ov102GraphicsWork;
void ov102_021E95E0(void *state, NARC *narc) {
    Ov102GraphicsWork *work = PTR(state, 0);
    int i;
    for (i = 0; i < 2; i++) {
        NNS_G2dInitImagePaletteProxy(&work->paletteProxy[i]);
        NNS_G2dInitImageProxy(&work->imageProxy[i]);
        GfGfxLoader_LoadImageMappingFromOpenNarc(narc, 9, TRUE, 0, 0, i + 1, 0, 0x23, &work->imageProxy[i]);
        GfGfxLoader_PartiallyLoadPaletteFromOpenNarc(narc, 10, (int)(i + 1), 0, 0x23, &work->paletteProxy[i]);
        work->cellBank[i] = GfGfxLoader_GetCellBankFromOpenNarc(narc, 7, TRUE, &work->cellLoad[i], 0x23);
        work->animBank[i] = GfGfxLoader_GetAnimBankFromOpenNarc(narc, 8, TRUE, &work->animLoad[i], 0x23);
    }
    sub_0203A964();
}

void ov102_021E9694(void *task, void *state) {
#pragma unused(task)
    u8 *p = state;
    u8 *work = PTR(p, 0);
    int i;
    u8 *walker;

    switch (*(u32 *)(p + 0x14)) {
    case 0:
        BeginNormalPaletteFade(0, 0, 0, 0, 5, 1, 0x23);
        (*(u32 *)(p + 0x14))++;
        return;
    case 1:
        if (!IsPaletteFadeFinished()) {
            return;
        }
        ov102_021EA380(PTR(PTR(p, 0), 0x1E0));
        ov102_021EB1E0(PTR(PTR(p, 0), 0x1E4));
        ov102_021EB6C8(PTR(PTR(p, 0), 0x1E8));
        ov102_021EBCBC(PTR(PTR(p, 0), 0x1EC));
        ov102_021EC240(PTR(PTR(p, 0), 0x1F0));
        ov102_021E937C(PTR(p, 0));
        for (i = 0, walker = work; i < 2; i++, walker += 4) {
            if (PTR(walker, 0x1D0) != NULL) {
                Heap_Free(PTR(walker, 0x1D0));
            }
            if (PTR(walker, 0x1D8) != NULL) {
                Heap_Free(PTR(walker, 0x1D8));
            }
        }
        FontID_Release(2);
        FreeBgTilemapBuffer(PTR(work, 0x20), 0);
        FreeBgTilemapBuffer(PTR(work, 0x20), 1);
        FreeBgTilemapBuffer(PTR(work, 0x20), 2);
        FreeBgTilemapBuffer(PTR(work, 0x20), 3);
        FreeBgTilemapBuffer(PTR(work, 0x20), 4);
        FreeBgTilemapBuffer(PTR(work, 0x20), 5);
        *(volatile u16 *)0x04000304 |= (u32)0x04000304 >> 11;
        ov102_021E94A4(p);
        return;
    }
}
