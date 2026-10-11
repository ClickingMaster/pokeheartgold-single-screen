#include "../include/overlay18_dex_window_private.h"

void PokedexEntryPopup_RemoveWindows(Ov18Work *work) {
    u32 i;
    for (i = 0; i < 9; i++) {
        RemoveWindow(&work->windows[i]);
    }
}

void PokedexEntryPopup_InitObjCharTransfer(Ov18Work *work) {
    ObjCharTransferTemplate template = sEntryPopupObjCharTransferTemplate;
    template.heapID = work->heapId;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_64K, (GXOBJVRamModeChar)16);
}

void PokedexEntryPopup_DestroyObjCharTransfer(void) {
    ObjCharTransfer_Destroy();
}

void PokedexEntryPopup_CreateResourceManagers(Ov18Work *work) {
    int i;
    for (i = 0; i < 4; i++) {
        work->gfxManagers[i] = Create2DGfxResObjMan(8, i, work->heapId);
    }
}
