#include "trainer_card_touch_input_private.h"

int ov51_021E6AAC(u8 *state, int *changed) {
    if (!System_GetTouchNew()) {
        return 0;
    }
    if (TouchscreenHitbox_TouchNewIsIn(_021E7DB8)) {
        *changed = 1;
        PlaySE(0x940);
        return 5;
    }
    if (*(void **)(state + 0x30F4) != NULL && (((u32)state[0x343A] << 30) >> 31) == 0 && TouchscreenHitbox_TouchNewIsIn(ov51_021E7DBC)) {
        *changed = 1;
        PlaySE(0x5DC);
        return 4;
    }
    if (TouchscreenHitbox_TouchNewIsIn(ov51_021E7DC0)) {
        state[0x3440] = *(u16 *)((u8 *)&gSystem + 0x60);
        state[0x3441] = *(u16 *)((u8 *)&gSystem + 0x62);
        *changed = 1;
        return 3;
    }
    return 0;
}
