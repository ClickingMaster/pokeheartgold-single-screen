#include "to50_overlay99_r9_r7_palette_private.h"

void ov99_021E87D0(Ov99R9R7Work *work) {
    u8 palette;
    u32 srcOffset;

    if (work->unkB0 != NULL) {
        palette = 2;
    } else {
        palette = 1;
    }
    srcOffset = (u8)(palette << 5);
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(work->narc, 0x33, GF_PAL_LOCATION_MAIN_BG, srcOffset, (enum GFPalSlotOffset)0x20, 0x20, work->heapId);
    GfGfxLoader_GXLoadPalWithSrcOffsetFromOpenNarc(work->narc, 0x33, GF_PAL_LOCATION_SUB_BG, srcOffset, (enum GFPalSlotOffset)0x20, 0x20, work->heapId);
}
