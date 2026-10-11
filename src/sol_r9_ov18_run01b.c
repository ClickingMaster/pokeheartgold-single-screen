#include "sol_r9_ov18_private.h"

void PokedexApp_SetupListWindows(void *work) {
    u32 first;
    u32 second;
    PokedexApp_AddWindows(work, sListWindowTemplates, 0x14);
    ov18_021EE508(work, 0, 0);
    ov18_021EE508(work, 1, 1);
    PokedexApp_DrawSeenAndCaughtCounts(work);
    first = PokedexApp_GetListCursorIndex(work);
    second = PokedexApp_GetCursorSpecies(work);
    PokedexApp_DrawListEntryHeader(work, second, first);
    PokedexApp_DrawListEntryDetails(work, second, first);
}
