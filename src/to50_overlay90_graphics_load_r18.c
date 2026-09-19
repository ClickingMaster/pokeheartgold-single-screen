#include "to50_overlay90_graphics_load_r18_private.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
void ov90_0225A850(void *work, int heapId) {
    void *narc = NARC_New(0xC8, heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, 0, 0, 0x120, heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, 4, 0, 0x120, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, PTR(work, 0x4C), 1, 0, 0, 0, heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 6, PTR(work, 0x4C), 5, 0, 0x4000, 0, heapId);

    PTR(work, 0x254) = GfGfxLoader_GetScrnDataFromOpenNarc(narc, 9, 0, (void **)((u8 *)work + 0x258), heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, PTR(work, 0x4C), 3, 0, 0, 0, heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 8, PTR(work, 0x4C), 5, 0, 0, 0, heapId);
    ov90_0225A65C(narc, U8(work, 7), heapId);
    PTR(work, 0x1E4) = ov90_02258E54((u8 *)work + 0x1CC, narc, 11, 5, 12, 13, 14, 0x1388, heapId);
    ov90_0225B254(work, narc, heapId);
    ov90_0225B59C((u8 *)work + 0x208, (u8 *)work + 0x4C, (u8 *)work + 0xA0, (u8 *)work + 0x1CC, narc, heapId);
    NARC_Delete(narc);
}
