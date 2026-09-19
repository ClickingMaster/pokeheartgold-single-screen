#include "overlay43_gap15_tail_private.h"

void ov43_0222BAB8(void *work, void *unused, void *ctx) {
    int i;
    u8 *p;
    Sprite_Delete(PTR(work, 0xf8));
    RemoveWindow((u8 *)work + 0xc8);
    String_Delete(PTR(work, 0xd8));
    i = 0;
    p = work;
    do {
        ListMenuItems_Delete(PTR(p, 0xe4));
        p += 4;
        i++;
    } while (i < 2);
    RemoveWindow((u8 *)work + 0xb8);
    ov42_02227274(PTR(work, 0xb4));
    ov42_02227114(PTR(work, 0xb4));
    i = 0;
    p = (u8 *)work + 8;
    do {
        ov43_0222C7EC(p, ctx);
        p += 0x38;
        i++;
    } while (i < 3);
}

void ov43_0222BB20(void *state, void *arg1, void *ctx, int arg3) {
    u8 *tile;
    ov43_0222A9F4(ctx, (u8 *)ctx + 0x64, 5);
    tile = PTR(ctx, 0x20c);
    CopyToBgTilemapRect(PTR(ctx, 0), 0, 0x11, 0x14, 0xe, 4, tile + 0xc, 3, 0, (u8)(U16(tile, 0) >> 3), (u8)(U16(tile, 2) >> 3));
    ScheduleWindowCopyToVram((u8 *)state + 0xb8);
    ScheduleBgTilemapBufferTransfer(PTR(ctx, 0), 0);
    ov43_0222AD00(ctx, 1);
    ov43_0222C378(state, arg1, ctx, S16(state, 0), S16(state, 4), 0, arg3);
    ov43_0222AD74(ctx, 1, 1);
    ov43_0222AD98(ctx, 1, 0x10);
}

void ov43_0222BBB8(void *state, void *unused, void *ctx) {
    int i;
    u8 *p;
    ov43_0222AA70(ctx);
    ClearWindowTilemapAndScheduleTransfer((u8 *)state + 0xb8);
    i = 0;
    p = (u8 *)state + 8;
    do {
        ov43_0222C9A4(p, ctx, (u8 *)ctx + 0x200);
        i++;
        p += 0x38;
    } while (i < 3);
    ov43_0222AD74(ctx, 0, 0xff);
    ov43_0222AD98(ctx, 1, 9);
    ov43_0222AD00(ctx, 0);
    CopyToBgTilemapRect(PTR(ctx, 0), 0, 0x11, 0x14, 0xe, 4, (u8 *)PTR(ctx, 0x20c) + 0xc, 0x11, 0, (u8)(U16(PTR(ctx, 0x20c), 0) >> 3), (u8)(U16(PTR(ctx, 0x20c), 2) >> 3));
    CopyToBgTilemapRect(PTR(ctx, 0), 0, 0x10, 0, 0x10, 4, (u8 *)PTR(ctx, 0x20c) + 0xc, 0x10, 4, (u8)(U16(PTR(ctx, 0x20c), 0) >> 3), (u8)(U16(PTR(ctx, 0x20c), 2) >> 3));
    ScheduleBgTilemapBufferTransfer(PTR(ctx, 0), 0);
}
