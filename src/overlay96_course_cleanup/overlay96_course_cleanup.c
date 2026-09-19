#include "to53_overlay96_r48_private.h"

BOOL ov96_021EF19C(void *course) {
    Ov96R48Work *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;

    NARC_Delete(work->narc);
    sub_0203A914();
    ov96_021EE5E0(work->unk34);
    ov98_0221EB84(work->message, 12);
    for (i = 0; i < 7; i++) {
        FreeBgTilemapBuffer(work->bgConfig, ov96_0221BA18[i]);
    }
    Heap_Free(work->bgConfig);
    for (i = 0; i < 4; i++) {
        if (work->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(work->sprites[i]);
            work->sprites[i] = NULL;
        }
    }
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    Heap_Destroy((enum HeapID)0x88);
    return TRUE;
}

void ov96_021EF23C(Ov96R48Work *work) {
    vu32 *base = (vu32 *)0x027E0000;

    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(work->bgConfig);
    base[0x3FF8 / sizeof(*base)] |= 1;
}

void ov96_021EF260(void) {
    GraphicsBanks banks = ov96_0221BAC0;
    GfGfx_SetBanks(&banks);
}

BOOL ov96_021EF280(u32 variable, s16 threshold) {
    s16 value;
    sub_02005B78(GF_GetCurrentPlayingBGM(), variable, &value);
    return value >= threshold;
}
