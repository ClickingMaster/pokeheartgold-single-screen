#include "to50_overlay99_r9_r6_private.h"

void ov99_021E856C(Ov99R9R6Work *work) {
    volatile u8 *base = (volatile u8 *)0x027e0000;
    SpriteSystem_TransferOam(work);
    DoScheduledBgGpuUpdates(work->bgConfig);
    *(volatile u32 *)(base + 0x3ff8) |= 1;
}

void ov99_021E8590(Ov99R9R6Work *work) {
    work->bgConfig = BgConfig_Alloc(work->heapId);
    {
        GraphicsModes modes = ov99_021EA374;
        SetBothScreensModesAndDisable(&modes);
    }
    {
        Ov99BgTemplates templates = ov99_021EA4F4;
        int i;
        for (i = 0; i < 6; i++) {
            InitBgFromTemplate(work->bgConfig, ov99_021EA59C[i], &templates.templates[i], 0);
            BgClearTilemapBufferAndCommit(work->bgConfig, ov99_021EA59C[i]);
            BG_ClearCharDataRange(ov99_021EA59C[i], 0x20, 0, work->heapId);
        }
    }
    work->narc = NARC_New((NarcId)0xb1, work->heapId);
}

void ov99_021E860C(Ov99R9R6Work *work) {
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 0x33, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x34, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x35, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x37, work->bgConfig, GF_BG_LYR_MAIN_2, 1, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 0x33, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x34, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x36, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x37, work->bgConfig, GF_BG_LYR_SUB_2, 1, 0, TRUE, work->heapId);
}
