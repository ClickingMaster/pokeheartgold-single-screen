#include "slop_r1_module_05_private.h"

void PokedexEntryPopup_StartFadeIn(SlopR1Module05Work *work) {
    PaletteData_BeginPaletteFade(work->paletteData, PLTTBUF_MAIN_BG_F | PLTTBUF_MAIN_OBJ_F, 0xFFFF, 1, 16, 0, RGB_BLACK);
    Pokepic_StartPaletteFade(work->pokepic, 16, 0, 0, RGB_BLACK);
    PaletteData_SetAutoTransparent(work->paletteData, FALSE);
}
