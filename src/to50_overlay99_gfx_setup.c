#include "to50_overlay99_gfx_setup_private.h"
void ov99_021E5D58(Ov99GfxWork *w)
{
    GfGfxLoader_GXLoadPalFromOpenNarc(w->narc, 0x1E, 0, 0, 0, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x1F, w->bgConfig, 1, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc, 0x20, w->bgConfig, 1, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x3E, w->bgConfig, 2, 0, 0, 1, w->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(w->narc, 0x24, 0, 0x1E0, 0x20, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x25, w->bgConfig, 3, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc, 0x26, w->bgConfig, 3, 0, 0, 1, w->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(w->narc, 0x21, 4, 0, 0, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x22, w->bgConfig, 4, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc, 0x23, w->bgConfig, 4, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x44, w->bgConfig, 5, 0, 0, 1, w->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(w->narc, 0x27, 4, 0x1E0, 0x20, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x28, w->bgConfig, 7, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc, 0x29, w->bgConfig, 7, 0, 0, 1, w->heapId);
}
