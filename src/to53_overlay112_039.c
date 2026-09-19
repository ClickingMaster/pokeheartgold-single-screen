#include "to53_overlay112_w09_private.h"


int ov112_021E9888(int index) {
    if (index >= 6) GF_AssertFail();
    return (int)TouchscreenHitbox_FindRectAtTouchNew(ov112_021FF514[index]);
}

void ov112_021E98A4(void *p) {
    void *narc = NARC_New(0xF9, 0x9A);
    ToggleBgLayer(5, 0);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x12, *(void **)((u8 *)p + 0x18), 5, 0, 0, 1, 0x9A);
    ToggleBgLayer(5, 1);
    NARC_Delete(narc);
}

void ov112_021E98E8(void *unused, int enabled) { ToggleBgLayer(5, enabled); }
