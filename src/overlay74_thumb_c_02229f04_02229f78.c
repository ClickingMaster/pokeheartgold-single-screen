#include "overlay74_thumb_w06_private.h"

void ov74_02229F04(void) {
    ov74_0223563C();
    ov74_02235690();
    ov74_02235728(0x71, 0xE, 0xB, 0xD, 0xC, 0);
}
void ov74_02229F28(void *p, int value) {
    if (value != -1) {
        PTR(p, 0x5C4) = ov74_02235930(0, PTR(p, 0x5C4), 0x80, 0x64, value);
    } else {
        Sprite_SetDrawFlag(PTR(p, 0x5C4), 0);
    }
}
void ov74_02229F60(void *window, int arg) {
    if (WindowIsInUse(window) == 1) {
        EraseFrameAndWindow1(window, arg);
    }
}
void ov74_02229F78(void *window, int arg) {
    if (WindowIsInUse(window) == 1) {
        ClearFrameAndWindow2(window, arg);
    }
}
