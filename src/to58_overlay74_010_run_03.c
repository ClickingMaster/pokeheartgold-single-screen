#include "to58_overlay74_010_private.h"

extern void ClearFrameAndWindow2(void *window, int);
extern void ClearWindowTilemapAndCopyToVram(void *window);
extern void RemoveWindow(void *window);

void ov74_02232DA4(void *window) {
    if (*(u32 *)window != 0) {
        ClearFrameAndWindow2(window, 0);
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
}
