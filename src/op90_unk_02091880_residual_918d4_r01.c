#pragma force_active on
#include "easy_chat_menu_private.h"
extern const ECMenuSpec sECMenuSpecs[12];
void sub_020918D4(ECMenu *menu);
// @fn sub_020918D4
void sub_020918D4(ECMenu *menu) {
    int i;
    u32 total;
    u32 n;

    i = 0;
    total = 0;
    for (; i < 12; i++) {
        menu->offsets[i] = total;
        n = sECMenuSpecs[i].func(menu, sECMenuSpecs[i].words, sECMenuSpecs[i].count, &menu->words[total]);
        menu->counts[i] = n;
        total += n;
    }
}
