#include "overlay43_palpad_list_head_private.h"

static inline int Sum(int lhs, int rhs) {
    return lhs + rhs;
}

void ov43_0222CB34(s16 *position, void *context, void *resource, int index) {
    int y = position[1];
    int x;
    int offset = index;
    u16 *screen;

    if (y < 0) {
        y = -y;
    }
    x = position[0];
    if (x < 0) {
        x = -x;
    }
    screen = *(u16 **)((u8 *)resource + 0xC);
    offset *= 10;
    CopyToBgTilemapRect(*(void **)context, 2,
        (u8)Sum(*(s16 *)((u8 *)ov43_0222EFA0 + offset), x),
        (u8)Sum(*(s16 *)((u8 *)ov43_0222EFA2 + offset), y),
        3, 4, screen + 6, 0, 0,
        ((u32)screen[0] << 21) >> 24,
        ((u32)screen[1] << 21) >> 24);
    ScheduleBgTilemapBufferTransfer(*(void **)context, 2);
}

void ov43_0222CBAC(void *sub, void *shared, void *base, int heapId) {
    ov43_0222CD3C(sub, shared, base, heapId);
}
