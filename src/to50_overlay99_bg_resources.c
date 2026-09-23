#include "to50_overlay99_bg_resources_private.h"
void ov99_021E7DA4(Ov99BgWork *w)
{
    GfGfxLoader_GXLoadPalFromOpenNarc(w->narc, 0x2A, 0, 0, 0x40, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x2C, w->bgConfig, 0, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc, 0x2E, w->bgConfig, 0, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x2C, w->bgConfig, 2, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc, 0x2F, w->bgConfig, 2, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x30, w->bgConfig, 3, 1, 0, 1, w->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(w->narc, 0x2B, 4, 0, 0x40, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x2D, w->bgConfig, 4, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc, 0x31, w->bgConfig, 4, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(w->narc, 0x2D, w->bgConfig, 6, 0, 0, 1, w->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc, 0x32, w->bgConfig, 6, 0, 0, 1, w->heapId);
}
