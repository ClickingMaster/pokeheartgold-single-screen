#include "to56_w21_private.h"

void ov08_0221D6CC(u8 *work, int state) {
    u8 pixels;
    u8 i;
    u8 *entry;
    u8 battler;
    u32 hp;
    u32 maxHp;

    if (state != 2) {
        return;
    }
    battler = (*(u8 **)work)[0x11];
    entry = work + 4 + battler * 0x50;
    if (((Ov08Entry *)entry)->percent < 100) {
        u32 base = *(u32 *)(entry + 0x20);
        maxHp = *(u32 *)(entry + 0x24) - base;
        hp = *(u32 *)(entry + 0x1c) - base;
    } else {
        maxHp = 0;
        hp = 0;
    }
    pixels = CalculateHpBarPixelsLength(hp, maxHp, 0x40);
    for (i = 0; i < 8; i++) {
        u16 fill;
        if (pixels >= 8) {
            fill = 0x1e;
        } else {
            fill = pixels + 0x16;
        }
        ov08_0221D74C(work, fill, i + 10, 8);
        if (pixels < 8) {
            pixels = 0;
        } else {
            pixels -= 8;
        }
    }
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)(work + 0x1e4), 7);
}

void ov08_0221D74C(u8 *work, int fill, u16 x, int y) {
    FillBgTilemapRect(*(BgConfig **)(work + 0x1e4), 7, fill, (u8)x, (u8)y, 1, 1, 0x10);
}
