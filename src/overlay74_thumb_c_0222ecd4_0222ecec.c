#include "overlay74_thumb_w07_private.h"

void ov74_0222ECD4(void *window, int copy) {
    if (WindowIsInUse(window) == TRUE) {
        sub_0200E5D4(window, copy);
    }
}
void ov74_0222ECEC(void *window, int copy) {
    if (WindowIsInUse(window) == TRUE) {
        ClearFrameAndWindow2(window, copy);
    }
}
