#include "to56_overlay08_bank24_private.h"
#pragma force_active on

int BattleBagMenu_StartFadeOut(u8 *work) {
    PaletteData_BeginPaletteFade(PTR(work, 8), 0xA, 0xFFFF, -8, 0, 0x10, 0);
    return 0xE;
}
