#pragma force_active on
#include "global.h"
#include "gf_gfx_loader.h"
void sub_0200EC84(BgConfig *a, u32 b, u16 c, BOOL d, u16 id, u32 heap);
// @fn sub_0200EC84
void sub_0200EC84(BgConfig *a, u32 b, u16 c, BOOL d, u16 id, u32 heap) {
    if (d == 0) id += 33; else id += 2;
    GfGfxLoader_LoadCharData((NarcId)36, id, a, (GFBgLayer)b, c, 768, FALSE, (enum HeapID)heap);
}
