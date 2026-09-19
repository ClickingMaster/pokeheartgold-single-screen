#include "global.h"

#include "bg_window.h"

typedef struct LocalScreenData {
    u32 width;
    volatile u32 height;
    u32 size;
    void *rawData;
} LocalScreenData;

void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void GfGfxLoader_GXLoadPalFromOpenNarc(void *, int, int, int, int, int);
void *GfGfxLoader_GetScrnDataFromOpenNarc(void *, int, int, void *, int);
void Heap_Free(void *);

void ov81_02243100(BgConfig *bgConfig, void *narc);
void ov81_02243140(BgConfig *bgConfig, void *narc, int kind);
void ov81_022431E0(BgConfig *bgConfig, int kind);

void ov81_02243100(BgConfig *bgConfig, void *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x86, bgConfig, 5, 0x3D9, 0, 1, 100);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC2, 4, 0x120, 0x40, 100);
}
