#include "overlay_18_dex_search_private.h"

OverlayManager *PokedexEntryPopup_Create(UnkStruct_50C *unkStruct) {
    PokedexEntryPopup *popup = Heap_Alloc(unkStruct->heapID, sizeof(PokedexEntryPopup));
    memset(popup, 0, sizeof(PokedexEntryPopup));
    memcpy(popup, unkStruct, sizeof(UnkStruct_50C));
    popup->gfxNarc = NARC_New(NARC_graphic_zukan_gra, popup->heapId);
    popup->unk_240 = 0;
    popup->unk_254 = 0;
    popup->task = SysTask_CreateOnMainQueue(PokedexEntryPopup_Task, popup, 0);
    return (OverlayManager *)popup;
}
