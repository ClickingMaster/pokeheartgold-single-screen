#include "to57_overlay08_bank14_private.h"
#pragma force_active on

void BattlePartyMenu_RemoveWindows(void *);

void BattlePartyMenu_RemoveWindows(void *work) {
    Ov08R2W11Work *w = work;
    u32 i;
    WindowArray_Delete(w->windowArray, w->windowCount);
    for (i = 0; i < 2; i++) RemoveWindow(&w->windows[i]);
}
