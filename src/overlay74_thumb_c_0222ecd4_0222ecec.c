#include "overlay74_thumb_w07_private.h"

void ov74_0222ECD4(void *window, int copy) {
    if (WindowIsInUse(window) == TRUE) {
        EraseFrameAndWindow1(window, copy);
    }
}
void ov74_0222ECEC(void *window, int copy) {
    if (WindowIsInUse(window) == TRUE) {
        ClearFrameAndWindow2(window, copy);
    }
}
