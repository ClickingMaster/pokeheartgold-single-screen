#include "landing_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef struct R16Context {
    void *bg;
    u8 pad[0x208];
    u16 *screen;
} R16Context;

void ov43_0222C378(void *position, void *arg1, void *context, int value, int selected, int page, int arg6) {
    int offset;
    u8 *item;

    if (page == 0) {
        S16(position, 0) = value;
        S16(position, 4) = selected;
        ov43_0222C3E8(position, context, value);
    }
    offset = page * 0x38;
    item = (u8 *)position + 8 + offset;
    ov43_0222C7F8(item, context, (u8 *)context + 0x200);
    ov43_0222C918(item, arg1, context, U32(position, 0xB4), (u8 *)context + 0x200, (u8 *)arg1 + 0x10, value, arg6);
    ov43_0222C844(item, context, selected);
}

void ov43_0222C3E8(void *unused, void *context, int index) {
    u16 *screen = PTR(context, 0x20C);
    CopyToBgTilemapRect(PTR(context, 0), 0, 0x10, 0, 0x10, 4, screen + 6, 0, 4, screen[0] >> 3, screen[1] >> 3);
    screen = PTR(context, 0x20C);
    CopyToBgTilemapRect(PTR(context, 0), 0, index * 4 + 0x10, 0, 4, 4, screen + 6, index * 4, 8, screen[0] >> 3, screen[1] >> 3);
    ScheduleBgTilemapBufferTransfer(PTR(context, 0), 0);
}

void ov43_0222C46C(void *unused0, void *context, void *unused2, int index) {
    R16Context *ctx = context;
    u16 *screen = ctx->screen;
    CopyToBgTilemapRect(ctx->bg, 0, index * 4 + 0x10, 0, 4, 4, screen + 6, index * 4 + 0x10, 8, screen[0] >> 3, screen[1] >> 3);
    ScheduleBgTilemapBufferTransfer(ctx->bg, 0);
}

void ov43_0222C4B8(void *unused, void *context, int index) {
    u16 *screen = PTR(context, 0x20C);
    CopyToBgTilemapRect(PTR(context, 0), 0, 0x10, 0, 0x10, 4, screen + 6, 0, 4, screen[0] >> 3, screen[1] >> 3);
    screen = PTR(context, 0x20C);
    CopyToBgTilemapRect(PTR(context, 0), 0, index * 4 + 0x10, 0, 4, 4, screen + 6, index * 4, 8, screen[0] >> 3, screen[1] >> 3);
    ScheduleBgTilemapBufferTransfer(PTR(context, 0), 0);
}
