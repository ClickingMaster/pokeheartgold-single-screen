#include "to58_overlay74_010_private.h"

extern void ClearFrameAndWindow2(void *window, int);
extern void ClearWindowTilemapAndCopyToVram(void *window);
extern void RemoveWindow(void *window);

int ov74_022319F8(u8 *base, void *string, int flags) {
    if (flags & 1) {
        int width = FontID_String_GetWidth(1, string, 0);
        return ((*(int *)(base + 0x10) * 8) - width) / 2;
    }
    return *(int *)(base + 0x18);
}
